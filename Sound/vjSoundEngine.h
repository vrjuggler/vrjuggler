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

#ifndef _VJ_SOUNDENGINE 
#define _VJ_SOUNDENGINE

#include <string>
#include <Math/vjMatrix.h>

class vjSound;
class vjConfigChunk;

//: SoundEngine
// Normally, use the vjSoundFactory factory to create a new sound engine.
// You shouldn't create one directly (but you could if you wanted).
//!PUBLIC_API:
class vjSoundEngine
{
public:
   vjSoundEngine();
   virtual void init() {}
   
   // your application must call this once per main loop.
   virtual void update()
   {
      //cout<<"[SoundEngine] Update\n"<<flush;
   }

	virtual void sync() 
	{
      //cout<<"[SoundEngine] Sync\n"<<flush;
	}
   
   // lookup a filename, given the sound's alias.
   // the "filename" returned can be used in a call to Sound::load()
   virtual void aliasToFileName( const char* const alias, std::string& filename );

   // set position of the observer (user)
   // TODO: multiple users?
   virtual void setPosition( const vjMatrix& position );
   void getPosition( vjMatrix& position ) const;
   
   // set position of the observer (user)
   // TODO: multiple users?
   virtual void setPosition( const float& x, const float& y, const float& z );
   void getPosition( float& x, float& y, float& z ) const;
   
   // given an alias, return the handle.
   // TODO: if called twice with name alias, should return same pointer.
   // memory managed by engine...
   // TODO: keep a list of these handles so engine can clean up. :)
   //         memory leaks for now.
   // returns NULL if invalid name.
   virtual vjSound* getHandle( const char* const alias );
   
   //: Factory function to create a new sound.
   // memory managed by engine
   virtual vjSound* newSound();
   
   virtual void kill() {}
  
	virtual bool config( vjConfigChunk* chunk ) 
	{
		return false;
	}

   vjMatrix mPosition;
};

#endif
