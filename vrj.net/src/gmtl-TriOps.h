// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

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
