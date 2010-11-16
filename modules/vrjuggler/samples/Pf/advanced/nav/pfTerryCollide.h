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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef TERRY_COLLIDE
#define TERRY_COLLIDE

#include <Performer/pf.h>
#include <Performer/pf/pfNode.h>

///////////////////////////////////////////////////////////////////////////////////
// base class for terry's nav routines.
////////////////////////////////////////////////////////////////////////////////

class pfTerryCollide
{
public:
   enum Flag
   {
      COLLIDE_STATIC = 0,
      COLLIDE_DYNAMIC = 1
   };
      
   //: Setup the node you want to collide with using
   //: nodeSetup(node to collide with, COLLIDE_STATIC or
   //: COLLIDE_DYNAMIC, mask)
   //
   // Usually you'll pass a node representing your entire
   // world and a simple mask such as 0x01.  The two different
   // collide modes control caching.  Objects that will always
   // remain in the same place should be COLLIDE_STATIC.
   // This function is not necessary if you don't want to
   // distinguish different geometry using different masks.
   void nodeSetup(pfNode* node, const int mode, const int mask);
   
   // data structure to get intersection result back from pfHit::mQuery
   class pfHitQueryResult
   {
   public:
       int     flags;
       int     whichseg;
       pfVec3  point;
       pfVec3  normal;
   };
};

#endif
