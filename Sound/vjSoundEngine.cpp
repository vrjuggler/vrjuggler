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

#include <string>
#include <Sound/vjSound.h>
#include <Sound/vjSoundEngine.h>

vjSoundEngine::vjSoundEngine() : mPosition()
{
   mPosition.setTrans( 0.0f, 0.0f, 0.0f );
}

// lookup a filename, given the sound's alias.
// the "filename" returned can be used in a call to Sound::load()
void vjSoundEngine::aliasToFileName( const char* const alias, std::string& filename )
{
   filename = alias;
}


// given an alias, return the handle.
// TODO: if called twice with name alias, should return same pointer.
// memory managed by engine...
// return NULL on load failure.
vjSound* vjSoundEngine::getHandle( const char* const alias )
{
   std::string filename;
   this->aliasToFileName( alias, filename );

   if (filename == "")
   {
      return NULL;
   }   
   
   vjSound* snd = this->newSound();
   bool didLoad = snd->load( filename.c_str() );
   
   if (didLoad)
   {
      return snd;
   }
   else
   {
      // return NULL on load failure.
      delete snd;
      return NULL;
   }   
}


//: Factory function to create a new sound.
// memory managed by engine
vjSound* vjSoundEngine::newSound()
{
   std::cout<<"new Sound()\n"<<std::flush;
   return new vjSound( *this );
}


// set position
void vjSoundEngine::setPosition( const vjMatrix& position )
{
   mPosition = position;
}

// set position
void vjSoundEngine::setPosition( const float& x, const float& y, const float& z )
{
   mPosition.makeTrans( x, y, z );
}

void vjSoundEngine::getPosition( vjMatrix& position ) const
{
   position = mPosition;
}

void vjSoundEngine::getPosition( float& x, float& y, float& z ) const
{
   vjVec3 point( 0.0f, 0.0f, 0.0f );
   point.xformFull( mPosition, point );
   x = point[0];
   y = point[1];
   z = point[2];
}
