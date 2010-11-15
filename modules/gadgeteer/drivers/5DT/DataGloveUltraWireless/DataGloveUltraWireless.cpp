/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/Devices/DriverConfig.h>

#include <gadget/Type/DeviceConstructor.h>
#include <gadget/gadgetParam.h>
#include <gadget/Util/Debug.h>

#include <jccl/Config/ConfigElement.h>
#include <vpr/System.h>

#include <drivers/5DT/DataGloveUltraWireless/DataGloveUltraWirelessStandalone.h> /* standalone DataGloveUltraWireless driver */
#include <drivers/5DT/DataGloveUltraWireless/DataGloveUltraWireless.h> /* Gadgeteer DataGloveUltraWireless driver */

#include <boost/bind.hpp>


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::DataGloveUltraWireless>(inputMgr);
}

}

namespace gadget
{

/** Construct */
DataGloveUltraWireless::DataGloveUltraWireless()
   : mDone(false)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "*** DataGloveUltraWireless::DataGloveUltraWireless() ***\n" << vprDEBUG_FLUSH;
}

bool DataGloveUltraWireless::config(jccl::ConfigElementPtr e)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "*** DataGloveUltraWireless::config() ***" << std::endl << vprDEBUG_FLUSH;

   if(! (Input::config(e) && Analog::config(e) && Command::config(e) ) )
   {
      return false;
   }

   mPortName = e->getProperty<std::string>("port");
   mBaudRate = e->getProperty<int>("baud");
   mPortAEnabled = e->getProperty<bool>("port_a_enabled");
   mPortBEnabled = e->getProperty<bool>("port_b_enabled");

   mGlove.setGestureThresholds(
      e->getProperty<float>("gesture_upper_threshold"),
      e->getProperty<float>("gesture_lower_threshold") );

   mGlove.setAutoRangeReset(
      e->getProperty<bool>("auto_range_reset") );

   vprASSERT(mThread == NULL);      // This should have been set by Input(c)
   return true;
}

std::string DataGloveUltraWireless::getElementType()
{
   return "data_glove_ultra_wireless";
}

DataGloveUltraWireless::~DataGloveUltraWireless()
{
   stopSampling();      // Stop the glove
}

bool DataGloveUltraWireless::startSampling()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "[DataGloveUltraWireless] Begin sampling\n"
         << vprDEBUG_FLUSH;


   bool started(false);

   if (mThread == NULL)
   {
      int maxAttempts=0;
      bool result = false;
      while (result == false && maxAttempts < 5)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
               << "[DataGloveUltraWireless] Connecting to "
               << mPortName << " at "
               << mBaudRate << "...\n"
               << vprDEBUG_FLUSH;

         result = mGlove.open( mPortName , mBaudRate, mPortAEnabled, mPortBEnabled );

         if (result == false)
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "[DataGloveUltraWireless] ERROR: Can't open or it is already opened."
               << vprDEBUG_FLUSH;

            vpr::System::usleep(14500);
            maxAttempts++;
         }
      }

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "[DataGloveUltraWireless] Successfully connected, Now sampling DataGloveUltraWireless data."
         << vprDEBUG_FLUSH;


      // Create a new thread to handle the control and set exit flag to false
      mDone = false;

      try
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "[DataGloveUltraWireless] Spawning control thread\n" << vprDEBUG_FLUSH;

         mThread = new vpr::Thread(boost::bind(&DataGloveUltraWireless::controlLoop, this));

         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "[DataGloveUltraWireless] DataGloveUltraWireless is active " << std::endl
            << vprDEBUG_FLUSH;

         mActive = true;
         started = true;
      }
      catch (vpr::Exception& ex)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR")
            << ": Failed to spawn thread for 5DT DataGloveUltraWireless driver!\n"
            << vprDEBUG_FLUSH;

         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << ex.what() << std::endl << vprDEBUG_FLUSH;
      }
   }

   return started;
}

void DataGloveUltraWireless::controlLoop()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "[DataGloveUltraWireless] Entered control thread\n"
         << vprDEBUG_FLUSH;

   // Go until mDone is set to true
   while( !mDone )
   {
      sample();

      vpr::System::msleep(10);
   }

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "[DataGloveUltraWireless] Exited control thread\n"
         << vprDEBUG_FLUSH;
}

bool DataGloveUltraWireless::sample()
{ 
   if( !isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "[DataGloveUltraWireless] not active\n"
         << vprDEBUG_FLUSH;
      return false;
   }

   mGlove.updateData();
   
   // Analog (flex) data
   const std::vector<float> tmp_vec = mGlove.getGloveData();

   std::vector<gadget::AnalogData>   cur_analog_samples;
   for(unsigned int i = 0; i < tmp_vec.size(); ++i)
   {
      AnalogData new_analog(tmp_vec[i]);
      new_analog.setTime();
      cur_analog_samples.push_back(new_analog);
   }

   addAnalogSample(cur_analog_samples);

   // Command (gesture) data
   std::vector<gadget::CommandData>   cur_command_samples;

   CommandData gesture_port_a(mGlove.getGloveAGesture());
   gesture_port_a.setTime();
   cur_command_samples.push_back(gesture_port_a);

   CommandData gesture_port_b(mGlove.getGloveBGesture());
   gesture_port_b.setTime();
   cur_command_samples.push_back(gesture_port_b);

   addCommandSample(cur_command_samples);

   return true;
}

void DataGloveUltraWireless::updateData()
{
   if(isActive())
   {
      swapAnalogBuffers();
      swapCommandBuffers();
   }
}

bool DataGloveUltraWireless::stopSampling()
{
   if(mThread != NULL)
   {
      mDone = true;

      //Wait for thread to exit normally after setting exit flag
      mThread->join();

      delete mThread;
      mThread = NULL;

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "[DataGloveUltraWireless] stopping DataGloveUltraWireless.." << std::endl << vprDEBUG_FLUSH;
   }

   return true;
}

} // End of gadget namespace
