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
#include <gmtl/Vec.h>
#include <gmtl/AxisAngle.h>
#include <gmtl/Coord.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4dAxisAngle* gmtl_Coord_gmtl_Vec_double_4__gmtl_AxisAngle_double____Coord__()
   {
      gmtl::Coord4dAxisAngle* obj = new gmtl::Coord4dAxisAngle();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4dAxisAngle* gmtl_Coord_gmtl_Vec_double_4__gmtl_AxisAngle_double____Coord__gmtl_Coord4dAxisAngle(const gmtl::Coord4dAxisAngle* p0)
   {
      const gmtl::Coord4dAxisAngle& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Coord4dAxisAngle* obj = new gmtl::Coord4dAxisAngle(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4dAxisAngle* gmtl_Coord_gmtl_Vec_double_4__gmtl_AxisAngle_double____Coord__gmtl_Vec4d_gmtl_AxisAngled(const gmtl::Vec4d* p0, const gmtl::AxisAngled* p1)
   {
      const gmtl::Vec4d& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::AxisAngled& marshal_p1 = *p1; // Pre-call marshaling for p1
      gmtl::Coord4dAxisAngle* obj = new gmtl::Coord4dAxisAngle(marshal_p0, marshal_p1);
      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      return obj;
   }

/*
   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4dAxisAngle* gmtl_Coord_gmtl_Vec_double_4__gmtl_AxisAngle_double____Coord__double_double_double_double_double_double(double p0, double p1, double p2, double p3, double p4, double p5)
   {
      gmtl::Coord4dAxisAngle* obj = new gmtl::Coord4dAxisAngle(p0, p1, p2, p3, p4, p5);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4dAxisAngle* gmtl_Coord_gmtl_Vec_double_4__gmtl_AxisAngle_double____Coord__double_double_double_double_double_double_double(double p0, double p1, double p2, double p3, double p4, double p5, double p6)
   {
      gmtl::Coord4dAxisAngle* obj = new gmtl::Coord4dAxisAngle(p0, p1, p2, p3, p4, p5, p6);
      return obj;
   }
*/

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4dAxisAngle* gmtl_Coord_gmtl_Vec_double_4__gmtl_AxisAngle_double____Coord__double_double_double_double_double_double_double_double(double p0, double p1, double p2, double p3, double p4, double p5, double p6, double p7)
   {
      gmtl::Coord4dAxisAngle* obj = new gmtl::Coord4dAxisAngle(p0, p1, p2, p3, p4, p5, p6, p7);
      return obj;
   }

   SHARPPY_API void delete_gmtl_Coord4dAxisAngle(gmtl::Coord4dAxisAngle* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<double,4>,gmtl::AxisAngle<double> >::getPos()
   SHARPPY_API const gmtl::Vec4d* gmtl_Coord_gmtl_Vec_double_4__gmtl_AxisAngle_double____getPos__0(gmtl::Coord4dAxisAngle* self_)
   {
      const gmtl::Vec4d* result;
      result = new const gmtl::Vec4d(self_->getPos());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<double,4>,gmtl::AxisAngle<double> >::getRot()
   SHARPPY_API const gmtl::AxisAngled* gmtl_Coord_gmtl_Vec_double_4__gmtl_AxisAngle_double____getRot__0(gmtl::Coord4dAxisAngle* self_)
   {
      const gmtl::AxisAngled* result;
      result = new const gmtl::AxisAngled(self_->getRot());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<double,4>,gmtl::AxisAngle<double> >::pos()
   SHARPPY_API gmtl::Vec4d* gmtl_Coord_gmtl_Vec_double_4__gmtl_AxisAngle_double____pos__0(gmtl::Coord4dAxisAngle* self_)
   {
      gmtl::Vec4d* result;
      result = new gmtl::Vec4d(self_->pos());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<double,4>,gmtl::AxisAngle<double> >::rot()
   SHARPPY_API gmtl::AxisAngled* gmtl_Coord_gmtl_Vec_double_4__gmtl_AxisAngle_double____rot__0(gmtl::Coord4dAxisAngle* self_)
   {
      gmtl::AxisAngled* result;
      result = new gmtl::AxisAngled(self_->rot());
      return result;
   }


} // extern "C" for gmtl::Coord4dAxisAngle


