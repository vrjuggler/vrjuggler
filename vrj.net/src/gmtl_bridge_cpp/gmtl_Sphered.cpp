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
#include <gmtl/Sphere.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Sphered* gmtl_Sphere_double__Sphere__()
   {
      gmtl::Sphered* obj = new gmtl::Sphered();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Sphered* gmtl_Sphere_double__Sphere__gmtl_Point3d_double(const gmtl::Point3d* p0, const double& p1)
   {
      const gmtl::Point3d& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Sphered* obj = new gmtl::Sphered(marshal_p0, p1);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Sphered* gmtl_Sphere_double__Sphere__gmtl_Sphered(const gmtl::Sphered* p0)
   {
      const gmtl::Sphered& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Sphered* obj = new gmtl::Sphered(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   SHARPPY_API void delete_gmtl_Sphered(gmtl::Sphered* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Sphere<double>::getCenter()
   SHARPPY_API const gmtl::Point3d* gmtl_Sphere_double__getCenter__0(gmtl::Sphered* self_)
   {
      const gmtl::Point3d* result;
      result = new const gmtl::Point3d(self_->getCenter());
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
   SHARPPY_API void gmtl_Sphere_double__setRadius__double1(gmtl::Sphered* self_, const double& p0)
   {
      self_->setRadius(p0);
   }


} // extern "C" for gmtl::Sphered


