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

#include <vector>
#include <string>

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <vpr/System.h>

#include <gadget/InputManager.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/InputMixer.h>
#include <gadget/Type/DeviceConstructor.h>


using namespace gadget;

/** The device driver class. */
class MyButtonDevice : public InputMixer<Input, Digital>
{
public:
   MyButtonDevice()
      : mSampleThread(NULL)
      , mRunning(false)
   {
      /* Do nothing. */ ;
   }

   virtual ~MyButtonDevice()
   {
      if ( mRunning )
      {
         this->stopSampling();
      }
   }

   virtual bool config(jccl::ConfigElementPtr e);
   virtual void updateData();

   virtual bool startSampling();
   virtual bool sample();
   virtual bool stopSampling();

   static std::string getElementType();

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   virtual void destroy()
   {
      delete this;
   }

private:
   static void   threadedSampleFunction( void* classPointer );
   int           mDigitalData;
   vpr::Thread*  mSampleThread;
   bool          mRunning;

   // configuration data set by config()
   std::string   mPort;
   int           mBaud;
};

/** Entry point function for the device driver plug-in. */
extern "C" GADGET_DRIVER_API(void) initDevice(InputManager* inputMgr)
{
   new DeviceConstructor<MyButtonDevice>(inputMgr);
}

/** Returns a string that matches this device's configuration element type. */
std::string MyButtonDevice::getElementType()
{
   return std::string("MyButtonDevice");
}

//: When the system detects a configuration change for your driver, it will
//  pass the new jccl::ConfigElement into this function.  See the documentation
//  on config elements, for information on how to access them.
bool MyButtonDevice::config(jccl::ConfigElementPtr e)
{
  if (!Digital::config(e))
  {
     return false;
  }

  mPort = e->getProperty<std::string>("port");
  mBaud = e->getProperty<int>("baud");

  return true;
}

void MyButtonDevice::updateData()
{
   if ( mRunning )
   {
      swapDigitalBuffers();
   }
}

/**
 * Spanws the sample thread, which calls MyButtonDevice::sample() repeatedly.
 */
bool MyButtonDevice::startSampling()
{
   mRunning = true;
   mSampleThread = new vpr::Thread(threadedSampleFunction, (void*) this);

   if ( ! mSampleThread->valid() )
   {
      mRunning = false;
      return false; // thread creation failed
   }
   else
   {
      return true; // thread creation success
   }
}

/**
 * Records (or samples) the current data.  This is called repeatedly by the
 * sample thread created by startSampling().
 */
bool MyButtonDevice::sample()
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

/** Kills the sample thread. */
bool MyButtonDevice::stopSampling()
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

/**
 * Our sampling function that is executed by the spawned sample thread.
 * This function is declared as a static member of MyButtonDevice.  It simply
 * calls MyButtonDevice::sample() over and over.
 */
void MyButtonDevice::threadedSampleFunction(void* classPointer)
{
   MyButtonDevice* this_ptr = static_cast<MyButtonDevice*>( classPointer );

   // spin until someone kills "mSampleThread"
   while ( this_ptr->mRunning )
   {
     this_ptr->sample();
     vpr::System::sleep(1); //specify some time here, so you don't waste CPU cycles
   }
}
