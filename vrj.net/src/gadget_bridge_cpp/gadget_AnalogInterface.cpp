// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/AnalogInterface.h>
#include <gadget/Type/DeviceInterface.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget::AnalogInterface* gadget_DeviceInterface_gadget_AnalogProxy__DeviceInterface__gadget_AnalogInterface1(const gadget::AnalogInterface* p0)
   {
      const gadget::AnalogInterface& marshal_p0 = *p0;
      gadget::AnalogInterface* obj = new gadget::AnalogInterface(gadget::AnalogInterface(marshal_p0));
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::AnalogInterface* gadget_DeviceInterface_gadget_AnalogProxy__DeviceInterface__0()
   {
      gadget::AnalogInterface* obj = new gadget::AnalogInterface(gadget::AnalogInterface());
      return obj;
   }

   SHARPPY_API void delete_gadget_AnalogInterface(gadget::AnalogInterface* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::DeviceInterface<gadget::AnalogProxy>::getProxy()
   SHARPPY_API gadget::AnalogProxy* gadget_DeviceInterface_gadget_AnalogProxy__getProxy__0(gadget::AnalogInterface* self_ptr)
   {
      gadget::AnalogProxy* result;
      result = (*self_ptr).getProxy();
      return result;
   }


   // Wrapper for virtual method gadget::DeviceInterface<gadget::AnalogProxy>::refresh()
   SHARPPY_API void gadget_DeviceInterface_gadget_AnalogProxy__refresh__0(gadget::AnalogInterface* self_ptr)
   {
      (*self_ptr).refresh();
   }


} // extern "C" for gadget::AnalogInterface


