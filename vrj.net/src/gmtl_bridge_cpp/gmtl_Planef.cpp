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
#include <gmtl/Plane.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Planef* gmtl_Plane_float__Plane__()
   {
      gmtl::Planef* obj = new gmtl::Planef();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planef* gmtl_Plane_float__Plane__gmtl_Point3f_gmtl_Point3f_gmtl_Point3f(const gmtl::Point3f* p0, const gmtl::Point3f* p1, const gmtl::Point3f* p2)
   {
      const gmtl::Point3f& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Point3f& marshal_p1 = *p1; // Pre-call marshaling for p1
      const gmtl::Point3f& marshal_p2 = *p2; // Pre-call marshaling for p2
      gmtl::Planef* obj = new gmtl::Planef(marshal_p0, marshal_p1, marshal_p2);
      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      ; // Post-call marshaling for p2
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planef* gmtl_Plane_float__Plane__gmtl_Vec3f_gmtl_Point3f(const gmtl::Vec3f* p0, const gmtl::Point3f* p1)
   {
      const gmtl::Vec3f& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Point3f& marshal_p1 = *p1; // Pre-call marshaling for p1
      gmtl::Planef* obj = new gmtl::Planef(marshal_p0, marshal_p1);
      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planef* gmtl_Plane_float__Plane__gmtl_Vec3f_float(const gmtl::Vec3f* p0, const float p1)
   {
      const gmtl::Vec3f& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Planef* obj = new gmtl::Planef(marshal_p0, p1);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planef* gmtl_Plane_float__Plane__gmtl_Planef(const gmtl::Planef* p0)
   {
      const gmtl::Planef& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Planef* obj = new gmtl::Planef(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   SHARPPY_API void delete_gmtl_Planef(gmtl::Planef* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Plane<float>::getNormal()
   SHARPPY_API const gmtl::Vec3f* gmtl_Plane_float__getNormal__0(gmtl::Planef* self_)
   {
      const gmtl::Vec3f* result;
      result = new const gmtl::Vec3f(self_->getNormal());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Plane<float>::setNormal()
   SHARPPY_API void gmtl_Plane_float__setNormal__gmtl_Vec3f1(gmtl::Planef* self_, const gmtl::Vec3f* p0)
   {
      const gmtl::Vec3f& marshal_p0 = *p0;
      self_->setNormal(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::Plane<float>::getOffset()
   SHARPPY_API  float gmtl_Plane_float__getOffset__0(gmtl::Planef* self_)
   {
       float result;
      result = self_->getOffset();
      return result;
   }


   // Wrapper for non-virtual method gmtl::Plane<float>::setOffset()
   SHARPPY_API void gmtl_Plane_float__setOffset__float1(gmtl::Planef* self_, const float p0)
   {
      self_->setOffset(p0);
   }


} // extern "C" for gmtl::Planef


