/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

// Awesome Performer collision routines by Terry Welsh
// February 8, 1998
//
// These functions provide a simulated volume collision
// detection routine and a ground collision detection
// routine.  The volume is simulated using eight vectors
// and provides suitable collision for most architectural
// walkthroughs and similar applications.  Any corner
// that is >= 90 degrees should be impossible to penetrate.
//
// Instructions:
//
// 1.  Setup the node you want to collide with using
// nodeSetup(node to collide with, COLLIDE_STATIC or
// COLLIDE_DYNAMIC, mask)
// Usually you'll pass a node representing your entire
// world and a simple mask such as 0x01.  The two different
// collide modes control caching.  Objects that will always
// remain in the same place should be COLLIDE_STATIC.
// This function is not necessary if you don't want to
// distinguish different geometry using different masks.
//
// 2.  Provide the VolumeSetup class with a mask for what you
// want to collide with (probably the same mask you used in
// nodeSetup.
//
// 3.  To collide with objects using the simulated volume
// routine, call collideVolumeRadius(radius of volume) and
// then call collideVolume(displacement vector, the whole
// world, your position).  You'll probably want to give
// one radius at the beginning of your application, but
// collideVolumeRadius can be called over and over if you
// want your radius to change.  Make sure the radius is
// larger than the fastest speed you can travel, otherwise
// you may penetrate some walls.  The world node that you
// provide represents your whole scene and is probably the
// same node you passed to nodeSetup if you only
// called it once.  This function returns a 1 if there was
// a collision and a 0 otherwise.  If there is a collision,
// the displacement vector will say what to offset your
// position by to eliminate the collision.
//
// 4.  For collisions with the ground, use collideGround(
// displacement vector, the whole world, mask, your position
// length of collision vector).  The world and mask will
// probably be the same ones you used in nodeSetup.
// Make sure that the length of the vector is longer than
// the rate at which you are falling or you might fall right
// through the ground.
//     collideGroundAndRide(displacement vector, position
// offset, rotation offset, the whole world, mask, your
// position) is just like collideGround but with two extra
// parameters.  If you have animated geometry that you can
// stand on, these parameters will tell you how much to
// change your position and rotation by each frame to stay
// on top of things.  If you want to stand up straight all
// the time, you'll probably just want to use the heading
// from the rotation offset.  The vector stores an hpr, but
// the pitch and roll probably won't be useful.

#ifndef _COLLIDE_H_
#define _COLLIDE_H_

#include <Performer/pf.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pr.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pfutil.h>
#include <iostream.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////////
// base class.
/////////////////////

class pfTerryCollide
{
public:
   enum Flag
   {
      COLLIDE_STATIC = 0,
      COLLIDE_DYNAMIC = 1
   };
   void nodeSetup( pfNode* node, int mode, int mask );
   
   // data structure to get intersection result back from pfHit::mQuery
   class pfHitQueryResult
   {
   public:
       int     flags;
       int     whichseg;
       pfVec3  point;
       pfVec3  normal;
   };
   pfHitQueryResult isectResult;
};



///////////////////////////////////////////////////////////////////////////////////
// volume collide
/////////////////////
class pfTerryVCollide : public pfTerryCollide
{
public:
   static const int COLLIDE_SEGMENTS;
   pfSegSet mCollideVolume;
   
   pfTerryVCollide( int isectMask = 0x01 );

   // set the intersect mask of this volume
   // needs to match every node tht you want to intersect with
   // read the performer docs for more info (see pfNode, or pfGeoSet::isect function)
   void setup( const int& isectMask ); 
   
   // set the radius of the collide volume
   void setVolumeRadius(float radius);
   int collideVolume(pfVec3 &bounce, pfNode *objNode, pfVec3 pos);
};


///////////////////////////////////////////////////////////////////////////////////
// ray collide
/////////////////////

class pfTerryRCollide : public pfTerryCollide
{
public:
   bool collideGround( pfVec3 &bounce, pfNode *objNode, int mask, pfVec3 pos, float length );
   bool collideGroundAndRide( pfVec3 &bounce, pfVec3 &ridexyz, pfVec3 &ridehpr, pfNode *objNode, int mask, pfVec3 pos, float length );
};
      
#endif
