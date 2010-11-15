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

#ifndef _GADGET_DEVICE_CONSTRUCTOR_BASE_H_
#define _GADGET_DEVICE_CONSTRUCTOR_BASE_H_
//#pragma once

#include <gadget/gadgetConfig.h>
#include <boost/concept_check.hpp>
#include <string>
#include <jccl/Config/ConfigElementPtr.h>

#include <vpr/Util/Debug.h>


namespace gadget
{

class Input;

/** \class DeviceConstructorBase DeviceConstructorBase.h gadget/Type/DeviceConstructorBase.h
 *
 * Base class for virtual construction of devices.
 * Implementations of this class are registered with the device factory
 * for each device in the system.
 */
class DeviceConstructorBase
{
public:
   /**
    * Constructor.
    * @post Device is registered.
    */
   DeviceConstructorBase()
   {;}

   virtual ~DeviceConstructorBase()
   {;}

   /** Creates the device. */
   virtual Input* createDevice(jccl::ConfigElementPtr element)
   {
      boost::ignore_unused_variable_warning(element);
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "ERROR: DeviceConstructorBase::createDevice: Should never be called\n"
         << vprDEBUG_FLUSH;
      return NULL;
   }

   /** Gets the string desc of the type of element we can create. */
   virtual std::string getElementType()
   {
      return std::string("BaseConstructor: Invalid type");
   }
};

} // End of gadget namespace


#endif
