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

#ifndef _VRJ_NET_GMTL_FUNCTIONS_H_
#define _VRJ_NET_GMTL_FUNCTIONS_H_

#include <gmtl/Math.h>
#include <gmtl/MatrixOps.h>

namespace gmtl
{
   template gmtl::Matrix33f& invert(gmtl::Matrix33f&, const gmtl::Matrix33f&);
   template gmtl::Matrix44f& invert(gmtl::Matrix44f&, const gmtl::Matrix44f&);

   template gmtl::Matrix33f& invert(gmtl::Matrix33f&);
   template gmtl::Matrix44f& invert(gmtl::Matrix44f&);

   template bool isEqual(const gmtl::Matrix33f&, const gmtl::Matrix33f&,
                         const float&);
   template bool isEqual(const gmtl::Matrix44f&, const gmtl::Matrix44f&,
                         const float&);
}


#endif /* _VRJ_NET_GMTL_FUNCTIONS_H_ */
