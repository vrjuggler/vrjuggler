#include "AwSoundEngine.h" // my sound engine to create
#include "SoundFactory.h"
#include "SoundEngineCreator.h" // abstract mini factory definition


// mini factory
class AwSoundEngineCreator : public SoundEngineCreator
{
public:
   AwSoundEngineCreator()
   {
      this->addDescriptor( "aw" );
      this->addDescriptor( "AW" );
      SoundFactory::registerCreator( &mSoundEngineCreator );
   }
   virtual SoundEngine* newEngine()
   {
      cout<<"Regging Audioworks Creator\n"<<flush;
      return new AwSoundEngine;
   }
   virtual ~AwSoundEngineCreator()
   {
      cout<<"Audioworks Creator Died (application probably exited, or library was unloaded)\n"<<flush;
   }
   
   static AwSoundEngineCreator mSoundEngineCreator;
};

AwSoundEngineCreator AwSoundEngineCreator::mSoundEngineCreator;
