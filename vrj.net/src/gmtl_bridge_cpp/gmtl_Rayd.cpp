// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Ray.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Rayd* gmtl_Ray_double__Ray__0()
   {
      gmtl::Rayd* obj = new gmtl::Rayd();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Rayd* gmtl_Ray_double__Ray__gmtl_Point3d_gmtl_Vec3d2(const gmtl::Point3d* p0, const gmtl::Vec3d* p1)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      const gmtl::Vec3d& marshal_p1 = *p1;
      gmtl::Rayd* obj = new gmtl::Rayd(marshal_p0, marshal_p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Rayd* gmtl_Ray_double__Ray__gmtl_Rayd1(const gmtl::Rayd* p0)
   {
      const gmtl::Rayd& marshal_p0 = *p0;
      gmtl::Rayd* obj = new gmtl::Rayd(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Rayd(gmtl::Rayd* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Ray<double>::getOrigin()
   SHARPPY_API gmtl::Point3d* gmtl_Ray_double__getOrigin__0(gmtl::Rayd* self_)
   {
      gmtl::Point3d* result;
      result = new gmtl::Point3d(self_->getOrigin());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Ray<double>::setOrigin()
   SHARPPY_API void gmtl_Ray_double__setOrigin__gmtl_Point3d1(gmtl::Rayd* self_, const gmtl::Point3d* p0)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      self_->setOrigin(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::Ray<double>::getDir()
   SHARPPY_API gmtl::Vec3d* gmtl_Ray_double__getDir__0(gmtl::Rayd* self_)
   {
      gmtl::Vec3d* result;
      result = new gmtl::Vec3d(self_->getDir());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Ray<double>::setDir()
   SHARPPY_API void gmtl_Ray_double__setDir__gmtl_Vec3d1(gmtl::Rayd* self_, const gmtl::Vec3d* p0)
   {
      const gmtl::Vec3d& marshal_p0 = *p0;
      self_->setDir(marshal_p0);
   }


} // extern "C" for gmtl::Rayd


