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

#ifndef _VRJ_NET_GMTL_SPHERE_OPS_H_
#define _VRJ_NET_GMTL_SPHERE_OPS_H_

#include <gmtl/Sphere.h>
#include <gmtl/SphereOps.h>


namespace gmtl
{
   template bool isEqual(const gmtl::Spheref&, const gmtl::Spheref&,
                         const float&);
   template bool isEqual(const gmtl::Sphered&, const gmtl::Sphered&,
                         const double&);
}


#endif /* _VRJ_NET_GMTL_SPHERE_OPS_H_ */
