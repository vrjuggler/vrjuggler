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

// Generated from Revision: 1.62 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/EulerAngle.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZXYf* gmtl_EulerAngle_float_gmtl_ZXY__EulerAngle__()
   {
      gmtl::EulerAngleZXYf* obj = new gmtl::EulerAngleZXYf();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZXYf* gmtl_EulerAngle_float_gmtl_ZXY__EulerAngle__gmtl_EulerAngleZXYf(const gmtl::EulerAngleZXYf* p0)
   {
      const gmtl::EulerAngleZXYf& marshal_p0 = *p0;
      gmtl::EulerAngleZXYf* obj = new gmtl::EulerAngleZXYf(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZXYf* gmtl_EulerAngle_float_gmtl_ZXY__EulerAngle__float_float_float(float p0, float p1, float p2)
   {
      gmtl::EulerAngleZXYf* obj = new gmtl::EulerAngleZXYf(p0, p1, p2);
      return obj;
   }

   SHARPPY_API void delete_gmtl_EulerAngleZXYf(gmtl::EulerAngleZXYf* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::EulerAngle<float,gmtl::ZXY>::set()
   SHARPPY_API void gmtl_EulerAngle_float_gmtl_ZXY__set__float_float_float3(gmtl::EulerAngleZXYf* self_, const float p0, const float p1, const float p2)
   {
      self_->set(p0, p1, p2);
   }


   // Wrapper for non-virtual method gmtl::EulerAngle<float,gmtl::ZXY>::getData()
   SHARPPY_API void gmtl_EulerAngle_float_gmtl_ZXY__getData__0(gmtl::EulerAngleZXYf* self_, float* arrayHolder)
   {
      float* temp_array;
      temp_array = self_->getData();
      for ( int i = 0; i < 3; ++i )
      {
         arrayHolder[i] = temp_array[i];
      }
   }

/*
   // Wrapper for non-virtual method gmtl::EulerAngle<float,gmtl::ZXY>::getData()
   SHARPPY_API void gmtl_EulerAngle_float_gmtl_ZXY__getData__0(gmtl::EulerAngleZXYf* self_, float* arrayHolder)
   {
      const float* temp_array;
      temp_array = self_->getData();
      for ( int i = 0; i < 3; ++i )
      {
         arrayHolder[i] = temp_array[i];
      }
   }
*/

} // extern "C" for gmtl::EulerAngleZXYf


