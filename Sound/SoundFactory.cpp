#include <vjConfig.h>

#include <Sound/SoundFactory.h>

// regged soundengines...
std::vector< SoundEngineCreator* > SoundFactory::mEngineCreators;

// pass in a description of the sound engine (like sl, aw, nosound)
// pass in a config file name to go with that sound engine.
// returns a non-NULL pointer on success, NULL on failure.
SoundEngine* SoundFactory::newEngine( const std::string& description, const char* const setupFile )
{
   assert( setupFile != NULL );

   SoundEngineCreator* soundEngineCreator;
   bool result = lookupCreator( description, soundEngineCreator );    
   // FIXME, retrn some default stubby soundengine... instead of assert
   assert( soundEngineCreator != NULL );

   // create an engine from the EngineCreator just returned
   SoundEngine* se = soundEngineCreator->newEngine();
   assert( se != NULL ); // FIXME, asserts stop the show.  bad.

   // init the sound engine.
   se->init( setupFile );

   // return the new soundEngine...ready to go.
   return se;
}   

// returns a creator mased on some description
// pointer to the found creator is returned
// success value is also returned, true if one is found.
bool SoundFactory::lookupCreator( const std::string& description, SoundEngineCreator* &sec )
{
   for (int x = 0; x < mEngineCreators.size(); ++x)
   {
      // if this creator matches the given description...
      if (mEngineCreators[x]->matchDescription( description ))
      {
         // ...then return the found creator.
         sec = mEngineCreators[x];
         return true;
      }
   }

   sec = NULL; // NULL to be safe.. (if there is such a thing.)
   return false;
}   

// register a creator to the factory.
// this is intended to be used by each api's registration method
void SoundFactory::registerCreator( SoundEngineCreator* sec )
{
   mEngineCreators.push_back( sec );
}   
