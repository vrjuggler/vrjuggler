/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include "pfTerryPogoCollide.h"

// posAboveYourFeet: either foot position or head position is generally computed from a navigation
//  routine.  you need foot position, which is the same as headposition + downVector * yourHeight
//  Once you have your foot position, choose a step height heightAboveYourFeet (generally at your waist)
//  posAboveYourFeet is going to be at heightAboveYourFeet above your feet, 
//  this causes a ray to be cast from this stepHeight down to your feet.
// i.e. to calculate:
// float heightAboveYourFeet( 3.0f ); // about as high as humans usually step.
// pfVec3 up( 0.0f, 0.0f, 1.0f );
// pfVec3 posAboveYourFeet = feetPosition + up * heightAboveYourFeet;
//
// correction is returned as a vector to correct your position if collision occured.
// objNode is your graph
// mask is whatever...
// posAboveYourFeet is set to a place inbetween your feet and head, 
//                  that would normally be comfortable height for a human to step.
// heightAboveYourFeet is the length from posAboveYourFeet to your feet
bool pfTerryPogoCollide::collide( pfVec3& correction, pfNode *objNode, int mask, pfVec3 posAboveYourFeet, float heightAboveYourFeet )
{
    pfHit **hit[1];
    pfSegSet segset;
    pfMatrix collidemat;
    
    // Make a ray looking "down" at terrain
    segset.segs[0].pos = posAboveYourFeet;
    segset.segs[0].dir = pfVec3(0.0, 0.0, -1.0);
    segset.segs[0].length = heightAboveYourFeet;

    segset.mode = PFTRAV_IS_PRIM|PFTRAV_IS_NORM|PFTRAV_IS_CULL_BACK;
    segset.userData = NULL;
    segset.isectMask = mask;
    segset.activeMask = 0x1;
    segset.bound = NULL;
    segset.discFunc = NULL;

   // Query for collision data
   if (objNode->isect(&segset, hit) > 0)
   {
      uint query[] = {PFQHIT_FLAGS, PFQHIT_SEGNUM, PFQHIT_POINT, NULL};
	   
      // get the intersection results.
      pfHitQueryResult isectResult;
	   hit[0][0]->mQuery( query, &isectResult );
      
	   if(isectResult.flags & PFHIT_XFORM)
      {    
         // correct for DCS transformations
	      hit[0][0]->query( PFQHIT_XFORM, &collidemat );
	      isectResult.point.xformPt( isectResult.point, collidemat );
	   }
      pfVec3 delta = isectResult.point - posAboveYourFeet;
	   correction = delta + pfVec3(0.0, 0.0, heightAboveYourFeet);
	   return true;
   }
   return false;
}



bool pfTerryPogoCollide::collideRide( pfVec3 &bounce, pfVec3 &ridexyz, pfVec3 &ridehpr, pfNode *objNode, int mask, pfVec3 pos, float length )
{
   pfHit** hit[1];
   pfSegSet segset;
   pfNode* newnode( NULL );
   pfVec3 originalpoint, oldpoint;
   pfCoord coord;
   pfMatrix collidemat;
   
   // TODO: remove statics.
   static pfNode* oldnode( NULL );
   static pfMatrix oldmat;

   // Make a ray looking "down" at terrain
   segset.segs[0].pos = pos;
   segset.segs[0].dir = pfVec3(0.0, 0.0, -1.0);
   segset.segs[0].length = length;

   segset.mode = PFTRAV_IS_PRIM|PFTRAV_IS_NORM|PFTRAV_IS_CULL_BACK;
   segset.userData = NULL;
   segset.isectMask = mask;
   segset.activeMask = 0x1;
   segset.bound = NULL;
   segset.discFunc = NULL;

   // Query for collision data
   if (objNode->isect( &segset, hit ) > 0)
   {
	   uint query[] = {PFQHIT_FLAGS, PFQHIT_SEGNUM, PFQHIT_POINT, NULL};
	   
      // get the intersection results.
      pfHitQueryResult isectResult;
      hit[0][0]->mQuery( query, &isectResult );
	   
      if (isectResult.flags & PFHIT_XFORM)
      {    
         // correct for DCS transformations
	      hit[0][0]->query( PFQHIT_XFORM, &collidemat );
	      hit[0][0]->query( PFQHIT_NODE, &newnode );
	      originalpoint = isectResult.point;
	      isectResult.point.xformPt( isectResult.point, collidemat );
	      if (newnode == oldnode)
         {
		      oldpoint.xformPt( originalpoint, oldmat );
		      ridexyz.sub( isectResult.point, oldpoint );
		      collidemat.getOrthoCoord( &coord );
		      ridehpr = coord.hpr;
		      oldmat.getOrthoCoord( &coord );
		      ridehpr.sub( ridehpr, coord.hpr );
         }

         else
         {
		      oldnode = newnode;
		      ridexyz = pfVec3( 0.0, 0.0, 0.0 );
		      ridehpr = pfVec3( 0.0, 0.0, 0.0 );
	      }

         oldmat = collidemat;
	   }
	   
      else
      {
	       oldnode = NULL;
	       ridexyz = pfVec3( 0.0, 0.0, 0.0 );
	       ridehpr = pfVec3( 0.0, 0.0, 0.0 );
	   }
	   pos.sub( isectResult.point, pos );
	   bounce.add( pos, pfVec3(0.0, 0.0, length) );
	   return true;
   }
   
   else
   {
	   oldnode = NULL;
	   ridexyz = pfVec3( 0.0, 0.0, 0.0 );
	   ridehpr = pfVec3( 0.0, 0.0, 0.0 );
   }
   return false;
}
