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

#ifndef SLSOUND_H
#define SLSOUND_H

#include <assert.h>
#include <stdio.h>
#include <string>
#include "sl.h"
#include <Sound/sl/SlSoundEngine.h>

#include <Sound/vjSound.h> // base class...

class SlSound : public vjSound
{
public:
   SlSound( vjSoundEngine& engine );
   virtual ~SlSound();
   
   // load the sound from filename...
   virtual bool load( const char* const filename );
   
   virtual void pitchBend( float amount = 1.0 );
   
   // allows a user to enable or disable a sound without worring about its state set by start or stop.
   // NOTE: to hear a sound enable() and play() must be active.
   //       default is ON
   virtual void enable( vjSound::BinaryState state = vjSound::ON );
    
   virtual void trigger();
   
   // stop the sound.
   // useful when a sound has been set to LOOP, this will stop it
   virtual void stop();
   
   // change the position of the sound.
   virtual void setPosition( float x, float y, float z );
   
   // output to stdout, useful for debugging.
   virtual void print();
   
   std::string mSoundName;
   slSample* mSample;
};

#endif
