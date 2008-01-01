/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <vrj/Draw/Pf/Util.h>
#include "pfPogoCollider.h"


pfPogoCollider::~pfPogoCollider()
{
}

// height corrector keeps feet position above all polys
// feetPosition: current position of feet
// feetDelta: distance you want to move user's feet.
// correction: return value to correct feetDelta upon collision with polys.
// returns true on collide
// NOTE: not a robust collider, use in combination with a ray collider, or robust volume collider.
bool pfPogoCollider::testMove(const gmtl::Vec3f& feetPosition,
                              const gmtl::Vec3f& feetDelta,
                              gmtl::Vec3f& correction,
                              bool feetPositionWithDelta)
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
