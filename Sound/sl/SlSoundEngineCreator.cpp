#include <vjConfig.h>

#include <Sound/sl/SlSoundEngine.h> // my sound engine to create
#include <Sound/SoundFactory.h>
#include <Sound/SoundEngineCreator.h> // abstract mini factory definition


// mini factory
class SlSoundEngineCreator : public SoundEngineCreator
{
public:
   SlSoundEngineCreator()
   {
      this->addDescriptor( "sl" );
      this->addDescriptor( "SL" );
      cout<<"[soundengine] Regging SL Creator\n"<<flush;
      SoundFactory::registerCreator( &mSoundEngineCreator );
   }
   virtual SoundEngine* newEngine()
   {
      return new SlSoundEngine;
   }
   virtual ~SlSoundEngineCreator()
   {
      cout<<"[soundengine] SL Creator Died (application probably exited, or library was unloaded)\n"<<flush;
   }
   
   static SlSoundEngineCreator mSoundEngineCreator;
};

SlSoundEngineCreator SlSoundEngineCreator::mSoundEngineCreator;
