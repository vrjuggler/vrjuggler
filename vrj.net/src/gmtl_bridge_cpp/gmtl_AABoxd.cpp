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

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::AABoxd* gmtl_AABox_double__AABox__()
   {
      gmtl::AABoxd* obj = new gmtl::AABoxd();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AABoxd* gmtl_AABox_double__AABox__gmtl_Point3d_gmtl_Point3d(const gmtl::Point3d* p0, const gmtl::Point3d* p1)
   {
      const gmtl::Point3d& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Point3d& marshal_p1 = *p1; // Pre-call marshaling for p1
      gmtl::AABoxd* obj = new gmtl::AABoxd(marshal_p0, marshal_p1);
      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AABoxd* gmtl_AABox_double__AABox__gmtl_AABoxd(const gmtl::AABoxd* p0)
   {
      const gmtl::AABoxd& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::AABoxd* obj = new gmtl::AABoxd(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   SHARPPY_API void delete_gmtl_AABoxd(gmtl::AABoxd* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::AABox<double>::getMin()
   SHARPPY_API const gmtl::Point3d* gmtl_AABox_double__getMin__0(gmtl::AABoxd* self_)
   {
      const gmtl::Point3d* result;
      result = new const gmtl::Point3d(self_->getMin());
      return result;
   }


   // Wrapper for non-virtual method gmtl::AABox<double>::getMax()
   SHARPPY_API const gmtl::Point3d* gmtl_AABox_double__getMax__0(gmtl::AABoxd* self_)
   {
      const gmtl::Point3d* result;
      result = new const gmtl::Point3d(self_->getMax());
      return result;
   }


   // Wrapper for non-virtual method gmtl::AABox<double>::isEmpty()
   SHARPPY_API bool gmtl_AABox_double__isEmpty__0(gmtl::AABoxd* self_)
   {
      bool result;
      result = self_->isEmpty();
      return result;
   }


   // Wrapper for non-virtual method gmtl::AABox<double>::setMin()
   SHARPPY_API void gmtl_AABox_double__setMin__gmtl_Point3d1(gmtl::AABoxd* self_, const gmtl::Point3d* p0)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      self_->setMin(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::AABox<double>::setMax()
   SHARPPY_API void gmtl_AABox_double__setMax__gmtl_Point3d1(gmtl::AABoxd* self_, const gmtl::Point3d* p0)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      self_->setMax(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::AABox<double>::setEmpty()
   SHARPPY_API void gmtl_AABox_double__setEmpty__bool1(gmtl::AABoxd* self_, bool p0)
   {
      self_->setEmpty(p0);
   }


} // extern "C" for gmtl::AABoxd


