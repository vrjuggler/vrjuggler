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
   SHARPPY_API gmtl::Trif* gmtl_Tri_float__Tri__0()
   {
      gmtl::Trif* obj = new gmtl::Trif();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Trif* gmtl_Tri_float__Tri__gmtl_Point3f_gmtl_Point3f_gmtl_Point3f3(const gmtl::Point3f* p0, const gmtl::Point3f* p1, const gmtl::Point3f* p2)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      const gmtl::Point3f& marshal_p2 = *p2;
      gmtl::Trif* obj = new gmtl::Trif(marshal_p0, marshal_p1, marshal_p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Trif* gmtl_Tri_float__Tri__gmtl_Trif1(const gmtl::Trif* p0)
   {
      const gmtl::Trif& marshal_p0 = *p0;
      gmtl::Trif* obj = new gmtl::Trif(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Trif(gmtl::Trif* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Tri<float>::edge()
   SHARPPY_API gmtl::Vec3f* gmtl_Tri_float__edge__int1(gmtl::Trif* self_, int p0)
   {
      gmtl::Vec3f* result;
      result = new gmtl::Vec3f(self_->edge(p0));
      return result;
   }


   // Wrapper for non-virtual method gmtl::Tri<float>::edge()
   SHARPPY_API gmtl::Vec3f* gmtl_Tri_float__edge__int_int2(gmtl::Trif* self_, int p0, int p1)
   {
      gmtl::Vec3f* result;
      result = new gmtl::Vec3f(self_->edge(p0, p1));
      return result;
   }


   // Wrapper for non-virtual method gmtl::Tri<float>::set()
   SHARPPY_API void gmtl_Tri_float__set__gmtl_Point3f_gmtl_Point3f_gmtl_Point3f3(gmtl::Trif* self_, const gmtl::Point3f* p0, const gmtl::Point3f* p1, const gmtl::Point3f* p2)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      const gmtl::Point3f& marshal_p2 = *p2;
      self_->set(marshal_p0, marshal_p1, marshal_p2);
   }


} // extern "C" for gmtl::Trif


