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
#include <gmtl/Matrix.h>
#include <gmtl/VecBase.h>
#include <gmtl/Vec.h>
#include <gmtl/Quat.h>
#include <gmtl/Point.h>
#include <gmtl/EulerAngle.h>
#include <gmtl/AxisAngle.h>
#include <gmtl/Sphere.h>
#include <gmtl/AABox.h>
#include <gmtl/Ray.h>
#include <gmtl/LineSeg.h>
#include <gmtl/Plane.h>
#include <gmtl/Tri.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Trii* gmtl_Tri_int__Tri__()
   {
      gmtl::Trii* obj = new gmtl::Trii();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Trii* gmtl_Tri_int__Tri__gmtl_Point3i_gmtl_Point3i_gmtl_Point3i(const gmtl::Point3i* p0, const gmtl::Point3i* p1, const gmtl::Point3i* p2)
   {
      const gmtl::Point3i& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Point3i& marshal_p1 = *p1; // Pre-call marshaling for p1
      const gmtl::Point3i& marshal_p2 = *p2; // Pre-call marshaling for p2
      gmtl::Trii* obj = new gmtl::Trii(marshal_p0, marshal_p1, marshal_p2);
      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      ; // Post-call marshaling for p2
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Trii* gmtl_Tri_int__Tri__gmtl_Trii(const gmtl::Trii* p0)
   {
      const gmtl::Trii& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Trii* obj = new gmtl::Trii(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   SHARPPY_API void delete_gmtl_Trii(gmtl::Trii* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Tri<int>::edge()
   SHARPPY_API gmtl::Vec3i* gmtl_Tri_int__edge__int1(gmtl::Trii* self_, int p0)
   {
      gmtl::Vec3i* result;
      result = new gmtl::Vec3i(self_->edge(p0));
      return result;
   }


   // Wrapper for non-virtual method gmtl::Tri<int>::edge()
   SHARPPY_API gmtl::Vec3i* gmtl_Tri_int__edge__int_int2(gmtl::Trii* self_, int p0, int p1)
   {
      gmtl::Vec3i* result;
      result = new gmtl::Vec3i(self_->edge(p0, p1));
      return result;
   }


   // Wrapper for non-virtual method gmtl::Tri<int>::set()
   SHARPPY_API void gmtl_Tri_int__set__gmtl_Point3i_gmtl_Point3i_gmtl_Point3i3(gmtl::Trii* self_, const gmtl::Point3i* p0, const gmtl::Point3i* p1, const gmtl::Point3i* p2)
   {
      const gmtl::Point3i& marshal_p0 = *p0;
      const gmtl::Point3i& marshal_p1 = *p1;
      const gmtl::Point3i& marshal_p2 = *p2;
      self_->set(marshal_p0, marshal_p1, marshal_p2);
   }


} // extern "C" for gmtl::Trii


