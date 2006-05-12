/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef TERRY_BOX_COLLIDE
#define TERRY_BOX_COLLIDE

#include <Performer/pf.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pr.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pfutil.h>
#include <iostream>
#include <math.h>
#include <vector>

#include "pfTerryCollide.h" // my base class

///////////////////////////////////////////////////////////////////////////////////
// uses vectors to approximate a box or sphere volume collider
///////////////////////////////////////////////////////////////

class pfTerryBoxCollide : public pfTerryCollide
{
public:
   static const int COLLIDE_SEGMENTS;
   pfSegSet                mCollideVolume;
   pfVec3                  mUnitBox[8];
      
   pfTerryBoxCollide( int isectMask = 0x01 );

   // set the intersect mask of this volume
   // needs to match every node tht you want to intersect with
   // read the performer manpage docs for more info 
   // (see pfNode, or pfGeoSet::isect function
   // NOTE: probably the same mask you used in nodeSetup
   void setup( const int& isectMask ); 
   
   // set the radius of the simulated box/sphere volume
   void setRadius(float radius);
   
   void setVelocity( pfVec3 velocityVec );
   
   //: To collide with objects using the simulated volume
   //: routine, call collide(displacement vector, the whole
   //: world, your position).  
   //
   // You'll probably want to give one radius at the beginning of 
   // your application, but setRadius can be called over and over 
   // if you want your radius to change.  Make sure the radius is
   // larger than the fastest speed you can travel, otherwise
   // you may penetrate some walls.  The world node that you
   // provide represents your whole scene and is probably the
   // same node you passed to nodeSetup if you only
   // called it once.  This function returns >1 if there was
   // a collision and 0 otherwise.  If there is a collision,
   // the displacement vector will say what to offset your
   // position by to eliminate the collision.
   //
   // NOTE: before calling this function call setRadius(radius of volume) 
   //       first
   int collide( pfVec3 &bounce, pfNode *objNode, pfVec3 pos);
};

#endif
