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
   SHARPPY_API gmtl::Point4i* gmtl_Point_int_4__Point__0()
   {
      gmtl::Point4i* obj = new gmtl::Point4i();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point4i* gmtl_Point_int_4__Point__gmtl_Point4i1(const gmtl::Point4i* p0)
   {
      const gmtl::Point4i& marshal_p0 = *p0;
      gmtl::Point4i* obj = new gmtl::Point4i(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point4i* gmtl_Point_int_4__Point__gmtl_VecBase_int_41(const gmtl::VecBase<int,4>* p0)
   {
      const gmtl::VecBase<int,4>& marshal_p0 = *p0;
      gmtl::Point4i* obj = new gmtl::Point4i(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point4i* gmtl_Point_int_4__Point__int_int2(const int p0, const int p1)
   {
      gmtl::Point4i* obj = new gmtl::Point4i(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point4i* gmtl_Point_int_4__Point__int_int_int3(const int p0, const int p1, const int p2)
   {
      gmtl::Point4i* obj = new gmtl::Point4i(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Point4i* gmtl_Point_int_4__Point__int_int_int_int4(const int p0, const int p1, const int p2, const int p3)
   {
      gmtl::Point4i* obj = new gmtl::Point4i(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Point4i(gmtl::Point4i* self_)
   {
      delete self_;
   }

} // extern "C" for gmtl::Point4i


