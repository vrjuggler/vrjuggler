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

// Generated from $Revision$ of $RCSfile$



#include "sharppy.h"
#include <gadget/Type/InputData.h>

extern "C"
{
   // Copy constructor wrapper.
   SHARPPY_API gadget::InputData* gadget_InputData_InputData__gadget_InputData(const gadget::InputData* p)
   {
      gadget::InputData* obj = new gadget::InputData(*p);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::InputData* gadget_InputData_InputData__()
   {
      gadget::InputData* obj = new gadget::InputData();
      return obj;
   }

   SHARPPY_API void delete_gadget_InputData(gadget::InputData* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::InputData::setTime()
   SHARPPY_API void gadget_InputData_setTime__(gadget::InputData* self_)
   {
      self_->setTime();
   }

   // Wrapper for non-virtual method gadget::InputData::setTime()
   SHARPPY_API void gadget_InputData_setTime__vpr_Interval(gadget::InputData* self_, const vpr::Interval* p0)
   {
      const vpr::Interval& marshal_p0 = *p0; // Pre-call marshaling for p0
      self_->setTime(marshal_p0);
      ; // Post-call marshaling for p0
   }

   // Wrapper for non-virtual method gadget::InputData::getTime()
   SHARPPY_API vpr::Interval* gadget_InputData_getTime__(gadget::InputData* self_)
   {
      vpr::Interval* result;
      result = new vpr::Interval(self_->getTime());

      return result;
   }





} // extern "C" for gadget::InputData


