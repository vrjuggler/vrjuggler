#include <assert.h>
#include <stdio.h>
#include <string>
#include "sl.h"                     // the SL library
#include <Sound/vjSoundEngine.h>    // engine baseclass
#include <Sound/sl/SlSoundEngine.h> // my engine
#include <Sound/vjSound.h>          // bass class...

#include <Sound/sl/SlSound.h>       // my header

//specify the sound's name in the adf file...
SlSound::SlSound( vjSoundEngine& engine ) : vjSound( engine )
{

}

// loads the sound
bool SlSound::load( const char* const filename )
{   
   mSoundName = filename;
   
   cout<<"[SL] "<<mSoundName.c_str()<<"\n"<<flush;
   SlSoundEngine& engine = dynamic_cast<SlSoundEngine&> (*mEngine);
   mSample = new slSample( const_cast<char*>(mSoundName.c_str()), &engine.mScheduler );

   if (mSample == NULL)
   {
      char buffer[256];
      sprintf( buffer, "[SL] Unable to locate sound %s\n", mSoundName.data() );
      return false;
   }

   return true;
}

SlSound::~SlSound() {}

void SlSound::pitchBend( float amount )
{
   assert( mSample != NULL );
   cout<<"[SL] Not implemented: pitchBend\n"<<flush;
}   

// allows a user to enable or disable a sound without worring about its state set by start or stop.
// NOTE: to hear a sound enable() and play() must be active.
//       default is ON
void SlSound::enable( vjSound::BinaryState state )
{
   assert( mSample != NULL );
   cout<<"[SL] Not implemented: enable\n"<<flush;
}

void SlSound::trigger()
{
   assert( mSample != NULL );
   SlSoundEngine& engine = dynamic_cast<SlSoundEngine&> (*mEngine);
   
   // set the flag... TODO, this is inelegant.  maybe tell the engine to play based on some handle.
   //engine.playFlag = true;
   
   switch (mLooping)
   {   
      float volume;
      this->getPercievedVolume( volume );
      
      // this affects the actual sound *data*! (yeah that's bad)
      //mSample->adjustVolume( volume );
      
      case 0:
         cout<<"[SL] Playing audio "<<mSoundName.data()<<"\n"<<flush;
         engine.mScheduler.playSample( mSample );
         break;
         
      default:
      case -1:
         cout<<"[SL] Looping audio "<<mSoundName.data()<<"\n"<<flush;
         engine.mScheduler.loopSample( mSample );
         break;
   }
}  

// stop the sound.
// useful when a sound has been set to LOOP, this will stop it
void SlSound::stop()
{
   assert( mSample != NULL );
   SlSoundEngine& engine = dynamic_cast<SlSoundEngine&> (*mEngine);
   engine.mScheduler.stopSample( mSample );
}

// change the position of the sound.
void SlSound::setPosition( float x, float y, float z )
{
   vjSound::setPosition( x, y, z ); //base functionality
   assert( mSample != NULL );
   cout<<"[SL] Not implemented: setposition\n"<<flush;
}

// output to stdout, useful for debugging.
void SlSound::print()
{
   cout<<"[SL] Stevebaker's plib SL sound engine: \n"
       <<"[SL] Sound name: "<<mSoundName.data()<<"\n\n"<<flush;
}
