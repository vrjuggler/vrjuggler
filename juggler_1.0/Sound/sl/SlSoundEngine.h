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

#ifndef SLSOUNDENGINE
#define SLSOUNDENGINE

#include <string>
#include <assert.h>
#include <unistd.h>
#include <sl.h>
#include <Sound/vjSoundEngine.h> // base class..
#include <Sound/vjSoundEngineConstructor.h>


#include <VPR/Threads/vjThread.h> // for when update is called.
#include <VPR/Threads/vjThreadFunctor.h> // for when update is called.

//: See AwSoundEngine for details on using the SL juggler module.
//  you'll link to -lJuggler_sl to use this.
//!PUBLIC_API:
class SlSoundEngine : public vjSoundEngine
{
public:
   SlSoundEngine();
   virtual ~SlSoundEngine();

   // pass the config filename here...
   virtual void init();
   
   // lookup a filename, given the sound's alias.
   // the "filename" returned can be used in a call to Sound::load()
   virtual void aliasToFileName( const char* const alias, std::string& filename );

   // set position
   virtual void setPosition( const vjMatrix& position );
   
   // set position
   virtual void setPosition( const float& x, const float& y, const float& z );
   
   //: Factory function to create a new sound.
   // memory managed by engine
   virtual vjSound* newSound();
   
   // call this from your app once per main loop.
   virtual void update();
   
   virtual void kill();
   
   std::map<std::string, std::string> mAliasDatabase;
   
	virtual bool config( vjConfigChunk* blowChunks );

	
	static std::string getChunkType() { return std::string( "SlSoundEngine" ); }


   // SL's sound engine..
   slScheduler mScheduler;
  
   // the control loop for update...
   void updateLoop( void* nullParam );
   
   vjThread* mUpdateThread;
};


vjSoundEngineConstructor<SlSoundEngine> sl_constructor; 
#endif
