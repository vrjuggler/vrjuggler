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

#include <vrj/vrjConfig.h>

#include <vrj/Math/Seg.h>
#include <vrj/Math/Plane.h>
#include <vrj/Math/Math.h>


namespace vrj
{

//: Finds the point on the seg nearest to pt.
// Returns the nearest point in nearPt
//
// Makes assumptions that all pt dir is normalized
void Seg::findNearestPt(const Vec3& pt, Vec3& nearPt)
{
   // GGI Pg. 300
   // Find plane through point pt normal to line
   // Isect the line with this plane
   vprASSERT(dir.isNormalized());
   Plane J;
   J.normal = dir;         // Plane normal
   J.offset = -(pt.dot(dir));
   float tDist;         // tDist along seg
   J.isectLine(*this, &tDist);

   // Find pt on the segment
   if (tDist < 0)               // Behind pos
      nearPt = pos;
   else if (tDist > length)     // Past end. NOTE: Assumes that dir is normalized
      nearPt = pos + (dir*length);
   else                        // It is on the line
      nearPt = pos + (dir*tDist);
}

bool Seg::isectTriangle(const Vec3 _v1, const Vec3 _v2, const Vec3 _v3, float* t)
{
   // Graphic Gems I: Page 392
   int i0, i1, i2;      // Axis indices
   float u0, u1, u2, v0, v1, v2, alpha, beta;
   i0 = 0;
   Plane triPlane;   // The plane the triangle is in
   float   tDist;      // Dist hit is along the seg
   Vec3  hitPt;      // Point hit on the plane

   triPlane.makePts(_v1, _v2, _v3);
   if (triPlane.isect(*this, &tDist))    // Does the seg hit the plane?
   {
      hitPt = pos + (dir * tDist);    // Find point hit

      // Find max index
      // Note: I added the abs because of bug in code.  NOT IN GGI
      for (int index=0;index<3;index++)
         if (Math::abs(triPlane.normal.vec[index]) > Math::abs(triPlane.normal.vec[i0]))
            i0 = index;

      if (i0 == 0)
      {
         i1 = 1; i2 = 2;
      }
      else if (i0 == 1)
      {
         i1 = 2; i2 = 0;
      }
      else
      {
         i1 =0; i2 = 1;
      }

      u0 = hitPt[i1] - _v1[i1];
      v0 = hitPt[i2] - _v1[i2];
      u1 = _v2[i1] - _v1[i1];
      u2 = _v3[i1] - _v1[i1];
      v1 = _v2[i2] - _v1[i2];
      v2 = _v3[i2] - _v1[i2];
      if (u1 == 0)
      {
         beta = (u0/u2);
         if ((beta >= 0) && (beta<= 1))
            alpha = (v0 - (beta*v2))/v1;
      }
      else
      {
         beta = ((v0*u1) - (u0*v1))/((v2*u1) - (u2*v1));
         if ((beta >= 0) && (beta<= 1))
            alpha = (u0 - (beta*u2))/u1;
      }

      if ((alpha >= 0) && (beta >= 0) && ((alpha+beta) <= 1))      // If it is in the tri
      {
         *t = tDist;
         return true;
      }
      else
      {
         return false;
      }

   }
   else
      return false;
}

};
