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

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Rayf* gmtl_Ray_float__Ray__()
   {
      gmtl::Rayf* obj = new gmtl::Rayf();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Rayf* gmtl_Ray_float__Ray__gmtl_Point3f_gmtl_Vec3f(const gmtl::Point3f* p0, const gmtl::Vec3f* p1)
   {
      const gmtl::Point3f& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Vec3f& marshal_p1 = *p1; // Pre-call marshaling for p1
      gmtl::Rayf* obj = new gmtl::Rayf(marshal_p0, marshal_p1);
      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Rayf* gmtl_Ray_float__Ray__gmtl_Rayf(const gmtl::Rayf* p0)
   {
      const gmtl::Rayf& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Rayf* obj = new gmtl::Rayf(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   SHARPPY_API void delete_gmtl_Rayf(gmtl::Rayf* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Ray<float>::getOrigin()
   SHARPPY_API const gmtl::Point3f* gmtl_Ray_float__getOrigin__(gmtl::Rayf* self_)
   {
      const gmtl::Point3f* result;
      result = new const gmtl::Point3f(self_->getOrigin());

      return result;
   }

   // Wrapper for non-virtual method gmtl::Ray<float>::setOrigin()
   SHARPPY_API void gmtl_Ray_float__setOrigin__gmtl_Point3f(gmtl::Rayf* self_, const gmtl::Point3f* p0)
   {
      const gmtl::Point3f& marshal_p0 = *p0; // Pre-call marshaling for p0
      self_->setOrigin(marshal_p0);
      ; // Post-call marshaling for p0
   }

   // Wrapper for non-virtual method gmtl::Ray<float>::getDir()
   SHARPPY_API const gmtl::Vec3f* gmtl_Ray_float__getDir__(gmtl::Rayf* self_)
   {
      const gmtl::Vec3f* result;
      result = new const gmtl::Vec3f(self_->getDir());

      return result;
   }

   // Wrapper for non-virtual method gmtl::Ray<float>::setDir()
   SHARPPY_API void gmtl_Ray_float__setDir__gmtl_Vec3f(gmtl::Rayf* self_, const gmtl::Vec3f* p0)
   {
      const gmtl::Vec3f& marshal_p0 = *p0; // Pre-call marshaling for p0
      self_->setDir(marshal_p0);
      ; // Post-call marshaling for p0
   }






} // extern "C" for gmtl::Rayf


