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
#include <gmtl-Math-functions.h>

extern "C"
{

   // Wrapper for free function gmtl::Math::deg2Rad()
   SHARPPY_API double gmtl_Math_deg2Rad__double(double p0)
   {
      double result;
      result = gmtl::Math::deg2Rad(p0);

      return result;
   }


   // Wrapper for free function gmtl::Math::deg2Rad()
   SHARPPY_API float gmtl_Math_deg2Rad__float(float p0)
   {
      float result;
      result = gmtl::Math::deg2Rad(p0);

      return result;
   }


   // Wrapper for free function gmtl::Math::rad2Deg()
   SHARPPY_API double gmtl_Math_rad2Deg__double(double p0)
   {
      double result;
      result = gmtl::Math::rad2Deg(p0);

      return result;
   }


   // Wrapper for free function gmtl::Math::rad2Deg()
   SHARPPY_API float gmtl_Math_rad2Deg__float(float p0)
   {
      float result;
      result = gmtl::Math::rad2Deg(p0);

      return result;
   }


} // extern "C" for free functions
