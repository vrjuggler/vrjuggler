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

#include <vjStdCaveNavigator.h>


vjStdCaveNavigator::vjStdCaveNavigator()
    : vjMatrix(), mNowTranslating( false ), mNowRotating( false )
{
   mVNav.setRotAxis(false, true, false);
   mVNav.setMode( velocityNav::FLY );
}

//: call once per frame (pre or post frame, not intra frame)
int vjStdCaveNavigator::update()
{
   vjDEBUG(vjDBG_ALL, vjDBG_VERB_LVL) << "b0: " << mNowTranslating
                         << "b1: " << mNowRotating
                         //<< "b2: " << button2_state
                         << "\t" << vjDEBUG_FLUSH;

   mVNav.update( mDeviceMatrix, mNowTranslating, mNowRotating);    // mat, trans, rot

   if(true == mNowTranslating)     // Translate
      mVNav.incTransVelocity();
   else
      mVNav.zeroTransVelocity();       // Set velocity to 0

   // Set the matrix to the navigation matrix
   vjMatrix cur_pos,world_pos;
   cur_pos = mVNav.getCurPos();  // Invert because we want to move the world
   //std::cerr << "Set Pos: " << vjCoord(cur_pos).pos << std::endl;
   world_pos.invert( cur_pos );
   this->copy( world_pos );


   return 1;
}
