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

#include <vjConfig.h>
#include <Sound/vjSoundManager.h>
#include <Sound/vjSoundFactory.h>

vjSoundManager* vjSoundManager::_instance = NULL;   //: The instance ptr

//: Add the chunk to the configuration
//! PRE: configCanHandle(chunk) == true
bool vjSoundManager::configAdd(vjConfigChunk* chunk)
{
	if (!configCanHandle( chunk ))
	{
		return false;
	}
	
	if (mSoundEngine != NULL)
	{
		//TODO: add to it
		vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) << "WARNING: Trying to configure sound manager when we already have one configured.\n" << vjDEBUG_FLUSH;
		return false;
	}

	// Allocate new engine, configure it, init it
	mSoundEngine = vjSoundFactory::loadEngine(chunk);
	if(mSoundEngine != NULL)
	{
		mSoundEngine->init();			// Initialize the new puppy *woof*
      return true;
	}
	else
	{
		vjDEBUG(vjDBG_ALL,vjDBG_CRITICAL_LVL) 
					<< clrOutNORM(clrRED,"ERROR:")
					<< "Failed to load sound engine\n" << vjDEBUG_FLUSH;
      return false;
	}
}

//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
bool vjSoundManager::configRemove(vjConfigChunk* chunk)
{
	return false;
}

//: Can the handler handle the given chunk?
//! RETURNS: true - Can handle it
//+          false - Can't handle it
bool vjSoundManager::configCanHandle(vjConfigChunk* chunk)
{
	return vjSoundFactory::recognizeEngine( chunk );
}

//: Enable a frame to be drawn
void vjSoundManager::update()
{
	if(mSoundEngine != NULL)
		mSoundEngine->update();
}

//: Blocks until the end of the frame
//! POST: The frame has been drawn
void vjSoundManager::sync()
{
	if(mSoundEngine != NULL)
		mSoundEngine->sync();
}

// given an alias, return the handle.
// TODO: if called twice with name alias, should return same pointer.
// memory managed by engine...
// returns NULL if invalid name.
vjSound* vjSoundManager::getHandle( const char* const alias )
{
	if(mSoundEngine != NULL)
		return mSoundEngine->getHandle( alias );
   else
      return NULL;
}

//: Factory function to create a new sound.
// memory managed by engine
vjSound* vjSoundManager::newSound()
{
	if(mSoundEngine != NULL)
		return mSoundEngine->newSound();
   else
      return NULL;
}

