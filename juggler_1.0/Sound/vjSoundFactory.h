#ifndef VJ_SOUND_FACTORY
#define VJ_SOUND_FACTORY

#include <Sound/vjSoundEngine.h>
#include <Sound/vjSound.h>
#include <Sound/vjSoundEngineConstructor.h>
#include <Kernel/vjDebug.h>
#include <vector>
#include <typeinfo>

// use the vjSoundFactory factory to create a new sound engine.
// do not create one directly.
class vjSoundFactory
{
public:
   // pass in a description of the sound engine (like sl, aw, nosound)
   // pass in a config file name to go with that sound engine.
   // returns a non-NULL pointer on success, NULL on failure.
   //static SoundEngine* newEngine( const std::string& description, const char* const setupFile = NULL );
   
	//: Query if the factory knows about the given sound engine
   //!PRE: chunk != NULL, chunk is a valid chunk
   //!ARGS: chunk - chunk we are requesting about knowledge to create
   //!RETURNS: true - factory knows how to create the engine
   //+          false - factory does not know how to create the engine
   static bool recognizeEngine(vjConfigChunk* chunk);

   //: Load the specified device
   //!PRE: recognizeDevice(chunk) == true
   //!ARGS: chunk - specification of the device to load
   //!RETURNS: null - Device failed to load
   //+         other - Pointer to the loaded device
   static vjSoundEngine* loadEngine(vjConfigChunk* chunk);

   
public:
   // register a creator to the factory.
   // this is intended to be used by each api's registration method
   static void registerConstructor( vjSoundEngineConstructorBase* constructor );
   
   // TODO: add a removeCreator function.
protected:
   /*
	// returns a creator mased on some description
   // pointer to the found creator is returned
   // success value is also returned, true if one is found.
   static bool lookupCreator( const std::string& description, SoundEngineCreator* &sec );
	*/

	//: Find a constructor for the given device type
   //!RETURNS: -1 - Not found
   //+            - Index of the constructorck
   static int   findConstructor(vjConfigChunk* chunk);

protected:   
   // regged soundengines...
   static std::vector< vjSoundEngineConstructorBase* > mConstructors;
};

#endif
