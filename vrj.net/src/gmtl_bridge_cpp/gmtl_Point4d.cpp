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
   SHARPPY_API gmtl::Point4d* gmtl_Point_double_4__Point__0()
   {
      gmtl::Point4d* obj = new gmtl::Point4d();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point4d* gmtl_Point_double_4__Point__gmtl_Point4d1(const gmtl::Point4d* p0)
   {
      const gmtl::Point4d& marshal_p0 = *p0;
      gmtl::Point4d* obj = new gmtl::Point4d(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point4d* gmtl_Point_double_4__Point__gmtl_VecBase_double_41(const gmtl::VecBase<double,4>* p0)
   {
      const gmtl::VecBase<double,4>& marshal_p0 = *p0;
      gmtl::Point4d* obj = new gmtl::Point4d(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point4d* gmtl_Point_double_4__Point__double_double2(const double p0, const double p1)
   {
      gmtl::Point4d* obj = new gmtl::Point4d(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point4d* gmtl_Point_double_4__Point__double_double_double3(const double p0, const double p1, const double p2)
   {
      gmtl::Point4d* obj = new gmtl::Point4d(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point4d* gmtl_Point_double_4__Point__double_double_double_double4(const double p0, const double p1, const double p2, const double p3)
   {
      gmtl::Point4d* obj = new gmtl::Point4d(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Point4d(gmtl::Point4d* self_)
   {
      delete self_;
   }

} // extern "C" for gmtl::Point4d


