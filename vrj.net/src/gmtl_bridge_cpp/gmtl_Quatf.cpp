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
#include <gmtl/Quat.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Quatf* gmtl_Quat_float__Quat__0()
   {
      gmtl::Quatf* obj = new gmtl::Quatf();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Quatf* gmtl_Quat_float__Quat__float_float_float_float4(const float p0, const float p1, const float p2, const float p3)
   {
      gmtl::Quatf* obj = new gmtl::Quatf(p0, p1, p2, p3);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Quatf* gmtl_Quat_float__Quat__gmtl_Quatf1(const gmtl::Quatf* p0)
   {
      const gmtl::Quatf& marshal_p0 = *p0;
      gmtl::Quatf* obj = new gmtl::Quatf(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Quatf(gmtl::Quatf* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Quat<float>::set()
   SHARPPY_API void gmtl_Quat_float__set__float_float_float_float4(gmtl::Quatf* self_, const float p0, const float p1, const float p2, const float p3)
   {
      self_->set(p0, p1, p2, p3);
   }


   // Wrapper for non-virtual method gmtl::Quat<float>::get()
   SHARPPY_API void gmtl_Quat_float__get__float_float_float_float4(gmtl::Quatf* self_, float& p0, float& p1, float& p2, float& p3)
   {
      self_->get(p0, p1, p2, p3);
   }

/*
   // Wrapper for non-virtual method gmtl::Quat<float>::getData()
   SHARPPY_API float* gmtl_Quat_float__getData__0(gmtl::Quatf* self_)
   {
      float* result;
      result = self_->getData();
      return result;
   }
*/

} // extern "C" for gmtl::Quatf


