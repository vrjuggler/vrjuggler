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

#ifndef VJ_HAND___
#define VJ_HAND___

#include <Input/vjGlove/vjFinger.h>

//  DIJ = Distal Interphalangeal Joint  --- vjFinger tip
//  PIJ = Proximal "              "     --- Middle joint
//  MPJ = Metacarpo "             "     --- closest to vjHand
//  ABDUCT = spread of vjFingers
//
// YAW and PITCH apply only to WRIST
class vjHand
{
public:
   vjHand( const float& yaw = 0, const float& pitch = 0 ) : 
         mYaw( yaw ), 
         mPitch( pitch ), 
         mPinky( -15, -45, -45, 0 ), 
         mRing( -15, -45, -45, 0 ), 
         mMiddle( -15, -45, -45, 0 ), 
         mIndex( -15, -45, -45, 0 ), 
         mThumb( -15, -45, -45, 0 )
   {
   }   
   
   inline vjFinger& pinky() { return mPinky; }
   inline vjFinger& ring() { return mRing; }
   inline vjFinger& middle() { return mMiddle; }
   inline vjFinger& index() { return mIndex; }
   inline vjFinger& thumb() { return mThumb; }
   inline const float& pitch() const { return mPitch; }
   inline const float& yaw() const { return mYaw; }
   
   inline void setPitch( const float& pitch ) { mPitch = pitch; }
   inline void setYaw( const float& yaw ) { mYaw = yaw; }
   
   // set vjFingers based on digital information.
   void setFingers( const bool& pinky, const bool& ring, const bool& middle, const bool& index, const bool& thumb )
   {
      if (pinky) 
         mPinky.set( -15, -45, -45, 0 );
      else
         mPinky.set( -15, -0, -45, 0 );
      
      if (ring) 
         mRing.set( -15, -45, -45, 0 );
      else
         mRing.set( -15, -0, -45, 0 );
      
      if (middle) 
         mMiddle.set( -15, -45, -45, 0 );
      else
         mMiddle.set( -15, -0, -45, 0 );
      
      if (index) 
         mIndex.set( -15, -45, -45, 0 );
      else
         mIndex.set( -15, -0, -45, 0 );
      
      if (thumb) 
         mThumb.set( -15, -45, -45, 0 );
      else
         mThumb.set( -15, -0, -45, 0 );
   }

private:  
   vjFinger mPinky, mRing, mMiddle, mIndex, mThumb;
   float mYaw, mPitch;
};


#endif
