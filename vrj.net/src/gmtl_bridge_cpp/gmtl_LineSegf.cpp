// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/LineSeg.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegf* gmtl_LineSeg_float__LineSeg__0()
   {
      gmtl::LineSegf* obj = new gmtl::LineSegf();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegf* gmtl_LineSeg_float__LineSeg__gmtl_Point3f_gmtl_Vec3f2(const gmtl::Point3f* p0, const gmtl::Vec3f* p1)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      const gmtl::Vec3f& marshal_p1 = *p1;
      gmtl::LineSegf* obj = new gmtl::LineSegf(marshal_p0, marshal_p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegf* gmtl_LineSeg_float__LineSeg__gmtl_LineSegf1(const gmtl::LineSegf* p0)
   {
      const gmtl::LineSegf& marshal_p0 = *p0;
      gmtl::LineSegf* obj = new gmtl::LineSegf(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegf* gmtl_LineSeg_float__LineSeg__gmtl_Point3f_gmtl_Point3f2(const gmtl::Point3f* p0, const gmtl::Point3f* p1)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      gmtl::LineSegf* obj = new gmtl::LineSegf(marshal_p0, marshal_p1);
      return obj;
   }

   SHARPPY_API void delete_gmtl_LineSegf(gmtl::LineSegf* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::LineSeg<float>::getLength()
   SHARPPY_API float gmtl_LineSeg_float__getLength__0(gmtl::LineSegf* self_)
   {
      float result;
      result = self_->getLength();
      return result;
   }


} // extern "C" for gmtl::LineSegf


