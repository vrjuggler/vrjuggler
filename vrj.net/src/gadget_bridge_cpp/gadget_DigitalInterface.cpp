// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/DeviceInterface.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget::DigitalInterface* gadget_DeviceInterface_gadget_DigitalProxy__DeviceInterface__gadget_DigitalInterface1(const gadget::DigitalInterface* p0)
   {
      const gadget::DigitalInterface& marshal_p0 = *p0;
      gadget::DigitalInterface* obj = new gadget::DigitalInterface(gadget::DigitalInterface(marshal_p0));
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::DigitalInterface* gadget_DeviceInterface_gadget_DigitalProxy__DeviceInterface__0()
   {
      gadget::DigitalInterface* obj = new gadget::DigitalInterface(gadget::DigitalInterface());
      return obj;
   }

   SHARPPY_API void delete_gadget_DigitalInterface(gadget::DigitalInterface* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::DeviceInterface<gadget::DigitalProxy>::getProxy()
   SHARPPY_API gadget::DigitalProxy* gadget_DeviceInterface_gadget_DigitalProxy__getProxy__0(gadget::DigitalInterface* self_ptr)
   {
      gadget::DigitalProxy* result;
      result = (*self_ptr).getProxy();
      return result;
   }


   // Wrapper for virtual method gadget::DeviceInterface<gadget::DigitalProxy>::refresh()
   SHARPPY_API void gadget_DeviceInterface_gadget_DigitalProxy__refresh__0(gadget::DigitalInterface* self_ptr)
   {
      (*self_ptr).refresh();
   }


} // extern "C" for gadget::DigitalInterface


