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

#ifndef _AW_SOUND_ENGINE_H_
#define _AW_SOUND_ENGINE_H_

#include <assert.h>
#include <unistd.h>
#include <iostream>
#include <aw.h> //audio works
#include <Sound/vjSoundEngine.h>
#include <Sound/vjSoundEngineConstructor.h>
class vjConfigChunk;

//: AwSoundEngine
// link to -lJuggler_aw to use this code (don't instantiate one yourself)
// calling vjSoundManager::instance()->getHandle("soundname") will get the sound of the correct API for you
// only if you've linked to the -lJuggler_aw module.
//
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
//!PUBLIC_API:
class AwSoundEngine : public vjSoundEngine
{
public:
   AwSoundEngine();
   virtual ~AwSoundEngine();

   // pass the config filename here...
   virtual void init();
   
   // lookup a filename, given the sound's alias.
   // the "filename" returned can be used in a call to Sound::load()
   virtual void aliasToFileName( const char* const alias, std::string& filename );

   // set position
   virtual void setPosition( const vjMatrix& position );
   
   // set position
   virtual void setPosition( const float& x, const float& y, const float& z );
   
   // given an alias, return the handle.
   // TODO: if called twice with name alias, should return same pointer.
   // memory managed by engine...
   //vjSound* getHandle( const char* const alias );
   
   //: Factory function to create a new sound.
   // memory managed by engine
   virtual vjSound* newSound();
   
   // call this once per main loop.
   //
   virtual void update();

   virtual void kill();

	virtual bool config( vjConfigChunk* chunk );

	static std::string getChunkType() { return std::string( "AwSoundEngine" ); }
	
   awPlayer*   mPlayer;
   awObserver* mObserver;
	std::string mAdfFileList;  // i.e: "file1.adf file2.adf file3.adf"
   bool        mInitialized;  // is audioworks initialized yet??
};

vjSoundEngineConstructor<AwSoundEngine> aw_constructor; 


#endif	/* _AW_SOUND_ENGINE_H_ */
