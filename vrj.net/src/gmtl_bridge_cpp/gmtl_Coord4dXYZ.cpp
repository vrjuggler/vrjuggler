// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Vec.h>
#include <gmtl/EulerAngle.h>
#include <gmtl/Coord.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4dXYZ* gmtl_Coord_gmtl_Vec_double_4__gmtl_EulerAngle_double_gmtl_XYZ____Coord__0()
   {
      gmtl::Coord4dXYZ* obj = new gmtl::Coord4dXYZ();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4dXYZ* gmtl_Coord_gmtl_Vec_double_4__gmtl_EulerAngle_double_gmtl_XYZ____Coord__gmtl_Coord4dXYZ1(const gmtl::Coord4dXYZ* p0)
   {
      const gmtl::Coord4dXYZ& marshal_p0 = *p0;
      gmtl::Coord4dXYZ* obj = new gmtl::Coord4dXYZ(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4dXYZ* gmtl_Coord_gmtl_Vec_double_4__gmtl_EulerAngle_double_gmtl_XYZ____Coord__gmtl_Vec4d_gmtl_EulerAngleXYZd2(const gmtl::Vec4d* p0, const gmtl::EulerAngleXYZd* p1)
   {
      const gmtl::Vec4d& marshal_p0 = *p0;
      const gmtl::EulerAngleXYZd& marshal_p1 = *p1;
      gmtl::Coord4dXYZ* obj = new gmtl::Coord4dXYZ(marshal_p0, marshal_p1);
      return obj;
   }

/*
   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4dXYZ* gmtl_Coord_gmtl_Vec_double_4__gmtl_EulerAngle_double_gmtl_XYZ____Coord__double_double_double_double_double_double6(double p0, double p1, double p2, double p3, double p4, double p5)
   {
      gmtl::Coord4dXYZ* obj = new gmtl::Coord4dXYZ(p0, p1, p2, p3, p4, p5);
      return obj;
   }
*/

   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4dXYZ* gmtl_Coord_gmtl_Vec_double_4__gmtl_EulerAngle_double_gmtl_XYZ____Coord__double_double_double_double_double_double_double7(double p0, double p1, double p2, double p3, double p4, double p5, double p6)
   {
      gmtl::Coord4dXYZ* obj = new gmtl::Coord4dXYZ(p0, p1, p2, p3, p4, p5, p6);
      return obj;
   }

/*
   // Constructor wrapper.
   SHARPPY_API gmtl::Coord4dXYZ* gmtl_Coord_gmtl_Vec_double_4__gmtl_EulerAngle_double_gmtl_XYZ____Coord__double_double_double_double_double_double_double_double8(double p0, double p1, double p2, double p3, double p4, double p5, double p6, double p7)
   {
      gmtl::Coord4dXYZ* obj = new gmtl::Coord4dXYZ(p0, p1, p2, p3, p4, p5, p6, p7);
      return obj;
   }
*/

   SHARPPY_API void delete_gmtl_Coord4dXYZ(gmtl::Coord4dXYZ* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<double,4>,gmtl::EulerAngle<double,gmtl::XYZ> >::getPos()
   SHARPPY_API gmtl::Vec4d* gmtl_Coord_gmtl_Vec_double_4__gmtl_EulerAngle_double_gmtl_XYZ____getPos__0(gmtl::Coord4dXYZ* self_)
   {
      gmtl::Vec4d* result;
      result = new gmtl::Vec4d(self_->getPos());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<double,4>,gmtl::EulerAngle<double,gmtl::XYZ> >::getRot()
   SHARPPY_API gmtl::EulerAngleXYZd* gmtl_Coord_gmtl_Vec_double_4__gmtl_EulerAngle_double_gmtl_XYZ____getRot__0(gmtl::Coord4dXYZ* self_)
   {
      gmtl::EulerAngleXYZd* result;
      result = new gmtl::EulerAngleXYZd(self_->getRot());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<double,4>,gmtl::EulerAngle<double,gmtl::XYZ> >::pos()
   SHARPPY_API gmtl::Vec4d* gmtl_Coord_gmtl_Vec_double_4__gmtl_EulerAngle_double_gmtl_XYZ____pos__0(gmtl::Coord4dXYZ* self_)
   {
      gmtl::Vec4d* result;
      result = new gmtl::Vec4d(self_->pos());
      return result;
   }


   // Wrapper for non-virtual method gmtl::Coord<gmtl::Vec<double,4>,gmtl::EulerAngle<double,gmtl::XYZ> >::rot()
   SHARPPY_API gmtl::EulerAngleXYZd* gmtl_Coord_gmtl_Vec_double_4__gmtl_EulerAngle_double_gmtl_XYZ____rot__0(gmtl::Coord4dXYZ* self_)
   {
      gmtl::EulerAngleXYZd* result;
      result = new gmtl::EulerAngleXYZd(self_->rot());
      return result;
   }


} // extern "C" for gmtl::Coord4dXYZ


