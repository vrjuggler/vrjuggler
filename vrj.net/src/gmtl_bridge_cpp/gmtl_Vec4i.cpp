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
   SHARPPY_API gmtl::Vec4i* gmtl_Vec_int_4__Vec__0()
   {
      gmtl::Vec4i* obj = new gmtl::Vec4i();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4i* gmtl_Vec_int_4__Vec__gmtl_Vec4i1(const gmtl::Vec4i* p0)
   {
      const gmtl::Vec4i& marshal_p0 = *p0;
      gmtl::Vec4i* obj = new gmtl::Vec4i(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4i* gmtl_Vec_int_4__Vec__gmtl_VecBase_int_41(const gmtl::VecBase<int,4>* p0)
   {
      const gmtl::VecBase<int,4>& marshal_p0 = *p0;
      gmtl::Vec4i* obj = new gmtl::Vec4i(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4i* gmtl_Vec_int_4__Vec__int_int2(const int p0, const int p1)
   {
      gmtl::Vec4i* obj = new gmtl::Vec4i(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4i* gmtl_Vec_int_4__Vec__int_int_int3(const int p0, const int p1, const int p2)
   {
      gmtl::Vec4i* obj = new gmtl::Vec4i(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4i* gmtl_Vec_int_4__Vec__int_int_int_int4(const int p0, const int p1, const int p2, const int p3)
   {
      gmtl::Vec4i* obj = new gmtl::Vec4i(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Vec4i(gmtl::Vec4i* self_)
   {
      delete self_;
   }

} // extern "C" for gmtl::Vec4i


