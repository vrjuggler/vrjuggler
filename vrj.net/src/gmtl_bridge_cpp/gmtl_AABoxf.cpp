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
#include <gmtl/AABox.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::AABoxf* gmtl_AABox_float__AABox__0()
   {
      gmtl::AABoxf* obj = new gmtl::AABoxf();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AABoxf* gmtl_AABox_float__AABox__gmtl_Point3f_gmtl_Point3f2(const gmtl::Point3f* p0, const gmtl::Point3f* p1)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      gmtl::AABoxf* obj = new gmtl::AABoxf(marshal_p0, marshal_p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AABoxf* gmtl_AABox_float__AABox__gmtl_AABoxf1(const gmtl::AABoxf* p0)
   {
      const gmtl::AABoxf& marshal_p0 = *p0;
      gmtl::AABoxf* obj = new gmtl::AABoxf(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_AABoxf(gmtl::AABoxf* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::AABox<float>::getMin()
   SHARPPY_API const gmtl::Point3f* gmtl_AABox_float__getMin__0(gmtl::AABoxf* self_)
   {
      const gmtl::Point3f* result;
      result = new const gmtl::Point3f(self_->getMin());
      return result;
   }


   // Wrapper for non-virtual method gmtl::AABox<float>::getMax()
   SHARPPY_API const gmtl::Point3f* gmtl_AABox_float__getMax__0(gmtl::AABoxf* self_)
   {
      const gmtl::Point3f* result;
      result = new const gmtl::Point3f(self_->getMax());
      return result;
   }


   // Wrapper for non-virtual method gmtl::AABox<float>::isEmpty()
   SHARPPY_API bool gmtl_AABox_float__isEmpty__0(gmtl::AABoxf* self_)
   {
      bool result;
      result = self_->isEmpty();
      return result;
   }


   // Wrapper for non-virtual method gmtl::AABox<float>::setMin()
   SHARPPY_API void gmtl_AABox_float__setMin__gmtl_Point3f1(gmtl::AABoxf* self_, const gmtl::Point3f* p0)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      self_->setMin(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::AABox<float>::setMax()
   SHARPPY_API void gmtl_AABox_float__setMax__gmtl_Point3f1(gmtl::AABoxf* self_, const gmtl::Point3f* p0)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      self_->setMax(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::AABox<float>::setEmpty()
   SHARPPY_API void gmtl_AABox_float__setEmpty__bool1(gmtl::AABoxf* self_, bool p0)
   {
      self_->setEmpty(p0);
   }


} // extern "C" for gmtl::AABoxf


