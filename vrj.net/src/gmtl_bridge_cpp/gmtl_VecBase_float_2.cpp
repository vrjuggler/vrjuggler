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

// Generated from Revision: 1.58 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Matrix.h>
#include <gmtl/VecBase.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<float,2>* gmtl_VecBase_float_2__VecBase__()
   {
      gmtl::VecBase<float,2>* obj = new gmtl::VecBase<float,2>();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<float,2>* gmtl_VecBase_float_2__VecBase__gmtl_VecBase_float_2(const gmtl::VecBase<float,2>* p0)
   {
      const gmtl::VecBase<float,2>& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::VecBase<float,2>* obj = new gmtl::VecBase<float,2>(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<float,2>* gmtl_VecBase_float_2__VecBase__float_float(const float& p0, const float& p1)
   {
      gmtl::VecBase<float,2>* obj = new gmtl::VecBase<float,2>(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<float,2>* gmtl_VecBase_float_2__VecBase__float_float_float(const float& p0, const float& p1, const float& p2)
   {
      gmtl::VecBase<float,2>* obj = new gmtl::VecBase<float,2>(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<float,2>* gmtl_VecBase_float_2__VecBase__float_float_float_float(const float& p0, const float& p1, const float& p2, const float& p3)
   {
      gmtl::VecBase<float,2>* obj = new gmtl::VecBase<float,2>(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_VecBase_float_2(gmtl::VecBase<float,2>* self_)
   {
      delete self_;
   }

/*
   // Wrapper for non-virtual method gmtl::VecBase<float,2>::set()
   SHARPPY_API void gmtl_VecBase_float_2__set__float(gmtl::VecBase<float,2>* self_, const float* p0)
   {
      self_->set(p0);
   }
*/

   // Wrapper for non-virtual method gmtl::VecBase<float,2>::set()
   SHARPPY_API void gmtl_VecBase_float_2__set__float(gmtl::VecBase<float,2>* self_, const float& p0)
   {
      self_->set(p0);
   }

   // Wrapper for non-virtual method gmtl::VecBase<float,2>::set()
   SHARPPY_API void gmtl_VecBase_float_2__set__float_float(gmtl::VecBase<float,2>* self_, const float& p0, const float& p1)
   {
      self_->set(p0, p1);
   }

   // Wrapper for non-virtual method gmtl::VecBase<float,2>::set()
   SHARPPY_API void gmtl_VecBase_float_2__set__float_float_float(gmtl::VecBase<float,2>* self_, const float& p0, const float& p1, const float& p2)
   {
      self_->set(p0, p1, p2);
   }

   // Wrapper for non-virtual method gmtl::VecBase<float,2>::set()
   SHARPPY_API void gmtl_VecBase_float_2__set__float_float_float_float(gmtl::VecBase<float,2>* self_, const float& p0, const float& p1, const float& p2, const float& p3)
   {
      self_->set(p0, p1, p2, p3);
   }

   // Wrapper for non-virtual method gmtl::VecBase<float,2>::getData()
   SHARPPY_API float* gmtl_VecBase_float_2__getData__(gmtl::VecBase<float,2>* self_)
   {
      float* result;
      result = self_->getData();

      return result;
   }

/*
   // Wrapper for non-virtual method gmtl::VecBase<float,2>::getData()
   SHARPPY_API const float* gmtl_VecBase_float_2__getData__(gmtl::VecBase<float,2>* self_)
   {
      const float* result;
      result = self_->getData();

      return result;
   }
*/





} // extern "C" for gmtl::VecBase<float,2>


