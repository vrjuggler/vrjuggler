#include <assert.h>
#include <stdio.h>
#include <string>
#include <aw.h> //audio works
#include "Sound.h"

#include "AwSound.h" // my header

//specify the sound's name in the adf file...
AwSound::AwSound( SoundEngine& engine ) : Sound( engine )
{
}

// loads the sound from an alias as specified in the audioworks .adf file
bool AwSound::load( const char* const alias )
{
   // default functionality...
   Sound::load( alias );

   mSoundName = alias;

   mSound = awFindSnd( mSoundName.data() );
   if (mSound == NULL)
   {
      char buffer[256];
      sprintf( buffer, "Unable to locate sound %s\n", mSoundName.data() );
      awNotify( AW_FATAL, AW_APP, buffer );
      return false;
   }
  
   // allow user to trigger sound by default
   // user must call play and stop to control sound
   awProp( mSound, AWSND_RETRIGGER, AWSND_ON );
   return true;
}   

AwSound::~AwSound() {}

void AwSound::pitchBend( float amount )
{
   assert( mSound != NULL );
   // turn on the sound by default
   cout<<"awProp( mSound, AWSND_PBEND, "<<amount<<" )\n"<<flush;
   awProp( mSound, AWSND_PBEND, amount );
}   

// allows a user to enable or disable a sound without worring about its state set by start or stop.
// NOTE: to hear a sound enable() and play() must be active.
//       default is ON
void AwSound::enable( Sound::BinaryState state )
{
   assert( mSound != NULL );
   switch (state)
   {
      case Sound::ON:
         cout<<"awProp( mSound, AWSND_ENABLED, "<<AWSND_ON<<" )\n"<<flush;
         awProp( mSound, AWSND_ENABLED, AWSND_ON );
         break;
      case Sound::OFF:
         cout<<"awProp( mSound, AWSND_ENABLED, "<<AWSND_OFF<<" )\n"<<flush;
         awProp( mSound, AWSND_ENABLED, AWSND_OFF );
         break;
   }
}

void AwSound::trigger()
{
   assert( mSound != NULL );
   // make sure that the state is observed     
   // TODO: this is messed up....
   switch (mLooping)
   {
      case 0:
         cout<<"Playing audio "<<mSoundName.data()<<"\n"<<flush;
         awProp( mSound, AWSND_STATE, AWSND_ON );
         break;
         
      default:
      case -1:
         cout<<"Looping audio "<<mSoundName.data()<<"\n"<<flush;
         awProp( mSound, AWSND_NLOOPS, mLooping );
         //awProp( mSound, AWSND_RELEASELOOP, AW_ON );
         awProp( mSound, AWSND_STATE, AWSND_ON );
         //awProp( mSound, AWSND_NLOOPS, mLooping );
         //awProp( mSound, AWSND_RELEASELOOP, AW_ON );
         
         break;
   }
}

// stop the sound.
// useful when a sound has been set to LOOP, this will stop it
void AwSound::stop()
{
   assert( mSound != NULL );
    awProp( mSound, AWSND_STATE, AWSND_OFF );
}

// change the position of the sound.
void AwSound::setPosition( float x, float y, float z )
{
   assert( mSound != NULL );
   // set the base functionality
   Sound::setPosition( x, y, z );
   
   float xyz[3] = {x, y, z};
   float hpr[3] = {0.0f, 0.0f, 0.0f};

   // hmmm.. Might have to be a player...
   awXYZHPR( mSound, xyz, hpr);
}

// output to stdout, useful for debugging.
void AwSound::print()
{
   assert( mSound != NULL );
   awPrint( mSound );
}
