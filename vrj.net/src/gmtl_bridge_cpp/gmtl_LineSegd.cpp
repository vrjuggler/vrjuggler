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
#include <gmtl/LineSeg.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegd* gmtl_LineSeg_double__LineSeg__()
   {
      gmtl::LineSegd* obj = new gmtl::LineSegd();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegd* gmtl_LineSeg_double__LineSeg__gmtl_Point_double_3_gmtl_Vec3d(const gmtl::Point<double,3>* p0, const gmtl::Vec3d* p1)
   {
      const gmtl::Point<double,3>& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Vec3d& marshal_p1 = *p1; // Pre-call marshaling for p1
      gmtl::LineSegd* obj = new gmtl::LineSegd(marshal_p0, marshal_p1);
      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegd* gmtl_LineSeg_double__LineSeg__gmtl_LineSegd(const gmtl::LineSegd* p0)
   {
      const gmtl::LineSegd& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::LineSegd* obj = new gmtl::LineSegd(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegd* gmtl_LineSeg_double__LineSeg__gmtl_Point_double_3_gmtl_Point_double_3(const gmtl::Point<double,3>* p0, const gmtl::Point<double,3>* p1)
   {
      const gmtl::Point<double,3>& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Point<double,3>& marshal_p1 = *p1; // Pre-call marshaling for p1
      gmtl::LineSegd* obj = new gmtl::LineSegd(marshal_p0, marshal_p1);
      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      return obj;
   }

   SHARPPY_API void delete_gmtl_LineSegd(gmtl::LineSegd* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::LineSeg<double>::getLength()
   SHARPPY_API double gmtl_LineSeg_double__getLength__(gmtl::LineSegd* self_)
   {
      double result;
      result = self_->getLength();

      return result;
   }






} // extern "C" for gmtl::LineSegd


