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
   SHARPPY_API gmtl::EulerAngleZYXf* gmtl_EulerAngle_float_gmtl_ZYX__EulerAngle__0()
   {
      gmtl::EulerAngleZYXf* obj = new gmtl::EulerAngleZYXf();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZYXf* gmtl_EulerAngle_float_gmtl_ZYX__EulerAngle__gmtl_EulerAngleZYXf1(const gmtl::EulerAngleZYXf* p0)
   {
      const gmtl::EulerAngleZYXf& marshal_p0 = *p0;
      gmtl::EulerAngleZYXf* obj = new gmtl::EulerAngleZYXf(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZYXf* gmtl_EulerAngle_float_gmtl_ZYX__EulerAngle__float_float_float3(float p0, float p1, float p2)
   {
      gmtl::EulerAngleZYXf* obj = new gmtl::EulerAngleZYXf(p0, p1, p2);
      return obj;
   }

   SHARPPY_API void delete_gmtl_EulerAngleZYXf(gmtl::EulerAngleZYXf* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::EulerAngle<float,gmtl::ZYX>::set()
   SHARPPY_API void gmtl_EulerAngle_float_gmtl_ZYX__set__float_float_float3(gmtl::EulerAngleZYXf* self_, const float p0, const float p1, const float p2)
   {
      self_->set(p0, p1, p2);
   }


   // Wrapper for non-virtual method gmtl::EulerAngle<float,gmtl::ZYX>::getData()
   SHARPPY_API void gmtl_EulerAngle_float_gmtl_ZYX__getData__0(gmtl::EulerAngleZYXf* self_, float* arrayHolder)
   {
      float* temp_array;
      temp_array = self_->getData();
      for ( int i = 0; i < 3; ++i )
      {
         arrayHolder[i] = temp_array[i];
      }
   }


} // extern "C" for gmtl::EulerAngleZYXf


