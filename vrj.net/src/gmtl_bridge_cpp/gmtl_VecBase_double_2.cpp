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
#include <gmtl/VecBase.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<double,2>* gmtl_VecBase_double_2__VecBase__()
   {
      gmtl::VecBase<double,2>* obj = new gmtl::VecBase<double,2>();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<double,2>* gmtl_VecBase_double_2__VecBase__gmtl_VecBase_double_2(const gmtl::VecBase<double,2>* p0)
   {
      const gmtl::VecBase<double,2>& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::VecBase<double,2>* obj = new gmtl::VecBase<double,2>(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<double,2>* gmtl_VecBase_double_2__VecBase__double_double(const double& p0, const double& p1)
   {
      gmtl::VecBase<double,2>* obj = new gmtl::VecBase<double,2>(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<double,2>* gmtl_VecBase_double_2__VecBase__double_double_double(const double& p0, const double& p1, const double& p2)
   {
      gmtl::VecBase<double,2>* obj = new gmtl::VecBase<double,2>(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<double,2>* gmtl_VecBase_double_2__VecBase__double_double_double_double(const double& p0, const double& p1, const double& p2, const double& p3)
   {
      gmtl::VecBase<double,2>* obj = new gmtl::VecBase<double,2>(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_VecBase_double_2(gmtl::VecBase<double,2>* self_)
   {
      delete self_;
   }

/*
   // Wrapper for non-virtual method gmtl::VecBase<double,2>::set()
   SHARPPY_API void gmtl_VecBase_double_2__set__double1(gmtl::VecBase<double,2>* self_, const double* p0)
   {
      self_->set(p0);
   }
*/

   // Wrapper for non-virtual method gmtl::VecBase<double,2>::set()
   SHARPPY_API void gmtl_VecBase_double_2__set__double1(gmtl::VecBase<double,2>* self_, const double& p0)
   {
      self_->set(p0);
   }


   // Wrapper for non-virtual method gmtl::VecBase<double,2>::set()
   SHARPPY_API void gmtl_VecBase_double_2__set__double_double2(gmtl::VecBase<double,2>* self_, const double& p0, const double& p1)
   {
      self_->set(p0, p1);
   }


   // Wrapper for non-virtual method gmtl::VecBase<double,2>::set()
   SHARPPY_API void gmtl_VecBase_double_2__set__double_double_double3(gmtl::VecBase<double,2>* self_, const double& p0, const double& p1, const double& p2)
   {
      self_->set(p0, p1, p2);
   }


   // Wrapper for non-virtual method gmtl::VecBase<double,2>::set()
   SHARPPY_API void gmtl_VecBase_double_2__set__double_double_double_double4(gmtl::VecBase<double,2>* self_, const double& p0, const double& p1, const double& p2, const double& p3)
   {
      self_->set(p0, p1, p2, p3);
   }


   // Wrapper for non-virtual method gmtl::VecBase<double,2>::getData()
   SHARPPY_API double* gmtl_VecBase_double_2__getData__0(gmtl::VecBase<double,2>* self_)
   {
      double* result;
      result = self_->getData();
      return result;
   }

/*
   // Wrapper for non-virtual method gmtl::VecBase<double,2>::getData()
   SHARPPY_API const double* gmtl_VecBase_double_2__getData__0(gmtl::VecBase<double,2>* self_)
   {
      const double* result;
      result = self_->getData();
      return result;
   }
*/

} // extern "C" for gmtl::VecBase<double,2>


