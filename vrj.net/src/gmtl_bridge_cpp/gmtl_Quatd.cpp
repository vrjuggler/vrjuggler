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

// Generated from Revision: 1.58 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gmtl/Matrix.h>
#include <gmtl/VecBase.h>
#include <gmtl/Vec.h>
#include <gmtl/Quat.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gmtl::Quatd* gmtl_Quat_double__Quat__()
   {
      gmtl::Quatd* obj = new gmtl::Quatd();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Quatd* gmtl_Quat_double__Quat__double_double_double_double(const double p0, const double p1, const double p2, const double p3)
   {
      gmtl::Quatd* obj = new gmtl::Quatd(p0, p1, p2, p3);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gmtl::Quatd* gmtl_Quat_double__Quat__gmtl_Quatd(const gmtl::Quatd* p0)
   {
      const gmtl::Quatd& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Quatd* obj = new gmtl::Quatd(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   SHARPPY_API void delete_gmtl_Quatd(gmtl::Quatd* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gmtl::Quat<double>::set()
   SHARPPY_API void gmtl_Quat_double__set__double_double_double_double(gmtl::Quatd* self_, const double p0, const double p1, const double p2, const double p3)
   {
      self_->set(p0, p1, p2, p3);
   }

   // Wrapper for non-virtual method gmtl::Quat<double>::get()
   SHARPPY_API void gmtl_Quat_double__get__double_double_double_double(gmtl::Quatd* self_, double& p0, double& p1, double& p2, double& p3)
   {
      self_->get(p0, p1, p2, p3);
   }

   // Wrapper for non-virtual method gmtl::Quat<double>::getData()
   SHARPPY_API const double* gmtl_Quat_double__getData__(gmtl::Quatd* self_)
   {
      const double* result;
      result = self_->getData();

      return result;
   }






} // extern "C" for gmtl::Quatd


