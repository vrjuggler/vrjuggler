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

// Generated from Revision: 1.1 of RCSfile: free_function_cxx.tmpl,v


#include "sharppy.h"
#include <gmtl-functions.h>

extern "C"
{

   // Wrapper for free function gmtl::invert()
   SHARPPY_API gmtl::Matrix44f* gmtl_invert__gmtl_Matrix44f_gmtl_Matrix44f(gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1)
   {
      gmtl::Matrix44f& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Matrix44f& marshal_p1 = *p1; // Pre-call marshaling for p1
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::invert(marshal_p0, marshal_p1));

      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      return result;
   }


   // Wrapper for free function gmtl::invert()
   SHARPPY_API gmtl::Matrix33f* gmtl_invert__gmtl_Matrix33f_gmtl_Matrix33f(gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1)
   {
      gmtl::Matrix33f& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Matrix33f& marshal_p1 = *p1; // Pre-call marshaling for p1
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::invert(marshal_p0, marshal_p1));

      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      return result;
   }


   // Wrapper for free function gmtl::invert()
   SHARPPY_API gmtl::Matrix44f* gmtl_invert__gmtl_Matrix44f(gmtl::Matrix44f* p0)
   {
      gmtl::Matrix44f& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(gmtl::invert(marshal_p0));

      ; // Post-call marshaling for p0
      return result;
   }


   // Wrapper for free function gmtl::invert()
   SHARPPY_API gmtl::Matrix33f* gmtl_invert__gmtl_Matrix33f(gmtl::Matrix33f* p0)
   {
      gmtl::Matrix33f& marshal_p0 = *p0; // Pre-call marshaling for p0
      gmtl::Matrix33f* result;
      result = new gmtl::Matrix33f(gmtl::invert(marshal_p0));

      ; // Post-call marshaling for p0
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Matrix44f_gmtl_Matrix44f_float(const gmtl::Matrix44f* p0, const gmtl::Matrix44f* p1, const float& p2)
   {
      const gmtl::Matrix44f& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Matrix44f& marshal_p1 = *p1; // Pre-call marshaling for p1
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);

      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      return result;
   }


   // Wrapper for free function gmtl::isEqual()
   SHARPPY_API bool gmtl_isEqual__gmtl_Matrix33f_gmtl_Matrix33f_float(const gmtl::Matrix33f* p0, const gmtl::Matrix33f* p1, const float& p2)
   {
      const gmtl::Matrix33f& marshal_p0 = *p0; // Pre-call marshaling for p0
      const gmtl::Matrix33f& marshal_p1 = *p1; // Pre-call marshaling for p1
      bool result;
      result = gmtl::isEqual(marshal_p0, marshal_p1, p2);

      ; // Post-call marshaling for p0
      ; // Post-call marshaling for p1
      return result;
   }


} // extern "C" for free functions
