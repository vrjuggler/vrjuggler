#ifndef SLSOUND_H
#define SLSOUND_H

#include <assert.h>
#include <stdio.h>
#include <string>
#include "sl.h"
#include "SlSoundEngine.h"

#include "Sound.h" // base class...

class SlSound : public Sound
{
public:
   SlSound( SoundEngine& engine );
   virtual ~SlSound();
   
   // load the sound from filename...
   virtual bool load( const char* const filename );
   
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
   slSample* mSample;
};

#endif
