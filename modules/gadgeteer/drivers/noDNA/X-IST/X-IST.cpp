/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <boost/bind.hpp>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/gadgetParam.h>

#include <drivers/noDNA/X-IST/X-ISTStandalone.h> /* standalone X-IST driver */
#include <drivers/noDNA/X-IST/X-IST.h> /* Gadgeteer X-IST driver */


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::X_IST>(inputMgr);
}

}

namespace gadget
{

X_IST::X_IST()
   : mGlove(NULL)
   , mExitFlag(false)
{
   /* Do nothing. */ ;
}

X_IST::~X_IST()
{
   stopSampling();      // Stop the glove

   if ( NULL != mGlove )
   {
      delete mGlove;    // Delete the glove
      mGlove = NULL;
   }
}

bool X_IST::config(jccl::ConfigElementPtr e)
{
   if ( ! (Input::config(e) /*&& Glove::config(e)*/) )
   {
      return false;
   }

   mGloveNumber = e->getProperty<int>("number");

   vprASSERT(mThread == NULL);      // This should have been set by Input(c)
   mGlove = new X_ISTStandalone();
   return true;
}

std::string X_IST::getElementType()
{
   return "x_ist_glove";
}

bool X_IST::startSampling()
{
   bool started(false);

   if ( mThread == NULL )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "[X-IST] Begin sampling\n" << vprDEBUG_FLUSH;

      bool result = mGlove->connectToHardware(mGloveNumber);

      if ( result == false )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "[X-IST] ERROR: Can't open or it is already opened.\n"
            << vprDEBUG_FLUSH;
         return false;
      }

      const unsigned int num_sensors = mGlove->getNumberSensors();
      mAnalogData = std::vector<AnalogData>(num_sensors, 1.0f);
/*
      for ( unsigned int i = 0; i < num_sensors; ++i )
      {
         mAnalogData[i] = 1.0f;
      }
*/

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "[X-IST] Successfully connected, Now sampling X-IST data.\n"
         << vprDEBUG_FLUSH;

// Create a new thread to handle the control and set exit flag to false
      mExitFlag = false;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "[X-IST] Spawning control thread.\n" << vprDEBUG_FLUSH;

      try
      {
         mThread = new vpr::Thread(boost::bind(&X_IST::controlLoop, this));
         mActive = true;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
            << "[X-IST] X_IST is active" << std::endl << vprDEBUG_FLUSH;
      }
      catch (vpr::Exception& ex)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR")
            << ": Failed to spawn thread for X-IST driver!\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << ex.what() << std::endl << vprDEBUG_FLUSH;
         started = false;
      }
   }

   return started;
}

void X_IST::updateData()
{
   swapAnalogBuffers();
}

void X_IST::controlLoop()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "[X-IST] Entered control thread\n" << vprDEBUG_FLUSH;
// Go until mExitFlag is set to true
   while ( ! mExitFlag )
   {
      sample();
   }
}

bool X_IST::sample()
{
   WORD values[24];
   if ( mGlove->ReadRecordsFromHardware(values) )
   {
      for ( unsigned int i = 0; i < mGlove->getNumberSensors(); ++i )
      {
         if ( values[i] <= 1024 )
         {
            mAnalogData[i] = 1.0f - static_cast<float>(values[i]) / 1024.0f;
         }
      }

      addAnalogSample(mAnalogData);
   }

   return true;
}

bool X_IST::stopSampling()
{
   if  ( mThread != NULL )
   {
      mExitFlag = true;

// Wait for thread to exit normally after setting exit flag
      mThread->join();
      delete mThread;
      mThread = NULL;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "[X-IST] stopping X-IST.." << std::endl << vprDEBUG_FLUSH;
   }

   return true;
}

} // End of gadget namespace
