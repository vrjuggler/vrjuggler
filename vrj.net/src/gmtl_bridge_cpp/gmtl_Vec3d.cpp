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
   SHARPPY_API gmtl::Vec3d* gmtl_Vec_double_3__Vec__0()
   {
      gmtl::Vec3d* obj = new gmtl::Vec3d();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3d* gmtl_Vec_double_3__Vec__gmtl_Vec3d1(const gmtl::Vec3d* p0)
   {
      const gmtl::Vec3d& marshal_p0 = *p0;
      gmtl::Vec3d* obj = new gmtl::Vec3d(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3d* gmtl_Vec_double_3__Vec__gmtl_VecBase_double_31(const gmtl::VecBase<double,3>* p0)
   {
      const gmtl::VecBase<double,3>& marshal_p0 = *p0;
      gmtl::Vec3d* obj = new gmtl::Vec3d(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3d* gmtl_Vec_double_3__Vec__double_double2(const double p0, const double p1)
   {
      gmtl::Vec3d* obj = new gmtl::Vec3d(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3d* gmtl_Vec_double_3__Vec__double_double_double3(const double p0, const double p1, const double p2)
   {
      gmtl::Vec3d* obj = new gmtl::Vec3d(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec3d* gmtl_Vec_double_3__Vec__double_double_double_double4(const double p0, const double p1, const double p2, const double p3)
   {
      gmtl::Vec3d* obj = new gmtl::Vec3d(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Vec3d(gmtl::Vec3d* self_)
   {
      delete self_;
   }

} // extern "C" for gmtl::Vec3d


