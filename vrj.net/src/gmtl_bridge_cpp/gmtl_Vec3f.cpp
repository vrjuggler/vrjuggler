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

// Generated from Revision: 1.60 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Vec.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3f* gmtl_Vec_float_3__Vec__()
   {
      gmtl::Vec3f* obj = new gmtl::Vec3f();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3f* gmtl_Vec_float_3__Vec__gmtl_Vec3f(const gmtl::Vec3f* p0)
   {
      const gmtl::Vec3f& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Vec3f* obj = new gmtl::Vec3f(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3f* gmtl_Vec_float_3__Vec__gmtl_VecBase_float_3(const gmtl::VecBase<float,3>* p0)
   {
      const gmtl::VecBase<float,3>& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Vec3f* obj = new gmtl::Vec3f(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3f* gmtl_Vec_float_3__Vec__float_float(const float p0, const float p1)
   {
      gmtl::Vec3f* obj = new gmtl::Vec3f(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3f* gmtl_Vec_float_3__Vec__float_float_float(const float p0, const float p1, const float p2)
   {
      gmtl::Vec3f* obj = new gmtl::Vec3f(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3f* gmtl_Vec_float_3__Vec__float_float_float_float(const float p0, const float p1, const float p2, const float p3)
   {
      gmtl::Vec3f* obj = new gmtl::Vec3f(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Vec3f(gmtl::Vec3f* self_)
   {
      delete self_;
   }

} // extern "C" for gmtl::Vec3f


