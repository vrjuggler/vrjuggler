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
   SHARPPY_API gmtl::Planed* gmtl_Plane_double__Plane__0()
   {
      gmtl::Planed* obj = new gmtl::Planed();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planed* gmtl_Plane_double__Plane__gmtl_Point3d_gmtl_Point3d_gmtl_Point3d3(const gmtl::Point3d* p0, const gmtl::Point3d* p1, const gmtl::Point3d* p2)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      const gmtl::Point3d& marshal_p1 = *p1;
      const gmtl::Point3d& marshal_p2 = *p2;
      gmtl::Planed* obj = new gmtl::Planed(marshal_p0, marshal_p1, marshal_p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planed* gmtl_Plane_double__Plane__gmtl_Vec3d_gmtl_Point3d2(const gmtl::Vec3d* p0, const gmtl::Point3d* p1)
   {
      const gmtl::Vec3d& marshal_p0 = *p0;
      const gmtl::Point3d& marshal_p1 = *p1;
      gmtl::Planed* obj = new gmtl::Planed(marshal_p0, marshal_p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planed* gmtl_Plane_double__Plane__gmtl_Vec3d_double2(const gmtl::Vec3d* p0, const double p1)
   {
      const gmtl::Vec3d& marshal_p0 = *p0;
      gmtl::Planed* obj = new gmtl::Planed(marshal_p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Planed* gmtl_Plane_double__Plane__gmtl_Planed1(const gmtl::Planed* p0)
   {
      const gmtl::Planed& marshal_p0 = *p0;
      gmtl::Planed* obj = new gmtl::Planed(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Planed(gmtl::Planed* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Plane<double>::getNormal()
   SHARPPY_API gmtl::Vec3d* gmtl_Plane_double__getNormal__0(gmtl::Planed* self_)
   {
      gmtl::Vec3d* result;
      result = new gmtl::Vec3d(self_->getNormal());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Plane<double>::setNormal()
   SHARPPY_API void gmtl_Plane_double__setNormal__gmtl_Vec3d1(gmtl::Planed* self_, const gmtl::Vec3d* p0)
   {
      const gmtl::Vec3d& marshal_p0 = *p0;
      self_->setNormal(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::Plane<double>::getOffset()
   SHARPPY_API  double gmtl_Plane_double__getOffset__0(gmtl::Planed* self_)
   {
       double result;
      result = self_->getOffset();
      return result;
   }


   // Wrapper for non-virtual method gmtl::Plane<double>::setOffset()
   SHARPPY_API void gmtl_Plane_double__setOffset__double1(gmtl::Planed* self_, const double p0)
   {
      self_->setOffset(p0);
   }


} // extern "C" for gmtl::Planed


