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

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Vec.h>
#include <gmtl/AxisAngle.h>
#include <gmtl/Coord.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fAxisAngle* gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__0()
   {
      gmtl::Coord4fAxisAngle* obj = new gmtl::Coord4fAxisAngle();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fAxisAngle* gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__gmtl_Coord4fAxisAngle1(const gmtl::Coord4fAxisAngle* p0)
   {
      const gmtl::Coord4fAxisAngle& marshal_p0 = *p0;
      gmtl::Coord4fAxisAngle* obj = new gmtl::Coord4fAxisAngle(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fAxisAngle* gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__gmtl_Vec4f_gmtl_AxisAnglef2(const gmtl::Vec4f* p0, const gmtl::AxisAnglef* p1)
   {
      const gmtl::Vec4f& marshal_p0 = *p0;
      const gmtl::AxisAnglef& marshal_p1 = *p1;
      gmtl::Coord4fAxisAngle* obj = new gmtl::Coord4fAxisAngle(marshal_p0, marshal_p1);
      return obj;
   }

/*
   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fAxisAngle* gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__float_float_float_float_float_float6(float p0, float p1, float p2, float p3, float p4, float p5)
   {
      gmtl::Coord4fAxisAngle* obj = new gmtl::Coord4fAxisAngle(p0, p1, p2, p3, p4, p5);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fAxisAngle* gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__float_float_float_float_float_float_float7(float p0, float p1, float p2, float p3, float p4, float p5, float p6)
   {
      gmtl::Coord4fAxisAngle* obj = new gmtl::Coord4fAxisAngle(p0, p1, p2, p3, p4, p5, p6);
      return obj;
   }
*/

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fAxisAngle* gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__float_float_float_float_float_float_float_float8(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7)
   {
      gmtl::Coord4fAxisAngle* obj = new gmtl::Coord4fAxisAngle(p0, p1, p2, p3, p4, p5, p6, p7);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Coord4fAxisAngle(gmtl::Coord4fAxisAngle* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<float,4>,gmtl::AxisAngle<float> >::getPos()
   SHARPPY_API gmtl::Vec4f* gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____getPos__0(gmtl::Coord4fAxisAngle* self_)
   {
      gmtl::Vec4f* result;
      result = new gmtl::Vec4f(self_->getPos());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<float,4>,gmtl::AxisAngle<float> >::getRot()
   SHARPPY_API gmtl::AxisAnglef* gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____getRot__0(gmtl::Coord4fAxisAngle* self_)
   {
      gmtl::AxisAnglef* result;
      result = new gmtl::AxisAnglef(self_->getRot());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<float,4>,gmtl::AxisAngle<float> >::pos()
   SHARPPY_API gmtl::Vec4f* gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____pos__0(gmtl::Coord4fAxisAngle* self_)
   {
      gmtl::Vec4f* result;
      result = new gmtl::Vec4f(self_->pos());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<float,4>,gmtl::AxisAngle<float> >::rot()
   SHARPPY_API gmtl::AxisAnglef* gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____rot__0(gmtl::Coord4fAxisAngle* self_)
   {
      gmtl::AxisAnglef* result;
      result = new gmtl::AxisAnglef(self_->rot());
      return result;
   }


} // extern "C" for gmtl::Coord4fAxisAngle


