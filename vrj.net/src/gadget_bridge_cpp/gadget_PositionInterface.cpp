// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

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


