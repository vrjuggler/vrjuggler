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

#ifndef JUGGLER_PFRIDE_COLLIDER
#define JUGGLER_PFRIDE_COLLIDER

#include <collider.h>
#include <collide.h>

//: Collider class for testing collisions in Performer
//
// This class will test collisions against a given world model
//
// NOTE: Remember that the navigation system is dealing
//     in Juggler (OpenGL) coordinates, not in Performer coords.
//     So there is some coord system conversion that will be
//     going on here.
class pfRideCollider : public collider
{
public:
   // ARGS: world - The node to start collision with
   //       it should be the one under the one being used for navigation ( ie. pfNaver)
   pfRideCollider(pfNode* world)
   {
      mWorldNode = world;
   }

   //: test a movement
   //!ARGS: whereYouAre - The current position we are at
   //!ARGS: delta - The amount we want to move
   //!ARGS: correction - The amount to correct the movement so that we do not collide
   //!RETURNS: true - There was a hit.
   virtual bool testMove(vjVec3 whereYouAre, vjVec3 delta, vjVec3& correction, bool whereYouAreWithDelta = false);

protected:
   pfNode* mWorldNode;        // The world to collide with
   pfTerryRCollide terryCollide;
};


bool pfRideCollider::testMove(vjVec3 whereYouAre, vjVec3 delta, vjVec3& correction, bool whereYouAreWithDelta)
{
   pfVec3 pf_cur_pos = vjGetPfVec(whereYouAre);
   pfVec3 pf_delta = vjGetPfVec(delta);
   pfVec3 pf_correction;         // Needs to be set

   float height(6.0f);

   pfVec3 pf_new_pos;
   pf_new_pos = (pf_cur_pos + pf_delta);
   float delta_mag = pf_delta.length();
   if(delta_mag > height)
      cerr << "ERROR: pfRideCollider: Trying to move faster than the vector is checking.\n";

   // This function tests to make sure that for height straight down, there is no collision
   // from the position we are testing.
   // So I need to move that position up in the air to make "room" for the collision vector
   // XXX: This is because of hacks in the collision code
   pf_new_pos += pfVec3(0,0,height);

   if (terryCollide.collideGround(pf_correction,mWorldNode,0x1,pf_new_pos, height))
   {
      correction = vjGetVjVec(pf_correction);

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
