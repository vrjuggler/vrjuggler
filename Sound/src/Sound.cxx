#include <SoundEngine.h>
#include "Sound.h"

// calculate the sound volume based on the observer position, sound position, falloff, and volume
void Sound::getPercievedVolume( float& volume ) const
{
   // get the observer's position
   vjVec3 observerPos;
   this->engine().getPosition( observerPos[0], observerPos[1], observerPos[2] );

   // what is the sound's position
   vjVec3 soundPos;
   this->getPosition( soundPos[0], soundPos[1], soundPos[2] );

   // how far away is the sound from the observer.
   vjVec3 displacement = observerPos - soundPos;
   float length = displacement.length();

   // set the sound's volume
   this->getVolume( volume );

   // calculate the percieved volume
   if (length > mFalloff)
   {
      volume = 0;
   }

   else //if (length <= mFalloff)
   {
      // position=0 is full volume
      // posiiton=falloff is no volume

      float amount = 1.0f - (length / mFalloff);
      volume *= amount;
   }
}   
