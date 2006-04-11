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

#ifndef TERRY_POGOSTICK_COLLIDER
#define TERRY_POGOSTICK_COLLIDER

#include <Performer/pf.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pr.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pfutil.h>
#include <iostream>
#include <math.h>

#include "pfTerryCollide.h" // my base class

///////////////////////////////////////////////////////////////////////////////////
// Down vector "Pogostick" (or ground) collision
/////////////////////////////////

class pfTerryPogoCollide : public pfTerryCollide
{
public:
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
   bool collide( pfVec3& correction, pfNode *objNode, int mask, pfVec3 posAboveYourFeet, float heightAboveYourFeet );

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
