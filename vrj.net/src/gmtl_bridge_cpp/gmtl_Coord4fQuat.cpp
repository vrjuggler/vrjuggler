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
#include <gmtl/Vec.h>
#include <gmtl/Quat.h>
#include <gmtl/Coord.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fQuat* gmtl_Coord_gmtl_Vec_float_4__gmtl_Quat_float____Coord__()
   {
      gmtl::Coord4fQuat* obj = new gmtl::Coord4fQuat();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fQuat* gmtl_Coord_gmtl_Vec_float_4__gmtl_Quat_float____Coord__gmtl_Coord4fQuat(const gmtl::Coord4fQuat* p0)
   {
      const gmtl::Coord4fQuat& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Coord4fQuat* obj = new gmtl::Coord4fQuat(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fQuat* gmtl_Coord_gmtl_Vec_float_4__gmtl_Quat_float____Coord__gmtl_Vec4f_gmtl_Quatf(const gmtl::Vec4f* p0, const gmtl::Quatf* p1)
   {
      const gmtl::Vec4f& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Quatf& marshal_p1 = *p1; // Pre-call marshaling for p1
      gmtl::Coord4fQuat* obj = new gmtl::Coord4fQuat(marshal_p0, marshal_p1);
      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      return obj;
   }

/*
   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fQuat* gmtl_Coord_gmtl_Vec_float_4__gmtl_Quat_float____Coord__float_float_float_float_float_float(float p0, float p1, float p2, float p3, float p4, float p5)
   {
      gmtl::Coord4fQuat* obj = new gmtl::Coord4fQuat(p0, p1, p2, p3, p4, p5);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fQuat* gmtl_Coord_gmtl_Vec_float_4__gmtl_Quat_float____Coord__float_float_float_float_float_float_float(float p0, float p1, float p2, float p3, float p4, float p5, float p6)
   {
      gmtl::Coord4fQuat* obj = new gmtl::Coord4fQuat(p0, p1, p2, p3, p4, p5, p6);
      return obj;
   }
*/

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4fQuat* gmtl_Coord_gmtl_Vec_float_4__gmtl_Quat_float____Coord__float_float_float_float_float_float_float_float(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7)
   {
      gmtl::Coord4fQuat* obj = new gmtl::Coord4fQuat(p0, p1, p2, p3, p4, p5, p6, p7);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Coord4fQuat(gmtl::Coord4fQuat* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<float,4>,gmtl::Quat<float> >::getPos()
   SHARPPY_API const gmtl::Vec4f* gmtl_Coord_gmtl_Vec_float_4__gmtl_Quat_float____getPos__0(gmtl::Coord4fQuat* self_)
   {
      const gmtl::Vec4f* result;
      result = new const gmtl::Vec4f(self_->getPos());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<float,4>,gmtl::Quat<float> >::getRot()
   SHARPPY_API const gmtl::Quatf* gmtl_Coord_gmtl_Vec_float_4__gmtl_Quat_float____getRot__0(gmtl::Coord4fQuat* self_)
   {
      const gmtl::Quatf* result;
      result = new const gmtl::Quatf(self_->getRot());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<float,4>,gmtl::Quat<float> >::pos()
   SHARPPY_API gmtl::Vec4f* gmtl_Coord_gmtl_Vec_float_4__gmtl_Quat_float____pos__0(gmtl::Coord4fQuat* self_)
   {
      gmtl::Vec4f* result;
      result = new gmtl::Vec4f(self_->pos());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<float,4>,gmtl::Quat<float> >::rot()
   SHARPPY_API gmtl::Quatf* gmtl_Coord_gmtl_Vec_float_4__gmtl_Quat_float____rot__0(gmtl::Coord4fQuat* self_)
   {
      gmtl::Quatf* result;
      result = new gmtl::Quatf(self_->rot());
      return result;
   }


} // extern "C" for gmtl::Coord4fQuat


