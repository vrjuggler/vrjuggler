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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/Devices/DriverConfig.h>

#include <vector>
#include <boost/bind.hpp>

#include <vpr/vpr.h>
#include <vpr/System.h>

#include <jccl/Config/ConfigElement.h>
#include <gadget/gadgetParam.h>
#include <gadget/InputManager.h>
#include <gadget/Type/DeviceConstructor.h>

#include "buttondevice.h"


/** Entry point function for the device driver plug-in. */
extern "C" GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

/** Entry point function for the device driver plug-in. */
extern "C" GADGET_DRIVER_EXPORT(void) initDevice(InputManager* inputMgr)
{
   new DeviceConstructor<ButtonDevice>(inputMgr);
}

// Returns a string that matches this device's configuration element type.
std::string ButtonDevice::getElementType()
{
   return std::string("button_device");
}

// When the system detects a configuration change for your driver, it will
// pass the jccl::ConfigElementPtr into this function.  See the documentation
// on config elements, for information on how to access them.
bool ButtonDevice::config(jccl::ConfigElementPtr e)
{
   // Configure all our base classes first.  If any of those fail, we cannot
   // finish configuring ourself.
   if ( ! Input::config(e) && ! Digital::config(e) )
   {
      return false;
   }

   mPort = e->getProperty<std::string>("port");
   mBaud = e->getProperty<int>("baud");

   return true;
}

// Spanws the sample thread, which calls ButtonDevice::sample() repeatedly.
bool ButtonDevice::startSampling()
{
   mRunning = false;

   try
   {
      mSampleThread =
         new vpr::Thread(boost::bind(&ButtonDevice::threadedSampleFunction,
                                     this));
      mRunning = true;
   }
   catch (vpr::Exception& ex)
   {
      std::cerr << "Failed to spawn sample thread!\n" << ex.what()
                << std::endl;
   }

   return mRunning;
}

// Records (or samples) the current data.  This is called repeatedly by the
// sample thread created by startSampling().
bool ButtonDevice::sample()
{
   bool status(false);

   if ( mRunning )
   {
      // Here you would add your code to sample the hardware for a button
      // press:
      std::vector<DigitalData> samples(1);
      samples[0] = 1;
      addDigitalSample(samples);

      // Successful sample.
      status = true;
   }

   return status;
}

// Kills the sample thread.
bool ButtonDevice::stopSampling()
{
   mRunning = false;

   if (mSampleThread != NULL)
   {
      mSampleThread->kill(); // Not guaranteed to work on all platforms
      mSampleThread->join();
      delete mSampleThread;
      mSampleThread = NULL;
   }
   return true;
}

void ButtonDevice::updateData()
{
   if ( mRunning )
   {
      swapDigitalBuffers();
   }
}

// Our sampling function that is executed by the spawned sample thread.
// This function is declared as a static member of ButtonDevice.  It simply
// calls ButtonDevice::sample() over and over.
void ButtonDevice::threadedSampleFunction()
{
   // spin until someone kills "mSampleThread"
   while ( mRunning )
   {
      sample();
      vpr::System::sleep(1); //specify some time here, so you don't waste CPU cycles
   }
}
