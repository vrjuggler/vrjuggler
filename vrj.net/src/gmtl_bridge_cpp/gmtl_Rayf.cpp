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
   SHARPPY_API gmtl::Rayf* gmtl_Ray_float__Ray__0()
   {
      gmtl::Rayf* obj = new gmtl::Rayf();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Rayf* gmtl_Ray_float__Ray__gmtl_Point3f_gmtl_Vec3f2(const gmtl::Point3f* p0, const gmtl::Vec3f* p1)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      const gmtl::Vec3f& marshal_p1 = *p1;
      gmtl::Rayf* obj = new gmtl::Rayf(marshal_p0, marshal_p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Rayf* gmtl_Ray_float__Ray__gmtl_Rayf1(const gmtl::Rayf* p0)
   {
      const gmtl::Rayf& marshal_p0 = *p0;
      gmtl::Rayf* obj = new gmtl::Rayf(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Rayf(gmtl::Rayf* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Ray<float>::getOrigin()
   SHARPPY_API gmtl::Point3f* gmtl_Ray_float__getOrigin__0(gmtl::Rayf* self_)
   {
      gmtl::Point3f* result;
      result = new gmtl::Point3f(self_->getOrigin());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Ray<float>::setOrigin()
   SHARPPY_API void gmtl_Ray_float__setOrigin__gmtl_Point3f1(gmtl::Rayf* self_, const gmtl::Point3f* p0)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      self_->setOrigin(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::Ray<float>::getDir()
   SHARPPY_API gmtl::Vec3f* gmtl_Ray_float__getDir__0(gmtl::Rayf* self_)
   {
      gmtl::Vec3f* result;
      result = new gmtl::Vec3f(self_->getDir());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Ray<float>::setDir()
   SHARPPY_API void gmtl_Ray_float__setDir__gmtl_Vec3f1(gmtl::Rayf* self_, const gmtl::Vec3f* p0)
   {
      const gmtl::Vec3f& marshal_p0 = *p0;
      self_->setDir(marshal_p0);
   }


} // extern "C" for gmtl::Rayf


