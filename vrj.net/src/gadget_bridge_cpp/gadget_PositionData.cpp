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

// Generated from Revision: 1.58 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/PositionData.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget::PositionData* gadget_PositionData_PositionData__gadget_PositionData(const gadget::PositionData* p0)
   {
      const gadget::PositionData& marshal_p0 = *p0; // Pre-call marshaling for p0
      gadget::PositionData* obj = new gadget::PositionData(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::PositionData* gadget_PositionData_PositionData__()
   {
      gadget::PositionData* obj = new gadget::PositionData();
      return obj;
   }

   SHARPPY_API void delete_gadget_PositionData(gadget::PositionData* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::PositionData::getPosition()
   SHARPPY_API gmtl::Matrix44f* gadget_PositionData_getPosition__(gadget::PositionData* self_)
   {
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(self_->getPosition());

      return result;
   }

   // Wrapper for non-virtual method gadget::PositionData::setPosition()
   SHARPPY_API void gadget_PositionData_setPosition__gmtl_Matrix44f(gadget::PositionData* self_, gmtl::Matrix44f p0)
   {
      self_->setPosition(p0);
   }






} // extern "C" for gadget::PositionData


