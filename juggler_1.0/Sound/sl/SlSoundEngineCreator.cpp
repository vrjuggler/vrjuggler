#include "SlSoundEngine.h" // my sound engine to create
#include "SoundFactory.h"
#include "SoundEngineCreator.h" // abstract mini factory definition


// mini factory
class SlSoundEngineCreator : public SoundEngineCreator
{
public:
   SlSoundEngineCreator()
   {
      this->addDescriptor( "sl" );
      this->addDescriptor( "SL" );
      SoundFactory::registerCreator( &mSoundEngineCreator );
   }
   virtual SoundEngine* newEngine()
   {
      cout<<"Regging SL Creator\n"<<flush;
      return new SlSoundEngine;
   }
   virtual ~SlSoundEngineCreator()
   {
      cout<<"SL Creator Died (application probably exited, or library was unloaded)\n"<<flush;
   }
   
   static SlSoundEngineCreator mSoundEngineCreator;
};

SlSoundEngineCreator SlSoundEngineCreator::mSoundEngineCreator;
