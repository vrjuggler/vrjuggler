#include <vjConfig.h>

#include <Sound/SoundEngine.h> // my sound engine to create
#include <Sound/SoundFactory.h>
#include <Sound/SoundEngineCreator.h> // abstract mini factory


// mini factory
class StubSoundEngineCreator : public SoundEngineCreator
{
public:
   StubSoundEngineCreator()
   {
      this->addDescriptor( "nosound" );
      this->addDescriptor( "stub" );
      this->addDescriptor( "NOSOUND" );
      this->addDescriptor( "STUB" );
      cout<<"[soundengine] Regging Stub Creator\n"<<flush;
      SoundFactory::registerCreator( &mSoundEngineCreator );
   }
   virtual SoundEngine* newEngine()
   {
      return new SoundEngine;
   }
   virtual ~StubSoundEngineCreator()
   {
      cout<<"[soundengine] Stub Creator Died (application probably exited, or library was unloaded)\n"<<flush;
   }
   
   static StubSoundEngineCreator mSoundEngineCreator;
};

StubSoundEngineCreator StubSoundEngineCreator::mSoundEngineCreator;
