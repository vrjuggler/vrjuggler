#ifndef SOUND_h
#define SOUND_h

#include <vjConfig.h>

#include <iostream.h>

#include <Math/vjVec3.h>

class SoundEngine;
class Sound
{
public:
   Sound( SoundEngine& engine ) : mPosition( 0.0f, 0.0f, 0.0f ), mVolume( 1.0f ), 
      mEngine(&engine), mFalloff( 20.0f ), mLooping( 0 ) 
   {
   }
   
   virtual ~Sound()
   {
   }
   
   // loads the sound from a file
   // reutrns false if didn't load.
   virtual bool load( const char* const soundname )
   {
      // be stupid and return true,
      // this allows use of the Sound class to stub out sound.
      // so... of course the sound loaded - this is a stub,
      // it's always a success!
      return true;
   }
   
   void setEngine( SoundEngine& se )
   {
      mEngine = &se;
   }
   
   SoundEngine& engine() { return *mEngine; }
   const SoundEngine& engine() const { return *mEngine; }
   
   virtual void pitchBend( float amount = 1.0 )
   {
   }
   
   // set to 0 to never loop
   // set to -1 to always loop
   // set to x to loop x number of times.
   virtual void setLooping( int times )
   {
      mLooping = times;
   } 
   
   enum BinaryState
   {
      ON, OFF
   };   
   
   // allows a user to enable or disable a sound without worring about its state set by start or stop.
   // NOTE: to hear a sound enable() and play() must be active.
   //       default is ON
   virtual void enable( BinaryState state = ON ) {}
    
   virtual void trigger() {}
   
   // stop the sound.
   // useful when a sound has been set to LOOP, this will stop it
   virtual void stop() {}
   
   // change the position of the sound.
   virtual void setPosition( float x, float y, float z ) 
   {
      mPosition.set( x, y, z );
   }
   
   void getPosition( float& x, float& y, float& z ) const
   {
      x = mPosition[0];
      y = mPosition[1];
      z = mPosition[2];
   }
   
   // output to stdout, useful for debugging.
   virtual void print() {}
   
   int mLooping;
   
   // calculate the sound volume based on the observer position, sound position, falloff, and volume
   void getPercievedVolume( float& volume ) const;
   
   // set the volume when observer position is same as sound position.
   // vol is between 0.0 and 1.0, where 1.0 is full volume.
   virtual void setVolume( const float& vol )
   {
      mVolume = vol;
   }   
   
   void getVolume( float& vol ) const
   {
      vol = mVolume;
   }
   
   // how fast the volume drops as you move away.
   // set fall == distace at which volume becomes 0
   virtual void setFalloff( const float& vol )
   {
      mVolume = vol;
   }   
   
   void getFalloff( float& vol ) const
   {
      vol = mVolume;
   }   

protected:
   // my associated engine...
   SoundEngine* mEngine;
   
   vjVec3 mPosition; // position of this sound in distance units where 1 is one unit.
   float mVolume;  // from 0.0 to 1.0
   float mFalloff; // in distance units where 1 is one unit.
};

#endif
