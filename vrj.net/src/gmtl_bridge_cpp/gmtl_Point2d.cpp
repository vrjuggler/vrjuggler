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
   SHARPPY_API gmtl::Point2d* gmtl_Point_double_2__Point__0()
   {
      gmtl::Point2d* obj = new gmtl::Point2d();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2d* gmtl_Point_double_2__Point__gmtl_Point2d1(const gmtl::Point2d* p0)
   {
      const gmtl::Point2d& marshal_p0 = *p0;
      gmtl::Point2d* obj = new gmtl::Point2d(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2d* gmtl_Point_double_2__Point__gmtl_VecBase_double_21(const gmtl::VecBase<double,2>* p0)
   {
      const gmtl::VecBase<double,2>& marshal_p0 = *p0;
      gmtl::Point2d* obj = new gmtl::Point2d(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2d* gmtl_Point_double_2__Point__double_double2(const double p0, const double p1)
   {
      gmtl::Point2d* obj = new gmtl::Point2d(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2d* gmtl_Point_double_2__Point__double_double_double3(const double p0, const double p1, const double p2)
   {
      gmtl::Point2d* obj = new gmtl::Point2d(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point2d* gmtl_Point_double_2__Point__double_double_double_double4(const double p0, const double p1, const double p2, const double p3)
   {
      gmtl::Point2d* obj = new gmtl::Point2d(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Point2d(gmtl::Point2d* self_)
   {
      delete self_;
   }

} // extern "C" for gmtl::Point2d


