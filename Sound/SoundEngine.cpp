#include <vjConfig.h>

#include <string>
#include <Sound/Sound.h>
#include <Sound/SoundEngine.h>

// lookup a filename, given the sound's alias.
// the "filename" returned can be used in a call to Sound::load()
void SoundEngine::aliasToFileName( const char* const alias, std::string& filename )
{
   filename = alias;
}


// given an alias, return the handle.
// TODO: if called twice with name alias, should return same pointer.
// memory managed by engine...
// return NULL on load failure.
Sound* SoundEngine::getHandle( const char* const alias )
{
   std::string filename;
   this->aliasToFileName( alias, filename );

   if (filename == "")
   {
      return NULL;
   }   
   
   Sound* snd = this->newSound();
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
Sound* SoundEngine::newSound()
{
   cout<<"new Sound()\n"<<flush;
   return new Sound( *this );
}


// set position
void SoundEngine::setPosition( const vjMatrix& position )
{
   mPosition = position;
}

// set position
void SoundEngine::setPosition( const float& x, const float& y, const float& z )
{
   mPosition.makeTrans( x, y, z );
}

void SoundEngine::getPosition( vjMatrix& position ) const
{
   position = mPosition;
}

void SoundEngine::getPosition( float& x, float& y, float& z ) const
{
   vjVec3 point( 0.0f, 0.0f, 0.0f );
   point.xformFull( mPosition, point );
   x = point[0];
   y = point[1];
   z = point[2];
}
