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
#include <gmtl/Vec.h>
#include <gmtl/Quat.h>
#include <gmtl/Point.h>
#include <gmtl/EulerAngle.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZYXf* gmtl_EulerAngle_float_gmtl_ZYX__EulerAngle__()
   {
      gmtl::EulerAngleZYXf* obj = new gmtl::EulerAngleZYXf();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZYXf* gmtl_EulerAngle_float_gmtl_ZYX__EulerAngle__gmtl_EulerAngleZYXf(const gmtl::EulerAngleZYXf* p0)
   {
      const gmtl::EulerAngleZYXf& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::EulerAngleZYXf* obj = new gmtl::EulerAngleZYXf(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZYXf* gmtl_EulerAngle_float_gmtl_ZYX__EulerAngle__float_float_float(float p0, float p1, float p2)
   {
      gmtl::EulerAngleZYXf* obj = new gmtl::EulerAngleZYXf(p0, p1, p2);
      return obj;
   }

   SHARPPY_API void delete_gmtl_EulerAngleZYXf(gmtl::EulerAngleZYXf* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::EulerAngle<float,gmtl::ZYX>::set()
   SHARPPY_API void gmtl_EulerAngle_float_gmtl_ZYX__set__float_float_float(gmtl::EulerAngleZYXf* self_, const float& p0, const float& p1, const float& p2)
   {
      self_->set(p0, p1, p2);
   }

   // Wrapper for non-virtual method gmtl::EulerAngle<float,gmtl::ZYX>::getData()
   SHARPPY_API float* gmtl_EulerAngle_float_gmtl_ZYX__getData__(gmtl::EulerAngleZYXf* self_)
   {
      float* result;
      result = self_->getData();

      return result;
   }

/*
   // Wrapper for non-virtual method gmtl::EulerAngle<float,gmtl::ZYX>::getData()
   SHARPPY_API const float* gmtl_EulerAngle_float_gmtl_ZYX__getData__(gmtl::EulerAngleZYXf* self_)
   {
      const float* result;
      result = self_->getData();

      return result;
   }
*/





} // extern "C" for gmtl::EulerAngleZYXf


