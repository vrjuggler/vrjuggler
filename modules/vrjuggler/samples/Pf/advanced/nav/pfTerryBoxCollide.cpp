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

#include "pfTerryBoxCollide.h"

const int pfTerryBoxCollide::COLLIDE_SEGMENTS( 8 );

pfTerryBoxCollide::pfTerryBoxCollide(const int isectMask)
{
   mUnitBox[0] = pfVec3(0.57735f, 0.57735f, 0.57735f);
   mUnitBox[1] = pfVec3(0.57735f, 0.57735f, -0.57735f);
   mUnitBox[2] = pfVec3(0.57735f, -0.57735f, 0.57735f);
   mUnitBox[3] = pfVec3(0.57735f, -0.57735f, -0.57735f);
   mUnitBox[4] = pfVec3(-0.57735f, 0.57735f, 0.57735f);
   mUnitBox[5] = pfVec3(-0.57735f, 0.57735f, -0.57735f);
   mUnitBox[6] = pfVec3(-0.57735f, -0.57735f, 0.57735f);
   mUnitBox[7] = pfVec3(-0.57735f, -0.57735f, -0.57735f);
   
    mCollideVolume.mode = PFTRAV_IS_PRIM|PFTRAV_IS_NORM|PFTRAV_IS_CULL_BACK;
    mCollideVolume.userData = NULL;
    mCollideVolume.isectMask = isectMask;
    mCollideVolume.activeMask = 0xff;		    // test all segments
    mCollideVolume.bound = NULL;
    mCollideVolume.discFunc = NULL;
    for (int x = 0; x < 8; ++x)
    {
       mCollideVolume.segs[x].dir = mUnitBox[x];
       mCollideVolume.segs[x].length = 1.0f;
    }
}

// set the intersect mask of this volume
// needs to match every node tht you want to intersect with
// read the performer docs for more info (see pfNode, or pfGeoSet::isect function)
void pfTerryBoxCollide::setup(const int isectMask)
{
   mCollideVolume.isectMask = isectMask;
}   

//  Effectively sets the radius of your bounding volume (box/sphere)
void pfTerryBoxCollide::setRadius(const float radius)
{
   int i;

   for (i = 0; i < COLLIDE_SEGMENTS; i++)
   {
      mCollideVolume.segs[i].length = radius;
   }
}

void pfTerryBoxCollide::setVelocity(const pfVec3& velocityVec)
{
   int i;
   float velocity_length = velocityVec.length();
   float minimum_dist_to_outside_box = 0.3f; // I guessed...
   float fudge = 2.0;
   if (velocity_length > minimum_dist_to_outside_box)
   {
      for (i = 0; i < COLLIDE_SEGMENTS; ++i)
      {
         pfVec3 warpedBox;
         warpedBox[0] = (mUnitBox[i])[0] + velocityVec[0]*fudge;
         warpedBox[1] = (mUnitBox[i])[1] + velocityVec[1]*fudge;
         warpedBox[2] = (mUnitBox[i])[2] + velocityVec[2]*fudge;
         float warpedBoxLength = warpedBox.length();
         warpedBox.normalize(); // normalize, mCollideVolume uses warpedLength, 
                                // so we only need the unit vector.
         mCollideVolume.segs[i].dir = warpedBox;
         mCollideVolume.segs[i].length = warpedBoxLength;
      }
      //std::cout<<velocity_length<<"\n"<<std::flush;
   }
}

int pfTerryBoxCollide::collide(pfVec3& correction, pfNode* objNode,
                               const pfVec3& pos)
{   
   int i, j, returnval = 0;
   float distance;
   pfVec3 tempvec;
   pfHit **hits[COLLIDE_SEGMENTS];
   //pfMatrix xform;
   pfMatrix collidemat;
   
   // this is const, but pf doesn't take a const...
   static unsigned int query[] = {PFQHIT_FLAGS, PFQHIT_SEGNUM, PFQHIT_POINT, PFQHIT_NORM, NULL};
   
   // Set collision positions
   for (i = 0; i < COLLIDE_SEGMENTS; i++)
   {
      mCollideVolume.segs[i].pos = pos;
   }

   // Query for collision data
   correction = pfVec3(0, 0, 0);
   if (objNode->isect(&mCollideVolume, hits) > 0)
   {
      std::cout<<"Box: Hit "<<std::flush;
            
      for (i = 0; i < COLLIDE_SEGMENTS; i++)
      {
         // get the intersection results.
         pfHitQueryResult isectResult;
         hits[i][0]->mQuery( query, &isectResult );
         
         if ((isectResult.flags & PFHIT_POINT) && (isectResult.flags & PFHIT_NORM))
         {
            if (isectResult.flags & PFHIT_XFORM)
            {   // correct for DCS transformations
               hits[i][0]->query(PFQHIT_XFORM, &collidemat);
               isectResult.point.xformPt(isectResult.point, collidemat);
               isectResult.normal.xformVec(isectResult.normal, collidemat);
               isectResult.normal.normalize();
            }
            tempvec.sub(isectResult.point, pos);
            distance = tempvec.length();
            distance = mCollideVolume.segs[isectResult.whichseg].length - distance;
            tempvec.negate(mCollideVolume.segs[isectResult.whichseg].dir);
            tempvec.scale(distance, tempvec);
            distance = tempvec.dot(isectResult.normal);
            tempvec.scale(distance, isectResult.normal);
            for (j = 0; j < 3; j++)
            {     // Add on only necessary vector components
               if (tempvec[j] > 0)
               {
                  if (correction[j] <= 0)
                     correction[j] += tempvec[j];
                  else
                  {
                     if (tempvec[j] > correction[j])
                        correction[j] = tempvec[j];
                  }
               }
               else
               {
                  if (correction[j] >= 0)
                     correction[j] += tempvec[j];
                  else
                  {
                     if (tempvec[j] < correction[j])
                        correction[j] = tempvec[j];
                  }
               }
            }
            returnval++;
         }
      }
      std::cout<<(pos+correction)[0]<<" "<<(pos+correction)[1]<<" "<<(pos+correction)[2]<<"\n"<<std::flush;
   
   }
   return returnval;
}
