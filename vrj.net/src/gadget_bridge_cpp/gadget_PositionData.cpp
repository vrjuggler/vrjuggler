// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/PositionData.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget::PositionData* gadget_PositionData_PositionData__gadget_PositionData1(const gadget::PositionData* p0)
   {
      const gadget::PositionData& marshal_p0 = *p0;
      gadget::PositionData* obj = new gadget::PositionData(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::PositionData* gadget_PositionData_PositionData__0()
   {
      gadget::PositionData* obj = new gadget::PositionData();
      return obj;
   }

   SHARPPY_API void delete_gadget_PositionData(gadget::PositionData* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::PositionData::getPosition()
   SHARPPY_API gmtl::Matrix44f* gadget_PositionData_getPosition__0(gadget::PositionData* self_)
   {
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(self_->getPosition());
      return result;
   }


   // Wrapper for non-virtual method gadget::PositionData::setPosition()
   SHARPPY_API void gadget_PositionData_setPosition__gmtl_Matrix44f1(gadget::PositionData* self_, gmtl::Matrix44f p0)
   {
      self_->setPosition(p0);
   }


} // extern "C" for gadget::PositionData


