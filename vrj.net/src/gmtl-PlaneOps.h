// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _VRJ_NET_GMTL_PLANE_OPS_H_
#define _VRJ_NET_GMTL_PLANE_OPS_H_

#include <gmtl/Plane.h>
#include <gmtl/PlaneOps.h>


namespace gmtl
{
   template gmtl::PlaneSide whichSide(const gmtl::Planef&,
                                      const gmtl::Point3f&);
   template gmtl::PlaneSide whichSide(const gmtl::Planed&,
                                      const gmtl::Point3d&);

   template gmtl::PlaneSide whichSide(const gmtl::Planef&,
                                      const gmtl::Point3f&, const float&);
   template gmtl::PlaneSide whichSide(const gmtl::Planed&,
                                      const gmtl::Point3d&, const double&);

   template float distance(const gmtl::Planef&, const gmtl::Point3f&);
   template double distance(const gmtl::Planed&, const gmtl::Point3d&);

   template float findNearestPt(const gmtl::Planef&, const gmtl::Point3f&,
                                gmtl::Point3f&);
   template double findNearestPt(const gmtl::Planed&, const gmtl::Point3d&,
                                 gmtl::Point3d&);

   template bool isEqual(const gmtl::Planef&, const gmtl::Planef&,
                         const float&);
   template bool isEqual(const gmtl::Planed&, const gmtl::Planed&,
                         const double&);
}

#endif /* _VRJ_NET_GMTL_PLANE_OPS_H_ */
