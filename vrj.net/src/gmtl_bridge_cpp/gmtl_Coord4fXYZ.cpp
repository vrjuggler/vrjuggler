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
#include <gmtl/Vec.h>
#include <gmtl/EulerAngle.h>
#include <gmtl/Coord.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fXYZ* gmtl_Coord_gmtl_Vec_float_4__gmtl_EulerAngle_float_gmtl_XYZ____Coord__()
   {
      gmtl::Coord4fXYZ* obj = new gmtl::Coord4fXYZ();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fXYZ* gmtl_Coord_gmtl_Vec_float_4__gmtl_EulerAngle_float_gmtl_XYZ____Coord__gmtl_Coord4fXYZ(const gmtl::Coord4fXYZ* p0)
   {
      const gmtl::Coord4fXYZ& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Coord4fXYZ* obj = new gmtl::Coord4fXYZ(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fXYZ* gmtl_Coord_gmtl_Vec_float_4__gmtl_EulerAngle_float_gmtl_XYZ____Coord__gmtl_Vec4f_gmtl_EulerAngleXYZf(const gmtl::Vec4f* p0, const gmtl::EulerAngleXYZf* p1)
   {
      const gmtl::Vec4f& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::EulerAngleXYZf& marshal_p1 = *p1; // Pre-call marshaling for p1
      gmtl::Coord4fXYZ* obj = new gmtl::Coord4fXYZ(marshal_p0, marshal_p1);
      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      return obj;
   }

/*
   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fXYZ* gmtl_Coord_gmtl_Vec_float_4__gmtl_EulerAngle_float_gmtl_XYZ____Coord__float_float_float_float_float_float(float p0, float p1, float p2, float p3, float p4, float p5)
   {
      gmtl::Coord4fXYZ* obj = new gmtl::Coord4fXYZ(p0, p1, p2, p3, p4, p5);
      return obj;
   }
*/

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fXYZ* gmtl_Coord_gmtl_Vec_float_4__gmtl_EulerAngle_float_gmtl_XYZ____Coord__float_float_float_float_float_float_float(float p0, float p1, float p2, float p3, float p4, float p5, float p6)
   {
      gmtl::Coord4fXYZ* obj = new gmtl::Coord4fXYZ(p0, p1, p2, p3, p4, p5, p6);
      return obj;
   }

/*
   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fXYZ* gmtl_Coord_gmtl_Vec_float_4__gmtl_EulerAngle_float_gmtl_XYZ____Coord__float_float_float_float_float_float_float_float(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7)
   {
      gmtl::Coord4fXYZ* obj = new gmtl::Coord4fXYZ(p0, p1, p2, p3, p4, p5, p6, p7);
      return obj;
   }
*/

   SHARPPY_API void delete_gmtl_Coord4fXYZ(gmtl::Coord4fXYZ* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<float,4>,gmtl::EulerAngle<float,gmtl::XYZ> >::getPos()
   SHARPPY_API const gmtl::Vec4f* gmtl_Coord_gmtl_Vec_float_4__gmtl_EulerAngle_float_gmtl_XYZ____getPos__(gmtl::Coord4fXYZ* self_)
   {
      const gmtl::Vec4f* result;
      result = new const gmtl::Vec4f(self_->getPos());

      return result;
   }

   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<float,4>,gmtl::EulerAngle<float,gmtl::XYZ> >::getRot()
   SHARPPY_API const gmtl::EulerAngleXYZf* gmtl_Coord_gmtl_Vec_float_4__gmtl_EulerAngle_float_gmtl_XYZ____getRot__(gmtl::Coord4fXYZ* self_)
   {
      const gmtl::EulerAngleXYZf* result;
      result = new const gmtl::EulerAngleXYZf(self_->getRot());

      return result;
   }

   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<float,4>,gmtl::EulerAngle<float,gmtl::XYZ> >::pos()
   SHARPPY_API gmtl::Vec4f* gmtl_Coord_gmtl_Vec_float_4__gmtl_EulerAngle_float_gmtl_XYZ____pos__(gmtl::Coord4fXYZ* self_)
   {
      gmtl::Vec4f* result;
      result = new gmtl::Vec4f(self_->pos());

      return result;
   }

   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<float,4>,gmtl::EulerAngle<float,gmtl::XYZ> >::rot()
   SHARPPY_API gmtl::EulerAngleXYZf* gmtl_Coord_gmtl_Vec_float_4__gmtl_EulerAngle_float_gmtl_XYZ____rot__(gmtl::Coord4fXYZ* self_)
   {
      gmtl::EulerAngleXYZf* result;
      result = new gmtl::EulerAngleXYZf(self_->rot());

      return result;
   }






} // extern "C" for gmtl::Coord4fXYZ


