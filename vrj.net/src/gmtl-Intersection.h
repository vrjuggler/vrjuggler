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
