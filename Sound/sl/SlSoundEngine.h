#ifndef SLSOUNDENGINE
#define SLSOUNDENGINE

#include <string>
#include <assert.h>
#include <unistd.h>
#include <iostream.h>
#include <sl.h>
#include <Sound/vjSoundEngine.h> // base class..
#include <Sound/vjSoundEngineConstructor.h>


#include <Threads/vjThread.h> // for when update is called.
#include <Threads/vjThreadFunctor.h> // for when update is called.

class SlSoundEngine : public vjSoundEngine
{
public:
   SlSoundEngine();
   virtual ~SlSoundEngine();

   // pass the config filename here...
   virtual void init();
   
   // lookup a filename, given the sound's alias.
   // the "filename" returned can be used in a call to Sound::load()
   virtual void aliasToFileName( const char* const alias, std::string& filename );

   // set position
   virtual void setPosition( const vjMatrix& position );
   
   // set position
   virtual void setPosition( const float& x, const float& y, const float& z );
   
   //: Factory function to create a new sound.
   // memory managed by engine
   virtual vjSound* newSound();
   
   // call this from your app once per main loop.
   virtual void update();
   
   virtual void kill();
   
   std::map<std::string, std::string> mAliasDatabase;
   
	virtual bool config( vjConfigChunk* blowChunks );

	
	static std::string getChunkType() { return std::string( "SlSoundEngine" ); }


   // SL's sound engine..
   slScheduler mScheduler;
  
   // the control loop for update...
   void updateLoop( void* nullParam );
   
   vjThread* mUpdateThread;
};


vjSoundEngineConstructor<SlSoundEngine> sl_constructor; 
#endif
