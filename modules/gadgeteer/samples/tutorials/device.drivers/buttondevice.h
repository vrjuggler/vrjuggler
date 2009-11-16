/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#ifndef _EXAMPLE_BUTTON_DEVICE_H_
#define _EXAMPLE_BUTTON_DEVICE_H_

#include <gadget/Devices/DriverConfig.h>

#include <stdlib.h>
#include <string>

#include <vpr/vpr.h>

#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/InputMixer.h>


using namespace gadget;

/** The device driver class. */
class ButtonDevice : public InputMixer<Input, Digital>
{
public:
   ButtonDevice()
      : mSampleThread(NULL)
      , mRunning(false)
   {
      /* Do nothing. */ ;
   }

   virtual ~ButtonDevice()
   {
      if ( mRunning )
      {
         this->stopSampling();
      }
   }

   /**
    * Returns a string that matches this device's configuration element type.
    */
   static std::string getElementType();

   /**
    * When the system detects a configuration change for your driver, it will
    * pass the jccl::ConfigElementPtr into this function.  See the
    * documentation on config elements, for information on how to access them.
    */
   virtual bool config(jccl::ConfigElementPtr e);

   /**
    * Spanws the sample thread, which calls ButtonDevice::sample() repeatedly.
    */
   virtual bool startSampling();

   /**
    * Records (or samples) the current data.  This is called repeatedly by the
    * sample thread created by startSampling().
    */
   virtual bool sample();

   /** Kills the sample thread. */
   virtual bool stopSampling();

   virtual void updateData();

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
   /**
    * Our sampling function that is executed by the spawned sample thread.
    * This function simply calls ButtonDevice::sample() over and over.
    */
   void threadedSampleFunction();

   vpr::Thread*  mSampleThread;
   bool          mRunning;

   // configuration data set by config()
   std::string   mPort;
   int           mBaud;
};

#endif /* _EXAMPLE_BUTTON_DEVICE_H_ */
