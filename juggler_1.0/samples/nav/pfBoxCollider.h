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

#ifndef JUGGLER_PFVOLUME_COLLIDER
#define JUGGLER_PFVOLUME_COLLIDER

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
class pfVolumeCollider : public collider
{
public:
   // ARGS: world - The node to start collision with
   //       it should be the one under the one being used for navigation ( ie. pfNaver)
   pfVolumeCollider(pfNode* world) : terryCollide( 0x1 )
   {
      mWorldNode = world;
   }

   //: test a movement
   //!ARGS: whereYouAre - The current position we are at
   //!ARGS: delta - The amount we want to move
   //!ARGS: correction - The amount to correct the movement so that we do not collide
   //!RETURNS: true - There was a hit.
   virtual bool testMove(vjVec3 whereYouAre, vjVec3 delta, vjVec3& correction, bool whereYouAreWithDelta = false);

public:
   pfNode* mWorldNode;        // The world to collide with
   pfTerryVCollide terryCollide;
};


bool pfVolumeCollider::testMove(vjVec3 whereYouAre, vjVec3 delta, vjVec3& correction, bool whereYouAreWithDelta)
{
   //cout<<"Pos["<<whereYouAre<<"] :|: Vel["<<delta<<"]\n"<<flush;
   pfVec3 pf_cur_pos = vjGetPfVec(whereYouAre);
   pfVec3 pf_delta = vjGetPfVec(delta);
   pfVec3 pf_correction( 0.0f, 0.0f, 0.0f );
   
   pfVec3 pf_new_pos;
   if(whereYouAreWithDelta)
      pf_new_pos = pf_cur_pos;
   else
      pf_new_pos = (pf_cur_pos + pf_delta);

   // get the speed.
   float speed = pf_delta.length();
   
   // get the radius..       // Terry hack
   float vol_radius = fabs( speed ); //* 2.0f;
  
   // constrain value to always have unit length or greater
   if(vol_radius < 1.0f)
      vol_radius = 1.0f;
      
   terryCollide.setVolumeRadius( vol_radius );    // Setup collision volume

   if (terryCollide.collideVolume( pf_correction, mWorldNode, pf_new_pos) )
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
