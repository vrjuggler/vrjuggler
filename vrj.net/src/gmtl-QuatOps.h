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

#ifndef _VRJ_NET_GMTL_QUAT_OPS_H_
#define _VRJ_NET_GMTL_QUAT_OPS_H_

#include <gmtl/Quat.h>
#include <gmtl/QuatOps.h>


namespace gmtl
{
   template gmtl::Quatf& mult(gmtl::Quatf&, const gmtl::Quatf&,
                              const gmtl::Quatf&);
   template gmtl::Quatd& mult(gmtl::Quatd&, const gmtl::Quatd&,
                              const gmtl::Quatd&);

   template gmtl::Quatf& mult(gmtl::Quatf&, const gmtl::Quatf&, float);
   template gmtl::Quatd& mult(gmtl::Quatd&, const gmtl::Quatd&, double);

   template gmtl::Quatf& negate(gmtl::Quatf&);
   template gmtl::Quatd& negate(gmtl::Quatd&);

   template gmtl::Quatf& div(gmtl::Quatf&, const gmtl::Quatf&, gmtl::Quatf);
   template gmtl::Quatd& div(gmtl::Quatd&, const gmtl::Quatd&, gmtl::Quatd);

   template gmtl::Quatf& div(gmtl::Quatf&, const gmtl::Quatf&, float);
   template gmtl::Quatd& div(gmtl::Quatd&, const gmtl::Quatd&, double);

   template gmtl::Quatf& add(gmtl::Quatf&, const gmtl::Quatf&,
                             const gmtl::Quatf&);
   template gmtl::Quatd& add(gmtl::Quatd&, const gmtl::Quatd&,
                             const gmtl::Quatd&);

   template gmtl::Quatf& sub(gmtl::Quatf&, const gmtl::Quatf&,
                             const gmtl::Quatf&);
   template gmtl::Quatd& sub(gmtl::Quatd&, const gmtl::Quatd&,
                             const gmtl::Quatd&);

   template float dot(const gmtl::Quatf&, const gmtl::Quatf&);
   template double dot(const gmtl::Quatd&, const gmtl::Quatd&);

   template float lengthSquared(const gmtl::Quatf&);
   template double lengthSquared(const gmtl::Quatd&);

   template float length(const gmtl::Quatf&);
   template double length(const gmtl::Quatd&);

   template gmtl::Quatf& normalize(gmtl::Quatf&);
   template gmtl::Quatd& normalize(gmtl::Quatd&);

   template bool isNormalized(const gmtl::Quatf&, const float);
   template bool isNormalized(const gmtl::Quatd&, const double);

   template gmtl::Quatf& conj(gmtl::Quatf&);
   template gmtl::Quatd& conj(gmtl::Quatd&);

   template gmtl::Quatf& invert(gmtl::Quatf&);
   template gmtl::Quatd& invert(gmtl::Quatd&);

   template gmtl::Quatf& exp(gmtl::Quatf&);
   template gmtl::Quatd& exp(gmtl::Quatd&);

   template gmtl::Quatf& log(gmtl::Quatf&);
   template gmtl::Quatd& log(gmtl::Quatd&);

   template void squad(gmtl::Quatf&, float, const gmtl::Quatf&,
                       const gmtl::Quatf&, const gmtl::Quatf&,
                       const gmtl::Quatf&);
   template void squad(gmtl::Quatd&, double, const gmtl::Quatd&,
                       const gmtl::Quatd&, const gmtl::Quatd&,
                       const gmtl::Quatd&);

   template gmtl::Quatf& slerp(gmtl::Quatf&, const float, const gmtl::Quatf&,
                               const gmtl::Quatf&, bool);
   template gmtl::Quatd& slerp(gmtl::Quatd&, const double, const gmtl::Quatd&,
                               const gmtl::Quatd&, bool);

   template Quatf& lerp(Quatf&, const float, const Quatf&, const Quatf&);
   template Quatd& lerp(Quatd&, const double, const Quatd&, const Quatd&);

   template bool isEquiv(const gmtl::Quatf&, const gmtl::Quatf&, float);
   template bool isEquiv(const gmtl::Quatd&, const gmtl::Quatd&, double);

   template bool isEqual(const gmtl::Quatf&, const gmtl::Quatf&, float);
   template bool isEqual(const gmtl::Quatd&, const gmtl::Quatd&, double);
}

#endif /* _VRJ_NET_GMTL_QUAT_OPS_H_ */
