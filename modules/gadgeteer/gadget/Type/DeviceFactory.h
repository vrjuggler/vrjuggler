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

#ifndef _GADGET_DEVICE_FACTORY_H_
#define _GADGET_DEVICE_FACTORY_H_
//#pragma once

#include <gadget/gadgetConfig.h>
#include <vector>
#include <jccl/Config/ConfigElementPtr.h>
#include <vpr/Util/Singleton.h>

#include <gadget/Type/DeviceConstructorBase.h>

#include <vpr/Util/Assert.h>

namespace gadget
{

/** \class DeviceFactory DeviceFactory.h gadget/Type/DeviceFactory.h
 *
 * Object used for creating devices.
 * @note Singleton
 */
class GADGET_CLASS_API DeviceFactory
{
private:
   /** Singleton so must be private. */
   DeviceFactory()
   {
      mConstructors = std::vector<DeviceConstructorBase*>(0);
      vprASSERT(mConstructors.size() == 0);
   }

   /**
    * Performs static loading of devices that cannot be loaded through the
    * dynamic plug-in mechanism.
    *
    * @post Devices known to the system statically at initialization time are
    *       loaded.
    */
   void loadKnownDevices();

public:
   void registerDevice(DeviceConstructorBase* constructor);

   /**
    * Queries if the factory knows about the given device.
    * @pre element != NULL, element is a valid element.
    * @param element The element we are requesting about knowledge to create.
    * @return true if the factory knows how to create the device; false if not.
    */
   bool recognizeDevice(jccl::ConfigElementPtr element);

   /**
    * Loads the specified device.
    * @pre recognizeDevice(element) == true.
    * @param element The specification of the device to load.
    * @return NULL is returned if the device failed to load.
    *         Otherwise, a pointer to the loaded device is returned.
    */
   Input* loadDevice(jccl::ConfigElementPtr element);

private:
   /**
    * Finds a constructor for the given device type.
    * @return -1 is returned if the constructor is not found.
    *         Otherwise, the index of the constructor is returned.
    */
   int findConstructor(jccl::ConfigElementPtr element);

   void debugDump();


private:
   std::vector<DeviceConstructorBase*> mConstructors;  /**<  List of the device constructors */

   vprSingletonHeaderWithInitFunc(DeviceFactory, loadKnownDevices);
};

} // end namespace gadget

#endif
