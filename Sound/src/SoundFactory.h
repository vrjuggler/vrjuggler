#include "SoundEngine.h"
#include "Sound.h"

class SoundFactory
{
public:
      
   enum EngineType
   {
      SL, AW, UNKNOWN, NOSOUND
   };
   
   static SoundEngine* newEngine( EngineType type, const char* const setupFile = NULL );
   //static Sound* newSound( EngineType type, const char* const soundAlias = NULL );
};
