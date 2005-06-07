/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/Devices/DriverConfig.h>

#include <string>
#include <boost/concept_check.hpp>
#include <vpr/System.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/gadgetParam.h>
#include <drivers/5DT/DataGlove/DataGloveStandalone.h> /* standalone dataglove driver */
#include <drivers/5DT/DataGlove/DataGlove.h> /* Gadgeteer dataglove driver */


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::DataGlove>(inputMgr);
}

}

namespace gadget
{

bool DataGlove::config(jccl::ConfigElementPtr e)
{
   if(! (Input::config(e) /*&& Glove::config(e)*/ ))
   {
      return false;
   }

   mPortName = e->getProperty<std::string>("port");
   mBaudRate = e->getProperty<int>("baud");

   vprASSERT(mThread == NULL);      // This should have been set by Input(c)
   mGlove = new DataGloveStandalone();
   return true;
}

std::string DataGlove::getElementType()
{
   return "data_glove";
}

DataGlove::~DataGlove ()
{
   stopSampling();      // Stop the glove
   delete mGlove;       // Delete the glove
}

bool DataGlove::startSampling()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "[dataglove] Begin sampling\n"
         << vprDEBUG_FLUSH;

   
   if (mThread == NULL)
   {
      int maxAttempts=0;
      bool result = false;
      while (result == false && maxAttempts < 5)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
               << "[dataglove] Connecting to "
               << mPortName << " at "
               << mBaudRate << "...\n"
               << vprDEBUG_FLUSH;
         result = mGlove->connectToHardware( mPortName , mBaudRate);
         if (result == false)
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "[dataglove] ERROR: Can't open or it is already opened."
               << vprDEBUG_FLUSH;
            vpr::System::usleep(14500);
            maxAttempts++;
         }
      }

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "[dataglove] Successfully connected, Now sampling dataglove data."
         << vprDEBUG_FLUSH;
      // Create a new thread to handle the control and set exit flag to false
      mExitFlag = false;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "[dataglove] Spawning control thread\n" << vprDEBUG_FLUSH;
      vpr::ThreadMemberFunctor<DataGlove>* memberFunctor =
         new vpr::ThreadMemberFunctor<DataGlove>(this, &DataGlove::controlLoop, NULL);
      mThread = new vpr::Thread(memberFunctor);

      if ( ! mThread->valid() )
      {
         return false;
      }
      else
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "[dataglove] DataGlove is active " << std::endl
            << vprDEBUG_FLUSH;
         mActive = true;
         return true;
      }
   }
   else
   {
      return false; // already sampling
   }
}

void DataGlove::controlLoop(void* nullParam)
{
   boost::ignore_unused_variable_warning(nullParam);

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "[dataglove] Entered control thread\n"
         << vprDEBUG_FLUSH;

   // Go until mExitFlag is set to true
   while(!mExitFlag)
   {
      sample();
   }
}

bool DataGlove::sample()
{
   int Eflag = mGlove->ReadRecordsFromHardware( &ch0, &ch1, &ch2, &ch3, &ch4, &ch5, &ch6, &ch7, &ch8 );

   if(Eflag)
   {
      mAnalogData[0]=ch0;    // Thumb (0.0 - 1.0)
      mAnalogData[1]=ch1;    // Index
      mAnalogData[2]=ch2;    // Middle
      mAnalogData[3]=ch3;    // Ring
      mAnalogData[4]=ch4;    // Pinky
      mAnalogData[5]=ch5;    // Pitch  (0 - 180)
      mAnalogData[6]=ch6;    // Roll   (0 - 180)
      mAnalogData[7]=ch7;    // Gesture  (0 - 15)
      mAnalogData[8]=ch8;    // Capabilty byte (0:A right hand, 6:Left Glove)
      addAnalogSample(mAnalogData);
    }
    return true;
}

void DataGlove::updateData()
{
    swapAnalogBuffers();
//    mDigitalSamples.swapBuffers();   // Somehow DigitalData is not working right!!!
    return;
}

bool DataGlove::stopSampling()
{
   if(mThread != NULL)
   {
      mExitFlag = true;
   
      //Wait for thread to exit normally after setting exit flag
      mThread->join();
      delete mThread;
      mThread = NULL;

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "[dataglove] stopping DataGlove.." << std::endl << vprDEBUG_FLUSH;
   }

   return true;
}

} // End of gadget namespace
