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
   SHARPPY_API gmtl::Trii* gmtl_Tri_int__Tri__0()
   {
      gmtl::Trii* obj = new gmtl::Trii();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Trii* gmtl_Tri_int__Tri__gmtl_Point3i_gmtl_Point3i_gmtl_Point3i3(const gmtl::Point3i* p0, const gmtl::Point3i* p1, const gmtl::Point3i* p2)
   {
      const gmtl::Point3i& marshal_p0 = *p0;
      const gmtl::Point3i& marshal_p1 = *p1;
      const gmtl::Point3i& marshal_p2 = *p2;
      gmtl::Trii* obj = new gmtl::Trii(marshal_p0, marshal_p1, marshal_p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Trii* gmtl_Tri_int__Tri__gmtl_Trii1(const gmtl::Trii* p0)
   {
      const gmtl::Trii& marshal_p0 = *p0;
      gmtl::Trii* obj = new gmtl::Trii(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Trii(gmtl::Trii* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Tri<int>::edge()
   SHARPPY_API gmtl::Vec3i* gmtl_Tri_int__edge__int1(gmtl::Trii* self_, int p0)
   {
      gmtl::Vec3i* result;
      result = new gmtl::Vec3i(self_->edge(p0));
      return result;
   }


   // Wrapper for non-virtual method gmtl::Tri<int>::edge()
   SHARPPY_API gmtl::Vec3i* gmtl_Tri_int__edge__int_int2(gmtl::Trii* self_, int p0, int p1)
   {
      gmtl::Vec3i* result;
      result = new gmtl::Vec3i(self_->edge(p0, p1));
      return result;
   }


   // Wrapper for non-virtual method gmtl::Tri<int>::set()
   SHARPPY_API void gmtl_Tri_int__set__gmtl_Point3i_gmtl_Point3i_gmtl_Point3i3(gmtl::Trii* self_, const gmtl::Point3i* p0, const gmtl::Point3i* p1, const gmtl::Point3i* p2)
   {
      const gmtl::Point3i& marshal_p0 = *p0;
      const gmtl::Point3i& marshal_p1 = *p1;
      const gmtl::Point3i& marshal_p2 = *p2;
      self_->set(marshal_p0, marshal_p1, marshal_p2);
   }


} // extern "C" for gmtl::Trii


