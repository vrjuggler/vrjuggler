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

// Generated from Revision: 1.3 of RCSfile: free_types_cxx.tmpl,v


#include "sharppy.h"
#include <gmtl-free-types.h>

extern "C"
{

   // Wrapper for free function gmtl::add()
   SHARPPY_API gmtl::Quatd* gmtl_add__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd3(gmtl::Quatd* p0, const gmtl::Quatd* p1, const gmtl::Quatd* p2)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p1 = *p1;
      const gmtl::Quatd& marshal_p2 = *p2;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::add(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::add()
   SHARPPY_API gmtl::Matrix44f* gmtl_add__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f3(gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1, const gmtl::Matrix44f* p2)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      const gmtl::Matrix44f& marshal_p2 = *p2;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::add(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::add()
   SHARPPY_API gmtl::Matrix33f* gmtl_add__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f3(gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1, const gmtl::Matrix33f* p2)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      const gmtl::Matrix33f& marshal_p2 = *p2;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::add(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::add()
   SHARPPY_API gmtl::Quatf* gmtl_add__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf3(gmtl::Quatf* p0, const gmtl::Quatf* p1, const gmtl::Quatf* p2)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p1 = *p1;
      const gmtl::Quatf& marshal_p2 = *p2;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::add(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::center()
   SHARPPY_API gmtl::Point3d* gmtl_center__gmtl_Trid1(const gmtl::Trid* p0)
   {
      const gmtl::Trid& marshal_p0 = *p0;
      gmtl::Point3d* result;
      result = new gmtl::Point3d(gmtl::center(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::center()
   SHARPPY_API gmtl::Point3f* gmtl_center__gmtl_Trif1(const gmtl::Trif* p0)
   {
      const gmtl::Trif& marshal_p0 = *p0;
      gmtl::Point3f* result;
      result = new gmtl::Point3f(gmtl::center(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::center()
   SHARPPY_API gmtl::Point3i* gmtl_center__gmtl_Trii1(const gmtl::Trii* p0)
   {
      const gmtl::Trii& marshal_p0 = *p0;
      gmtl::Point3i* result;
      result = new gmtl::Point3i(gmtl::center(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::conj()
   SHARPPY_API gmtl::Quatd* gmtl_conj__gmtl_Quatd1(gmtl::Quatd* p0)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::conj(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::conj()
   SHARPPY_API gmtl::Quatf* gmtl_conj__gmtl_Quatf1(gmtl::Quatf* p0)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::conj(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::cross()
   SHARPPY_API gmtl::Vec3f* gmtl_cross__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f3(gmtl::Vec3f* p0, const gmtl::Vec3f* p1, const gmtl::Vec3f* p2)
   {
      gmtl::Vec3f& marshal_p0 = *p0;
      const gmtl::Vec3f& marshal_p1 = *p1;
      const gmtl::Vec3f& marshal_p2 = *p2;
      gmtl::Vec3f* result;
      result = new gmtl::Vec3f(gmtl::cross(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::cross()
   SHARPPY_API gmtl::Vec3i* gmtl_cross__gmtl_Vec3i_gmtl_Vec3i_gmtl_Vec3i3(gmtl::Vec3i* p0, const gmtl::Vec3i* p1, const gmtl::Vec3i* p2)
   {
      gmtl::Vec3i& marshal_p0 = *p0;
      const gmtl::Vec3i& marshal_p1 = *p1;
      const gmtl::Vec3i& marshal_p2 = *p2;
      gmtl::Vec3i* result;
      result = new gmtl::Vec3i(gmtl::cross(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::cross()
   SHARPPY_API gmtl::Vec3d* gmtl_cross__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d3(gmtl::Vec3d* p0, const gmtl::Vec3d* p1, const gmtl::Vec3d* p2)
   {
      gmtl::Vec3d& marshal_p0 = *p0;
      const gmtl::Vec3d& marshal_p1 = *p1;
      const gmtl::Vec3d& marshal_p2 = *p2;
      gmtl::Vec3d* result;
      result = new gmtl::Vec3d(gmtl::cross(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::distance()
   SHARPPY_API double gmtl_distance__gmtl_LineSegd_gmtl_Point3d2(const gmtl::LineSegd* p0, const gmtl::Point3d* p1)
   {
      const gmtl::LineSegd& marshal_p0 = *p0;
      const gmtl::Point3d& marshal_p1 = *p1;
      double result;
      result = gmtl::distance(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::distance()
   SHARPPY_API float gmtl_distance__gmtl_LineSegf_gmtl_Point3f2(const gmtl::LineSegf* p0, const gmtl::Point3f* p1)
   {
      const gmtl::LineSegf& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      float result;
      result = gmtl::distance(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::distance()
   SHARPPY_API double gmtl_distance__gmtl_Planed_gmtl_Point3d2(const gmtl::Planed* p0, const gmtl::Point3d* p1)
   {
      const gmtl::Planed& marshal_p0 = *p0;
      const gmtl::Point3d& marshal_p1 = *p1;
      double result;
      result = gmtl::distance(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::distance()
   SHARPPY_API float gmtl_distance__gmtl_Planef_gmtl_Point3f2(const gmtl::Planef* p0, const gmtl::Point3f* p1)
   {
      const gmtl::Planef& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      float result;
      result = gmtl::distance(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::distanceSquared()
   SHARPPY_API double gmtl_distanceSquared__gmtl_LineSegd_gmtl_Point3d2(const gmtl::LineSegd* p0, const gmtl::Point3d* p1)
   {
      const gmtl::LineSegd& marshal_p0 = *p0;
      const gmtl::Point3d& marshal_p1 = *p1;
      double result;
      result = gmtl::distanceSquared(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::distanceSquared()
   SHARPPY_API float gmtl_distanceSquared__gmtl_LineSegf_gmtl_Point3f2(const gmtl::LineSegf* p0, const gmtl::Point3f* p1)
   {
      const gmtl::LineSegf& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      float result;
      result = gmtl::distanceSquared(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::div()
   SHARPPY_API gmtl::Quatf* gmtl_div__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf3(gmtl::Quatf* p0, const gmtl::Quatf* p1, gmtl::Quatf p2)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p1 = *p1;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::div(marshal_p0, marshal_p1, p2));
      return result;
   }


   // Wrapper for free function gmtl::div()
   SHARPPY_API gmtl::Quatf* gmtl_div__gmtl_Quatf_gmtl_Quatf_float3(gmtl::Quatf* p0, const gmtl::Quatf* p1, float p2)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p1 = *p1;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::div(marshal_p0, marshal_p1, p2));
      return result;
   }


   // Wrapper for free function gmtl::div()
   SHARPPY_API gmtl::Quatd* gmtl_div__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd3(gmtl::Quatd* p0, const gmtl::Quatd* p1, gmtl::Quatd p2)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p1 = *p1;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::div(marshal_p0, marshal_p1, p2));
      return result;
   }


   // Wrapper for free function gmtl::div()
   SHARPPY_API gmtl::Quatd* gmtl_div__gmtl_Quatd_gmtl_Quatd_double3(gmtl::Quatd* p0, const gmtl::Quatd* p1, double p2)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p1 = *p1;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::div(marshal_p0, marshal_p1, p2));
      return result;
   }


   // Wrapper for free function gmtl::dot()
   SHARPPY_API float gmtl_dot__gmtl_Quatf_gmtl_Quatf2(const gmtl::Quatf* p0, const gmtl::Quatf* p1)
   {
      const gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p1 = *p1;
      float result;
      result = gmtl::dot(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::dot()
   SHARPPY_API double gmtl_dot__gmtl_Quatd_gmtl_Quatd2(const gmtl::Quatd* p0, const gmtl::Quatd* p1)
   {
      const gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p1 = *p1;
      double result;
      result = gmtl::dot(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::dot()
   SHARPPY_API double gmtl_dot__gmtl_Vec3d_gmtl_Vec3d2(const gmtl::Vec3d* p0, const gmtl::Vec3d* p1)
   {
      const gmtl::Vec3d& marshal_p0 = *p0;
      const gmtl::Vec3d& marshal_p1 = *p1;
      double result;
      result = gmtl::dot(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::dot()
   SHARPPY_API float gmtl_dot__gmtl_Vec3f_gmtl_Vec3f2(const gmtl::Vec3f* p0, const gmtl::Vec3f* p1)
   {
      const gmtl::Vec3f& marshal_p0 = *p0;
      const gmtl::Vec3f& marshal_p1 = *p1;
      float result;
      result = gmtl::dot(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::dot()
   SHARPPY_API float gmtl_dot__gmtl_Vec4f_gmtl_Vec4f2(const gmtl::Vec4f* p0, const gmtl::Vec4f* p1)
   {
      const gmtl::Vec4f& marshal_p0 = *p0;
      const gmtl::Vec4f& marshal_p1 = *p1;
      float result;
      result = gmtl::dot(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::dot()
   SHARPPY_API double gmtl_dot__gmtl_Vec4d_gmtl_Vec4d2(const gmtl::Vec4d* p0, const gmtl::Vec4d* p1)
   {
      const gmtl::Vec4d& marshal_p0 = *p0;
      const gmtl::Vec4d& marshal_p1 = *p1;
      double result;
      result = gmtl::dot(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::exp()
   SHARPPY_API gmtl::Quatd* gmtl_exp__gmtl_Quatd1(gmtl::Quatd* p0)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::exp(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::exp()
   SHARPPY_API gmtl::Quatf* gmtl_exp__gmtl_Quatf1(gmtl::Quatf* p0)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::exp(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::findNearestPt()
   SHARPPY_API gmtl::Point3d* gmtl_findNearestPt__gmtl_LineSegd_gmtl_Point3d2(const gmtl::LineSegd* p0, const gmtl::Point3d* p1)
   {
      const gmtl::LineSegd& marshal_p0 = *p0;
      const gmtl::Point3d& marshal_p1 = *p1;
      gmtl::Point3d* result;
      result = new gmtl::Point3d(gmtl::findNearestPt(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::findNearestPt()
   SHARPPY_API gmtl::Point3f* gmtl_findNearestPt__gmtl_LineSegf_gmtl_Point3f2(const gmtl::LineSegf* p0, const gmtl::Point3f* p1)
   {
      const gmtl::LineSegf& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      gmtl::Point3f* result;
      result = new gmtl::Point3f(gmtl::findNearestPt(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::findNearestPt()
   SHARPPY_API float gmtl_findNearestPt__gmtl_Planef_gmtl_Point3f_gmtl_Point3f3(const gmtl::Planef* p0, const gmtl::Point3f* p1, gmtl::Point3f* p2)
   {
      const gmtl::Planef& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      gmtl::Point3f& marshal_p2 = *p2;
      float result;
      result = gmtl::findNearestPt(marshal_p0, marshal_p1, marshal_p2);
      return result;
   }


   // Wrapper for free function gmtl::findNearestPt()
   SHARPPY_API double gmtl_findNearestPt__gmtl_Planed_gmtl_Point3d_gmtl_Point3d3(const gmtl::Planed* p0, const gmtl::Point3d* p1, gmtl::Point3d* p2)
   {
      const gmtl::Planed& marshal_p0 = *p0;
      const gmtl::Point3d& marshal_p1 = *p1;
      gmtl::Point3d& marshal_p2 = *p2;
      double result;
      result = gmtl::findNearestPt(marshal_p0, marshal_p1, marshal_p2);
      return result;
   }


   // Wrapper for free function gmtl::identity()
   SHARPPY_API gmtl::Matrix44f* gmtl_identity__gmtl_Matrix44f1(gmtl::Matrix44f* p0)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::identity(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::identity()
   SHARPPY_API gmtl::Matrix33f* gmtl_identity__gmtl_Matrix33f1(gmtl::Matrix33f* p0)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::identity(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::invert()
   SHARPPY_API gmtl::Quatd* gmtl_invert__gmtl_Quatd1(gmtl::Quatd* p0)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::invert(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::invert()
   SHARPPY_API gmtl::Quatf* gmtl_invert__gmtl_Quatf1(gmtl::Quatf* p0)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::invert(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::invert()
   SHARPPY_API gmtl::Matrix44f* gmtl_invert__gmtl_Matrix44f1(gmtl::Matrix44f* p0)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::invert(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::invert()
   SHARPPY_API gmtl::Matrix33f* gmtl_invert__gmtl_Matrix33f1(gmtl::Matrix33f* p0)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::invert(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::invert()
   SHARPPY_API gmtl::Matrix44f* gmtl_invert__gmtl_Matrix44f_gmtl_Matrix44f2(gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::invert(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::invert()
   SHARPPY_API gmtl::Matrix33f* gmtl_invert__gmtl_Matrix33f_gmtl_Matrix33f2(gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::invert(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::invertAffine()
   SHARPPY_API gmtl::Matrix44f* gmtl_invertAffine__gmtl_Matrix44f_gmtl_Matrix44f2(gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::invertAffine(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::invertAffine()
   SHARPPY_API gmtl::Matrix33f* gmtl_invertAffine__gmtl_Matrix33f_gmtl_Matrix33f2(gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::invertAffine(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::invertFull()
   SHARPPY_API gmtl::Matrix44f* gmtl_invertFull__gmtl_Matrix44f_gmtl_Matrix44f2(gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::invertFull(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::invertFull()
   SHARPPY_API gmtl::Matrix33f* gmtl_invertFull__gmtl_Matrix33f_gmtl_Matrix33f2(gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::invertFull(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::invertOrthogonal()
   SHARPPY_API gmtl::Matrix33f* gmtl_invertOrthogonal__gmtl_Matrix33f_gmtl_Matrix33f2(gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::invertOrthogonal(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::invertOrthogonal()
   SHARPPY_API gmtl::Matrix44f* gmtl_invertOrthogonal__gmtl_Matrix44f_gmtl_Matrix44f2(gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::invertOrthogonal(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::invertTrans()
   SHARPPY_API gmtl::Matrix33f* gmtl_invertTrans__gmtl_Matrix33f_gmtl_Matrix33f2(gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::invertTrans(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::invertTrans()
   SHARPPY_API gmtl::Matrix44f* gmtl_invertTrans__gmtl_Matrix44f_gmtl_Matrix44f2(gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::invertTrans(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Sphered_gmtl_Sphered_double3(const gmtl::Sphered* p0, const gmtl::Sphered* p1, const double& p2)
   {
      const gmtl::Sphered& marshal_p0 = *p0;
      const gmtl::Sphered& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Spheref_gmtl_Spheref_float3(const gmtl::Spheref* p0, const gmtl::Spheref* p1, const float& p2)
   {
      const gmtl::Spheref& marshal_p0 = *p0;
      const gmtl::Spheref& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_AxisAngled_gmtl_AxisAngled_double2(const gmtl::AxisAngled* p0, const gmtl::AxisAngled* p1)
   {
      const gmtl::AxisAngled& marshal_p0 = *p0;
      const gmtl::AxisAngled& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_AxisAngled_gmtl_AxisAngled_double3(const gmtl::AxisAngled* p0, const gmtl::AxisAngled* p1, const double& p2)
   {
      const gmtl::AxisAngled& marshal_p0 = *p0;
      const gmtl::AxisAngled& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_AABoxf_gmtl_AABoxf_float3(const gmtl::AABoxf* p0, const gmtl::AABoxf* p1, const float& p2)
   {
      const gmtl::AABoxf& marshal_p0 = *p0;
      const gmtl::AABoxf& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_VecBase_double_4_gmtl_VecBase_double_4_double3(const gmtl::VecBase<double,4>* p0, const gmtl::VecBase<double,4>* p1, const double& p2)
   {
      const gmtl::VecBase<double,4>& marshal_p0 = *p0;
      const gmtl::VecBase<double,4>& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_VecBase_double_3_gmtl_VecBase_double_3_double3(const gmtl::VecBase<double,3>* p0, const gmtl::VecBase<double,3>* p1, const double& p2)
   {
      const gmtl::VecBase<double,3>& marshal_p0 = *p0;
      const gmtl::VecBase<double,3>& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_AxisAnglef_gmtl_AxisAnglef_float2(const gmtl::AxisAnglef* p0, const gmtl::AxisAnglef* p1)
   {
      const gmtl::AxisAnglef& marshal_p0 = *p0;
      const gmtl::AxisAnglef& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_AxisAnglef_gmtl_AxisAnglef_float3(const gmtl::AxisAnglef* p0, const gmtl::AxisAnglef* p1, const float& p2)
   {
      const gmtl::AxisAnglef& marshal_p0 = *p0;
      const gmtl::AxisAnglef& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_AABoxd_gmtl_AABoxd_double3(const gmtl::AABoxd* p0, const gmtl::AABoxd* p1, const double& p2)
   {
      const gmtl::AABoxd& marshal_p0 = *p0;
      const gmtl::AABoxd& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_VecBase_float_4_gmtl_VecBase_float_4_float3(const gmtl::VecBase<float,4>* p0, const gmtl::VecBase<float,4>* p1, const float& p2)
   {
      const gmtl::VecBase<float,4>& marshal_p0 = *p0;
      const gmtl::VecBase<float,4>& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_VecBase_float_3_gmtl_VecBase_float_3_float3(const gmtl::VecBase<float,3>* p0, const gmtl::VecBase<float,3>* p1, const float& p2)
   {
      const gmtl::VecBase<float,3>& marshal_p0 = *p0;
      const gmtl::VecBase<float,3>& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4dAxisAngle_gmtl_Coord4dAxisAngle_double2(const gmtl::Coord4dAxisAngle* p0, const gmtl::Coord4dAxisAngle* p1)
   {
      const gmtl::Coord4dAxisAngle& marshal_p0 = *p0;
      const gmtl::Coord4dAxisAngle& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4dAxisAngle_gmtl_Coord4dAxisAngle_double3(const gmtl::Coord4dAxisAngle* p0, const gmtl::Coord4dAxisAngle* p1, double p2)
   {
      const gmtl::Coord4dAxisAngle& marshal_p0 = *p0;
      const gmtl::Coord4dAxisAngle& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3dAxisAngle_gmtl_Coord3dAxisAngle_double2(const gmtl::Coord3dAxisAngle* p0, const gmtl::Coord3dAxisAngle* p1)
   {
      const gmtl::Coord3dAxisAngle& marshal_p0 = *p0;
      const gmtl::Coord3dAxisAngle& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3dAxisAngle_gmtl_Coord3dAxisAngle_double3(const gmtl::Coord3dAxisAngle* p0, const gmtl::Coord3dAxisAngle* p1, double p2)
   {
      const gmtl::Coord3dAxisAngle& marshal_p0 = *p0;
      const gmtl::Coord3dAxisAngle& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4dXYZ_gmtl_Coord4dXYZ_double2(const gmtl::Coord4dXYZ* p0, const gmtl::Coord4dXYZ* p1)
   {
      const gmtl::Coord4dXYZ& marshal_p0 = *p0;
      const gmtl::Coord4dXYZ& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4dXYZ_gmtl_Coord4dXYZ_double3(const gmtl::Coord4dXYZ* p0, const gmtl::Coord4dXYZ* p1, double p2)
   {
      const gmtl::Coord4dXYZ& marshal_p0 = *p0;
      const gmtl::Coord4dXYZ& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3fAxisAngle_gmtl_Coord3fAxisAngle_float2(const gmtl::Coord3fAxisAngle* p0, const gmtl::Coord3fAxisAngle* p1)
   {
      const gmtl::Coord3fAxisAngle& marshal_p0 = *p0;
      const gmtl::Coord3fAxisAngle& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3fAxisAngle_gmtl_Coord3fAxisAngle_float3(const gmtl::Coord3fAxisAngle* p0, const gmtl::Coord3fAxisAngle* p1, float p2)
   {
      const gmtl::Coord3fAxisAngle& marshal_p0 = *p0;
      const gmtl::Coord3fAxisAngle& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4dQuat_gmtl_Coord4dQuat_double2(const gmtl::Coord4dQuat* p0, const gmtl::Coord4dQuat* p1)
   {
      const gmtl::Coord4dQuat& marshal_p0 = *p0;
      const gmtl::Coord4dQuat& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4dQuat_gmtl_Coord4dQuat_double3(const gmtl::Coord4dQuat* p0, const gmtl::Coord4dQuat* p1, double p2)
   {
      const gmtl::Coord4dQuat& marshal_p0 = *p0;
      const gmtl::Coord4dQuat& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4fAxisAngle_gmtl_Coord4fAxisAngle_float2(const gmtl::Coord4fAxisAngle* p0, const gmtl::Coord4fAxisAngle* p1)
   {
      const gmtl::Coord4fAxisAngle& marshal_p0 = *p0;
      const gmtl::Coord4fAxisAngle& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4fAxisAngle_gmtl_Coord4fAxisAngle_float3(const gmtl::Coord4fAxisAngle* p0, const gmtl::Coord4fAxisAngle* p1, float p2)
   {
      const gmtl::Coord4fAxisAngle& marshal_p0 = *p0;
      const gmtl::Coord4fAxisAngle& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3fQuat_gmtl_Coord3fQuat_float2(const gmtl::Coord3fQuat* p0, const gmtl::Coord3fQuat* p1)
   {
      const gmtl::Coord3fQuat& marshal_p0 = *p0;
      const gmtl::Coord3fQuat& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3fQuat_gmtl_Coord3fQuat_float3(const gmtl::Coord3fQuat* p0, const gmtl::Coord3fQuat* p1, float p2)
   {
      const gmtl::Coord3fQuat& marshal_p0 = *p0;
      const gmtl::Coord3fQuat& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4dZXY_gmtl_Coord4dZXY_double2(const gmtl::Coord4dZXY* p0, const gmtl::Coord4dZXY* p1)
   {
      const gmtl::Coord4dZXY& marshal_p0 = *p0;
      const gmtl::Coord4dZXY& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4dZXY_gmtl_Coord4dZXY_double3(const gmtl::Coord4dZXY* p0, const gmtl::Coord4dZXY* p1, double p2)
   {
      const gmtl::Coord4dZXY& marshal_p0 = *p0;
      const gmtl::Coord4dZXY& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4fQuat_gmtl_Coord4fQuat_float2(const gmtl::Coord4fQuat* p0, const gmtl::Coord4fQuat* p1)
   {
      const gmtl::Coord4fQuat& marshal_p0 = *p0;
      const gmtl::Coord4fQuat& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4fQuat_gmtl_Coord4fQuat_float3(const gmtl::Coord4fQuat* p0, const gmtl::Coord4fQuat* p1, float p2)
   {
      const gmtl::Coord4fQuat& marshal_p0 = *p0;
      const gmtl::Coord4fQuat& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3dQuat_gmtl_Coord3dQuat_double2(const gmtl::Coord3dQuat* p0, const gmtl::Coord3dQuat* p1)
   {
      const gmtl::Coord3dQuat& marshal_p0 = *p0;
      const gmtl::Coord3dQuat& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3dQuat_gmtl_Coord3dQuat_double3(const gmtl::Coord3dQuat* p0, const gmtl::Coord3dQuat* p1, double p2)
   {
      const gmtl::Coord3dQuat& marshal_p0 = *p0;
      const gmtl::Coord3dQuat& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4dZYX_gmtl_Coord4dZYX_double2(const gmtl::Coord4dZYX* p0, const gmtl::Coord4dZYX* p1)
   {
      const gmtl::Coord4dZYX& marshal_p0 = *p0;
      const gmtl::Coord4dZYX& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4dZYX_gmtl_Coord4dZYX_double3(const gmtl::Coord4dZYX* p0, const gmtl::Coord4dZYX* p1, double p2)
   {
      const gmtl::Coord4dZYX& marshal_p0 = *p0;
      const gmtl::Coord4dZYX& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3fXYZ_gmtl_Coord3fXYZ_float2(const gmtl::Coord3fXYZ* p0, const gmtl::Coord3fXYZ* p1)
   {
      const gmtl::Coord3fXYZ& marshal_p0 = *p0;
      const gmtl::Coord3fXYZ& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3fXYZ_gmtl_Coord3fXYZ_float3(const gmtl::Coord3fXYZ* p0, const gmtl::Coord3fXYZ* p1, float p2)
   {
      const gmtl::Coord3fXYZ& marshal_p0 = *p0;
      const gmtl::Coord3fXYZ& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3fZYX_gmtl_Coord3fZYX_float2(const gmtl::Coord3fZYX* p0, const gmtl::Coord3fZYX* p1)
   {
      const gmtl::Coord3fZYX& marshal_p0 = *p0;
      const gmtl::Coord3fZYX& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3fZYX_gmtl_Coord3fZYX_float3(const gmtl::Coord3fZYX* p0, const gmtl::Coord3fZYX* p1, float p2)
   {
      const gmtl::Coord3fZYX& marshal_p0 = *p0;
      const gmtl::Coord3fZYX& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3dXYZ_gmtl_Coord3dXYZ_double2(const gmtl::Coord3dXYZ* p0, const gmtl::Coord3dXYZ* p1)
   {
      const gmtl::Coord3dXYZ& marshal_p0 = *p0;
      const gmtl::Coord3dXYZ& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3dXYZ_gmtl_Coord3dXYZ_double3(const gmtl::Coord3dXYZ* p0, const gmtl::Coord3dXYZ* p1, double p2)
   {
      const gmtl::Coord3dXYZ& marshal_p0 = *p0;
      const gmtl::Coord3dXYZ& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3dZYX_gmtl_Coord3dZYX_double2(const gmtl::Coord3dZYX* p0, const gmtl::Coord3dZYX* p1)
   {
      const gmtl::Coord3dZYX& marshal_p0 = *p0;
      const gmtl::Coord3dZYX& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3dZYX_gmtl_Coord3dZYX_double3(const gmtl::Coord3dZYX* p0, const gmtl::Coord3dZYX* p1, double p2)
   {
      const gmtl::Coord3dZYX& marshal_p0 = *p0;
      const gmtl::Coord3dZYX& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3dZXY_gmtl_Coord3dZXY_double2(const gmtl::Coord3dZXY* p0, const gmtl::Coord3dZXY* p1)
   {
      const gmtl::Coord3dZXY& marshal_p0 = *p0;
      const gmtl::Coord3dZXY& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3dZXY_gmtl_Coord3dZXY_double3(const gmtl::Coord3dZXY* p0, const gmtl::Coord3dZXY* p1, double p2)
   {
      const gmtl::Coord3dZXY& marshal_p0 = *p0;
      const gmtl::Coord3dZXY& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4fXYZ_gmtl_Coord4fXYZ_float2(const gmtl::Coord4fXYZ* p0, const gmtl::Coord4fXYZ* p1)
   {
      const gmtl::Coord4fXYZ& marshal_p0 = *p0;
      const gmtl::Coord4fXYZ& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4fXYZ_gmtl_Coord4fXYZ_float3(const gmtl::Coord4fXYZ* p0, const gmtl::Coord4fXYZ* p1, float p2)
   {
      const gmtl::Coord4fXYZ& marshal_p0 = *p0;
      const gmtl::Coord4fXYZ& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4fZYX_gmtl_Coord4fZYX_float2(const gmtl::Coord4fZYX* p0, const gmtl::Coord4fZYX* p1)
   {
      const gmtl::Coord4fZYX& marshal_p0 = *p0;
      const gmtl::Coord4fZYX& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4fZYX_gmtl_Coord4fZYX_float3(const gmtl::Coord4fZYX* p0, const gmtl::Coord4fZYX* p1, float p2)
   {
      const gmtl::Coord4fZYX& marshal_p0 = *p0;
      const gmtl::Coord4fZYX& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4fZXY_gmtl_Coord4fZXY_float2(const gmtl::Coord4fZXY* p0, const gmtl::Coord4fZXY* p1)
   {
      const gmtl::Coord4fZXY& marshal_p0 = *p0;
      const gmtl::Coord4fZXY& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord4fZXY_gmtl_Coord4fZXY_float3(const gmtl::Coord4fZXY* p0, const gmtl::Coord4fZXY* p1, float p2)
   {
      const gmtl::Coord4fZXY& marshal_p0 = *p0;
      const gmtl::Coord4fZXY& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_EulerAngleXYZd_gmtl_EulerAngleXYZd_double2(const gmtl::EulerAngleXYZd* p0, const gmtl::EulerAngleXYZd* p1)
   {
      const gmtl::EulerAngleXYZd& marshal_p0 = *p0;
      const gmtl::EulerAngleXYZd& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_EulerAngleXYZd_gmtl_EulerAngleXYZd_double3(const gmtl::EulerAngleXYZd* p0, const gmtl::EulerAngleXYZd* p1, const double& p2)
   {
      const gmtl::EulerAngleXYZd& marshal_p0 = *p0;
      const gmtl::EulerAngleXYZd& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_EulerAngleZYXd_gmtl_EulerAngleZYXd_double2(const gmtl::EulerAngleZYXd* p0, const gmtl::EulerAngleZYXd* p1)
   {
      const gmtl::EulerAngleZYXd& marshal_p0 = *p0;
      const gmtl::EulerAngleZYXd& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_EulerAngleZYXd_gmtl_EulerAngleZYXd_double3(const gmtl::EulerAngleZYXd* p0, const gmtl::EulerAngleZYXd* p1, const double& p2)
   {
      const gmtl::EulerAngleZYXd& marshal_p0 = *p0;
      const gmtl::EulerAngleZYXd& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_EulerAngleZYXf_gmtl_EulerAngleZYXf_float2(const gmtl::EulerAngleZYXf* p0, const gmtl::EulerAngleZYXf* p1)
   {
      const gmtl::EulerAngleZYXf& marshal_p0 = *p0;
      const gmtl::EulerAngleZYXf& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_EulerAngleZYXf_gmtl_EulerAngleZYXf_float3(const gmtl::EulerAngleZYXf* p0, const gmtl::EulerAngleZYXf* p1, const float& p2)
   {
      const gmtl::EulerAngleZYXf& marshal_p0 = *p0;
      const gmtl::EulerAngleZYXf& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_EulerAngleZXYf_gmtl_EulerAngleZXYf_float2(const gmtl::EulerAngleZXYf* p0, const gmtl::EulerAngleZXYf* p1)
   {
      const gmtl::EulerAngleZXYf& marshal_p0 = *p0;
      const gmtl::EulerAngleZXYf& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_EulerAngleZXYf_gmtl_EulerAngleZXYf_float3(const gmtl::EulerAngleZXYf* p0, const gmtl::EulerAngleZXYf* p1, const float& p2)
   {
      const gmtl::EulerAngleZXYf& marshal_p0 = *p0;
      const gmtl::EulerAngleZXYf& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_EulerAngleZXYd_gmtl_EulerAngleZXYd_double2(const gmtl::EulerAngleZXYd* p0, const gmtl::EulerAngleZXYd* p1)
   {
      const gmtl::EulerAngleZXYd& marshal_p0 = *p0;
      const gmtl::EulerAngleZXYd& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_EulerAngleZXYd_gmtl_EulerAngleZXYd_double3(const gmtl::EulerAngleZXYd* p0, const gmtl::EulerAngleZXYd* p1, const double& p2)
   {
      const gmtl::EulerAngleZXYd& marshal_p0 = *p0;
      const gmtl::EulerAngleZXYd& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_EulerAngleXYZf_gmtl_EulerAngleXYZf_float2(const gmtl::EulerAngleXYZf* p0, const gmtl::EulerAngleXYZf* p1)
   {
      const gmtl::EulerAngleXYZf& marshal_p0 = *p0;
      const gmtl::EulerAngleXYZf& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_EulerAngleXYZf_gmtl_EulerAngleXYZf_float3(const gmtl::EulerAngleXYZf* p0, const gmtl::EulerAngleXYZf* p1, const float& p2)
   {
      const gmtl::EulerAngleXYZf& marshal_p0 = *p0;
      const gmtl::EulerAngleXYZf& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Trif_gmtl_Trif_float3(const gmtl::Trif* p0, const gmtl::Trif* p1, const float& p2)
   {
      const gmtl::Trif& marshal_p0 = *p0;
      const gmtl::Trif& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Trid_gmtl_Trid_double3(const gmtl::Trid* p0, const gmtl::Trid* p1, const double& p2)
   {
      const gmtl::Trid& marshal_p0 = *p0;
      const gmtl::Trid& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Quatd_gmtl_Quatd_double2(const gmtl::Quatd* p0, const gmtl::Quatd* p1)
   {
      const gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Quatd_gmtl_Quatd_double3(const gmtl::Quatd* p0, const gmtl::Quatd* p1, double p2)
   {
      const gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Quatf_gmtl_Quatf_float2(const gmtl::Quatf* p0, const gmtl::Quatf* p1)
   {
      const gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Quatf_gmtl_Quatf_float3(const gmtl::Quatf* p0, const gmtl::Quatf* p1, float p2)
   {
      const gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3fZXY_gmtl_Coord3fZXY_float2(const gmtl::Coord3fZXY* p0, const gmtl::Coord3fZXY* p1)
   {
      const gmtl::Coord3fZXY& marshal_p0 = *p0;
      const gmtl::Coord3fZXY& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Coord3fZXY_gmtl_Coord3fZXY_float3(const gmtl::Coord3fZXY* p0, const gmtl::Coord3fZXY* p1, float p2)
   {
      const gmtl::Coord3fZXY& marshal_p0 = *p0;
      const gmtl::Coord3fZXY& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Planef_gmtl_Planef_float3(const gmtl::Planef* p0, const gmtl::Planef* p1, const float& p2)
   {
      const gmtl::Planef& marshal_p0 = *p0;
      const gmtl::Planef& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Planed_gmtl_Planed_double3(const gmtl::Planed* p0, const gmtl::Planed* p1, const double& p2)
   {
      const gmtl::Planed& marshal_p0 = *p0;
      const gmtl::Planed& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Matrix33f_gmtl_Matrix33f_float2(const gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1)
   {
      const gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Matrix33f_gmtl_Matrix33f_float3(const gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1, const float& p2)
   {
      const gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Matrix44f_gmtl_Matrix44f_float2(const gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1)
   {
      const gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Matrix44f_gmtl_Matrix44f_float3(const gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1, const float& p2)
   {
      const gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEquiv()
   SHARPPY_API bool gmtl_isEquiv__gmtl_Quatd_gmtl_Quatd_double2(const gmtl::Quatd* p0, const gmtl::Quatd* p1)
   {
      const gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEquiv(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEquiv()
   SHARPPY_API bool gmtl_isEquiv__gmtl_Quatd_gmtl_Quatd_double3(const gmtl::Quatd* p0, const gmtl::Quatd* p1, double p2)
   {
      const gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEquiv(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isEquiv()
   SHARPPY_API bool gmtl_isEquiv__gmtl_Quatf_gmtl_Quatf_float2(const gmtl::Quatf* p0, const gmtl::Quatf* p1)
   {
      const gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEquiv(marshal_p0, marshal_p1);
      return result;
   }

   // Wrapper for free function gmtl::isEquiv()
   SHARPPY_API bool gmtl_isEquiv__gmtl_Quatf_gmtl_Quatf_float3(const gmtl::Quatf* p0, const gmtl::Quatf* p1, float p2)
   {
      const gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p1 = *p1;
      bool result;
      result = gmtl::isEquiv(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::isNormalized()
   SHARPPY_API bool gmtl_isNormalized__gmtl_Quatd_double1(const gmtl::Quatd* p0)
   {
      const gmtl::Quatd& marshal_p0 = *p0;
      bool result;
      result = gmtl::isNormalized(marshal_p0);
      return result;
   }

   // Wrapper for free function gmtl::isNormalized()
   SHARPPY_API bool gmtl_isNormalized__gmtl_Quatd_double2(const gmtl::Quatd* p0, const double p1)
   {
      const gmtl::Quatd& marshal_p0 = *p0;
      bool result;
      result = gmtl::isNormalized(marshal_p0, p1);
      return result;
   }


   // Wrapper for free function gmtl::isNormalized()
   SHARPPY_API bool gmtl_isNormalized__gmtl_Quatf_float1(const gmtl::Quatf* p0)
   {
      const gmtl::Quatf& marshal_p0 = *p0;
      bool result;
      result = gmtl::isNormalized(marshal_p0);
      return result;
   }

   // Wrapper for free function gmtl::isNormalized()
   SHARPPY_API bool gmtl_isNormalized__gmtl_Quatf_float2(const gmtl::Quatf* p0, const float p1)
   {
      const gmtl::Quatf& marshal_p0 = *p0;
      bool result;
      result = gmtl::isNormalized(marshal_p0, p1);
      return result;
   }


   // Wrapper for free function gmtl::isNormalized()
   SHARPPY_API bool gmtl_isNormalized__gmtl_Vec3f_float1(const gmtl::Vec3f* p0)
   {
      const gmtl::Vec3f& marshal_p0 = *p0;
      bool result;
      result = gmtl::isNormalized(marshal_p0);
      return result;
   }

   // Wrapper for free function gmtl::isNormalized()
   SHARPPY_API bool gmtl_isNormalized__gmtl_Vec3f_float2(const gmtl::Vec3f* p0, const float p1)
   {
      const gmtl::Vec3f& marshal_p0 = *p0;
      bool result;
      result = gmtl::isNormalized(marshal_p0, p1);
      return result;
   }


   // Wrapper for free function gmtl::isNormalized()
   SHARPPY_API bool gmtl_isNormalized__gmtl_Vec4f_float1(const gmtl::Vec4f* p0)
   {
      const gmtl::Vec4f& marshal_p0 = *p0;
      bool result;
      result = gmtl::isNormalized(marshal_p0);
      return result;
   }

   // Wrapper for free function gmtl::isNormalized()
   SHARPPY_API bool gmtl_isNormalized__gmtl_Vec4f_float2(const gmtl::Vec4f* p0, const float p1)
   {
      const gmtl::Vec4f& marshal_p0 = *p0;
      bool result;
      result = gmtl::isNormalized(marshal_p0, p1);
      return result;
   }


   // Wrapper for free function gmtl::length()
   SHARPPY_API float gmtl_length__gmtl_Quatf1(const gmtl::Quatf* p0)
   {
      const gmtl::Quatf& marshal_p0 = *p0;
      float result;
      result = gmtl::length(marshal_p0);
      return result;
   }


   // Wrapper for free function gmtl::length()
   SHARPPY_API double gmtl_length__gmtl_Quatd1(const gmtl::Quatd* p0)
   {
      const gmtl::Quatd& marshal_p0 = *p0;
      double result;
      result = gmtl::length(marshal_p0);
      return result;
   }


   // Wrapper for free function gmtl::length()
   SHARPPY_API float gmtl_length__gmtl_Vec4f1(const gmtl::Vec4f* p0)
   {
      const gmtl::Vec4f& marshal_p0 = *p0;
      float result;
      result = gmtl::length(marshal_p0);
      return result;
   }


   // Wrapper for free function gmtl::length()
   SHARPPY_API float gmtl_length__gmtl_Vec3f1(const gmtl::Vec3f* p0)
   {
      const gmtl::Vec3f& marshal_p0 = *p0;
      float result;
      result = gmtl::length(marshal_p0);
      return result;
   }


   // Wrapper for free function gmtl::length()
   SHARPPY_API double gmtl_length__gmtl_Vec3d1(const gmtl::Vec3d* p0)
   {
      const gmtl::Vec3d& marshal_p0 = *p0;
      double result;
      result = gmtl::length(marshal_p0);
      return result;
   }


   // Wrapper for free function gmtl::lengthSquared()
   SHARPPY_API float gmtl_lengthSquared__gmtl_Quatf1(const gmtl::Quatf* p0)
   {
      const gmtl::Quatf& marshal_p0 = *p0;
      float result;
      result = gmtl::lengthSquared(marshal_p0);
      return result;
   }


   // Wrapper for free function gmtl::lengthSquared()
   SHARPPY_API double gmtl_lengthSquared__gmtl_Quatd1(const gmtl::Quatd* p0)
   {
      const gmtl::Quatd& marshal_p0 = *p0;
      double result;
      result = gmtl::lengthSquared(marshal_p0);
      return result;
   }


   // Wrapper for free function gmtl::lengthSquared()
   SHARPPY_API float gmtl_lengthSquared__gmtl_Vec4f1(const gmtl::Vec4f* p0)
   {
      const gmtl::Vec4f& marshal_p0 = *p0;
      float result;
      result = gmtl::lengthSquared(marshal_p0);
      return result;
   }


   // Wrapper for free function gmtl::lengthSquared()
   SHARPPY_API float gmtl_lengthSquared__gmtl_Vec3f1(const gmtl::Vec3f* p0)
   {
      const gmtl::Vec3f& marshal_p0 = *p0;
      float result;
      result = gmtl::lengthSquared(marshal_p0);
      return result;
   }


   // Wrapper for free function gmtl::lengthSquared()
   SHARPPY_API double gmtl_lengthSquared__gmtl_Vec3d1(const gmtl::Vec3d* p0)
   {
      const gmtl::Vec3d& marshal_p0 = *p0;
      double result;
      result = gmtl::lengthSquared(marshal_p0);
      return result;
   }


   // Wrapper for free function gmtl::lerp()
   SHARPPY_API gmtl::VecBase<float,3>* gmtl_lerp__gmtl_VecBase_float_3_float_gmtl_VecBase_float_3_gmtl_VecBase_float_34(gmtl::VecBase<float,3>* p0, const float& p1, const gmtl::VecBase<float,3>* p2, const gmtl::VecBase<float,3>* p3)
   {
      gmtl::VecBase<float,3>& marshal_p0 = *p0;
      const gmtl::VecBase<float,3>& marshal_p2 = *p2;
      const gmtl::VecBase<float,3>& marshal_p3 = *p3;
      gmtl::VecBase<float,3>* result;
      result = new gmtl::VecBase<float,3>(gmtl::lerp(marshal_p0, p1, marshal_p2, marshal_p3));
      return result;
   }


   // Wrapper for free function gmtl::lerp()
   SHARPPY_API gmtl::VecBase<float,4>* gmtl_lerp__gmtl_VecBase_float_4_float_gmtl_VecBase_float_4_gmtl_VecBase_float_44(gmtl::VecBase<float,4>* p0, const float& p1, const gmtl::VecBase<float,4>* p2, const gmtl::VecBase<float,4>* p3)
   {
      gmtl::VecBase<float,4>& marshal_p0 = *p0;
      const gmtl::VecBase<float,4>& marshal_p2 = *p2;
      const gmtl::VecBase<float,4>& marshal_p3 = *p3;
      gmtl::VecBase<float,4>* result;
      result = new gmtl::VecBase<float,4>(gmtl::lerp(marshal_p0, p1, marshal_p2, marshal_p3));
      return result;
   }


   // Wrapper for free function gmtl::lerp()
   SHARPPY_API gmtl::VecBase<double,3>* gmtl_lerp__gmtl_VecBase_double_3_double_gmtl_VecBase_double_3_gmtl_VecBase_double_34(gmtl::VecBase<double,3>* p0, const double& p1, const gmtl::VecBase<double,3>* p2, const gmtl::VecBase<double,3>* p3)
   {
      gmtl::VecBase<double,3>& marshal_p0 = *p0;
      const gmtl::VecBase<double,3>& marshal_p2 = *p2;
      const gmtl::VecBase<double,3>& marshal_p3 = *p3;
      gmtl::VecBase<double,3>* result;
      result = new gmtl::VecBase<double,3>(gmtl::lerp(marshal_p0, p1, marshal_p2, marshal_p3));
      return result;
   }


   // Wrapper for free function gmtl::lerp()
   SHARPPY_API gmtl::VecBase<double,4>* gmtl_lerp__gmtl_VecBase_double_4_double_gmtl_VecBase_double_4_gmtl_VecBase_double_44(gmtl::VecBase<double,4>* p0, const double& p1, const gmtl::VecBase<double,4>* p2, const gmtl::VecBase<double,4>* p3)
   {
      gmtl::VecBase<double,4>& marshal_p0 = *p0;
      const gmtl::VecBase<double,4>& marshal_p2 = *p2;
      const gmtl::VecBase<double,4>& marshal_p3 = *p3;
      gmtl::VecBase<double,4>* result;
      result = new gmtl::VecBase<double,4>(gmtl::lerp(marshal_p0, p1, marshal_p2, marshal_p3));
      return result;
   }


   // Wrapper for free function gmtl::lerp()
   SHARPPY_API gmtl::Quatd* gmtl_lerp__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd4(gmtl::Quatd* p0, const double p1, const gmtl::Quatd* p2, const gmtl::Quatd* p3)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p2 = *p2;
      const gmtl::Quatd& marshal_p3 = *p3;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::lerp(marshal_p0, p1, marshal_p2, marshal_p3));
      return result;
   }


   // Wrapper for free function gmtl::lerp()
   SHARPPY_API gmtl::Quatf* gmtl_lerp__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf4(gmtl::Quatf* p0, const float p1, const gmtl::Quatf* p2, const gmtl::Quatf* p3)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p2 = *p2;
      const gmtl::Quatf& marshal_p3 = *p3;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::lerp(marshal_p0, p1, marshal_p2, marshal_p3));
      return result;
   }


   // Wrapper for free function gmtl::log()
   SHARPPY_API gmtl::Quatf* gmtl_log__gmtl_Quatf1(gmtl::Quatf* p0)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::log(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::log()
   SHARPPY_API gmtl::Quatd* gmtl_log__gmtl_Quatd1(gmtl::Quatd* p0)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::log(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::mult()
   SHARPPY_API gmtl::Quatf* gmtl_mult__gmtl_Quatf_gmtl_Quatf_float3(gmtl::Quatf* p0, const gmtl::Quatf* p1, float p2)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p1 = *p1;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::mult(marshal_p0, marshal_p1, p2));
      return result;
   }


   // Wrapper for free function gmtl::mult()
   SHARPPY_API gmtl::Quatd* gmtl_mult__gmtl_Quatd_gmtl_Quatd_double3(gmtl::Quatd* p0, const gmtl::Quatd* p1, double p2)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p1 = *p1;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::mult(marshal_p0, marshal_p1, p2));
      return result;
   }


   // Wrapper for free function gmtl::mult()
   SHARPPY_API gmtl::Matrix44f* gmtl_mult__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f3(gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1, const gmtl::Matrix44f* p2)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      const gmtl::Matrix44f& marshal_p2 = *p2;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::mult(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::mult()
   SHARPPY_API gmtl::Matrix33f* gmtl_mult__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f3(gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1, const gmtl::Matrix33f* p2)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      const gmtl::Matrix33f& marshal_p2 = *p2;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::mult(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::mult()
   SHARPPY_API gmtl::Matrix44f* gmtl_mult__gmtl_Matrix44f_float2(gmtl::Matrix44f* p0, float p1)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::mult(marshal_p0, p1));
      return result;
   }


   // Wrapper for free function gmtl::mult()
   SHARPPY_API gmtl::Matrix33f* gmtl_mult__gmtl_Matrix33f_gmtl_Matrix33f_float3(gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1, const float& p2)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::mult(marshal_p0, marshal_p1, p2));
      return result;
   }


   // Wrapper for free function gmtl::mult()
   SHARPPY_API gmtl::Matrix44f* gmtl_mult__gmtl_Matrix44f_gmtl_Matrix44f_float3(gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1, const float& p2)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::mult(marshal_p0, marshal_p1, p2));
      return result;
   }


   // Wrapper for free function gmtl::mult()
   SHARPPY_API gmtl::Matrix33f* gmtl_mult__gmtl_Matrix33f_float2(gmtl::Matrix33f* p0, float p1)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::mult(marshal_p0, p1));
      return result;
   }


   // Wrapper for free function gmtl::mult()
   SHARPPY_API gmtl::Quatd* gmtl_mult__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd3(gmtl::Quatd* p0, const gmtl::Quatd* p1, const gmtl::Quatd* p2)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p1 = *p1;
      const gmtl::Quatd& marshal_p2 = *p2;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::mult(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::mult()
   SHARPPY_API gmtl::Quatf* gmtl_mult__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf3(gmtl::Quatf* p0, const gmtl::Quatf* p1, const gmtl::Quatf* p2)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p1 = *p1;
      const gmtl::Quatf& marshal_p2 = *p2;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::mult(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::negate()
   SHARPPY_API gmtl::Quatf* gmtl_negate__gmtl_Quatf1(gmtl::Quatf* p0)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::negate(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::negate()
   SHARPPY_API gmtl::Quatd* gmtl_negate__gmtl_Quatd1(gmtl::Quatd* p0)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::negate(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::postMult()
   SHARPPY_API gmtl::Matrix44f* gmtl_postMult__gmtl_Matrix44f_gmtl_Matrix44f2(gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::postMult(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::postMult()
   SHARPPY_API gmtl::Matrix33f* gmtl_postMult__gmtl_Matrix33f_gmtl_Matrix33f2(gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::postMult(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::preMult()
   SHARPPY_API gmtl::Matrix44f* gmtl_preMult__gmtl_Matrix44f_gmtl_Matrix44f2(gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::preMult(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::preMult()
   SHARPPY_API gmtl::Matrix33f* gmtl_preMult__gmtl_Matrix33f_gmtl_Matrix33f2(gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::preMult(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::reflect()
   SHARPPY_API gmtl::VecBase<double,4>* gmtl_reflect__gmtl_VecBase_double_4_gmtl_VecBase_double_4_gmtl_Vec4d3(gmtl::VecBase<double,4>* p0, const gmtl::VecBase<double,4>* p1, const gmtl::Vec4d* p2)
   {
      gmtl::VecBase<double,4>& marshal_p0 = *p0;
      const gmtl::VecBase<double,4>& marshal_p1 = *p1;
      const gmtl::Vec4d& marshal_p2 = *p2;
      gmtl::VecBase<double,4>* result;
      result = new gmtl::VecBase<double,4>(gmtl::reflect(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::reflect()
   SHARPPY_API gmtl::VecBase<double,3>* gmtl_reflect__gmtl_VecBase_double_3_gmtl_VecBase_double_3_gmtl_Vec3d3(gmtl::VecBase<double,3>* p0, const gmtl::VecBase<double,3>* p1, const gmtl::Vec3d* p2)
   {
      gmtl::VecBase<double,3>& marshal_p0 = *p0;
      const gmtl::VecBase<double,3>& marshal_p1 = *p1;
      const gmtl::Vec3d& marshal_p2 = *p2;
      gmtl::VecBase<double,3>* result;
      result = new gmtl::VecBase<double,3>(gmtl::reflect(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::reflect()
   SHARPPY_API gmtl::VecBase<float,4>* gmtl_reflect__gmtl_VecBase_float_4_gmtl_VecBase_float_4_gmtl_Vec4f3(gmtl::VecBase<float,4>* p0, const gmtl::VecBase<float,4>* p1, const gmtl::Vec4f* p2)
   {
      gmtl::VecBase<float,4>& marshal_p0 = *p0;
      const gmtl::VecBase<float,4>& marshal_p1 = *p1;
      const gmtl::Vec4f& marshal_p2 = *p2;
      gmtl::VecBase<float,4>* result;
      result = new gmtl::VecBase<float,4>(gmtl::reflect(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::reflect()
   SHARPPY_API gmtl::VecBase<float,3>* gmtl_reflect__gmtl_VecBase_float_3_gmtl_VecBase_float_3_gmtl_Vec3f3(gmtl::VecBase<float,3>* p0, const gmtl::VecBase<float,3>* p1, const gmtl::Vec3f* p2)
   {
      gmtl::VecBase<float,3>& marshal_p0 = *p0;
      const gmtl::VecBase<float,3>& marshal_p1 = *p1;
      const gmtl::Vec3f& marshal_p2 = *p2;
      gmtl::VecBase<float,3>* result;
      result = new gmtl::VecBase<float,3>(gmtl::reflect(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::slerp()
   SHARPPY_API gmtl::Quatd* gmtl_slerp__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd_bool4(gmtl::Quatd* p0, const double p1, const gmtl::Quatd* p2, const gmtl::Quatd* p3)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p2 = *p2;
      const gmtl::Quatd& marshal_p3 = *p3;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::slerp(marshal_p0, p1, marshal_p2, marshal_p3));
      return result;
   }

   // Wrapper for free function gmtl::slerp()
   SHARPPY_API gmtl::Quatd* gmtl_slerp__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd_bool5(gmtl::Quatd* p0, const double p1, const gmtl::Quatd* p2, const gmtl::Quatd* p3, bool p4)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p2 = *p2;
      const gmtl::Quatd& marshal_p3 = *p3;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::slerp(marshal_p0, p1, marshal_p2, marshal_p3, p4));
      return result;
   }


   // Wrapper for free function gmtl::slerp()
   SHARPPY_API gmtl::Quatf* gmtl_slerp__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf_bool4(gmtl::Quatf* p0, const float p1, const gmtl::Quatf* p2, const gmtl::Quatf* p3)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p2 = *p2;
      const gmtl::Quatf& marshal_p3 = *p3;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::slerp(marshal_p0, p1, marshal_p2, marshal_p3));
      return result;
   }

   // Wrapper for free function gmtl::slerp()
   SHARPPY_API gmtl::Quatf* gmtl_slerp__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf_bool5(gmtl::Quatf* p0, const float p1, const gmtl::Quatf* p2, const gmtl::Quatf* p3, bool p4)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p2 = *p2;
      const gmtl::Quatf& marshal_p3 = *p3;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::slerp(marshal_p0, p1, marshal_p2, marshal_p3, p4));
      return result;
   }


   // Wrapper for free function gmtl::squad()
   SHARPPY_API void gmtl_squad__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf_gmtl_Quatf_gmtl_Quatf6(gmtl::Quatf* p0, float p1, const gmtl::Quatf* p2, const gmtl::Quatf* p3, const gmtl::Quatf* p4, const gmtl::Quatf* p5)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p2 = *p2;
      const gmtl::Quatf& marshal_p3 = *p3;
      const gmtl::Quatf& marshal_p4 = *p4;
      const gmtl::Quatf& marshal_p5 = *p5;
      gmtl::squad(marshal_p0, p1, marshal_p2, marshal_p3, marshal_p4, marshal_p5);
   }


   // Wrapper for free function gmtl::squad()
   SHARPPY_API void gmtl_squad__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd_gmtl_Quatd_gmtl_Quatd6(gmtl::Quatd* p0, double p1, const gmtl::Quatd* p2, const gmtl::Quatd* p3, const gmtl::Quatd* p4, const gmtl::Quatd* p5)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p2 = *p2;
      const gmtl::Quatd& marshal_p3 = *p3;
      const gmtl::Quatd& marshal_p4 = *p4;
      const gmtl::Quatd& marshal_p5 = *p5;
      gmtl::squad(marshal_p0, p1, marshal_p2, marshal_p3, marshal_p4, marshal_p5);
   }


   // Wrapper for free function gmtl::sub()
   SHARPPY_API gmtl::Quatd* gmtl_sub__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd3(gmtl::Quatd* p0, const gmtl::Quatd* p1, const gmtl::Quatd* p2)
   {
      gmtl::Quatd& marshal_p0 = *p0;
      const gmtl::Quatd& marshal_p1 = *p1;
      const gmtl::Quatd& marshal_p2 = *p2;
      gmtl::Quatd* result;
      result = new gmtl::Quatd(gmtl::sub(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::sub()
   SHARPPY_API gmtl::Quatf* gmtl_sub__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf3(gmtl::Quatf* p0, const gmtl::Quatf* p1, const gmtl::Quatf* p2)
   {
      gmtl::Quatf& marshal_p0 = *p0;
      const gmtl::Quatf& marshal_p1 = *p1;
      const gmtl::Quatf& marshal_p2 = *p2;
      gmtl::Quatf* result;
      result = new gmtl::Quatf(gmtl::sub(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::sub()
   SHARPPY_API gmtl::Matrix44f* gmtl_sub__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f3(gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1, const gmtl::Matrix44f* p2)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      const gmtl::Matrix44f& marshal_p2 = *p2;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::sub(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::sub()
   SHARPPY_API gmtl::Matrix33f* gmtl_sub__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f3(gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1, const gmtl::Matrix33f* p2)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      const gmtl::Matrix33f& marshal_p2 = *p2;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::sub(marshal_p0, marshal_p1, marshal_p2));
      return result;
   }


   // Wrapper for free function gmtl::transpose()
   SHARPPY_API gmtl::Matrix33f* gmtl_transpose__gmtl_Matrix33f1(gmtl::Matrix33f* p0)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::transpose(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::transpose()
   SHARPPY_API gmtl::Matrix44f* gmtl_transpose__gmtl_Matrix44f1(gmtl::Matrix44f* p0)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::transpose(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::transpose()
   SHARPPY_API gmtl::Matrix44f* gmtl_transpose__gmtl_Matrix44f_gmtl_Matrix44f2(gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      const gmtl::Matrix44f& marshal_p1 = *p1;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::transpose(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::transpose()
   SHARPPY_API gmtl::Matrix33f* gmtl_transpose__gmtl_Matrix33f_gmtl_Matrix33f2(gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      const gmtl::Matrix33f& marshal_p1 = *p1;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::transpose(marshal_p0, marshal_p1));
      return result;
   }


   // Wrapper for free function gmtl::whichSide()
   SHARPPY_API gmtl::PlaneSide gmtl_whichSide__gmtl_Planed_gmtl_Point3d2(const gmtl::Planed* p0, const gmtl::Point3d* p1)
   {
      const gmtl::Planed& marshal_p0 = *p0;
      const gmtl::Point3d& marshal_p1 = *p1;
      gmtl::PlaneSide result;
      result = gmtl::whichSide(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::whichSide()
   SHARPPY_API gmtl::PlaneSide gmtl_whichSide__gmtl_Planef_gmtl_Point3f2(const gmtl::Planef* p0, const gmtl::Point3f* p1)
   {
      const gmtl::Planef& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      gmtl::PlaneSide result;
      result = gmtl::whichSide(marshal_p0, marshal_p1);
      return result;
   }


   // Wrapper for free function gmtl::whichSide()
   SHARPPY_API gmtl::PlaneSide gmtl_whichSide__gmtl_Planef_gmtl_Point3f_float3(const gmtl::Planef* p0, const gmtl::Point3f* p1, const float& p2)
   {
      const gmtl::Planef& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      gmtl::PlaneSide result;
      result = gmtl::whichSide(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::whichSide()
   SHARPPY_API gmtl::PlaneSide gmtl_whichSide__gmtl_Planed_gmtl_Point3d_double3(const gmtl::Planed* p0, const gmtl::Point3d* p1, const double& p2)
   {
      const gmtl::Planed& marshal_p0 = *p0;
      const gmtl::Point3d& marshal_p1 = *p1;
      gmtl::PlaneSide result;
      result = gmtl::whichSide(marshal_p0, marshal_p1, p2);
      return result;
   }


   // Wrapper for free function gmtl::zero()
   SHARPPY_API gmtl::Matrix44f* gmtl_zero__gmtl_Matrix44f1(gmtl::Matrix44f* p0)
   {
      gmtl::Matrix44f& marshal_p0 = *p0;
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::zero(marshal_p0));
      return result;
   }


   // Wrapper for free function gmtl::zero()
   SHARPPY_API gmtl::Matrix33f* gmtl_zero__gmtl_Matrix33f1(gmtl::Matrix33f* p0)
   {
      gmtl::Matrix33f& marshal_p0 = *p0;
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::zero(marshal_p0));
      return result;
   }


} // extern "C" for free functions
