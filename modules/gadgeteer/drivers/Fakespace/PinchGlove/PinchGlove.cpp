/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/Devices/DriverConfig.h>

#include <iomanip>
#include <algorithm>
#include <boost/bind.hpp>

#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>
#include <gadget/Type/DeviceConstructor.h>
#include <jccl/Config/ConfigElement.h>
#include <vpr/vpr.h>
#include <vpr/System.h>

// Fakespace pinch driver
#include <drivers/Fakespace/PinchGlove/PinchGloveStandalone.h>
// Gadgeteer pinch driver
#include <drivers/Fakespace/PinchGlove/PinchGlove.h>


extern "C"
{
   GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
   {
      return __GADGET_version;
   }

   GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
   {
      new gadget::DeviceConstructor<gadget::PinchGlove>(inputMgr);
   }
}

namespace gadget
{

std::string PinchGlove::getElementType()
{
   return "pinch_glove";
}

bool PinchGlove::config(jccl::ConfigElementPtr e)
{
   Input::config(e);
   mPortName = e->getProperty<std::string>("port");
   mBaud = e->getProperty<int>("baud");
   
   // This should have been set by Input(c)
   vprASSERT(mThread == NULL);
   
   return true;
}

PinchGlove::~PinchGlove()
{
   stopSampling();      // Stop the glove
   delete mGlove;       // Delete the glove
}

bool PinchGlove::startSampling()
{
   if (NULL != mThread)
   {
      // Already sampling
      return 0; 
   }
   else
   {
      // Create a new PinchGloveStandalone device.
      mGlove = new PinchGloveStandalone();
    
      // Formatting so that the [OK] or [FAILED] lines up with the rest.
      const int port_width(17);
     
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_STATUS_LVL)
         << "[PinchGlove] Connecting to PinchGlove on port: "
         << std::setiosflags(std::ios::right) << std::setfill(' ') 
         << std::setw(port_width) << mPortName << std::endl << vprDEBUG_FLUSH;
     
      // Attempt to connect to the PinchGlove. 
      if(!mGlove->connect(mPortName, mBaud).success())
      {
         return 0;
      }

      // Create a vector of strings to hold information about PinchGlove.
      std::vector<std::string> info(0);

      // Query the PinchGlove for its harware information.
      if(!mGlove->printHardwareInformation(info).success())
      {
         return 0;
      }
      
      // Time stamps are on by default. (Pinch Glove Manual p. 8)
      if(!mGlove->setTimestampsOn(true).success())
      {
         return 0;   
      }
      
      // Print out the hardware information.
      for(std::vector<std::string>::iterator itr = info.begin() ; itr != info.end() ; ++itr)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_STATUS_LVL) 
            << (*itr) << std::endl << vprDEBUG_FLUSH;
      }

      mExitFlag = false;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL) 
      << "[PinchGlove] Spawning control thread." << std::endl  << vprDEBUG_FLUSH;

      // Create a new thread to handle the control.
      mThread = new vpr::Thread(boost::bind(&PinchGlove::controlLoop, this));

      if (!mThread->valid())
      {
         return 0;
      }
      else
      {
         // We want to add an open hand sample first because the pinch glove
         // will not return data until there is a pinch. And until then, the
         // hand will be open.
         std::vector<DigitalData> digital_sample(10, 0);
         addDigitalSample(digital_sample);
         std::vector<GloveData> gloveData = getGloveDataFromDigitalData(digital_sample);
         addGloveSample(gloveData);
         swapDigitalBuffers();
         swapGloveBuffers();
         
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "[PinchGlove] PinchGlove is active " << std::endl
            << vprDEBUG_FLUSH;
         return 1;
      }
   }
}

void PinchGlove::controlLoop()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "[PinchGlove] Entered control thread"
      << std::endl << vprDEBUG_FLUSH;

   // Looping until we are told to exit.
   while ( !mExitFlag )
   {
      sample();
   }
}

bool PinchGlove::sample()
{
   // Create a vector of DigitalData's to hold our sample.
   std::vector<DigitalData> digital_sample(10);
  
   // Create a vector to retrieve data from the PinchGloveStandalone
   // driver with.
   std::vector<int> data(10, 0);

   // NOTE: The timestamp retrieved from the PinchGlove is not used at this
   //       time. This is partly because it saturates at 16382 ticks.
   //       (13.6511206 seconds) (p. 10 PinchGlove Manual)"
   int timestamp = 0;
   
   // Get data from PinchGlove;
   if(mGlove->sample(data, timestamp).success())
   {
      // Copy the data into a new digital sample.
      std::copy(data.begin(), data.end(), digital_sample.begin());
      
      // Add a new digital sample to the buffer.
      addDigitalSample(digital_sample);

      std::vector<GloveData> gloveData = getGloveDataFromDigitalData(digital_sample);
      addGloveSample(gloveData);
      
      // Debug print.
      //for(unsigned int i = 0 ; i < data.size() ; ++i)
      //{
      //   std::cout << (data[i] > 0 ? "1" : "0");
      //}
      //std::cout << std::endl;
      return 1;
   }
   return 0;
}

void PinchGlove::updateData()
{
   swapDigitalBuffers();
   swapGloveBuffers();
   return;
}

bool PinchGlove::stopSampling()
{
   if ( mThread != NULL )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_STATUS_LVL)
         << "[PinchGlove] Stopping PinchGlove." << std::endl << vprDEBUG_FLUSH;
      
      //Signal to thread that it should exit
      mExitFlag = true;

      mThread->join();
      delete mThread;
      mThread = NULL;

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_STATUS_LVL)
         << "[PinchGlove] PinchGlove stopped." << std::endl << vprDEBUG_FLUSH;
   }
   return 1;
}

} // namespace gadget
