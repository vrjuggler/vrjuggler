
/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

/****************** <AJ heading BEGIN do not edit this line> *****************
 *
 * Audio Juggler
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <AJ heading END do not edit this line> ******************/


#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>

#include <stdio.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/alut.h>

const int buf_len = 10 * (512 * 3) * 1024;
unsigned short buf_data[buf_len];
ALuint bufferID = 0;
ALuint sourceID = 0;
void*       mContextId = NULL;
ALCdevice*  mDev = NULL;

void startup()
{
   mDev = alcOpenDevice( NULL );
   if (mDev == NULL) {
      assert( false && "ERROR: Could not open device" );
      return;
   }

   // create context
   int attrlist[] = { ALC_FREQUENCY, 22050, ALC_INVALID };
   mContextId = alcCreateContext( mDev, attrlist );
   if (mContextId == NULL) {
      assert( false && "ERROR: Could not open context" );
      return;
   }

   // make context active...
   alcMakeContextCurrent( mContextId );
}

void shutdown()
{
   if (mContextId != NULL)
   {
      alcDestroyContext( mContextId );
   }

   if (mDev != NULL)
   {
      alcCloseDevice(  mDev  );
   }

   mContextId = NULL;
   mDev = NULL;
}

void trigger()
{
   alSourcePlay( sourceID );
}

void load()
{
   FILE *fh = fopen("sample.wav", "rb");
   fread( buf_data, 1, buf_len, fh );
   fclose( fh );
   
   alGenBuffers( 1, &bufferID ); //AL_FORMAT_WAVE_EXT
   alBufferData( bufferID, AL_FORMAT_WAVE_EXT, buf_data, buf_len, 0 );
   alGenSources( 1, &sourceID );
   alSourcei( sourceID, AL_BUFFER, bufferID );
   alSourcei( sourceID, AL_LOOPING, AL_FALSE );
}

void unload()
{
   alDeleteSources( 1, &sourceID );
   alDeleteBuffers( 1, &bufferID );
}

void main()
{
   printf( "1st startup (no triggered sound)\n" );
   startup();
   load();
   sleep( 2 );
   unload();
   shutdown();
   
   
   printf( "2st startup (trigger... should hear sample.wav playing...)\n" );
   startup();
   load();
   trigger();
   sleep( 2 );
   unload();
   shutdown();
   
   
   printf( "3nd startup (bad... trigger causes a click and no sound)\n" );
   startup();
   load();
   trigger();
   sleep( 10 );
   unload();
   shutdown();
}
