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

#ifndef _VRJ_NET_GMTL_TRI_OPS_H_
#define _VRJ_NET_GMTL_TRI_OPS_H_

#include <gmtl/Tri.h>
#include <gmtl/TriOps.h>


namespace gmtl
{
   template gmtl::Point3f center(const gmtl::Trif&);
   template gmtl::Point3d center(const gmtl::Trid&);
   template gmtl::Point3i center(const gmtl::Trii&);

   template gmtl::Vec3f normal(const gmtl::Trif&);
   template gmtl::Vec3d normal(const gmtl::Trid&);
//   template gmtl::Vec3i normal(const gmtl::Trii&);

   template bool isEqual(const gmtl::Trif&, const gmtl::Trif&, const float&);
   template bool isEqual(const gmtl::Trid&, const gmtl::Trid&, const double&);
}

#endif /* _VRJ_NET_GMTL_TRI_OPS_H_ */
