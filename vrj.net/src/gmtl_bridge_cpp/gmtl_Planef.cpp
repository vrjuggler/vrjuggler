// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Plane.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Planef* gmtl_Plane_float__Plane__0()
   {
      gmtl::Planef* obj = new gmtl::Planef();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planef* gmtl_Plane_float__Plane__gmtl_Point3f_gmtl_Point3f_gmtl_Point3f3(const gmtl::Point3f* p0, const gmtl::Point3f* p1, const gmtl::Point3f* p2)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      const gmtl::Point3f& marshal_p2 = *p2;
      gmtl::Planef* obj = new gmtl::Planef(marshal_p0, marshal_p1, marshal_p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planef* gmtl_Plane_float__Plane__gmtl_Vec3f_gmtl_Point3f2(const gmtl::Vec3f* p0, const gmtl::Point3f* p1)
   {
      const gmtl::Vec3f& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      gmtl::Planef* obj = new gmtl::Planef(marshal_p0, marshal_p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planef* gmtl_Plane_float__Plane__gmtl_Vec3f_float2(const gmtl::Vec3f* p0, const float p1)
   {
      const gmtl::Vec3f& marshal_p0 = *p0;
      gmtl::Planef* obj = new gmtl::Planef(marshal_p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planef* gmtl_Plane_float__Plane__gmtl_Planef1(const gmtl::Planef* p0)
   {
      const gmtl::Planef& marshal_p0 = *p0;
      gmtl::Planef* obj = new gmtl::Planef(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Planef(gmtl::Planef* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Plane<float>::getNormal()
   SHARPPY_API gmtl::Vec3f* gmtl_Plane_float__getNormal__0(gmtl::Planef* self_)
   {
      gmtl::Vec3f* result;
      result = new gmtl::Vec3f(self_->getNormal());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Plane<float>::setNormal()
   SHARPPY_API void gmtl_Plane_float__setNormal__gmtl_Vec3f1(gmtl::Planef* self_, const gmtl::Vec3f* p0)
   {
      const gmtl::Vec3f& marshal_p0 = *p0;
      self_->setNormal(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::Plane<float>::getOffset()
   SHARPPY_API  float gmtl_Plane_float__getOffset__0(gmtl::Planef* self_)
   {
       float result;
      result = self_->getOffset();
      return result;
   }


   // Wrapper for non-virtual method gmtl::Plane<float>::setOffset()
   SHARPPY_API void gmtl_Plane_float__setOffset__float1(gmtl::Planef* self_, const float p0)
   {
      self_->setOffset(p0);
   }


} // extern "C" for gmtl::Planef


