// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Quat.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Quatd* gmtl_Quat_double__Quat__0()
   {
      gmtl::Quatd* obj = new gmtl::Quatd();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Quatd* gmtl_Quat_double__Quat__double_double_double_double4(const double p0, const double p1, const double p2, const double p3)
   {
      gmtl::Quatd* obj = new gmtl::Quatd(p0, p1, p2, p3);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Quatd* gmtl_Quat_double__Quat__gmtl_Quatd1(const gmtl::Quatd* p0)
   {
      const gmtl::Quatd& marshal_p0 = *p0;
      gmtl::Quatd* obj = new gmtl::Quatd(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Quatd(gmtl::Quatd* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Quat<double>::set()
   SHARPPY_API void gmtl_Quat_double__set__double_double_double_double4(gmtl::Quatd* self_, const double p0, const double p1, const double p2, const double p3)
   {
      self_->set(p0, p1, p2, p3);
   }


   // Wrapper for non-virtual method gmtl::Quat<double>::get()
   SHARPPY_API void gmtl_Quat_double__get__double_double_double_double4(gmtl::Quatd* self_, double& p0, double& p1, double& p2, double& p3)
   {
      self_->get(p0, p1, p2, p3);
   }

/*
   // Wrapper for non-virtual method gmtl::Quat<double>::getData()
   SHARPPY_API double* gmtl_Quat_double__getData__0(gmtl::Quatd* self_)
   {
      double* result;
      result = self_->getData();
      return result;
   }
*/

} // extern "C" for gmtl::Quatd


