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

#ifndef JUGGLER_PFRAY_COLLIDER
#define JUGGLER_PFRAY_COLLIDER

#include <collider.h>

//: Collider class for testing collisions in Performer
//
// This class will test collisions against a given world model
//
// NOTE: Remember that the navigation system is dealing
//     in Juggler (OpenGL) coordinates, not in Performer coords.
//     So there is some coord system conversion that will be
//     going on here.
class pfRayCollider : public collider
{
public:
   // ARGS: world - The node to start collision with
   //       it should be the one under the one being used for navigation ( ie. pfNaver)
   pfRayCollider( pfNode* world )
   {
      mWorldNode = world;
   }

   //: test a movement
   //!ARGS: whereYouAre - The current position we are at
   //!ARGS: delta - The amount we want to move
   //!ARGS: correction - The amount to correct the movement so that we do not collide
   //!RETURNS: true - There was a hit.
   virtual bool testMove( Vec3 whereYouAre, Vec3 delta, Vec3& correction, bool whereYouAreWithDelta = false );

   // general ray collision routine with a scene.
   // correctionVector - should be added to whereYouWantToBe after calling this function
   bool testRayCollision( pfVec3& intersectionPt,
                        pfVec3& whereYouReallyAre,
                        pfNode *objNode, int mask,
                        pfVec3 whereYouAre,
                        pfVec3 whereYouWantToBe );

   // data structure to get intersection result back from pfHit::mQuery
   class pfHitQueryResult
   {
   public:
       int     flags;
       int     whichseg;
       pfVec3  point;
       pfVec3  normal;
   };


protected:
   pfNode* mWorldNode;        // The world to collide with
};

// general ray collision routine with a scene.
// correctionVector - should be added to whereYouWantToBe after calling this function
bool pfRayCollider::testRayCollision( pfVec3& intersectionPt, pfVec3& whereYouReallyAre, pfNode *objNode, int mask, pfVec3 whereYouAre, pfVec3 whereYouWantToBe )
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


bool pfRayCollider::testMove(Vec3 _whereYouAre, Vec3 _delta, Vec3& correction, bool whereYouAreWithDelta)
{
   pfVec3 whereYouAre = GetPfVec(_whereYouAre);
   pfVec3 delta = GetPfVec(_delta);
   pfVec3 whereYouWantToBe = whereYouAre + delta;

   pfVec3 intersectionPt;
   pfVec3 whereYouReallyAre;
   if (this->testRayCollision(intersectionPt, whereYouReallyAre, mWorldNode,0x1, whereYouAre, whereYouWantToBe))
   {
      pfVec3 pf_correction = (whereYouReallyAre - whereYouWantToBe);
      correction = GetVjVec( pf_correction);

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

#endif
