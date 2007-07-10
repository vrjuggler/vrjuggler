/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef JUGGLER_PFRIDE_COLLIDER
#define JUGGLER_PFRIDE_COLLIDER

#include <collider.h>
#include <pfTerryPogoCollide.h>

//: Collider class for testing collisions in Performer
//
// This class will test collisions against a given world model
//
// NOTE: Remember that the navigation system is dealing
//     in Juggler (OpenGL) coordinates, not in Performer coords.
//     So there is some coord system conversion that will be
//     going on here.
class pfPogoCollider : public collider
{
public:
   // ARGS: world - The node to start collision with
   //       it should be the one under the one being used for navigation ( ie. pfNaver)
   pfPogoCollider(pfNode* world)
   {
      mWorldNode = world;
   }

   virtual ~pfPogoCollider()
   {
   }

   //: test a movement
   //!ARGS: feetPosition - The current position we are at
   //!ARGS: delta - The amount we want to move
   //!ARGS: correction - The amount to correct the movement so that we do not collide
   //!RETURNS: true - There was a hit.
   virtual bool testMove(gmtl::Vec3f feetPosition, gmtl::Vec3f delta, gmtl::Vec3f& correction, bool feetPositionWithDelta = false);

protected:
   pfNode* mWorldNode;        // The world to collide with
   pfTerryPogoCollide terryCollide;
};


// height corrector keeps feet position above all polys
// feetPosition: current position of feet
// feetDelta: distance you want to move user's feet.
// correction: return value to correct feetDelta upon collision with polys.
// returns true on collide
// NOTE: not a robust collider, use in combination with a ray collider, or robust volume collider.
inline bool pfPogoCollider::testMove( gmtl::Vec3f feetPosition, gmtl::Vec3f feetDelta, gmtl::Vec3f& correction, bool feetPositionWithDelta )
{
   pfVec3 pf_feet_position    = vrj::pf::GetPfVec(feetPosition);
   pfVec3 pf_feet_delta       = vrj::pf::GetPfVec(feetDelta);
   pfVec3 pf_feet_destination = pf_feet_position + pf_feet_delta;
   pfVec3 pf_correction;      // Needs to be set

   // whats the maximum step height?
   float step_height( 3.5f ); // value picked out of my ass.
   pfVec3 up( 0.0f, 0.0f, 1.0f );
   // whats the ray start position that would test for that step height?
   pfVec3 pf_step_destination = pf_feet_destination + up * step_height;

   if (terryCollide.collide( pf_correction, mWorldNode, 0x1, pf_step_destination, step_height))
   {
      correction = vrj::pf::GetVjVec(pf_correction);

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
