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

#include <Sound/vjSoundEngine.h>
#include <Sound/vjSound.h>

// calculate the sound volume based on the observer position, sound position, falloff, and volume
void vjSound::getPercievedVolume( float& volume ) const
{
   // get the observer's position
   vjVec3 observerPos;
   this->engine().getPosition( observerPos[0], observerPos[1], observerPos[2] );

   // what is the sound's position
   vjVec3 soundPos;
   this->getPosition( soundPos[0], soundPos[1], soundPos[2] );

   // how far away is the sound from the observer.
   vjVec3 displacement = observerPos - soundPos;
   float length = displacement.length();

   // set the sound's volume
   this->getVolume( volume );

   // calculate the percieved volume
   if (length > mFalloff)
   {
      volume = 0;
   }

   else //if (length <= mFalloff)
   {
      // position=0 is full volume
      // posiiton=falloff is no volume

      float amount = 1.0f - (length / mFalloff);
      volume *= amount;
   }
}   
