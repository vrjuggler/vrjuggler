#ifndef SLSOUNDENGINE
#define SLSOUNDENGINE

#include <vjConfig.h>

#include <string>
#include <assert.h>
#include <unistd.h>
#include <iostream.h>
#include <Sound/AliasMapper.h> // used to map names to audio files.
#include <sl.h>
#include <Sound/SoundEngine.h> // base class..


#include <Threads/vjThread.h> // for when update is called.
#include <Threads/vjThreadFunctor.h> // for when update is called.

class SlSoundEngine : public SoundEngine
{
public:
   SlSoundEngine();
   virtual ~SlSoundEngine();

   // pass the config filename here...
   virtual void init( const char* const aliasDatabase );
   
   // lookup a filename, given the sound's alias.
   // the "filename" returned can be used in a call to Sound::load()
   virtual void aliasToFileName( const char* const alias, std::string& filename );

   // set position
   virtual void setPosition( const vjMatrix& position );
   
   // set position
   virtual void setPosition( const float& x, const float& y, const float& z );
   
   //: Factory function to create a new sound.
   // memory managed by engine
   virtual Sound* newSound();
   
   // call this from your app once per main loop.
   virtual void update();
   
   virtual void kill();
   
   AliasMapper mAliasDatabase;
   
   // SL's sound engine..
   slScheduler mScheduler;
  
   // the control loop for update...
   void updateLoop( void* nullParam );
   
   vjThread* mUpdateThread;
};

#endif
