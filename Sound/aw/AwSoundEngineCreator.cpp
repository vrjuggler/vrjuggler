#include <vjConfig.h>

#include <Sound/aw/AwSoundEngine.h> // my sound engine to create
#include <Sound/SoundFactory.h>
#include <Sound/SoundEngineCreator.h> // abstract mini factory definition


// mini factory
class AwSoundEngineCreator : public SoundEngineCreator
{
public:
   AwSoundEngineCreator()
   {
      this->addDescriptor( "aw" );
      this->addDescriptor( "AW" );
      cout<<"[soundengine] Regging Audioworks Creator\n"<<flush;
      SoundFactory::registerCreator( &mSoundEngineCreator );
   }
   virtual SoundEngine* newEngine()
   {
      return new AwSoundEngine;
   }
   virtual ~AwSoundEngineCreator()
   {
      cout<<"[soundengine] Audioworks Creator Died (application probably exited, or library was unloaded)\n"<<flush;
   }
   
   static AwSoundEngineCreator mSoundEngineCreator;
};

AwSoundEngineCreator AwSoundEngineCreator::mSoundEngineCreator;
