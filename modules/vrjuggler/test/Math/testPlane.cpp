/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
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

#include <iostream>

#include <Math/vjPlane.h>
#include <Math/vjVec3.h>
#include <Math/vjSeg.h>

// Program to test the functionality of the Plane class
//
//
int main(void)
{
   // Create some planes and segs to use in the tests
   const Vec3 x_axis(1,0,0);
   const Vec3 y_axis(0,1,0);
   const Vec3 z_axis(0,0,1);
   const Vec3 origin(0,0,0);
   Plane zy_plane; zy_plane.makePts(z_axis,origin,y_axis);

   // Test finding nearest point
   Vec3 near_pt;
   float dist;
   dist = zy_plane.findNearestPt(Vec3(1,0,0), near_pt);

   std::cout << "Testing findNearestPt: ";
   if((dist > 0.0f) && (near_pt == origin))
      std::cout << "Passed.\n";
   else
      std::cout << "FAILED!!!\n";

   // Test isect with a seg
   Seg seg_through, seg_toward, seg_parallel;
   float t_dist;
   bool hit;

   seg_through.makePts(x_axis,-1*x_axis);    // Passes through the plane at the origin
   seg_toward.makePts(x_axis, x_axis/2.0f);  // Points toward the plane at the origin
   seg_parallel.makePts(x_axis, (x_axis+z_axis));  // Is parallel to the plane

   hit = zy_plane.isect(seg_through,&t_dist);
   std::cout << "Testing isect (through): ";
   if(hit && Math::isZero(t_dist - 1.0f))
      std::cout << "Passed.\n";
   else
      std::cout << "FAILED!!!!\n";

   hit = zy_plane.isect(seg_toward,&t_dist);
   std::cout << "Testing isect (toward): ";
   if(!hit)
      std::cout << "Passed.\n";
   else
      std::cout << "FAILED!!!!\n";

   hit = zy_plane.isect(seg_parallel,&t_dist);
   std::cout << "Testing isect (parallel): ";
   if(!hit)
      std::cout << "Passed.\n";
   else
      std::cout << "FAILED!!!!\n";


   // Test isect with a line
   hit = zy_plane.isectLine(seg_through,&t_dist);
   std::cout << "Testing isectLine (through): ";
   if(hit && Math::isZero(t_dist - 1.0f))
      std::cout << "Passed.\n";
   else
      std::cout << "FAILED!!!!\n";

   hit = zy_plane.isectLine(seg_toward,&t_dist);
   std::cout << "Testing isectLine (toward): ";
   if(hit && Math::isZero(t_dist - 1.0f))
      std::cout << "Passed.\n";
   else
      std::cout << "FAILED!!!!\n";

   hit = zy_plane.isectLine(seg_parallel,&t_dist);
   std::cout << "Testing isectLine (parallel): ";
   if(!hit)
      std::cout << "Passed.\n";
   else
      std::cout << "FAILED!!!!\n";

}
