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


