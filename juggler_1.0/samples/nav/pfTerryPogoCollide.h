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

#ifndef TERRY_POGOSTICK_COLLIDER
#define TERRY_POGOSTICK_COLLIDER

#include <Performer/pf.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pr.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pfutil.h>
#include <iostream.h>
#include <math.h>

#include "pfTerryCollide.h" // my base class

///////////////////////////////////////////////////////////////////////////////////
// Down vector "Pogostick" (or ground) collision
/////////////////////////////////

class pfTerryPogoCollide : public pfTerryCollide
{
public:
   //: For collisions with the ground, use collide(
   //: displacement vector, the whole world, mask, your position
   //: length of collision vector).  
   //
   // The world and mask will
   // probably be the same ones you used in nodeSetup.
   // Make sure that the length of the vector is longer than
   // the rate at which you are falling or you might fall right
   // through the ground.
   bool collide( pfVec3 &bounce, pfNode *objNode, int mask, pfVec3 pos, float length );
   
   //: collideRide(displacement vector, position
   //: offset, rotation offset, the whole world, mask, your
   //: position) 
   //
   // is just like collide(...) but with two extra
   // parameters.  If you have animated geometry that you can
   // stand on, these parameters will tell you how much to
   // change your position and rotation by each frame to stay
   // on top of things.  If you want to stand up straight all
   // the time, you'll probably just want to use the heading
   // from the rotation offset.  The vector stores an hpr, but
   // the pitch and roll probably won't be useful.
   bool collideRide( pfVec3 &bounce, pfVec3 &ridexyz, pfVec3 &ridehpr, pfNode *objNode, int mask, pfVec3 pos, float length );
};
      

#endif
