
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>// for FILE

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/alut.h>

#include "aj/Matrix44.h"
#include "aj/Vec3.h"
#include "aj/MatVec.h"
#include "aj/FileIO.h"
#include "aj/SoundImplementation.h"
#include "aj/SoundInfo.h"

#include "aj/plugins/OpenALSoundImplementation.h"

#include "aj/SoundFactory.h"
namespace aj
{
aj::SoundFactoryReg<OpenALSoundImplementation> openAlRegistrator( "OpenAL" );

void OpenALSoundImplementation::step( const float & timeElapsed )
{
   assert( mContextId != NULL && mDev != NULL && "startAPI must be called prior to this function" );
   
   aj::SoundImplementation::step( timeElapsed );
}


void OpenALSoundImplementation::remove( const std::string alias )
{
   aj::SoundImplementation::remove( alias );
}
OpenALSoundImplementation::OpenALSoundImplementation() : aj::SoundImplementation(), mContextId( NULL ), mDev( NULL ), mBindLookup()
{
   // TODO: set up the defaults for openal...
   //mSoundAPIInfo.
}

OpenALSoundImplementation::~OpenALSoundImplementation()
{
   this->shutdownAPI();
}  

/**
 * @input alias of the sound to trigger, and number of times to play
 * @preconditions alias does not have to be associated with a loaded sound.
 * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
 * @semantics Triggers a sound
 */
void OpenALSoundImplementation::trigger( const std::string& alias, const unsigned int& looping )
{
   assert( mContextId != NULL && mDev != NULL && "startAPI must be called prior to this function" );
   
   aj::SoundImplementation::trigger( alias, looping );

   // if sound data hasn't been loaded into sound API yet, then do so
   if (mBindLookup.count( alias ) == 0)
   {
      this->bind( alias );
   }
   
   // if data is bound (bind() succeeded), then play it.
   if (mBindLookup.count( alias ) > 0)
   {
      alSourcePlay( mBindLookup[alias].source );
   }
}

bool OpenALSoundImplementation::isPlaying( const std::string& alias )
{
   assert( mContextId != NULL && mDev != NULL && "startAPI must be called prior to this function" );
   
   if (mBindLookup.count( alias ) > 0)
   {
      assert( alIsSource( mBindLookup[alias].source ) != AL_FALSE && "weird, shouldn't happen...\n" );

      ALint state( AL_INITIAL ); // initialized
	   alGetSourceiv( mBindLookup[alias].source, AL_SOURCE_STATE, &state );

      switch(state) 
      {
		   case AL_PLAYING:
		   case AL_PAUSED:
		     return true;
		   default:
           return false;
		     break;
	   }
   }

	return false;
}

/**
 * @semantics stop the sound
 * @input alias of the sound to be stopped
 */
void OpenALSoundImplementation::stop( const std::string& alias )
{
   assert( mContextId != NULL && mDev != NULL && "startAPI must be called prior to this function" );
   
   aj::SoundImplementation::stop( alias );

   if (mBindLookup.count( alias ) > 0)
   {
      alSourceStop( mBindLookup[alias].source );
   }
}

/**
 * associate a name (alias) to the description
 * @preconditions provide an alias and a SoundInfo which describes the sound
 * @postconditions alias will point to loaded sound data
 * @semantics associate an alias to sound data.  later this alias can be used to operate on this sound data.
 */
void OpenALSoundImplementation::configure( const std::string& alias, const aj::SoundInfo& description )
{
   aj::SoundImplementation::configure( alias, description );
}

/**
 * set sound's 3D position 
 */
void OpenALSoundImplementation::setPosition( const std::string& alias, float x, float y, float z )
{
   assert( mContextId != NULL && mDev != NULL && "startAPI must be called prior to this function" );
   aj::SoundImplementation::setPosition( alias, x, y, z );

   if (mBindLookup.count( alias ) > 0)
   {
      float pos[3] = { x, y, z };
      alSourcefv( mBindLookup[alias].source, AL_POSITION, pos );
   }
}

/**
 * get sound's 3D position
 * @input alias is a name that has been associate()d with some sound data
 * @output x,y,z are returned in OpenGL coordinates.
 */
void OpenALSoundImplementation::getPosition( const std::string& alias, float& x, float& y, float& z )
{
   aj::SoundImplementation::getPosition( alias, x, y, z );
}

/**
 * set the position of the listener
 */
void OpenALSoundImplementation::setListenerPosition( const aj::Matrix44& mat )
{
   assert( mContextId != NULL && mDev != NULL && "startAPI must be called prior to this function" );
   
   aj::SoundImplementation::setListenerPosition( mat );

   // extract position from the matrix
   ALfloat position[3];
   mat.getTrans( position[0], position[1], position[2] );

   // extract orientation from the matrix
   const aj::Vec3 forward( 0.0f, 0.0f, -1.0f );
   const aj::Vec3 up( 0.0f, 1.0f, 0.0f );
   aj::Vec3 forward_modified, up_modified;
   forward_modified = aj::xformVec( mat, forward );
   up_modified = aj::xformVec( mat, up );

   // openal wants a pair of 3 tuples: { forward, up }
   ALfloat orientation[]  = { forward_modified[0], forward_modified[1], forward_modified[2],
                             up_modified[0], up_modified[1], up_modified[2] };

   // set position
   alListenerfv( AL_POSITION, position );

   // set orientation
	alListenerfv( AL_ORIENTATION, orientation );
}

/**
 * get the position of the listener
 */
void OpenALSoundImplementation::getListenerPosition( aj::Matrix44& mat )
{
   aj::SoundImplementation::getListenerPosition( mat );
}


/**
 * start the sound API, creating any contexts or other configurations at startup
 * @postconditions sound API is ready to go.
 * @semantics this function should be called before using the other functions in the class.
 */
void OpenALSoundImplementation::startAPI()
{
   if (mContextId == NULL && mDev == NULL)
   {
      mDev = alcOpenDevice( NULL );
	   if (mDev == NULL) 
      {
		   std::cerr << "Could not open device\n" << std::flush;
   	   return;
	   }

      // Initialize ALUT
      std::vector<int> attrlist;
      attrlist.push_back( ALC_FREQUENCY );
      attrlist.push_back( 22050 );
      attrlist.push_back( ALC_INVALID );

      // create context
	   mContextId = alcCreateContext( mDev, &attrlist[0] );
      if (mContextId == NULL) 
      {
         std::string err = (char*)alGetString( alcGetError() );
		   std::cerr << "Could not open context: " << err.c_str() << "\n" << std::flush;
		   return;
	   }

      // make context active...
	   alcMakeContextCurrent( mContextId );
   }
   else
   {
      std::cerr << "startAPI called when API is already started\n" << std::flush;
   }      

   // init the listener...
   this->setListenerPosition( mListenerPos );

   // ALfloat velocity[] = { 0.0f, 0.0f,  0.0f };
	// alListenerfv( AL_VELOCITY, velocity );
}

/**
 * kill the sound API, deallocating any sounds, etc...
 * @postconditions sound API is ready to go.
 * @semantics this function could be called any time, the function could be called multiple times, so it should be smart.
 */
void OpenALSoundImplementation::shutdownAPI()
{
   if (mDev != NULL && mContextId != NULL)
   {
      this->unbindAll();
      
      alcDestroyContext( mContextId );
	   alcCloseDevice(  mDev  );

      mContextId = NULL;
      mDev = NULL;
   }
}   

/**
 * clear all associate()tions.
 * @semantics any existing aliases will be stubbed. sounds will be unbind()ed
 */
void OpenALSoundImplementation::clear()
{
   aj::SoundImplementation::clear();
}   

/**
 * bind: load (or reload) all associate()d sounds
 * @postconditions all sound associations are buffered by the sound API
 */
void OpenALSoundImplementation::bindAll()
{
   std::map< std::string, aj::SoundInfo >::iterator it;
   for( it = mSounds.begin(); it != mSounds.end(); ++it)
   {
      std::cout<<(*it).first<<"\n"<<std::flush;
      this->bind( (*it).first );
   }
}   

/**
 * unbind: unload/deallocate all associate()d sounds.
 * @postconditions all sound associations are unbuffered by the sound API
 */
void OpenALSoundImplementation::unbindAll()
{
   std::map< std::string, AlSoundInfo >::iterator it;
   for( it = mBindLookup.begin(); it != mBindLookup.end(); ++it)
   {
      this->unbind( (*it).first );
   }

   assert( mBindLookup.size() == 0 && "unbindAll failed" );
}



/**
 * load/allocate the sound data this alias refers to the sound API
 * @postconditions the sound API has the sound buffered.
 */
void OpenALSoundImplementation::bind( const std::string& alias )
{
   aj::SoundInfo& soundInfo = this->lookup( alias );

   // if alias is already bound, then unbind it...
   // TODO: we want a way to force a rebind, but do we _always_ want to force it?
   if (mBindLookup.count( alias ) > 0)
   {
      this->unbind( alias );
   }

   // load the data 
   switch (soundInfo.datasource)
   {
      default:
      case aj::SoundInfo::FILESYSTEM:
      {
         ALuint bufferID;
         ALuint sourceID;

         // open the file as readonly binary
	      if (!ajFileIO::fileExists( soundInfo.filename.c_str() )) 
         {
		      std::cerr<<"file doesn't exist: "<<soundInfo.filename<<"\n" << std::flush;
            return;
	      }

         // read the data from the file.
         std::cout<<"loading: "<<soundInfo.filename<<"... " << std::flush;
         ajFileIO::fileLoad( soundInfo.filename.c_str(), mBindLookup[alias].data );
         std::cout<<"done\n" << std::flush;

	      // put the data in an OpenAL buffer
	      alGenBuffers( 1, &bufferID );
         alBufferData( bufferID, AL_FORMAT_WAVE_EXT, &(mBindLookup[alias].data[0]), mBindLookup[alias].data.size(), 0 );
         if (alGetError() != AL_NO_ERROR)
         {
		      std::cerr << "Could not buffer data\n" << std::flush;
            alDeleteBuffers( 1, &bufferID );
            mBindLookup.erase( alias );
		      return;
	      }
      
         // associate a source with the buffer
	      alGenSources( 1, &sourceID );
         if (alGetError() != AL_NO_ERROR)
         {
		      std::cerr << "Could not gen a source\n" << std::flush;
            alDeleteBuffers( 1, &bufferID );
            mBindLookup.erase( alias );
		      return;
	      }
	      alSourcei( sourceID, AL_BUFFER, bufferID );
	      alSourcei( sourceID, AL_LOOPING, AL_FALSE );

         this->setPosition( alias, soundInfo.position[0], soundInfo.position[1], soundInfo.position[2] );
         
         // store the resource IDs for later use...
         mBindLookup[alias].source = sourceID;
         mBindLookup[alias].buffer = bufferID;
         break;
      }
   }

}   

/**
 * unload/deallocate the sound data this alias refers from the sound API
 * @postconditions the sound API no longer has the sound buffered.
 */
void OpenALSoundImplementation::unbind( const std::string& alias )
{
   if (mBindLookup.count( alias ) > 0)
   {
      alDeleteSources( 1, &mBindLookup[alias].source );
      alDeleteBuffers( 1, &mBindLookup[alias].buffer );
      mBindLookup.erase( alias );
   }

   assert( mBindLookup.count( alias ) == 0 && "unbind failed" );
}

}; // end namespace
