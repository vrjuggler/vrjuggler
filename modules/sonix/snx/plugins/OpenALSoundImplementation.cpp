
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
		   std::cerr << "[aj]OpenAL| ERROR: Could not open device\n" << std::flush;
   	   return;
	   }

      // Initialize ALUT
      int attrlist[] = { ALC_FREQUENCY, 22050, ALC_INVALID };

      // create context
	   mContextId = alcCreateContext( mDev, attrlist );
      if (mContextId == NULL) 
      {
         std::string err = (char*)alGetString( alcGetError() );
		   std::cerr << "[aj]OpenAL| ERROR: Could not open context: " << err.c_str() << "\n" << std::flush;
		   return;
	   }

      // make context active...
	   alcMakeContextCurrent( mContextId );
      
      std::cerr<<"[aj]OpenAL| NOTICE: OpenAL API started: [dev="<<(int)mDev<<",ctx="<<(int)mContextId<<"]\n"<<std::flush;
   }
   else
   {
      std::cerr << "[aj]OpenAL| WARNING: startAPI called when API is already started\n" << std::flush;
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
   if (this->isStarted() == false)
   {
      std::cerr << "[aj]OpenAL| WARNING: API not started, nothing to shutdown [dev="<<(int)mDev<<",ctx="<<(int)mContextId<<"]\n" << std::flush;
      return;
   }
   
   this->unbindAll();

   if (mContextId != NULL)
   {
      alcDestroyContext( mContextId );
   }

   if (mDev != NULL)
   {
      alcCloseDevice(  mDev  );
   }

   mContextId = NULL;
   mDev = NULL;

   std::cerr<<"[aj]OpenAL| NOTICE: OpenAL API closed: [dev="<<(int)mDev<<",ctx="<<(int)mContextId<<"]\n"<<std::flush;
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
 * load/allocate the sound data this alias refers to the sound API
 * @postconditions the sound API has the sound buffered.
 */
void OpenALSoundImplementation::bind( const std::string& alias )
{
   // need to initialize the get error thingie, otherwise it can misreport errors...
   int err = alGetError(); 

   if (this->isStarted() == false)
   {
      std::cerr << "[aj]OpenAL| ERROR: API not started, bind() failed\n" << std::flush;
      return;
   }
   
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
         ALuint bufferID( 0 );
         ALuint sourceID( 0 );

         // open the file as readonly binary
	      if (!ajFileIO::fileExists( soundInfo.filename.c_str() )) 
         {
		      std::cerr<<"[aj]OpenAL| file doesn't exist: "<<soundInfo.filename<<"\n" << std::flush;
            break;
	      }

         // read the data from the file.
         std::cout<<"[aj]OpenAL| NOTIFY: loading: "<<soundInfo.filename<<"... " << std::flush;
         ajFileIO::fileLoad( soundInfo.filename.c_str(), mBindLookup[alias].data );
         std::cout<<"done("<<mBindLookup[alias].data.size()<<")\n" << std::flush;

	      // create a new buffer to put our loaded data into...
         alGenBuffers( 1, &bufferID );
         err = alGetError();
         if (err != AL_NO_ERROR)
         {
            std::cerr << "ERROR: Could not gen a buffer [err="<<err<<"]\n" << std::flush;
            switch (err)
            {
               case AL_INVALID_VALUE:
                  std::cerr << "       invalid value < 0\n" << std::flush;
                  break;
               case AL_OUT_OF_MEMORY:
                  std::cerr << "       out of memory\n" << std::flush;
                  break;
               default:
                  std::cerr << "       unknown error\n" << std::flush;
            }
         }
         
         // put the data into an OpenAL buffer
         alBufferData( bufferID, AL_FORMAT_WAVE_EXT, &(mBindLookup[alias].data[0]), mBindLookup[alias].data.size(), 0 );
         err = alGetError();
         if (err != AL_NO_ERROR)
         {
            std::cerr << "[aj]OpenAL| ERROR: Could not buffer data [bufferID="<<bufferID<<",err="<<err<<"]\n" << std::flush;
            switch (err)
            {
               case AL_ILLEGAL_COMMAND:
                  std::cout<<"       Streaming buffers cannot use alBufferData\n"<<std::flush;
                  break;
               case AL_INVALID_NAME:
                  std::cout<<"       bufferID is not a valid buffer name\n"<<std::flush;
                  break;
               //case AL_INVALID_ENUM:
               //   std::cout<<"       format is invalid\n"<<std::flush;
               //   break;
               case AL_OUT_OF_MEMORY:
                  std::cout<<"       not enough memory is available to make a copy of this data\n"<<std::flush;
                  break;
               default:
                  std::cout<<"       unknown error\n"<<std::flush;
            }            
		      alDeleteBuffers( 1, &bufferID );
            mBindLookup.erase( alias );
		      break;
	      }
         
         else
         {
            //std::cerr << "DEBUG: buffered data success [bufferID="<<bufferID<<"]\n" << std::flush;
         }

      
         // associate a source with the buffer
	      alGenSources( 1, &sourceID );
         if (alGetError() != AL_NO_ERROR)
         {
		      std::cerr << "[aj]OpenAL| ERROR: Could not generate a source\n" << std::flush;
            alDeleteBuffers( 1, &bufferID );
            mBindLookup.erase( alias );
		      break;
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

   // was it playing?  if so, then start it up again...
   if (soundInfo.triggerOnNextBind == true)
   {
      soundInfo.triggerOnNextBind = false; // done...
      std::cout<<"[aj]OpenAL| NOTIFY: triggering reconfigured sound\n"<<std::flush;
      this->trigger( alias );
   }
}   

/**
 * unload/deallocate the sound data this alias refers from the sound API
 * @postconditions the sound API no longer has the sound buffered.
 */
void OpenALSoundImplementation::unbind( const std::string& alias )
{
   if (this->isStarted() == false)
   {
      std::cerr << "[aj]OpenAL| ERROR: API not started, unbind() failed\n" << std::flush;
      return;
   }
 
   // is it currently playing?  if so, stop it
   if (this->isPlaying( alias ) == true)
   {
      this->stop( alias );
      
      // should trigger next time, since we just stopped it
      if (mSounds.count( alias ) > 0)
      {
         mSounds[alias].triggerOnNextBind = true; 
      }
      else
      {
         std::cout<<"[aj]OpenAL| ERROR: can't trigger on next bind. alias not registered when it should be\n"<<std::flush;
      }      
   }
   
   // if alias is bound, then unbind it...
   if (mBindLookup.count( alias ) > 0)
   {
      int err = alGetError();
      alDeleteSources( 1, &mBindLookup[alias].source );
      err = alGetError();
      if (err != AL_NO_ERROR)
      {
         std::cout<<"[aj]OpenAL| ERROR: unbind() deleting source\n"<<std::flush;
      }
      alDeleteBuffers( 1, &mBindLookup[alias].buffer );
      err = alGetError();
      if (err != AL_NO_ERROR)
      {
         std::cout<<"[aj]OpenAL| ERROR: unbind() deleting buffer\n"<<std::flush;
      }
      mBindLookup.erase( alias );
   }

   assert( mBindLookup.count( alias ) == 0 && "unbind failed" );
}

}; // end namespace
