/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VRJ.NET is (C) Copyright 2004 by Patrick Hartling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

// Generated from Revision: 1.60 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Vec.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4i* gmtl_Vec_int_4__Vec__()
   {
      gmtl::Vec4i* obj = new gmtl::Vec4i();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4i* gmtl_Vec_int_4__Vec__gmtl_Vec4i(const gmtl::Vec4i* p0)
   {
      const gmtl::Vec4i& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Vec4i* obj = new gmtl::Vec4i(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4i* gmtl_Vec_int_4__Vec__gmtl_VecBase_int_4(const gmtl::VecBase<int,4>* p0)
   {
      const gmtl::VecBase<int,4>& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Vec4i* obj = new gmtl::Vec4i(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4i* gmtl_Vec_int_4__Vec__int_int(const int p0, const int p1)
   {
      gmtl::Vec4i* obj = new gmtl::Vec4i(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4i* gmtl_Vec_int_4__Vec__int_int_int(const int p0, const int p1, const int p2)
   {
      gmtl::Vec4i* obj = new gmtl::Vec4i(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Vec4i* gmtl_Vec_int_4__Vec__int_int_int_int(const int p0, const int p1, const int p2, const int p3)
   {
      gmtl::Vec4i* obj = new gmtl::Vec4i(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Vec4i(gmtl::Vec4i* self_)
   {
      delete self_;
   }

} // extern "C" for gmtl::Vec4i


