#include "SoundEngine.h"
#include "Sound.h"

#include "AwSoundEngine.h"
#include "AwSound.h"

#include "SlSoundEngine.h"
#include "SlSound.h"


#include "SoundFactory.h" // my header

SoundEngine* SoundFactory::newEngine( EngineType type, const char* const dataFile )
{
   SoundEngine* se;
   switch (type)
   {
   case AW:
      se = new AwSoundEngine();
      break;
   case SL:
      se = new SlSoundEngine();
      break;
   default:
      se = new SoundEngine();
      break;
   }
   
   if (dataFile != NULL)
      se->init( dataFile );
   
   return se;
}   
/*
Sound* SoundFactory::newSound( EngineType type, const char* const soundAlias )
{
   Sound* snd;
   
   switch (type)
   {
   case AW:
      snd = new AwSound();
      break;
   case SL:
      snd = new SlSound();
      break;
   default:
      snd = new Sound();
      break;
   }
   
   if (soundAlias != NULL)
      snd->init( soundAlias );
   
   return snd;
}
*/
