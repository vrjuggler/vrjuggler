#ifndef AWSOUND_H
#define AWSOUND_H

#include <vjConfig.h>

#include <assert.h>
#include <stdio.h>
#include <string>
#include <aw.h> //audio works
#include <Sound/Sound.h>

// you need an Observer node in your .adf file named you
// usually set it's position to 0,0,0
// every sound is positoined reletive to this observer
// this observer is updated by the soundengine's setPosition function.
//  it may be controlled by head tracking and navigation (or not at all)
//
// Positional sounds need to be specified using awPlayer nodes in the .adf file
// awSound node's position can only change with observer position change,
//  so i'd keep it flexible by using awPlayer nodes for each sound you want.
// Ambient sounds can be defined as plain awSound nodes.
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
   awPlayer* mSoundPlayer; // will be either this one...
   awSound* mSound; // or this one... not both.
};

#endif
