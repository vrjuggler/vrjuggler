#ifndef SOUNDENGINE 
#define SOUNDENGINE

#include <string>
#include <vjMatrix.h>

class Sound;

// Normally, use the SoundFactory factory to create a new sound engine.
// You shouldn't create one directly (but you could if you wanted).
class SoundEngine
{
public:
   virtual void init( const char* const adfFileName ) {}
   
   // your application must call this once per main loop.
   virtual void update() {}
   
   // lookup a filename, given the sound's alias.
   // the "filename" returned can be used in a call to Sound::load()
   virtual void aliasToFileName( const char* const alias, std::string& filename );

   // set position of the observer (user)
   // TODO: multiple users?
   virtual void setPosition( const vjMatrix& position );
   void getPosition( vjMatrix& position ) const;
   
   // set position of the observer (user)
   // TODO: multiple users?
   virtual void setPosition( const float& x, const float& y, const float& z );
   void getPosition( float& x, float& y, float& z ) const;
   
   // given an alias, return the handle.
   // TODO: if called twice with name alias, should return same pointer.
   // memory managed by engine...
   // returns NULL if invalid name.
   Sound* getHandle( const char* const alias );
   
   //: Factory function to create a new sound.
   // memory managed by engine
   virtual Sound* newSound();
   
   virtual void kill() {}
   
   
   vjMatrix mPosition;
};

#endif
