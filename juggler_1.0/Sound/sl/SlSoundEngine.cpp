#include <assert.h>
#include <unistd.h>
#include <iostream.h>
#include "sl.h"
#include <Sound/vjSoundEngine.h> // base class..
#include <Sound/sl/SlSound.h>     // my sound handle type...

#include <Sound/sl/SlSoundEngine.h> //my header


SlSoundEngine::SlSoundEngine() : mUpdateThread( NULL ), mScheduler( 8000 ) 
{
}

SlSoundEngine::~SlSoundEngine()
{
   if (mUpdateThread != NULL)
   {
      mUpdateThread->kill();
      delete mUpdateThread;
      mUpdateThread = NULL;
      cout<<"Killed the update thread\n"<<flush;
   }
}

// pass the config filename here...
void SlSoundEngine::init()
{
   //sched.setSafetyMargin( 0.128f );
}

// lookup a filename, given the sound's alias.
// the "filename" returned can be used in a call to Sound::load()
void SlSoundEngine::aliasToFileName( const char* const alias, std::string& filename )
{
	std::string alias_name( alias );
   std::map<std::string, std::string>::iterator i = mAliasDatabase.find( alias_name );

	if (i == mAliasDatabase.end())
	{
		//then not found.
		filename = "file_not_found_in_alias_database";
	} else {
		filename = (*i).second;
	}
}

//: Factory function to create a new sound.
// memory managed by engine
vjSound* SlSoundEngine::newSound()
{ 
   // return the sound handle, user must delete.
   return new SlSound( *this );
}


// call this once per main loop.
// TODO: should I do this update in a threaded *loop*
//    or should I spawn a thread that updates every time I call this update function (of course, only spawning a new thread if the old one has finished).
//    a constant loop is nice because it ensures that update is getting called as needed - but is SL thread safe?  It doesn't seem to cause problems for me this way - so far.
// FIXME: put a pause or sleep(0) at the end of the while(1) loop in the functor.
void SlSoundEngine::update()
{
   if (mUpdateThread == NULL)
   {
      // Create a new thread to handle the control
      vjThreadMemberFunctor<SlSoundEngine>* memberFunctor =
      new vjThreadMemberFunctor<SlSoundEngine>(this, &SlSoundEngine::updateLoop, NULL);

      // I also set mControlThread in SlSoundEngine::updateLoop
      // to avoid the race condition.  I set it here to avoid spawning more than one..
      mUpdateThread = new vjThread( memberFunctor, 0 );
   }
}

void SlSoundEngine::updateLoop( void* nullParam )
{
   while (0 == vjThread::self());
      mUpdateThread = (vjThread*) vjThread::self();

   while (mUpdateThread != NULL)
   {
      mScheduler.update();
      mUpdateThread->yield();
   }
}

void SlSoundEngine::kill()
{
   if (mUpdateThread != NULL)
   {
      mUpdateThread->kill();
      delete mUpdateThread;
      mUpdateThread = NULL;
      cout<<"Killed the update thread\n"<<flush;
   }
}

// set position
void SlSoundEngine::setPosition( const vjMatrix& position )
{
   vjSoundEngine::setPosition( position );
   
   //TODO: compute the SL sound volume
   // can't do this, because sl's setVolume member actually changes the entire sound data.
   //this would reduce the sound stream to noisyness pretty quickly
}

// set position
void SlSoundEngine::setPosition( const float& x, const float& y, const float& z )
{
   vjSoundEngine::setPosition( x, y, z );
   
   //float xyz[3] = { x, y, z };
   //float hpr[3] = { 0.0f, 0.0f, 0.0f };

   //TODO: compute the SL sound volume
   // can't do this, because sl's setVolume member actually changes the entire sound data.
   //this would reduce the sound stream to noisyness pretty quickly
}

bool SlSoundEngine::config( vjConfigChunk* chunk )
{
	vjASSERT( (std::string)chunk->getType() == SlSoundEngine::getChunkType() );

	int num_sounds = chunk->getNum( "Sounds" );
	for (int x = 0; x < num_sounds; ++x)
	{
		vjConfigChunk* sound_chunk = (vjConfigChunk*)chunk->getProperty("Sounds",x);
		vjASSERT(NULL != sound_chunk);
		std::string alias = sound_chunk->getProperty( "Alias" );
		std::string filename = sound_chunk->getProperty( "Filename" );
		mAliasDatabase[alias] = filename;
	}
   return true;
}

