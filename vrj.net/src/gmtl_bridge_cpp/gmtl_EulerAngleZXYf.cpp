// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/EulerAngle.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZXYf* gmtl_EulerAngle_float_gmtl_ZXY__EulerAngle__0()
   {
      gmtl::EulerAngleZXYf* obj = new gmtl::EulerAngleZXYf();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZXYf* gmtl_EulerAngle_float_gmtl_ZXY__EulerAngle__gmtl_EulerAngleZXYf1(const gmtl::EulerAngleZXYf* p0)
   {
      const gmtl::EulerAngleZXYf& marshal_p0 = *p0;
      gmtl::EulerAngleZXYf* obj = new gmtl::EulerAngleZXYf(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZXYf* gmtl_EulerAngle_float_gmtl_ZXY__EulerAngle__float_float_float3(float p0, float p1, float p2)
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


} // extern "C" for gmtl::EulerAngleZXYf


