// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/VecBase.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<double,3>* gmtl_VecBase_double_3__VecBase__0()
   {
      gmtl::VecBase<double,3>* obj = new gmtl::VecBase<double,3>();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<double,3>* gmtl_VecBase_double_3__VecBase__gmtl_VecBase_double_31(const gmtl::VecBase<double,3>* p0)
   {
      const gmtl::VecBase<double,3>& marshal_p0 = *p0;
      gmtl::VecBase<double,3>* obj = new gmtl::VecBase<double,3>(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<double,3>* gmtl_VecBase_double_3__VecBase__double_double2(const double p0, const double p1)
   {
      gmtl::VecBase<double,3>* obj = new gmtl::VecBase<double,3>(p0, p1);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<double,3>* gmtl_VecBase_double_3__VecBase__double_double_double3(const double p0, const double p1, const double p2)
   {
      gmtl::VecBase<double,3>* obj = new gmtl::VecBase<double,3>(p0, p1, p2);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::VecBase<double,3>* gmtl_VecBase_double_3__VecBase__double_double_double_double4(const double p0, const double p1, const double p2, const double p3)
   {
      gmtl::VecBase<double,3>* obj = new gmtl::VecBase<double,3>(p0, p1, p2, p3);
      return obj;
   }

   SHARPPY_API void delete_gmtl_VecBase_double_3(gmtl::VecBase<double,3>* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::VecBase<double,3>::set()
   SHARPPY_API void gmtl_VecBase_double_3__set__double1(gmtl::VecBase<double,3>* self_, const double* p0)
   {
      self_->set(p0);
   }

/*
   // Wrapper for non-virtual method gmtl::VecBase<double,3>::set()
   SHARPPY_API void gmtl_VecBase_double_3__set__double1(gmtl::VecBase<double,3>* self_, const double p0)
   {
      self_->set(p0);
   }
*/

   // Wrapper for non-virtual method gmtl::VecBase<double,3>::set()
   SHARPPY_API void gmtl_VecBase_double_3__set__double_double2(gmtl::VecBase<double,3>* self_, const double p0, const double p1)
   {
      self_->set(p0, p1);
   }


   // Wrapper for non-virtual method gmtl::VecBase<double,3>::set()
   SHARPPY_API void gmtl_VecBase_double_3__set__double_double_double3(gmtl::VecBase<double,3>* self_, const double p0, const double p1, const double p2)
   {
      self_->set(p0, p1, p2);
   }


   // Wrapper for non-virtual method gmtl::VecBase<double,3>::set()
   SHARPPY_API void gmtl_VecBase_double_3__set__double_double_double_double4(gmtl::VecBase<double,3>* self_, const double p0, const double p1, const double p2, const double p3)
   {
      self_->set(p0, p1, p2, p3);
   }


   // Wrapper for non-virtual method gmtl::VecBase<double,3>::getData()
   SHARPPY_API void gmtl_VecBase_double_3__getData__0(gmtl::VecBase<double,3>* self_, double* arrayHolder)
   {
      double* temp_array;
      temp_array = self_->getData();
      for ( int i = 0; i < 3; ++i )
      {
         arrayHolder[i] = temp_array[i];
      }
   }


} // extern "C" for gmtl::VecBase<double,3>


