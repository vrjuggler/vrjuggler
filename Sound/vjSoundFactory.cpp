#include "vjSoundFactory.h"
/*
// regged soundengines...
std::vector< SoundEngineCreator* > vjSoundFactory::mEngineCreators;

// pass in a description of the sound engine (like sl, aw, nosound)
// pass in a config file name to go with that sound engine.
// returns a non-NULL pointer on success, NULL on failure.
SoundEngine* vjSoundFactory::newEngine( const std::string& description, const char* const setupFile )
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
bool vjSoundFactory::lookupCreator( const std::string& description, SoundEngineCreator* &sec )
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
void vjSoundFactory::registerConstructor( vjSoundEngineConstructorBase* constructor )
{
   mEngineCreators.push_back( constructor );
}   

*/
// register a creator to the factory.
// this is intended to be used by each api's registration method

void vjSoundFactory::registerConstructor( vjSoundEngineConstructorBase* constructor )
{
   vjASSERT(constructor != NULL);
   mConstructors.push_back(constructor);     // Add the constructor to the list
   vjDEBUG(vjDBG_INPUT_MGR,1) << "vjSoundFactory::registerEngine: Sound engine registered for: "
		    << constructor->getChunkType().c_str()
		    << "   :" << (void*)constructor
		    << " type:" << typeid(*constructor).name() << endl << vjDEBUG_FLUSH;
}

bool vjSoundFactory::recognizeEngine(vjConfigChunk* chunk)
{
	if(vjSoundFactory::findConstructor(chunk) == -1)
      return false;
   else
      return true;
}


vjSoundEngine* vjSoundFactory::loadEngine(vjConfigChunk* chunk)
{
	vjASSERT(recognizeEngine(chunk));

   int index = findConstructor(chunk);

   vjSoundEngine* new_engine;
   vjSoundEngineConstructorBase* constructor = mConstructors[index];

   vjDEBUG(vjDBG_INPUT_MGR,3) << "vjSoundFactory::loadEngine: Loading device: "
              << chunk->getType() << "  with: "
              << typeid(*constructor).name() << endl << vjDEBUG_FLUSH;

   new_engine = constructor->createEngine(chunk);
   return new_engine;
}

int vjSoundFactory::findConstructor(vjConfigChunk* chunk)
{
   std::string chunk_type;
   chunk_type = (std::string)chunk->getType();

   for(unsigned int i=0;i<mConstructors.size();i++)
   {
      // Get next constructor
      vjSoundEngineConstructorBase* construct = mConstructors[i];
      vjASSERT(construct != NULL);

      if(construct->getChunkType() == chunk_type)
         return i;
   }

   return -1;
}
