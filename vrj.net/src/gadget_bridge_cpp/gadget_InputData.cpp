// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/InputData.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget::InputData* gadget_InputData_InputData__gadget_InputData1(const gadget::InputData* p0)
   {
      const gadget::InputData& marshal_p0 = *p0;
      gadget::InputData* obj = new gadget::InputData(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::InputData* gadget_InputData_InputData__0()
   {
      gadget::InputData* obj = new gadget::InputData();
      return obj;
   }

   SHARPPY_API void delete_gadget_InputData(gadget::InputData* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::InputData::setTime()
   SHARPPY_API void gadget_InputData_setTime__0(gadget::InputData* self_)
   {
      self_->setTime();
   }


   // Wrapper for non-virtual method gadget::InputData::setTime()
   SHARPPY_API void gadget_InputData_setTime__vpr_Interval1(gadget::InputData* self_, const vpr::Interval* p0)
   {
      const vpr::Interval& marshal_p0 = *p0;
      self_->setTime(marshal_p0);
   }


   // Wrapper for non-virtual method gadget::InputData::getTime()
   SHARPPY_API vpr::Interval* gadget_InputData_getTime__0(gadget::InputData* self_)
   {
      vpr::Interval* result;
      result = new vpr::Interval(self_->getTime());
      return result;
   }


} // extern "C" for gadget::InputData


