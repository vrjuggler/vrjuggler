// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _VRJ_NET_GMTL_CONTAINMENT_H_
#define _VRJ_NET_GMTL_CONTAINMENT_H_

#include <gmtl/Containment.h>


namespace gmtl
{
   template bool isInVolume(const gmtl::Spheref&, const gmtl::Point3f&);
   template bool isInVolume(const gmtl::Sphered&, const gmtl::Point3d&);

   template bool isInVolume(const gmtl::Spheref&, const gmtl::Spheref&);
   template bool isInVolume(const gmtl::Sphered&, const gmtl::Sphered&);

   template void extendVolume(gmtl::Spheref&, const gmtl::Point3f&);
   template void extendVolume(gmtl::Sphered&, const gmtl::Point3d&);

   template void extendVolume(gmtl::Spheref&, const gmtl::Spheref&);
   template void extendVolume(gmtl::Sphered&, const gmtl::Sphered&);

   template void makeVolume(gmtl::Spheref&, const std::vector<gmtl::Point3f>&);
   template void makeVolume(gmtl::Sphered&, const std::vector<gmtl::Point3d>&);

   template bool isOnVolume(const gmtl::Spheref&, const gmtl::Point3f&,
                            const float&);
   template bool isOnVolume(const gmtl::Sphered&, const gmtl::Point3d&,
                            const double&);

   template void extendVolume(gmtl::AABoxf&, const gmtl::Point3f&);
   template void extendVolume(gmtl::AABoxd&, const gmtl::Point3d&);

   template void extendVolume(gmtl::AABoxf&, const gmtl::AABoxf&);
   template void extendVolume(gmtl::AABoxd&, const gmtl::AABoxd&);

   template void makeVolume(gmtl::AABoxf&, const gmtl::Spheref&);
   template void makeVolume(gmtl::AABoxd&, const gmtl::Sphered&);
}

#endif /* _PYGMTL_CONTAINMENT_H_ */
