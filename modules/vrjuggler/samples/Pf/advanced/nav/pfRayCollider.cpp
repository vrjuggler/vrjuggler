/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include "pfRayCollider.h"


// general ray collision routine with a scene.
// correctionVector - should be added to whereYouWantToBe after calling this function
bool pfRayCollider::testRayCollision(pfVec3& intersectionPt,
                                     pfVec3& whereYouReallyAre,
                                     pfNode *objNode,
                                     int mask, const pfVec3& whereYouAre,
                                     const pfVec3& whereYouWantToBe)
{
   pfHit **hit[1];
   pfSegSet segset;
   pfMatrix collidemat;

   pfVec3 dir = whereYouWantToBe - whereYouAre;
   float length = dir.length();
   dir.normalize();

   // Make a ray looking "down" at terrain
   segset.segs[0].pos = whereYouAre;
   segset.segs[0].dir = dir;
   segset.segs[0].length = length;

   segset.mode = PFTRAV_IS_PRIM|PFTRAV_IS_NORM|PFTRAV_IS_CULL_BACK;
   segset.userData = NULL;
   segset.isectMask = mask;
   segset.activeMask = 0x1;
   segset.bound = NULL;
   segset.discFunc = NULL;

   // Query for collision data
   if (objNode->isect(&segset, hit) > 0)
   {
      std::cout<<"testRayCollision: Collided\n"<<std::flush;

      pfHitQueryResult isectResult;
      uint query[] = {PFQHIT_FLAGS, PFQHIT_SEGNUM, PFQHIT_POINT, PFQHIT_NORM, 0};
      hit[0][0]->mQuery( query, &isectResult );
      if(isectResult.flags & PFHIT_XFORM)
      {
         // correct for DCS transformations
         hit[0][0]->query( PFQHIT_XFORM, &collidemat );
         isectResult.point.xformPt( isectResult.point, collidemat );
         isectResult.normal.xformVec(isectResult.normal, collidemat);
         isectResult.normal.normalize();
      }
      // save the intersection point (returned from function)
      intersectionPt = isectResult.point;

      // calculate where you really are.
      pfVec3 correctionVector;
      correctionVector = whereYouWantToBe - intersectionPt;
      //whereYouReallyAre = intersectionPt;

      whereYouReallyAre = isectResult.normal * correctionVector.length();

      //pfVec3 bounceDirection = correctionVector;
      //bounceDirection.normalize();
      //whereYouReallyAre += bounceDirection * 2;
      return true;
   }
   return false;
}

bool pfRayCollider::testMove(const gmtl::Vec3f& _whereYouAre,
                             const gmtl::Vec3f& _delta,
                             gmtl::Vec3f& correction,
                             bool whereYouAreWithDelta)
{
   pfVec3 whereYouAre = vrj::pf::GetPfVec(_whereYouAre);
   pfVec3 delta = vrj::pf::GetPfVec(_delta);
   pfVec3 whereYouWantToBe = whereYouAre + delta;

   pfVec3 intersectionPt;
   pfVec3 whereYouReallyAre;
   if (this->testRayCollision(intersectionPt, whereYouReallyAre, mWorldNode,0x1, whereYouAre, whereYouWantToBe))
   {
      pfVec3 pf_correction = (whereYouReallyAre - whereYouWantToBe);
      correction = vrj::pf::GetVjVec(pf_correction);

      std::cout<<"pfRayCollider: Collided "<<correction<<"\n"<<std::flush;

      setDidCollide(true);
      return true;
   }
   else
   {
      setDidCollide(false);
      return false;
   }
}
