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
   SHARPPY_API gmtl::EulerAngleXYZf* gmtl_EulerAngle_float_gmtl_XYZ__EulerAngle__0()
   {
      gmtl::EulerAngleXYZf* obj = new gmtl::EulerAngleXYZf();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleXYZf* gmtl_EulerAngle_float_gmtl_XYZ__EulerAngle__gmtl_EulerAngleXYZf1(const gmtl::EulerAngleXYZf* p0)
   {
      const gmtl::EulerAngleXYZf& marshal_p0 = *p0;
      gmtl::EulerAngleXYZf* obj = new gmtl::EulerAngleXYZf(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleXYZf* gmtl_EulerAngle_float_gmtl_XYZ__EulerAngle__float_float_float3(float p0, float p1, float p2)
   {
      gmtl::EulerAngleXYZf* obj = new gmtl::EulerAngleXYZf(p0, p1, p2);
      return obj;
   }

   SHARPPY_API void delete_gmtl_EulerAngleXYZf(gmtl::EulerAngleXYZf* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::EulerAngle<float,gmtl::XYZ>::set()
   SHARPPY_API void gmtl_EulerAngle_float_gmtl_XYZ__set__float_float_float3(gmtl::EulerAngleXYZf* self_, const float p0, const float p1, const float p2)
   {
      self_->set(p0, p1, p2);
   }


   // Wrapper for non-virtual method gmtl::EulerAngle<float,gmtl::XYZ>::getData()
   SHARPPY_API void gmtl_EulerAngle_float_gmtl_XYZ__getData__0(gmtl::EulerAngleXYZf* self_, float* arrayHolder)
   {
      float* temp_array;
      temp_array = self_->getData();
      for ( int i = 0; i < 3; ++i )
      {
         arrayHolder[i] = temp_array[i];
      }
   }


} // extern "C" for gmtl::EulerAngleXYZf


