// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Vec.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3f* gmtl_Vec_float_3__Vec__0()
   {
      gmtl::Vec3f* obj = new gmtl::Vec3f();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3f* gmtl_Vec_float_3__Vec__gmtl_Vec3f1(const gmtl::Vec3f* p0)
   {
      const gmtl::Vec3f& marshal_p0 = *p0;
      gmtl::Vec3f* obj = new gmtl::Vec3f(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3f* gmtl_Vec_float_3__Vec__gmtl_VecBase_float_31(const gmtl::VecBase<float,3>* p0)
   {
      const gmtl::VecBase<float,3>& marshal_p0 = *p0;
      gmtl::Vec3f* obj = new gmtl::Vec3f(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3f* gmtl_Vec_float_3__Vec__float_float2(const float p0, const float p1)
   {
      gmtl::Vec3f* obj = new gmtl::Vec3f(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3f* gmtl_Vec_float_3__Vec__float_float_float3(const float p0, const float p1, const float p2)
   {
      gmtl::Vec3f* obj = new gmtl::Vec3f(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3f* gmtl_Vec_float_3__Vec__float_float_float_float4(const float p0, const float p1, const float p2, const float p3)
   {
      gmtl::Vec3f* obj = new gmtl::Vec3f(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Vec3f(gmtl::Vec3f* self_)
   {
      delete self_;
   }

} // extern "C" for gmtl::Vec3f


