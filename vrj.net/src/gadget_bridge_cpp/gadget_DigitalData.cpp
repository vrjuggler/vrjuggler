// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/DigitalData.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget::DigitalData* gadget_DigitalData_DigitalData__gadget_DigitalData1(const gadget::DigitalData* p0)
   {
      const gadget::DigitalData& marshal_p0 = *p0;
      gadget::DigitalData* obj = new gadget::DigitalData(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::DigitalData* gadget_DigitalData_DigitalData__0()
   {
      gadget::DigitalData* obj = new gadget::DigitalData();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::DigitalData* gadget_DigitalData_DigitalData__int1(const int p0)
   {
      gadget::DigitalData* obj = new gadget::DigitalData(p0);
      return obj;
   }

   SHARPPY_API void delete_gadget_DigitalData(gadget::DigitalData* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::DigitalData::getDigital()
   SHARPPY_API int gadget_DigitalData_getDigital__0(gadget::DigitalData* self_)
   {
      int result;
      result = self_->getDigital();
      return result;
   }


   // Wrapper for non-virtual method gadget::DigitalData::setDigital()
   SHARPPY_API void gadget_DigitalData_setDigital__int1(gadget::DigitalData* self_, const int p0)
   {
      self_->setDigital(p0);
   }


} // extern "C" for gadget::DigitalData


