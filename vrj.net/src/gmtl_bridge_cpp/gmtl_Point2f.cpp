// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Point.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Point2f* gmtl_Point_float_2__Point__0()
   {
      gmtl::Point2f* obj = new gmtl::Point2f();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2f* gmtl_Point_float_2__Point__gmtl_Point2f1(const gmtl::Point2f* p0)
   {
      const gmtl::Point2f& marshal_p0 = *p0;
      gmtl::Point2f* obj = new gmtl::Point2f(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2f* gmtl_Point_float_2__Point__gmtl_VecBase_float_21(const gmtl::VecBase<float,2>* p0)
   {
      const gmtl::VecBase<float,2>& marshal_p0 = *p0;
      gmtl::Point2f* obj = new gmtl::Point2f(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2f* gmtl_Point_float_2__Point__float_float2(const float p0, const float p1)
   {
      gmtl::Point2f* obj = new gmtl::Point2f(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2f* gmtl_Point_float_2__Point__float_float_float3(const float p0, const float p1, const float p2)
   {
      gmtl::Point2f* obj = new gmtl::Point2f(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2f* gmtl_Point_float_2__Point__float_float_float_float4(const float p0, const float p1, const float p2, const float p3)
   {
      gmtl::Point2f* obj = new gmtl::Point2f(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Point2f(gmtl::Point2f* self_)
   {
      delete self_;
   }

} // extern "C" for gmtl::Point2f


