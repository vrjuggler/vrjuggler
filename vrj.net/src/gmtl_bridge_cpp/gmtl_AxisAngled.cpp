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
   SHARPPY_API gmtl::AxisAngled* gmtl_AxisAngle_double__AxisAngle__()
   {
      gmtl::AxisAngled* obj = new gmtl::AxisAngled();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AxisAngled* gmtl_AxisAngle_double__AxisAngle__gmtl_AxisAngled(const gmtl::AxisAngled* p0)
   {
      const gmtl::AxisAngled& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::AxisAngled* obj = new gmtl::AxisAngled(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AxisAngled* gmtl_AxisAngle_double__AxisAngle__double_double_double_double(const double p0, const double p1, const double p2, const double p3)
   {
      gmtl::AxisAngled* obj = new gmtl::AxisAngled(p0, p1, p2, p3);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AxisAngled* gmtl_AxisAngle_double__AxisAngle__double_gmtl_Vec3d(const double p0, const gmtl::Vec3d* p1)
   {
      const gmtl::Vec3d& marshal_p1 = *p1; // Pre-call marshaling for p1
      gmtl::AxisAngled* obj = new gmtl::AxisAngled(p0, marshal_p1);
      ; // Post-call marshaling for p1
      return obj;
   }

   SHARPPY_API void delete_gmtl_AxisAngled(gmtl::AxisAngled* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::AxisAngle<double>::set()
   SHARPPY_API void gmtl_AxisAngle_double__set__double_double_double_double4(gmtl::AxisAngled* self_, const double p0, const double p1, const double p2, const double p3)
   {
      self_->set(p0, p1, p2, p3);
   }


   // Wrapper for non-virtual method gmtl::AxisAngle<double>::set()
   SHARPPY_API void gmtl_AxisAngle_double__set__double_gmtl_Vec3d2(gmtl::AxisAngled* self_, const double p0, const gmtl::Vec3d* p1)
   {
      const gmtl::Vec3d& marshal_p1 = *p1;
      self_->set(p0, marshal_p1);
   }


   // Wrapper for non-virtual method gmtl::AxisAngle<double>::setAxis()
   SHARPPY_API void gmtl_AxisAngle_double__setAxis__gmtl_Vec3d1(gmtl::AxisAngled* self_, const gmtl::Vec3d* p0)
   {
      const gmtl::Vec3d& marshal_p0 = *p0;
      self_->setAxis(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::AxisAngle<double>::setAngle()
   SHARPPY_API void gmtl_AxisAngle_double__setAngle__double1(gmtl::AxisAngled* self_, const double p0)
   {
      self_->setAngle(p0);
   }


   // Wrapper for non-virtual method gmtl::AxisAngle<double>::getAxis()
   SHARPPY_API gmtl::Vec3d* gmtl_AxisAngle_double__getAxis__0(gmtl::AxisAngled* self_)
   {
      gmtl::Vec3d* result;
      result = new gmtl::Vec3d(self_->getAxis());
      return result;
   }


   // Wrapper for non-virtual method gmtl::AxisAngle<double>::getAngle()
   SHARPPY_API  double gmtl_AxisAngle_double__getAngle__0(gmtl::AxisAngled* self_)
   {
       double result;
      result = self_->getAngle();
      return result;
   }


} // extern "C" for gmtl::AxisAngled


