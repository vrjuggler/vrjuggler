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
#include <gadget/Type/AnalogData.h>

extern "C"
{
   // Copy constructor wrapper.
   SHARPPY_API gadget::AnalogData* gadget_AnalogData_AnalogData__gadget_AnalogData(const gadget::AnalogData* p)
   {
      gadget::AnalogData* obj = new gadget::AnalogData(*p);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::AnalogData* gadget_AnalogData_AnalogData__()
   {
      gadget::AnalogData* obj = new gadget::AnalogData();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::AnalogData* gadget_AnalogData_AnalogData__float(float p0)
   {
      gadget::AnalogData* obj = new gadget::AnalogData(p0);
      return obj;
   }

   SHARPPY_API void delete_gadget_AnalogData(gadget::AnalogData* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::AnalogData::getAnalog()
   SHARPPY_API float gadget_AnalogData_getAnalog__(gadget::AnalogData* self_)
   {
      float result;
      result = self_->getAnalog();

      return result;
   }

   // Wrapper for non-virtual method gadget::AnalogData::setAnalog()
   SHARPPY_API void gadget_AnalogData_setAnalog__float(gadget::AnalogData* self_, const float p0)
   {
      self_->setAnalog(p0);
   }





} // extern "C" for gadget::AnalogData


