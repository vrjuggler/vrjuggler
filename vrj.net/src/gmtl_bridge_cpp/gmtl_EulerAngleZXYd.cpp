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
#include <gmtl/EulerAngle.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZXYd* gmtl_EulerAngle_double_gmtl_ZXY__EulerAngle__()
   {
      gmtl::EulerAngleZXYd* obj = new gmtl::EulerAngleZXYd();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZXYd* gmtl_EulerAngle_double_gmtl_ZXY__EulerAngle__gmtl_EulerAngleZXYd(const gmtl::EulerAngleZXYd* p0)
   {
      const gmtl::EulerAngleZXYd& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::EulerAngleZXYd* obj = new gmtl::EulerAngleZXYd(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::EulerAngleZXYd* gmtl_EulerAngle_double_gmtl_ZXY__EulerAngle__double_double_double(double p0, double p1, double p2)
   {
      gmtl::EulerAngleZXYd* obj = new gmtl::EulerAngleZXYd(p0, p1, p2);
      return obj;
   }

   SHARPPY_API void delete_gmtl_EulerAngleZXYd(gmtl::EulerAngleZXYd* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::EulerAngle<double,gmtl::ZXY>::set()
   SHARPPY_API void gmtl_EulerAngle_double_gmtl_ZXY__set__double_double_double3(gmtl::EulerAngleZXYd* self_, const double& p0, const double& p1, const double& p2)
   {
      self_->set(p0, p1, p2);
   }


   // Wrapper for non-virtual method gmtl::EulerAngle<double,gmtl::ZXY>::getData()
   SHARPPY_API double* gmtl_EulerAngle_double_gmtl_ZXY__getData__0(gmtl::EulerAngleZXYd* self_)
   {
      double* result;
      result = self_->getData();
      return result;
   }

/*
   // Wrapper for non-virtual method gmtl::EulerAngle<double,gmtl::ZXY>::getData()
   SHARPPY_API const double* gmtl_EulerAngle_double_gmtl_ZXY__getData__0(gmtl::EulerAngleZXYd* self_)
   {
      const double* result;
      result = self_->getData();
      return result;
   }
*/

} // extern "C" for gmtl::EulerAngleZXYd


