/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VRJ.NET is (C) Copyright 2004 by Patrick Hartling
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

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DeviceInterface.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget::PositionInterface* gadget_DeviceInterface_gadget_PositionProxy__DeviceInterface__gadget_PositionInterface1(const gadget::PositionInterface* p0)
   {
      const gadget::PositionInterface& marshal_p0 = *p0;
      gadget::PositionInterface* obj = new gadget::PositionInterface(gadget::PositionInterface(marshal_p0));
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::PositionInterface* gadget_DeviceInterface_gadget_PositionProxy__DeviceInterface__0()
   {
      gadget::PositionInterface* obj = new gadget::PositionInterface(gadget::PositionInterface());
      return obj;
   }

   SHARPPY_API void delete_gadget_PositionInterface(gadget::PositionInterface* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::DeviceInterface<gadget::PositionProxy>::getProxy()
   SHARPPY_API gadget::PositionProxy* gadget_DeviceInterface_gadget_PositionProxy__getProxy__0(gadget::PositionInterface* self_ptr)
   {
      gadget::PositionProxy* result;
      result = (*self_ptr).getProxy();
      return result;
   }


   // Wrapper for virtual method gadget::DeviceInterface<gadget::PositionProxy>::refresh()
   SHARPPY_API void gadget_DeviceInterface_gadget_PositionProxy__refresh__0(gadget::PositionInterface* self_ptr)
   {
      (*self_ptr).refresh();
   }


} // extern "C" for gadget::PositionInterface


