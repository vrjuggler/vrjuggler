#include "SoundEngine.h" // my sound engine to create
#include "SoundFactory.h"
#include "SoundEngineCreator.h" // abstract mini factory


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
      SoundFactory::registerCreator( &mSoundEngineCreator );
   }
   virtual SoundEngine* newEngine()
   {
      cout<<"Regging Stub Creator\n"<<flush;
      return new SoundEngine;
   }
   virtual ~StubSoundEngineCreator()
   {
      cout<<"Stub Creator Died (application probably exited, or library was unloaded)\n"<<flush;
   }
   
   static StubSoundEngineCreator mSoundEngineCreator;
};

StubSoundEngineCreator StubSoundEngineCreator::mSoundEngineCreator;
