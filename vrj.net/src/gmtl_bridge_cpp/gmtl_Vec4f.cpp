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

// Generated from Revision: 1.62 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Vec.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4f* gmtl_Vec_float_4__Vec__()
   {
      gmtl::Vec4f* obj = new gmtl::Vec4f();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4f* gmtl_Vec_float_4__Vec__gmtl_Vec4f(const gmtl::Vec4f* p0)
   {
      const gmtl::Vec4f& marshal_p0 = *p0;
      gmtl::Vec4f* obj = new gmtl::Vec4f(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4f* gmtl_Vec_float_4__Vec__gmtl_VecBase_float_4(const gmtl::VecBase<float,4>* p0)
   {
      const gmtl::VecBase<float,4>& marshal_p0 = *p0;
      gmtl::Vec4f* obj = new gmtl::Vec4f(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4f* gmtl_Vec_float_4__Vec__float_float(const float p0, const float p1)
   {
      gmtl::Vec4f* obj = new gmtl::Vec4f(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4f* gmtl_Vec_float_4__Vec__float_float_float(const float p0, const float p1, const float p2)
   {
      gmtl::Vec4f* obj = new gmtl::Vec4f(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4f* gmtl_Vec_float_4__Vec__float_float_float_float(const float p0, const float p1, const float p2, const float p3)
   {
      gmtl::Vec4f* obj = new gmtl::Vec4f(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Vec4f(gmtl::Vec4f* self_)
   {
      delete self_;
   }

} // extern "C" for gmtl::Vec4f


