#include <vjConfig.h>

#include <vector>
#include <Sound/SoundEngine.h>
#include <Sound/Sound.h>
#include <Sound/SoundEngineCreator.h>

// use the SoundFactory factory to create a new sound engine.
// do not create one directly.
class SoundFactory
{
public:
   // pass in a description of the sound engine (like sl, aw, nosound)
   // pass in a config file name to go with that sound engine.
   // returns a non-NULL pointer on success, NULL on failure.
   static SoundEngine* newEngine( const std::string& description, const char* const setupFile = NULL );
   
   // returns a creator mased on some description
   // pointer to the found creator is returned
   // success value is also returned, true if one is found.
   static bool lookupCreator( const std::string& description, SoundEngineCreator* &sec );
   
   // register a creator to the factory.
   // this is intended to be used by each api's registration method
   static void registerCreator( SoundEngineCreator* sec );
   
   // TODO: add a removeCreator function.
   
   // regged soundengines...
   static std::vector< SoundEngineCreator* > mEngineCreators;
};
