/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
#include <vpr/System.h>

#include <jccl/Config/ConfigChunk.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Devices/5DT/DataGlove/DataGloveStandalone.h> /* standalone dataglove driver */
#include <gadget/Devices/5DT/DataGlove/DataGlove.h> /* Gadgeteer dataglove driver */


void initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::DataGlove>(inputMgr);
}

namespace gadget
{

bool DataGlove::config(jccl::ConfigChunkPtr c)
{
   if(! (Input::config(c) /*&& Glove::config(c)*/ ))
      return false;
   
   mPortName = c->getProperty<std::string>("port");
   mBaudRate = c->getProperty<int>("baud");

   vprASSERT(mThread == NULL);      // This should have been set by Input(c)
   mGlove = new DataGloveStandalone();
   return true;
}

DataGlove::~DataGlove ()
{
   stopSampling();      // Stop the glove
   delete mGlove;       // Delete the glove
}

int DataGlove::startSampling()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, 0) << "[dataglove] Begin sampling\n"
                                    << vprDEBUG_FLUSH;
   if (mThread == NULL)
   {
      int maxAttempts=0;
      bool result = false;
      while (result == false && maxAttempts < 5)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, 0) << "[dataglove] Connecting to "
                                          << mPortName << " at "
                                          << mBaudRate << "...\n"
                                          << vprDEBUG_FLUSH;
         result = mGlove->connectToHardware( mPortName , mBaudRate);
         if (result == false)
         {
            vprDEBUG(gadgetDBG_INPUT_MGR,0)
               << "[dataglove] ERROR: Can't open or it is already opened."
               << vprDEBUG_FLUSH;
            vpr::System::usleep(14500);
            maxAttempts++;
         }
      }

      vprDEBUG(gadgetDBG_INPUT_MGR,0)
         << "[dataglove] Successfully connected, Now sampling dataglove data."
         << vprDEBUG_FLUSH;
      // Create a new thread to handle the control
      vprDEBUG(gadgetDBG_INPUT_MGR, 0)
         << "[dataglove] Spawning control thread\n" << vprDEBUG_FLUSH;
      vpr::ThreadMemberFunctor<DataGlove>* memberFunctor =
         new vpr::ThreadMemberFunctor<DataGlove>(this, &DataGlove::controlLoop, NULL);
      mThread = new vpr::Thread(memberFunctor);

      if ( ! mThread->valid() )
      {
         return 0;
      }
      else
      {
         vprDEBUG(gadgetDBG_INPUT_MGR,1) << "[dataglove] DataGlove is active "
                                         << std::endl << vprDEBUG_FLUSH;
         mActive = true;
         return 1;
      }
   }
   else
   {
      return 0; // already sampling
   }
}

void DataGlove::controlLoop(void* nullParam)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, 0) << "[dataglove] Entered control thread\n"
                                    << vprDEBUG_FLUSH;

   while(1)
   {
      sample();
   }
}

int DataGlove::sample()
{

    std::string gesture;
    mGlove->ReadRecordsFromHardware( &ch0, &ch1, &ch2, &ch3, &ch4, &ch5, &ch6, &ch7, &ch8 );
   
    mAnalogData[0]=ch0;    // Thumb (0.0 - 1.0)
    mAnalogData[1]=ch1;    // Index
    mAnalogData[2]=ch2;    // Middle
    mAnalogData[3]=ch3;    // Ring
    mAnalogData[4]=ch4;    // Pinky
    mAnalogData[5]=ch5;    // Pitch  (0 - 180)
    mAnalogData[6]=ch6;    // Roll   (0 - 180)
    mAnalogData[7]=ch7;    // Gesture  (0 - 15)
    mAnalogData[8]=ch8;    // Capabilty byte (0:A right hand, 6:Left Glove) 
//    mDigitalData[0]=ch7;   // Somehow DigitalData is not working right!!!
                             // AnalogData is being used instead of DigitalData.
    
    addAnalogSample(mAnalogData);
    return 1;
}

void DataGlove::updateData()
{
    swapAnalogBuffers();
//    mDigitalSamples.swapBuffers();   // Somehow DigitalData is not working right!!!
    return;
}

int DataGlove::stopSampling()
{
   if (mThread != NULL)
   {
      mThread->kill();
      delete mThread;
      mThread = NULL;
      vpr::System::usleep(100);

      vprDEBUG(gadgetDBG_INPUT_MGR,1) << "[dataglove] stopping DataGlove.."
                                      << std::endl << vprDEBUG_FLUSH;
   }
   return 1;
}

} // End of gadget namespace
