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
   SHARPPY_API gmtl::LineSegd* gmtl_LineSeg_double__LineSeg__0()
   {
      gmtl::LineSegd* obj = new gmtl::LineSegd();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegd* gmtl_LineSeg_double__LineSeg__gmtl_Point3d_gmtl_Vec3d2(const gmtl::Point3d* p0, const gmtl::Vec3d* p1)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      const gmtl::Vec3d& marshal_p1 = *p1;
      gmtl::LineSegd* obj = new gmtl::LineSegd(marshal_p0, marshal_p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegd* gmtl_LineSeg_double__LineSeg__gmtl_LineSegd1(const gmtl::LineSegd* p0)
   {
      const gmtl::LineSegd& marshal_p0 = *p0;
      gmtl::LineSegd* obj = new gmtl::LineSegd(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegd* gmtl_LineSeg_double__LineSeg__gmtl_Point3d_gmtl_Point3d2(const gmtl::Point3d* p0, const gmtl::Point3d* p1)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      const gmtl::Point3d& marshal_p1 = *p1;
      gmtl::LineSegd* obj = new gmtl::LineSegd(marshal_p0, marshal_p1);
      return obj;
   }

   SHARPPY_API void delete_gmtl_LineSegd(gmtl::LineSegd* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::LineSeg<double>::getLength()
   SHARPPY_API double gmtl_LineSeg_double__getLength__0(gmtl::LineSegd* self_)
   {
      double result;
      result = self_->getLength();
      return result;
   }


} // extern "C" for gmtl::LineSegd


