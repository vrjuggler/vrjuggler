#ifndef _AW_SOUNDENGINE_H_
#define _AW_SOUNDENGINE_H_

#include <vjConfig.h>

#include <assert.h>
#include <unistd.h>
#include <iostream.h>
#include <aw.h> //audio works
#include <Sound/SoundEngine.h>

// you need an Observer node in your .adf file named you
// usually set it's position to 0,0,0
// every sound is positoined reletive to this observer
// this observer is updated by the soundengine's setPosition function.
//  it may be controlled by head tracking and navigation (or not at all)
//
// Positional sounds need to be specified using awPlayer nodes in the .adf file
// awSound node's position can only change with observer position change,
//  so i'd keep it flexible by using awPlayer nodes for each sound you want.
// Ambient sounds can be defined as plain awSound nodes.
class AwSoundEngine : public SoundEngine
{
public:
   AwSoundEngine();
   virtual ~AwSoundEngine();

   // pass the config filename here...
   virtual void init( const char* const aliasDatabase );
   
   // lookup a filename, given the sound's alias.
   // the "filename" returned can be used in a call to Sound::load()
   virtual void aliasToFileName( const char* const alias, std::string& filename );

   // set position
   virtual void setPosition( const vjMatrix& position );
   
   // set position
   virtual void setPosition( const float& x, const float& y, const float& z );
   
   // given an alias, return the handle.
   // TODO: if called twice with name alias, should return same pointer.
   // memory managed by engine...
   Sound* getHandle( const char* const alias );
   
   //: Factory function to create a new sound.
   // memory managed by engine
   virtual Sound* newSound();
   
   // call this once per main loop.
   //
   virtual void update();

   virtual void kill();

   awPlayer*   mPlayer;
   awObserver* mObserver;
};


#endif
