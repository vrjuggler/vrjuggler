#ifndef AWSOUND_H
#define AWSOUND_H

#include <assert.h>
#include <stdio.h>
#include <string>
#include <aw.h> //audio works
#include "Sound.h"

class AwSound : public Sound
{
public:
   //specify the sound's name in the adf file...
   AwSound( SoundEngine& engine );
   
   // loads the sound from an alias as specified in the audioworks .adf file
   virtual bool load( const char* const alias );
   
   virtual ~AwSound();
   
   virtual void pitchBend( float amount = 1.0 );
   
   // allows a user to enable or disable a sound without worring about its state set by start or stop.
   // NOTE: to hear a sound enable() and play() must be active.
   //       default is ON
   virtual void enable( Sound::BinaryState state = Sound::ON );
    
   virtual void trigger();
   
   // stop the sound.
   // useful when a sound has been set to LOOP, this will stop it
   virtual void stop();
   
   // change the position of the sound.
   virtual void setPosition( float x, float y, float z );
   
   // output to stdout, useful for debugging.
   virtual void print();
   
   std::string mSoundName;
   awSound* mSound;
};

#endif
