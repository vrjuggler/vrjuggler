// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Tri.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Trid* gmtl_Tri_double__Tri__0()
   {
      gmtl::Trid* obj = new gmtl::Trid();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Trid* gmtl_Tri_double__Tri__gmtl_Point3d_gmtl_Point3d_gmtl_Point3d3(const gmtl::Point3d* p0, const gmtl::Point3d* p1, const gmtl::Point3d* p2)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      const gmtl::Point3d& marshal_p1 = *p1;
      const gmtl::Point3d& marshal_p2 = *p2;
      gmtl::Trid* obj = new gmtl::Trid(marshal_p0, marshal_p1, marshal_p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Trid* gmtl_Tri_double__Tri__gmtl_Trid1(const gmtl::Trid* p0)
   {
      const gmtl::Trid& marshal_p0 = *p0;
      gmtl::Trid* obj = new gmtl::Trid(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Trid(gmtl::Trid* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Tri<double>::edge()
   SHARPPY_API gmtl::Vec3d* gmtl_Tri_double__edge__int1(gmtl::Trid* self_, int p0)
   {
      gmtl::Vec3d* result;
      result = new gmtl::Vec3d(self_->edge(p0));
      return result;
   }


   // Wrapper for non-virtual method gmtl::Tri<double>::edge()
   SHARPPY_API gmtl::Vec3d* gmtl_Tri_double__edge__int_int2(gmtl::Trid* self_, int p0, int p1)
   {
      gmtl::Vec3d* result;
      result = new gmtl::Vec3d(self_->edge(p0, p1));
      return result;
   }


   // Wrapper for non-virtual method gmtl::Tri<double>::set()
   SHARPPY_API void gmtl_Tri_double__set__gmtl_Point3d_gmtl_Point3d_gmtl_Point3d3(gmtl::Trid* self_, const gmtl::Point3d* p0, const gmtl::Point3d* p1, const gmtl::Point3d* p2)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      const gmtl::Point3d& marshal_p1 = *p1;
      const gmtl::Point3d& marshal_p2 = *p2;
      self_->set(marshal_p0, marshal_p1, marshal_p2);
   }


} // extern "C" for gmtl::Trid


