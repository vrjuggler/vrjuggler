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
#include <gmtl/AxisAngle.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::AxisAnglef* gmtl_AxisAngle_float__AxisAngle__()
   {
      gmtl::AxisAnglef* obj = new gmtl::AxisAnglef();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AxisAnglef* gmtl_AxisAngle_float__AxisAngle__gmtl_AxisAnglef(const gmtl::AxisAnglef* p0)
   {
      const gmtl::AxisAnglef& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::AxisAnglef* obj = new gmtl::AxisAnglef(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AxisAnglef* gmtl_AxisAngle_float__AxisAngle__float_float_float_float(const float& p0, const float& p1, const float& p2, const float& p3)
   {
      gmtl::AxisAnglef* obj = new gmtl::AxisAnglef(p0, p1, p2, p3);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AxisAnglef* gmtl_AxisAngle_float__AxisAngle__float_gmtl_Vec3f(const float& p0, const gmtl::Vec3f* p1)
   {
      const gmtl::Vec3f& marshal_p1 = *p1; // Pre-call marshaling for p1
      gmtl::AxisAnglef* obj = new gmtl::AxisAnglef(p0, marshal_p1);
      ; // Post-call marshaling for p1
      return obj;
   }

   SHARPPY_API void delete_gmtl_AxisAnglef(gmtl::AxisAnglef* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::AxisAngle<float>::set()
   SHARPPY_API void gmtl_AxisAngle_float__set__float_float_float_float4(gmtl::AxisAnglef* self_, const float& p0, const float& p1, const float& p2, const float& p3)
   {
      self_->set(p0, p1, p2, p3);
   }


   // Wrapper for non-virtual method gmtl::AxisAngle<float>::set()
   SHARPPY_API void gmtl_AxisAngle_float__set__float_gmtl_Vec3f2(gmtl::AxisAnglef* self_, const float& p0, const gmtl::Vec3f* p1)
   {
      const gmtl::Vec3f& marshal_p1 = *p1;
      self_->set(p0, marshal_p1);
   }


   // Wrapper for non-virtual method gmtl::AxisAngle<float>::setAxis()
   SHARPPY_API void gmtl_AxisAngle_float__setAxis__gmtl_Vec3f1(gmtl::AxisAnglef* self_, const gmtl::Vec3f* p0)
   {
      const gmtl::Vec3f& marshal_p0 = *p0;
      self_->setAxis(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::AxisAngle<float>::setAngle()
   SHARPPY_API void gmtl_AxisAngle_float__setAngle__float1(gmtl::AxisAnglef* self_, const float& p0)
   {
      self_->setAngle(p0);
   }


   // Wrapper for non-virtual method gmtl::AxisAngle<float>::getAxis()
   SHARPPY_API gmtl::Vec3f* gmtl_AxisAngle_float__getAxis__0(gmtl::AxisAnglef* self_)
   {
      gmtl::Vec3f* result;
      result = new gmtl::Vec3f(self_->getAxis());
      return result;
   }


   // Wrapper for non-virtual method gmtl::AxisAngle<float>::getAngle()
   SHARPPY_API  float gmtl_AxisAngle_float__getAngle__0(gmtl::AxisAnglef* self_)
   {
       float result;
      result = self_->getAngle();
      return result;
   }


} // extern "C" for gmtl::AxisAnglef


