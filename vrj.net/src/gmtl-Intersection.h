// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

#ifndef _VRJ_NET_GMTL_INTERSECTION_H_
#define _VRJ_NET_GMTL_INTERSECTION_H_

#include <gmtl/Intersection.h>


namespace gmtl
{
   template bool intersect(const gmtl::AABoxd&, const gmtl::AABoxd&);
   template bool intersect(const gmtl::AABoxf&, const gmtl::AABoxf&);

   template bool intersect(const gmtl::AABoxd&, const gmtl::Point3d&);
   template bool intersect(const gmtl::AABoxf&, const gmtl::Point3f&);

   template bool intersect(const gmtl::AABoxd&, const gmtl::Vec3d&,
                           const gmtl::AABoxd&, const gmtl::Vec3d&,
                           double&, double&);
   template bool intersect(const gmtl::AABoxf&, const gmtl::Vec3f&,
                           const gmtl::AABoxf&, const gmtl::Vec3f&,
                           float&, float&);

   template bool intersect(const gmtl::Sphered&, const gmtl::Vec3d&,
                           const gmtl::Sphered&, const gmtl::Vec3d&,
                           double&, double&);
   template bool intersect(const gmtl::Spheref&, const gmtl::Vec3f&,
                           const gmtl::Spheref&, const gmtl::Vec3f&,
                           float&, float&);

   template bool intersect(const gmtl::AABoxd&, const gmtl::Sphered&);
   template bool intersect(const gmtl::AABoxf&, const gmtl::Spheref&);

   template bool intersect(const gmtl::Sphered&, const gmtl::AABoxd&);
   template bool intersect(const gmtl::Spheref&, const gmtl::AABoxf&);

   template bool intersect(const gmtl::Sphered&, const gmtl::Point3d&);
   template bool intersect(const gmtl::Spheref&, const gmtl::Point3f&);

   template bool intersect(const gmtl::Sphered&, const gmtl::Rayd&, int&,
                           float&, float&);
   template bool intersect(const gmtl::Spheref&, const gmtl::Rayf&, int&,
                           float&, float&);

   template bool intersect(const gmtl::Sphered&, const gmtl::LineSegd&, int&,
                           float&, float&);
   template bool intersect(const gmtl::Spheref&, const gmtl::LineSegf&, int&,
                           float&, float&);

   template bool intersectVolume(const gmtl::Sphered&, const gmtl::LineSegd&,
                                 int&, float&, float&);
   template bool intersectVolume(const gmtl::Spheref&, const gmtl::LineSegf&,
                                 int&, float&, float&);

   template bool intersectVolume(const gmtl::Sphered&, const gmtl::Rayd&,
                                 int&, float&, float&);
   template bool intersectVolume(const gmtl::Spheref&, const gmtl::Rayf&,
                                 int&, float&, float&);

   template bool intersect(const gmtl::Planed&, const gmtl::Rayd&, double&);
   template bool intersect(const gmtl::Planef&, const gmtl::Rayf&, float&);

   template bool intersect(const gmtl::Planed&, const gmtl::LineSegd&, double&);
   template bool intersect(const gmtl::Planef&, const gmtl::LineSegf&, float&);

   template bool intersect(const gmtl::Trid&, const gmtl::Rayd&, float&,
                           float&, float&);
   template bool intersect(const gmtl::Trif&, const gmtl::Rayf&, float&,
                           float&, float&);

   template bool intersect(const gmtl::Trid&, const gmtl::LineSegd&, float&,
                           float&, float&);
   template bool intersect(const gmtl::Trif&, const gmtl::LineSegf&, float&,
                           float&, float&);
}

#endif /* _VRJ_NET_GMTL_INTERSECTION_H_ */
