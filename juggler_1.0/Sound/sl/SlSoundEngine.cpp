#include <assert.h>
#include <unistd.h>
#include <iostream.h>
#include "sl.h"
#include "SoundEngine.h" // base class..
#include "SlSound.h"     // my sound handle type...

#include "SlSoundEngine.h" //my header


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
void SlSoundEngine::init( const char* const aliasDatabase )
{
   mAliasDatabase.loadDatabase( aliasDatabase );
   //sched.setSafetyMargin( 0.128f );
}

// lookup a filename, given the sound's alias.
// the "filename" returned can be used in a call to Sound::load()
void SlSoundEngine::aliasToFileName( const char* const alias, std::string& filename )
{
   filename = mAliasDatabase.lookup( alias );
}   

//: Factory function to create a new sound.
// memory managed by engine
Sound* SlSoundEngine::newSound()
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
   SoundEngine::setPosition( position );
   
   //TODO: compute the SL sound volume
}

// set position
void SlSoundEngine::setPosition( const float& x, const float& y, const float& z )
{
   SoundEngine::setPosition( x, y, z );
   
   float xyz[3] = { x, y, z };
   float hpr[3] = { 0.0f, 0.0f, 0.0f };

   //TODO: compute the SL sound volume
}
