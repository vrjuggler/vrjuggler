// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _VRJ_NET_GMTL_LINE_SEG_OPS_H_
#define _VRJ_NET_GMTL_LINE_SEG_OPS_H_

#include <gmtl/LineSeg.h>
#include <gmtl/LineSegOps.h>


namespace gmtl
{
   template float distanceSquared(const gmtl::LineSegf&, const gmtl::Point3f&);
   template double distanceSquared(const gmtl::LineSegd&, const gmtl::Point3d&);

   template gmtl::Point3f findNearestPt(const gmtl::LineSegf&,
                                        const gmtl::Point3f&);
   template gmtl::Point3d findNearestPt(const gmtl::LineSegd&,
                                        const gmtl::Point3d&);

   template float distance(const gmtl::LineSegf&, const gmtl::Point3f&);
   template double distance(const gmtl::LineSegd&, const gmtl::Point3d&);
}

#endif /* _VRJ_NET_GMTL_LINE_SEG_OPS_H_ */
