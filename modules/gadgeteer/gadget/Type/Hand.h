/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#ifndef _VJ_HAND_H_
#define _VJ_HAND_H_

#include <vrj/vrjConfig.h>
#include <gad/Type/Finger.h>

namespace vrj
{

//  DIJ = Distal Interphalangeal Joint  --- Finger tip
//  PIJ = Proximal "              "     --- Middle joint
//  MPJ = Metacarpo "             "     --- closest to Hand
//  ABDUCT = spread of Fingers
//
// YAW and PITCH apply only to WRIST
class Hand
{
public:
   Hand( const float& yaw = 0, const float& pitch = 0 ) :
         mPinky( -15, -45, -45, 0 ),
         mRing( -15, -45, -45, 0 ),
         mMiddle( -15, -45, -45, 0 ),
         mIndex( -15, -45, -45, 0 ),
         mThumb( -15, -45, -45, 0 ),
         mYaw( yaw ),
         mPitch( pitch )
   {
   }

   inline Finger& pinky() { return mPinky; }
   inline Finger& ring() { return mRing; }
   inline Finger& middle() { return mMiddle; }
   inline Finger& index() { return mIndex; }
   inline Finger& thumb() { return mThumb; }
   inline const float& pitch() const { return mPitch; }
   inline const float& yaw() const { return mYaw; }

   inline void setPitch( const float& pitch ) { mPitch = pitch; }
   inline void setYaw( const float& yaw ) { mYaw = yaw; }

   // set Fingers based on digital information.
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
   Finger mPinky, mRing, mMiddle, mIndex, mThumb;
   float mYaw, mPitch;
};


};

#endif
