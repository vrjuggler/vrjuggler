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
   SHARPPY_API gmtl::AABoxf* gmtl_AABox_float__AABox__0()
   {
      gmtl::AABoxf* obj = new gmtl::AABoxf();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AABoxf* gmtl_AABox_float__AABox__gmtl_Point3f_gmtl_Point3f2(const gmtl::Point3f* p0, const gmtl::Point3f* p1)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      const gmtl::Point3f& marshal_p1 = *p1;
      gmtl::AABoxf* obj = new gmtl::AABoxf(marshal_p0, marshal_p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::AABoxf* gmtl_AABox_float__AABox__gmtl_AABoxf1(const gmtl::AABoxf* p0)
   {
      const gmtl::AABoxf& marshal_p0 = *p0;
      gmtl::AABoxf* obj = new gmtl::AABoxf(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_gmtl_AABoxf(gmtl::AABoxf* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::AABox<float>::getMin()
   SHARPPY_API gmtl::Point3f* gmtl_AABox_float__getMin__0(gmtl::AABoxf* self_)
   {
      gmtl::Point3f* result;
      result = new gmtl::Point3f(self_->getMin());
      return result;
   }


   // Wrapper for non-virtual method gmtl::AABox<float>::getMax()
   SHARPPY_API gmtl::Point3f* gmtl_AABox_float__getMax__0(gmtl::AABoxf* self_)
   {
      gmtl::Point3f* result;
      result = new gmtl::Point3f(self_->getMax());
      return result;
   }


   // Wrapper for non-virtual method gmtl::AABox<float>::isEmpty()
   SHARPPY_API bool gmtl_AABox_float__isEmpty__0(gmtl::AABoxf* self_)
   {
      bool result;
      result = self_->isEmpty();
      return result;
   }


   // Wrapper for non-virtual method gmtl::AABox<float>::setMin()
   SHARPPY_API void gmtl_AABox_float__setMin__gmtl_Point3f1(gmtl::AABoxf* self_, const gmtl::Point3f* p0)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      self_->setMin(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::AABox<float>::setMax()
   SHARPPY_API void gmtl_AABox_float__setMax__gmtl_Point3f1(gmtl::AABoxf* self_, const gmtl::Point3f* p0)
   {
      const gmtl::Point3f& marshal_p0 = *p0;
      self_->setMax(marshal_p0);
   }


   // Wrapper for non-virtual method gmtl::AABox<float>::setEmpty()
   SHARPPY_API void gmtl_AABox_float__setEmpty__bool1(gmtl::AABoxf* self_, bool p0)
   {
      self_->setEmpty(p0);
   }


} // extern "C" for gmtl::AABoxf


