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

#ifndef AWSOUND_H
#define AWSOUND_H

#include <assert.h>
#include <stdio.h>
#include <string>
#include <aw.h> //audio works
#include <Sound/vjSound.h>

// you need an Observer node in your .adf file named you
// usually set it's position to 0,0,0
// every sound is positoined reletive to this observer
// this observer is updated by the soundengine's setPosition function.
//  it may be controlled by head tracking and navigation (or not at all)
//
// Positional sounds need to be specified using awPlayer nodes in the .adf file
// awSound node's position can only change with observer position change,
//  so i'd keep it flexible by using awPlayer nodes for each sound you want.
// Ambient sounds can be defined as plain awSound nodes.
class AwSound : public vjSound
{
public:
   //specify the sound's name in the adf file...
   AwSound( vjSoundEngine& engine );
   
   // loads the sound from an alias as specified in the audioworks .adf file
   virtual bool load( const char* const alias );
   
   virtual ~AwSound();
   
   virtual void pitchBend( float amount = 1.0 );
   
   // allows a user to enable (restart) or disable (pause) a sound 
   // without worring about its state set by start or stop.
   // NOTE: to hear a sound enable() and play() must be active.
   //       default is true
   virtual void enable( const bool& state = true );
    
   // trigger (play) the sound
   virtual void trigger();
   
   // stop the sound.
   // stops the sound before it finishes playing
   virtual void stop();
   
   // change the position of the sound.
   virtual void setPosition( float x, float y, float z );
   
   // output to stdout, useful for debugging.
   virtual void print();
   
   std::string mSoundName;
   awPlayer* mSoundPlayer; // will be either this one...
   awSound* mSound; // or this one... not both.
};

#endif
