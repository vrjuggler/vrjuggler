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
#include <gmtl/Matrix.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Matrix44f* gmtl_Matrix_float_4_4__Matrix__0()
   {
      gmtl::Matrix44f* obj = new gmtl::Matrix44f();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Matrix44f* gmtl_Matrix_float_4_4__Matrix__gmtl_Matrix44f1(const gmtl::Matrix44f* p0)
   {
      const gmtl::Matrix44f& marshal_p0 = *p0;
      gmtl::Matrix44f* obj = new gmtl::Matrix44f(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Matrix44f(gmtl::Matrix44f* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Matrix<float,4,4>::set()
   SHARPPY_API void gmtl_Matrix_float_4_4__set__float_float_float_float4(gmtl::Matrix44f* self_, float p0, float p1, float p2, float p3)
   {
      self_->set(p0, p1, p2, p3);
   }


   // Wrapper for non-virtual method gmtl::Matrix<float,4,4>::set()
   SHARPPY_API void gmtl_Matrix_float_4_4__set__float_float_float_float_float_float6(gmtl::Matrix44f* self_, float p0, float p1, float p2, float p3, float p4, float p5)
   {
      self_->set(p0, p1, p2, p3, p4, p5);
   }


   // Wrapper for non-virtual method gmtl::Matrix<float,4,4>::set()
   SHARPPY_API void gmtl_Matrix_float_4_4__set__float_float_float_float_float_float_float_float_float9(gmtl::Matrix44f* self_, float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8)
   {
      self_->set(p0, p1, p2, p3, p4, p5, p6, p7, p8);
   }


   // Wrapper for non-virtual method gmtl::Matrix<float,4,4>::set()
   SHARPPY_API void gmtl_Matrix_float_4_4__set__float_float_float_float_float_float_float_float_float_float_float_float12(gmtl::Matrix44f* self_, float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10, float p11)
   {
      self_->set(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
   }


   // Wrapper for non-virtual method gmtl::Matrix<float,4,4>::set()
   SHARPPY_API void gmtl_Matrix_float_4_4__set__float_float_float_float_float_float_float_float_float_float_float_float_float_float_float_float16(gmtl::Matrix44f* self_, float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10, float p11, float p12, float p13, float p14, float p15)
   {
      self_->set(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
   }


   // Wrapper for non-virtual method gmtl::Matrix<float,4,4>::set()
   SHARPPY_API void gmtl_Matrix_float_4_4__set__float1(gmtl::Matrix44f* self_, const float* p0)
   {
      self_->set(p0);
   }


   // Wrapper for non-virtual method gmtl::Matrix<float,4,4>::setTranspose()
   SHARPPY_API void gmtl_Matrix_float_4_4__setTranspose__float1(gmtl::Matrix44f* self_, const float* p0)
   {
      self_->setTranspose(p0);
   }


   // Wrapper for non-virtual method gmtl::Matrix<float,4,4>::getData()
   SHARPPY_API void gmtl_Matrix_float_4_4__getData__0(gmtl::Matrix44f* self_, float* arrayHolder)
   {
      const float* temp_array;
      temp_array = self_->getData();
      for ( int i = 0; i < 16; ++i )
      {
         arrayHolder[i] = temp_array[i];
      }
   }


   // Wrapper for non-virtual method gmtl::Matrix<float,4,4>::isError()
   SHARPPY_API bool gmtl_Matrix_float_4_4__isError__0(gmtl::Matrix44f* self_)
   {
      bool result;
      result = self_->isError();
      return result;
   }


   // Wrapper for non-virtual method gmtl::Matrix<float,4,4>::setError()
   SHARPPY_API void gmtl_Matrix_float_4_4__setError__0(gmtl::Matrix44f* self_)
   {
      self_->setError();
   }


} // extern "C" for gmtl::Matrix44f

// Nested class gmtl::Matrix44f::RowAccessor.
extern "C"
{
   SHARPPY_API void delete_gmtl_Matrix44f_RowAccessor(gmtl::Matrix44f::RowAccessor* self_)
   {
      delete self_;
   }

} // extern "C" for gmtl::Matrix44f::RowAccessor


// Nested class gmtl::Matrix44f::ConstRowAccessor.
extern "C"
{
   SHARPPY_API void delete_gmtl_Matrix44f_ConstRowAccessor(gmtl::Matrix44f::ConstRowAccessor* self_)
   {
      delete self_;
   }

} // extern "C" for gmtl::Matrix44f::ConstRowAccessor



