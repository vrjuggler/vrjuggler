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

#ifndef VJ_SOUND_FACTORY
#define VJ_SOUND_FACTORY

#include <Sound/vjSoundEngine.h>
#include <Sound/vjSound.h>
#include <Kernel/vjDebug.h>
#include <vector>
#include <typeinfo>

//: soundengines use the SEC to register themselves with the sound factory
class vjSoundEngineConstructorBase;


//: use the vjSoundFactory factory to create a new sound engine.
// do not create an engine directly.
//!PUBLIC_API:
class vjSoundFactory
{
public:
   // pass in a description of the sound engine (like sl, aw, nosound)
   // pass in a config file name to go with that sound engine.
   // returns a non-NULL pointer on success, NULL on failure.
   //static SoundEngine* newEngine( const std::string& description, const char* const setupFile = NULL );
   
	//: Query if the factory knows about the given sound engine
   //!PRE: chunk != NULL, chunk is a valid chunk
   //!ARGS: chunk - chunk we are requesting about knowledge to create
   //!RETURNS: true - factory knows how to create the engine
   //+          false - factory does not know how to create the engine
   bool recognizeEngine(vjConfigChunk* chunk);

   //: Load the specified device
   //!PRE: recognizeDevice(chunk) == true
   //!ARGS: chunk - specification of the device to load
   //!RETURNS: null - Device failed to load
   //+         other - Pointer to the loaded device
   vjSoundEngine* loadEngine(vjConfigChunk* chunk);

   
public:
   // register a creator to the factory.
   // this is intended to be used by each api's registration method
   void registerConstructor( vjSoundEngineConstructorBase* constructor );
   
   // TODO: add a removeCreator function.
protected:
   /*
	// returns a creator mased on some description
   // pointer to the found creator is returned
   // success value is also returned, true if one is found.
   bool lookupCreator( const std::string& description, SoundEngineCreator* &sec );
	*/

	//: Find a constructor for the given device type
   //!RETURNS: -1 - Not found
   //+            - Index of the constructorck
   int   findConstructor(vjConfigChunk* chunk);

protected:   
   // regged soundengines...
   std::vector< vjSoundEngineConstructorBase* > mConstructors;

public:
   //: Get instance of singleton object
   static vjSoundFactory* instance()
   {
      if (_instance == NULL)
         _instance = new vjSoundFactory;
      return _instance;
   }

protected:
   //: Constructor.
   vjSoundFactory();

private:
   static vjSoundFactory* _instance;   //: The instance
};

#endif
