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

// Generated from Revision: 1.60 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DeviceInterface.h>

struct gadget_PositionInterface_Holder
{
   gadget::PositionInterface mPtr;
};

extern "C"
{
   SHARPPY_API gadget_PositionInterface_Holder* new_gadget_PositionInterface_Holder(gadget::PositionInterface ptr)
   {
      gadget_PositionInterface_Holder* h = new gadget_PositionInterface_Holder;
      h->mPtr = ptr;
      return h;
   }

   // Constructor wrapper.
   SHARPPY_API gadget_PositionInterface_Holder* gadget_DeviceInterface_gadget_PositionProxy__DeviceInterface__gadget_PositionInterface(const gadget::PositionInterface* p0)
   {
      const gadget::PositionInterface& marshal_p0 = *p0; // Pre-call marshaling for p0
      gadget_PositionInterface_Holder* obj = new gadget_PositionInterface_Holder;
      obj->mPtr = gadget::PositionInterface(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget_PositionInterface_Holder* gadget_DeviceInterface_gadget_PositionProxy__DeviceInterface__()
   {
      gadget_PositionInterface_Holder* obj = new gadget_PositionInterface_Holder;
      obj->mPtr = gadget::PositionInterface();
      return obj;
   }

   SHARPPY_API void delete_gadget_PositionInterface(gadget_PositionInterface_Holder* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::DeviceInterface<gadget::PositionProxy>::getProxy()
   SHARPPY_API gadget::PositionProxy* gadget_DeviceInterface_gadget_PositionProxy__getProxy__0(gadget_PositionInterface_Holder* self_)
   {
      gadget::PositionProxy* result;
      result = self_->mPtr.getProxy();
      return result;
   }


   // Wrapper for virtual method gadget::DeviceInterface<gadget::PositionProxy>::refresh()
   SHARPPY_API void gadget_DeviceInterface_gadget_PositionProxy__refresh__0(gadget_PositionInterface_Holder* self_)
   {
      self_->mPtr.refresh();
   }


} // extern "C" for gadget::PositionInterface


