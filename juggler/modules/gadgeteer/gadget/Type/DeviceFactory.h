/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#ifndef _VRJ_DEVICE_FACTORY_H_
#define _VRJ_DEVICE_FACTORY_H_
//#pragma once

#include <vrj/vrjConfig.h>
#include <vrj/Input/Type/Input.h>
#include <vrj/Config/ConfigChunk.h>
#include <vpr/Util/Singleton.h>

#include <vrj/Util/Debug.h>
#include <vpr/Util/Assert.h>

namespace vrj
{

//: Base class for virtual construction of devices
// Implementations of this class are registered with the device factory
// for each device in the system
class DeviceConstructorBase
{
public:
   //: Constructor
   //! POST: Device is registered
   DeviceConstructorBase() {;}

   //: Create the device
   virtual Input* createDevice(ConfigChunk* chunk)
   {
      vjDEBUG(vjDBG_ALL,0) << "ERROR: DeviceConstructorBase::createDevice: Should never be called" << vjDEBUG_FLUSH;
      return NULL;
   }

   //: Get the string desc of the type of chunk we can create
   virtual std::string    getChunkType()
   { return std::string("BaseConstructor: Invalid type"); }
};


template <class DEV>
class DeviceConstructor : public DeviceConstructorBase
{
public:
   DeviceConstructor();

   Input* createDevice(ConfigChunk* chunk)
   {
      DEV* new_dev = new DEV;
      bool success = new_dev->config(chunk);
      if(success)
      {
         return new_dev;
      } else {
         delete new_dev;
         return NULL;
      }
   }

   virtual std::string getChunkType()
   { return DEV::getChunkType(); }
};

//: Object used for creating devices
//!NOTE: Singleton
class DeviceFactory
{
private:
   // Singleton so must be private
   DeviceFactory()
   {
      mConstructors = std::vector<DeviceConstructorBase*>(0);
      vprASSERT(mConstructors.size() == 0);
   }

   // This should be replaced with device plugins
   //! POST: Devices are loaded that the system knows about
   void hackLoadKnownDevices();

public:
   void registerDevice(DeviceConstructorBase* constructor);

   //: Query if the factory knows about the given device
   //!PRE: chunk != NULL, chunk is a valid chunk
   //!ARGS: chunk - chunk we are requesting about knowledge to create
   //!RETURNS: true - factory knows how to create the device
   //+          false - factory does not know how to create the device
   bool recognizeDevice(ConfigChunk* chunk);

   //: Load the specified device
   //!PRE: recognizeDevice(chunk) == true
   //!ARGS: chunk - specification of the device to load
   //!RETURNS: null - Device failed to load
   //+         other - Pointer to the loaded device
   Input* loadDevice(ConfigChunk* chunk);

private:
   //: Find a constructor for the given device type
   //!RETURNS: -1 - Not found
   //+            - Index of the constructorck
   int   findConstructor(ConfigChunk* chunk);

   void debugDump();


private:
   std::vector<DeviceConstructorBase*> mConstructors;  //: List of the device constructors

   vprSingletonHeaderWithInitFunc(DeviceFactory, hackLoadKnownDevices);
};

} // end namespace vrj

#endif
