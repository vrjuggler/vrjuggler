// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Sphere.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Sphered* gmtl_Sphere_double__Sphere__0()
   {
      gmtl::Sphered* obj = new gmtl::Sphered();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Sphered* gmtl_Sphere_double__Sphere__gmtl_Point3d_double2(const gmtl::Point3d* p0, const double p1)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      gmtl::Sphered* obj = new gmtl::Sphered(marshal_p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Sphered* gmtl_Sphere_double__Sphere__gmtl_Sphered1(const gmtl::Sphered* p0)
   {
      const gmtl::Sphered& marshal_p0 = *p0;
      gmtl::Sphered* obj = new gmtl::Sphered(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Sphered(gmtl::Sphered* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Sphere<double>::getCenter()
   SHARPPY_API gmtl::Point3d* gmtl_Sphere_double__getCenter__0(gmtl::Sphered* self_)
   {
      gmtl::Point3d* result;
      result = new gmtl::Point3d(self_->getCenter());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Sphere<double>::getRadius()
   SHARPPY_API  double gmtl_Sphere_double__getRadius__0(gmtl::Sphered* self_)
   {
       double result;
      result = self_->getRadius();
      return result;
   }


   // Wrapper for non-virtual method gmtl::Sphere<double>::setCenter()
   SHARPPY_API void gmtl_Sphere_double__setCenter__gmtl_Point3d1(gmtl::Sphered* self_, const gmtl::Point3d* p0)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      self_->setCenter(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::Sphere<double>::setRadius()
   SHARPPY_API void gmtl_Sphere_double__setRadius__double1(gmtl::Sphered* self_, const double p0)
   {
      self_->setRadius(p0);
   }


} // extern "C" for gmtl::Sphered


