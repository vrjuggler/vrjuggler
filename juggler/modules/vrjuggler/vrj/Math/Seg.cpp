/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>
#include <Math/vjSeg.h>
#include <Math/vjPlane.h>


//: Finds the point on the seg nearest to pt.
// Returns the nearest point in nearPt
//
// Makes assumptions that all pt dir is normalized
void vjSeg::findNearestPt(const vjVec3& pt, vjVec3& nearPt)
{
   // GGI Pg. 300
   // Find plane through point pt normal to line
   // Isect the line with this plane
   vjASSERT(dir.isNormalized());
   vjPlane J;
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

bool vjSeg::isectTriangle(const vjVec3 _v1, const vjVec3 _v2, const vjVec3 _v3, float* t)
{
   // Graphic Gems I: Page 392
   int i0, i1, i2;      // Axis indices
   float u0, u1, u2, v0, v1, v2, alpha, beta;
   i0 = 0;
   vjPlane triPlane;   // The plane the triangle is in
   float   tDist;      // Dist hit is along the seg
   vjVec3  hitPt;      // Point hit on the plane

   triPlane.makePts(_v1, _v2, _v3);
   if (triPlane.isect(*this, &tDist))    // Does the seg hit the plane?
   {
      hitPt = pos + (dir * tDist);    // Find point hit

      // Find max index
      // Note: I added the fabs because of bug in code.  NOT IN GGI
      for (int index=0;index<3;index++)
         if (fabs(triPlane.normal.vec[index]) > fabs(triPlane.normal.vec[i0]))
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
