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
   SHARPPY_API gmtl::EulerAngleZXYd* gmtl_EulerAngle_double_gmtl_ZXY__EulerAngle__0()
   {
      gmtl::EulerAngleZXYd* obj = new gmtl::EulerAngleZXYd();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZXYd* gmtl_EulerAngle_double_gmtl_ZXY__EulerAngle__gmtl_EulerAngleZXYd1(const gmtl::EulerAngleZXYd* p0)
   {
      const gmtl::EulerAngleZXYd& marshal_p0 = *p0;
      gmtl::EulerAngleZXYd* obj = new gmtl::EulerAngleZXYd(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZXYd* gmtl_EulerAngle_double_gmtl_ZXY__EulerAngle__double_double_double3(double p0, double p1, double p2)
   {
      gmtl::EulerAngleZXYd* obj = new gmtl::EulerAngleZXYd(p0, p1, p2);
      return obj;
   }

   SHARPPY_API void delete_gmtl_EulerAngleZXYd(gmtl::EulerAngleZXYd* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::EulerAngle<double,gmtl::ZXY>::set()
   SHARPPY_API void gmtl_EulerAngle_double_gmtl_ZXY__set__double_double_double3(gmtl::EulerAngleZXYd* self_, const double p0, const double p1, const double p2)
   {
      self_->set(p0, p1, p2);
   }


   // Wrapper for non-virtual method gmtl::EulerAngle<double,gmtl::ZXY>::getData()
   SHARPPY_API void gmtl_EulerAngle_double_gmtl_ZXY__getData__0(gmtl::EulerAngleZXYd* self_, double* arrayHolder)
   {
      double* temp_array;
      temp_array = self_->getData();
      for ( int i = 0; i < 3; ++i )
      {
         arrayHolder[i] = temp_array[i];
      }
   }


} // extern "C" for gmtl::EulerAngleZXYd


