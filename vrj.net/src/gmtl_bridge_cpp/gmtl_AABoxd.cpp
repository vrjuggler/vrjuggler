// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/AABox.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::AABoxd* gmtl_AABox_double__AABox__0()
   {
      gmtl::AABoxd* obj = new gmtl::AABoxd();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AABoxd* gmtl_AABox_double__AABox__gmtl_Point3d_gmtl_Point3d2(const gmtl::Point3d* p0, const gmtl::Point3d* p1)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      const gmtl::Point3d& marshal_p1 = *p1;
      gmtl::AABoxd* obj = new gmtl::AABoxd(marshal_p0, marshal_p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AABoxd* gmtl_AABox_double__AABox__gmtl_AABoxd1(const gmtl::AABoxd* p0)
   {
      const gmtl::AABoxd& marshal_p0 = *p0;
      gmtl::AABoxd* obj = new gmtl::AABoxd(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_AABoxd(gmtl::AABoxd* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::AABox<double>::getMin()
   SHARPPY_API gmtl::Point3d* gmtl_AABox_double__getMin__0(gmtl::AABoxd* self_)
   {
      gmtl::Point3d* result;
      result = new gmtl::Point3d(self_->getMin());
      return result;
   }


   // Wrapper for non-virtual method gmtl::AABox<double>::getMax()
   SHARPPY_API gmtl::Point3d* gmtl_AABox_double__getMax__0(gmtl::AABoxd* self_)
   {
      gmtl::Point3d* result;
      result = new gmtl::Point3d(self_->getMax());
      return result;
   }


   // Wrapper for non-virtual method gmtl::AABox<double>::isEmpty()
   SHARPPY_API bool gmtl_AABox_double__isEmpty__0(gmtl::AABoxd* self_)
   {
      bool result;
      result = self_->isEmpty();
      return result;
   }


   // Wrapper for non-virtual method gmtl::AABox<double>::setMin()
   SHARPPY_API void gmtl_AABox_double__setMin__gmtl_Point3d1(gmtl::AABoxd* self_, const gmtl::Point3d* p0)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      self_->setMin(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::AABox<double>::setMax()
   SHARPPY_API void gmtl_AABox_double__setMax__gmtl_Point3d1(gmtl::AABoxd* self_, const gmtl::Point3d* p0)
   {
      const gmtl::Point3d& marshal_p0 = *p0;
      self_->setMax(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::AABox<double>::setEmpty()
   SHARPPY_API void gmtl_AABox_double__setEmpty__bool1(gmtl::AABoxd* self_, bool p0)
   {
      self_->setEmpty(p0);
   }


} // extern "C" for gmtl::AABoxd


