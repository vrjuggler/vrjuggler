#include <string>
#include <Sound/vjSound.h>
#include <Sound/vjSoundEngine.h>

// lookup a filename, given the sound's alias.
// the "filename" returned can be used in a call to Sound::load()
void vjSoundEngine::aliasToFileName( const char* const alias, std::string& filename )
{
   filename = alias;
}


// given an alias, return the handle.
// TODO: if called twice with name alias, should return same pointer.
// memory managed by engine...
// return NULL on load failure.
vjSound* vjSoundEngine::getHandle( const char* const alias )
{
   std::string filename;
   this->aliasToFileName( alias, filename );

   if (filename == "")
   {
      return NULL;
   }   
   
   vjSound* snd = this->newSound();
   bool didLoad = snd->load( filename.c_str() );
   
   if (didLoad)
   {
      return snd;
   }
   else
   {
      // return NULL on load failure.
      delete snd;
      return NULL;
   }   
}


//: Factory function to create a new sound.
// memory managed by engine
vjSound* vjSoundEngine::newSound()
{
   cout<<"new Sound()\n"<<flush;
   return new vjSound( *this );
}


// set position
void vjSoundEngine::setPosition( const vjMatrix& position )
{
   mPosition = position;
}

// set position
void vjSoundEngine::setPosition( const float& x, const float& y, const float& z )
{
   mPosition.makeTrans( x, y, z );
}

void vjSoundEngine::getPosition( vjMatrix& position ) const
{
   position = mPosition;
}

void vjSoundEngine::getPosition( float& x, float& y, float& z ) const
{
   vjVec3 point( 0.0f, 0.0f, 0.0f );
   point.xformFull( mPosition, point );
   x = point[0];
   y = point[1];
   z = point[2];
}
