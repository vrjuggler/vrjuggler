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

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Planed* gmtl_Plane_double__Plane__()
   {
      gmtl::Planed* obj = new gmtl::Planed();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planed* gmtl_Plane_double__Plane__gmtl_Point3d_gmtl_Point3d_gmtl_Point3d(const gmtl::Point3d* p0, const gmtl::Point3d* p1, const gmtl::Point3d* p2)
   {
      const gmtl::Point3d& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Point3d& marshal_p1 = *p1; // Pre-call marshaling for p1
      const gmtl::Point3d& marshal_p2 = *p2; // Pre-call marshaling for p2
      gmtl::Planed* obj = new gmtl::Planed(marshal_p0, marshal_p1, marshal_p2);
      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      ; // Post-call marshaling for p2
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planed* gmtl_Plane_double__Plane__gmtl_Vec3d_gmtl_Point3d(const gmtl::Vec3d* p0, const gmtl::Point3d* p1)
   {
      const gmtl::Vec3d& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Point3d& marshal_p1 = *p1; // Pre-call marshaling for p1
      gmtl::Planed* obj = new gmtl::Planed(marshal_p0, marshal_p1);
      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planed* gmtl_Plane_double__Plane__gmtl_Vec3d_double(const gmtl::Vec3d* p0, const double& p1)
   {
      const gmtl::Vec3d& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Planed* obj = new gmtl::Planed(marshal_p0, p1);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planed* gmtl_Plane_double__Plane__gmtl_Planed(const gmtl::Planed* p0)
   {
      const gmtl::Planed& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Planed* obj = new gmtl::Planed(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   SHARPPY_API void delete_gmtl_Planed(gmtl::Planed* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Plane<double>::getNormal()
   SHARPPY_API const gmtl::Vec3d* gmtl_Plane_double__getNormal__0(gmtl::Planed* self_)
   {
      const gmtl::Vec3d* result;
      result = new const gmtl::Vec3d(self_->getNormal());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Plane<double>::setNormal()
   SHARPPY_API void gmtl_Plane_double__setNormal__gmtl_Vec3d1(gmtl::Planed* self_, const gmtl::Vec3d* p0)
   {
      const gmtl::Vec3d& marshal_p0 = *p0;
      self_->setNormal(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::Plane<double>::getOffset()
   SHARPPY_API  double gmtl_Plane_double__getOffset__0(gmtl::Planed* self_)
   {
       double result;
      result = self_->getOffset();
      return result;
   }


   // Wrapper for non-virtual method gmtl::Plane<double>::setOffset()
   SHARPPY_API void gmtl_Plane_double__setOffset__double1(gmtl::Planed* self_, const double& p0)
   {
      self_->setOffset(p0);
   }


} // extern "C" for gmtl::Planed


