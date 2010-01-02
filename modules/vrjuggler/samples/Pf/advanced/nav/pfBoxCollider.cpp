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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/Draw/Pf/Util.h>
#include "pfBoxCollider.h"


pfBoxCollider::~pfBoxCollider()
{
}

bool pfBoxCollider::testMove(const gmtl::Vec3f& feetPosition,
                             const gmtl::Vec3f& feetDelta,
                             gmtl::Vec3f& correction,
                             bool whereYouAreWithDelta)
{
   pfVec3 pf_feet_position = vrj::pf::GetPfVec(feetPosition);
   pfVec3 pf_feet_delta = vrj::pf::GetPfVec(feetDelta);
   pfVec3 pf_feet_destination = (pf_feet_position + pf_feet_delta);
   pfVec3 pf_correction;         // Needs to be set

   // whats the maximum step height?
   float step_height( 5.0f ); // value picked out of my ass.
   pfVec3 up( 0.0f, 0.0f, 1.0f );
   // whats the ray start position that would test for that step height?
   pfVec3 pf_step_destination = pf_feet_destination + up * step_height;

   // get the speed, can only be positive bye definition of .length()
   /*
   float speed = pf_feet_delta.length();

   // get the radius..       // Terry hack
   float vol_radius = speed * 2.0f;

   // constrain value to always have unit length or greater
   if(vol_radius < 1.0f)
      vol_radius = 1.0f;

   terryCollide.setRadius( vol_radius );    // Setup collision volume
   */

   terryCollide.setVelocity(pf_feet_delta);

   if ( terryCollide.collide(pf_correction, mWorldNode, pf_step_destination) )
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
