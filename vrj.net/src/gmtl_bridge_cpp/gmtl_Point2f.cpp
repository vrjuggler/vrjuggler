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

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Point2f* gmtl_Point_float_2__Point__()
   {
      gmtl::Point2f* obj = new gmtl::Point2f();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2f* gmtl_Point_float_2__Point__gmtl_Point2f(const gmtl::Point2f* p0)
   {
      const gmtl::Point2f& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Point2f* obj = new gmtl::Point2f(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2f* gmtl_Point_float_2__Point__gmtl_VecBase_float_2(const gmtl::VecBase<float,2>* p0)
   {
      const gmtl::VecBase<float,2>& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Point2f* obj = new gmtl::Point2f(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2f* gmtl_Point_float_2__Point__float_float(const float& p0, const float& p1)
   {
      gmtl::Point2f* obj = new gmtl::Point2f(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2f* gmtl_Point_float_2__Point__float_float_float(const float& p0, const float& p1, const float& p2)
   {
      gmtl::Point2f* obj = new gmtl::Point2f(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2f* gmtl_Point_float_2__Point__float_float_float_float(const float& p0, const float& p1, const float& p2, const float& p3)
   {
      gmtl::Point2f* obj = new gmtl::Point2f(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Point2f(gmtl::Point2f* self_)
   {
      delete self_;
   }






} // extern "C" for gmtl::Point2f


