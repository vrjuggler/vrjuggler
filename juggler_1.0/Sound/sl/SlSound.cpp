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

#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <string>
#include "sl.h"                     // the SL library
#include <Sound/vjSoundEngine.h>    // engine baseclass
#include <Sound/sl/SlSoundEngine.h> // my engine
#include <Sound/vjSound.h>          // bass class...

#include <Sound/sl/SlSound.h>       // my header

//specify the sound's name in the adf file...
SlSound::SlSound( vjSoundEngine& engine ) : vjSound( engine )
{

}

// loads the sound
bool SlSound::load( const char* const filename )
{   
   mSoundName = filename;
   
   std::cout<<"[SL] "<<mSoundName.c_str()<<"\n"<<std::flush;
   SlSoundEngine& engine = dynamic_cast<SlSoundEngine&> (*mEngine);
   mSample = new slSample( const_cast<char*>(mSoundName.c_str()), &engine.mScheduler );

   if (mSample == NULL)
   {
      char buffer[256];
      sprintf( buffer, "[SL] Unable to locate sound %s\n", mSoundName.data() );
      return false;
   }

   return true;
}

SlSound::~SlSound() {}

void SlSound::pitchBend( float amount )
{
   assert( mSample != NULL );
   std::cout<<"[SL] Not implemented: pitchBend\n"<<std::flush;
}   

// allows a user to enable or disable a sound without worring about its state set by start or stop.
// NOTE: to hear a sound enable() and play() must be active.
//       default is true
void SlSound::enable( const bool& state )
{
   vjSound::enabled( state );
   assert( mSample != NULL );
   std::cout<<"[SL] Not implemented: enable\n"<<std::flush;
}

void SlSound::trigger()
{
   assert( mSample != NULL );
   SlSoundEngine& engine = dynamic_cast<SlSoundEngine&> (*mEngine);
   
   if (this->isEnabled())
   {
      switch (mLooping)
      {   
         // this affects the actual sound *data*! (yeah that's bad)
         //float volume;
         //this->getPercievedVolume( volume );
         //mSample->adjustVolume( volume );

         case 0:
            std::cout<<"[SL] Playing audio "<<mSoundName.data()<<"\n"
                     <<std::flush;
            engine.mScheduler.playSample( mSample );
            break;

         default:
         case -1:
            std::cout<<"[SL] Looping audio "<<mSoundName.data()<<"\n"
                     <<std::flush;
            engine.mScheduler.loopSample( mSample );
            break;
      }
   }
}  

// stop the sound.
// useful when a sound has been set to LOOP, this will stop it
void SlSound::stop()
{
   assert( mSample != NULL );
   SlSoundEngine& engine = dynamic_cast<SlSoundEngine&> (*mEngine);
   engine.mScheduler.stopSample( mSample );
}

// change the position of the sound.
void SlSound::setPosition( float x, float y, float z )
{
   vjSound::setPosition( x, y, z ); //base functionality
   assert( mSample != NULL );
   std::cout<<"[SL] Not implemented: setposition\n"<<std::flush;
}

// output to stdout, useful for debugging.
void SlSound::print()
{
   std::cout<<"[SL] Stevebaker's plib SL sound engine: \n"
       <<"[SL] Sound name: "<<mSoundName.data()<<"\n\n"<<std::flush;
}
