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

// Generated from Revision: 1.62 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/LineSeg.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegf* gmtl_LineSeg_float__LineSeg__()
   {
      gmtl::LineSegf* obj = new gmtl::LineSegf();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegf* gmtl_LineSeg_float__LineSeg__gmtl_Point3f_gmtl_Vec3f(const gmtl::Point3f* p0, const gmtl::Vec3f* p1)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      const gmtl::Vec3f& marshal_p1 = *p1;
      gmtl::LineSegf* obj = new gmtl::LineSegf(marshal_p0, marshal_p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegf* gmtl_LineSeg_float__LineSeg__gmtl_LineSegf(const gmtl::LineSegf* p0)
   {
      const gmtl::LineSegf& marshal_p0 = *p0;
      gmtl::LineSegf* obj = new gmtl::LineSegf(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::LineSegf* gmtl_LineSeg_float__LineSeg__gmtl_Point3f_gmtl_Point3f(const gmtl::Point3f* p0, const gmtl::Point3f* p1)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      gmtl::LineSegf* obj = new gmtl::LineSegf(marshal_p0, marshal_p1);
      return obj;
   }

   SHARPPY_API void delete_gmtl_LineSegf(gmtl::LineSegf* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::LineSeg<float>::getLength()
   SHARPPY_API float gmtl_LineSeg_float__getLength__0(gmtl::LineSegf* self_)
   {
      float result;
      result = self_->getLength();
      return result;
   }


} // extern "C" for gmtl::LineSegf


