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
#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/DeviceInterface.h>

struct gadget_DigitalInterface_Holder
{
   gadget::DigitalInterface mPtr;
};

extern "C"
{
   SHARPPY_API gadget_DigitalInterface_Holder* new_gadget_DigitalInterface_Holder(gadget::DigitalInterface ptr)
   {
      gadget_DigitalInterface_Holder* h = new gadget_DigitalInterface_Holder;
      h->mPtr = ptr;
      return h;
   }

   // Constructor wrapper.
   SHARPPY_API gadget_DigitalInterface_Holder* gadget_DeviceInterface_gadget_DigitalProxy__DeviceInterface__gadget_DigitalInterface(const gadget::DigitalInterface* p0)
   {
      const gadget::DigitalInterface& marshal_p0 = *p0; // Pre-call marshaling for p0
      gadget_DigitalInterface_Holder* obj = new gadget_DigitalInterface_Holder;
      obj->mPtr = gadget::DigitalInterface(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget_DigitalInterface_Holder* gadget_DeviceInterface_gadget_DigitalProxy__DeviceInterface__()
   {
      gadget_DigitalInterface_Holder* obj = new gadget_DigitalInterface_Holder;
      obj->mPtr = gadget::DigitalInterface();
      return obj;
   }

   SHARPPY_API void delete_gadget_DigitalInterface(gadget_DigitalInterface_Holder* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::DeviceInterface<gadget::DigitalProxy>::getProxy()
   SHARPPY_API gadget::DigitalProxy* gadget_DeviceInterface_gadget_DigitalProxy__getProxy__0(gadget_DigitalInterface_Holder* self_)
   {
      gadget::DigitalProxy* result;
      result = self_->mPtr.getProxy();
      return result;
   }


   // Wrapper for virtual method gadget::DeviceInterface<gadget::DigitalProxy>::refresh()
   SHARPPY_API void gadget_DeviceInterface_gadget_DigitalProxy__refresh__0(gadget_DigitalInterface_Holder* self_)
   {
      self_->mPtr.refresh();
   }


} // extern "C" for gadget::DigitalInterface


