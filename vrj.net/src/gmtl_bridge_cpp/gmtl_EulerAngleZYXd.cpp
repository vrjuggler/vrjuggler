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
   SHARPPY_API gmtl::EulerAngleZYXd* gmtl_EulerAngle_double_gmtl_ZYX__EulerAngle__0()
   {
      gmtl::EulerAngleZYXd* obj = new gmtl::EulerAngleZYXd();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZYXd* gmtl_EulerAngle_double_gmtl_ZYX__EulerAngle__gmtl_EulerAngleZYXd1(const gmtl::EulerAngleZYXd* p0)
   {
      const gmtl::EulerAngleZYXd& marshal_p0 = *p0;
      gmtl::EulerAngleZYXd* obj = new gmtl::EulerAngleZYXd(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZYXd* gmtl_EulerAngle_double_gmtl_ZYX__EulerAngle__double_double_double3(double p0, double p1, double p2)
   {
      gmtl::EulerAngleZYXd* obj = new gmtl::EulerAngleZYXd(p0, p1, p2);
      return obj;
   }

   SHARPPY_API void delete_gmtl_EulerAngleZYXd(gmtl::EulerAngleZYXd* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::EulerAngle<double,gmtl::ZYX>::set()
   SHARPPY_API void gmtl_EulerAngle_double_gmtl_ZYX__set__double_double_double3(gmtl::EulerAngleZYXd* self_, const double p0, const double p1, const double p2)
   {
      self_->set(p0, p1, p2);
   }


   // Wrapper for non-virtual method gmtl::EulerAngle<double,gmtl::ZYX>::getData()
   SHARPPY_API void gmtl_EulerAngle_double_gmtl_ZYX__getData__0(gmtl::EulerAngleZYXd* self_, double* arrayHolder)
   {
      double* temp_array;
      temp_array = self_->getData();
      for ( int i = 0; i < 3; ++i )
      {
         arrayHolder[i] = temp_array[i];
      }
   }


} // extern "C" for gmtl::EulerAngleZYXd


