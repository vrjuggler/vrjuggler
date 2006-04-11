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

#ifndef _GADGET_DEVICE_CONSTRUCTOR_BASE_H_
#define _GADGET_DEVICE_CONSTRUCTOR_BASE_H_
//#pragma once

#include <gadget/gadgetConfig.h>
#include <string>
#include <jccl/Config/ConfigChunkPtr.h>

#include <vpr/Util/Debug.h>


namespace gadget
{

class Input;

/**
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
   DeviceConstructorBase() {;}

   /** Creates the device. */
   virtual Input* createDevice(jccl::ConfigChunkPtr chunk)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "ERROR: DeviceConstructorBase::createDevice: Should never be called\n"
         << vprDEBUG_FLUSH;
      return NULL;
   }

   /** Gets the string desc of the type of chunk we can create. */
   virtual std::string getChunkType()
   {
      return std::string("BaseConstructor: Invalid type");
   }
};

} // End of gadget namespace


#endif
