#include <vjConfig.h>
#include <Sound/vjSoundManager.h>
#include <Sound/vjSoundFactory.h>

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

