// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/AnalogData.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget::AnalogData* gadget_AnalogData_AnalogData__gadget_AnalogData1(const gadget::AnalogData* p0)
   {
      const gadget::AnalogData& marshal_p0 = *p0;
      gadget::AnalogData* obj = new gadget::AnalogData(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::AnalogData* gadget_AnalogData_AnalogData__0()
   {
      gadget::AnalogData* obj = new gadget::AnalogData();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::AnalogData* gadget_AnalogData_AnalogData__float1(float p0)
   {
      gadget::AnalogData* obj = new gadget::AnalogData(p0);
      return obj;
   }

   SHARPPY_API void delete_gadget_AnalogData(gadget::AnalogData* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::AnalogData::getAnalog()
   SHARPPY_API float gadget_AnalogData_getAnalog__0(gadget::AnalogData* self_)
   {
      float result;
      result = self_->getAnalog();
      return result;
   }


   // Wrapper for non-virtual method gadget::AnalogData::setAnalog()
   SHARPPY_API void gadget_AnalogData_setAnalog__float1(gadget::AnalogData* self_, const float p0)
   {
      self_->setAnalog(p0);
   }


} // extern "C" for gadget::AnalogData


