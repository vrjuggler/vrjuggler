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

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/VecBase.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<int,3>* gmtl_VecBase_int_3__VecBase__0()
   {
      gmtl::VecBase<int,3>* obj = new gmtl::VecBase<int,3>();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<int,3>* gmtl_VecBase_int_3__VecBase__gmtl_VecBase_int_31(const gmtl::VecBase<int,3>* p0)
   {
      const gmtl::VecBase<int,3>& marshal_p0 = *p0;
      gmtl::VecBase<int,3>* obj = new gmtl::VecBase<int,3>(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<int,3>* gmtl_VecBase_int_3__VecBase__int_int2(const int p0, const int p1)
   {
      gmtl::VecBase<int,3>* obj = new gmtl::VecBase<int,3>(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<int,3>* gmtl_VecBase_int_3__VecBase__int_int_int3(const int p0, const int p1, const int p2)
   {
      gmtl::VecBase<int,3>* obj = new gmtl::VecBase<int,3>(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<int,3>* gmtl_VecBase_int_3__VecBase__int_int_int_int4(const int p0, const int p1, const int p2, const int p3)
   {
      gmtl::VecBase<int,3>* obj = new gmtl::VecBase<int,3>(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_VecBase_int_3(gmtl::VecBase<int,3>* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::VecBase<int,3>::set()
   SHARPPY_API void gmtl_VecBase_int_3__set__int1(gmtl::VecBase<int,3>* self_, const int* p0)
   {
      self_->set(p0);
   }

/*
   // Wrapper for non-virtual method gmtl::VecBase<int,3>::set()
   SHARPPY_API void gmtl_VecBase_int_3__set__int1(gmtl::VecBase<int,3>* self_, const int p0)
   {
      self_->set(p0);
   }
*/

   // Wrapper for non-virtual method gmtl::VecBase<int,3>::set()
   SHARPPY_API void gmtl_VecBase_int_3__set__int_int2(gmtl::VecBase<int,3>* self_, const int p0, const int p1)
   {
      self_->set(p0, p1);
   }


   // Wrapper for non-virtual method gmtl::VecBase<int,3>::set()
   SHARPPY_API void gmtl_VecBase_int_3__set__int_int_int3(gmtl::VecBase<int,3>* self_, const int p0, const int p1, const int p2)
   {
      self_->set(p0, p1, p2);
   }


   // Wrapper for non-virtual method gmtl::VecBase<int,3>::set()
   SHARPPY_API void gmtl_VecBase_int_3__set__int_int_int_int4(gmtl::VecBase<int,3>* self_, const int p0, const int p1, const int p2, const int p3)
   {
      self_->set(p0, p1, p2, p3);
   }


   // Wrapper for non-virtual method gmtl::VecBase<int,3>::getData()
   SHARPPY_API void gmtl_VecBase_int_3__getData__0(gmtl::VecBase<int,3>* self_, int* arrayHolder)
   {
      int* temp_array;
      temp_array = self_->getData();
      for ( int i = 0; i < 3; ++i )
      {
         arrayHolder[i] = temp_array[i];
      }
   }


} // extern "C" for gmtl::VecBase<int,3>


