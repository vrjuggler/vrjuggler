/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert
 *
 ****************** <SNX heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <snx/PluginConfig.h>

#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <boost/version.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

#if defined(HAVE_AL_AL_H)
#  include <AL/al.h>
#  include <AL/alc.h>

#  if ! defined(WIN32) && ! defined(WIN64)
#     include <AL/alext.h>
#  endif
#else
#  include <al.h>
#  include <alc.h>
#endif

#if defined(HAVE_AL_ALUT_H)
#  include <AL/alut.h>
#else
#  include <alut.h>
#endif

#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Xforms.h>

#include <vpr/Util/Debug.h>

#include <snx/SoundImplementation.h>
#include <snx/SoundInfo.h>
#include <snx/SoundFactory.h>
#include <snx/Util/Debug.h>

#include "OpenALSoundImplementation.h"

#if defined(HAVE_ALUT_LOAD_MEMORY_FROM_FILE) || \
	defined(ALUT_API_MAJOR_VERSION) && ALUT_API_MAJOR_VERSION >= 1
#  define USE_ALUT_1_0
#endif

// On non-Windows platforms, the following symbols are defined through
// detection tests performed by the configure script. On Windows, we have
// to do it manually depending on whether we have ALUT 1.0 or newer.
#if defined(VPR_OS_Windows) && defined(USE_ALUT_1_0)
#  define HAVE_ALUT_GET_ERROR
#  define HAVE_ALUT_INIT_WITHOUT_CONTEXT
#endif

/////////////////////////
// plugin API:
#ifdef NO_SELF_REGISTER
extern "C"
{

SNX_PLUGIN_EXPORT const char* getVersion()
{
   return "sonix xx.xx.xx";
}

SNX_PLUGIN_EXPORT const char* getName()
{
   return "OpenAL";
}

SNX_PLUGIN_EXPORT snx::ISoundImplementation* newPlugin()
{
   return new snx::OpenALSoundImplementation;
}

}
#endif
/////////////////////////

namespace
{

std::string getAlErrorString(const ALenum errCode)
{
   std::string msg;

   switch ( errCode )
   {
      case AL_NO_ERROR:
         msg = "AL_NO_ERROR";
         break;
      case AL_INVALID_NAME:
         msg = "AL_INVALID_NAME";
         break;
      case AL_INVALID_ENUM:
         msg = "AL_INVALID_ENUM";
         break;
      case AL_INVALID_VALUE:
         msg = "AL_INVALID_VALUE";
         break;
      case AL_INVALID_OPERATION:
         msg = "AL_INVALID_OPERATION";
         break;
      case AL_OUT_OF_MEMORY:
         msg = "AL_OUT_OF_MEMORY";
         break;
   }

   return msg;
}

}

namespace snx
{
#ifndef NO_SELF_REGISTER
snx::SoundFactoryReg<OpenALSoundImplementation> openAlRegistrator( "OpenAL" );
#endif

OpenALSoundImplementation::OpenALSoundImplementation()
   : snx::SoundImplementation()
   , mBindLookup()
   , mContextId(NULL)
   , mDev(NULL)
{
   /* Do nothing. */ ;
}

OpenALSoundImplementation::~OpenALSoundImplementation()
{
   this->shutdownAPI();
}

/**
 * @input alias of the sound to trigger, and number of times to play
 * @preconditions alias does not have to be associated with a loaded sound.
 * @postconditions if it is, then the loaded sound is triggered.  if it isn't
 *                 then nothing happens.
 * @semantics Triggers a sound
 */
void OpenALSoundImplementation::trigger(const std::string& alias,
                                        const int looping)
{
   vprASSERT(mContextId != NULL && mDev != NULL &&
             "startAPI must be called prior to this function");

   snx::SoundImplementation::trigger( alias, looping );

   // if sound data hasn't been loaded into sound API yet, then do so
   if (mBindLookup.count( alias ) == 0)
   {
      this->bind( alias );
   }

   // if data is bound (bind() succeeded), then play it.
   if (mBindLookup.count( alias ) > 0)
   {
      bool retriggerable = this->isRetriggerable( alias );
      bool is_not_playing = !this->isPlaying( alias );
      bool is_paused = this->isPaused( alias );
      if (is_paused || retriggerable || is_not_playing)
      {
         // @todo: handle ability to loop openal sounds n number of times.
         //        only once or infinite is supported by OpenAL.
         if (looping == -1 || looping > 1)
         {
            alSourcei( mBindLookup[alias].source, AL_LOOPING, true );
         }
         else
         {
            alSourcei( mBindLookup[alias].source, AL_LOOPING, false );
         }

         alSourcePlay( mBindLookup[alias].source );
      }
   }
}

bool OpenALSoundImplementation::isPlaying( const std::string& alias )
{
   vprASSERT(mContextId != NULL && mDev != NULL &&
             "startAPI must be called prior to this function");

   if (mBindLookup.count( alias ) > 0)
   {
      vprASSERT(alIsSource(mBindLookup[alias].source) != AL_FALSE &&
                "weird, shouldn't happen...\n");

      ALint state( AL_INITIAL ); // initialized

#if defined(VPR_OS_Windows) || defined(VPR_OS_Darwin)
      alGetSourcei(mBindLookup[alias].source, AL_SOURCE_STATE, &state);
#else
      alGetSourceiv(mBindLookup[alias].source, AL_SOURCE_STATE, &state);
#endif

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

/** if the sound is paused, then return true. */
bool OpenALSoundImplementation::isPaused( const std::string& alias )
{
   vprASSERT(mContextId != NULL && mDev != NULL &&
             "startAPI must be called prior to this function");

   if (mBindLookup.count( alias ) > 0)
   {
      vprASSERT(alIsSource(mBindLookup[alias].source) != AL_FALSE &&
                "weird, shouldn't happen...\n");
      ALint state( AL_INITIAL ); // initialized
#if defined(VPR_OS_Windows) || defined(VPR_OS_Darwin)
      alGetSourcei(mBindLookup[alias].source, AL_SOURCE_STATE, &state);
#else
      alGetSourceiv(mBindLookup[alias].source, AL_SOURCE_STATE, &state);
#endif
      //std::cout << "state: " << state << (AL_PAUSED == state)
      //          << (AL_PLAYING == state) << (AL_STOPPED == state)
      //          << std::endl;
      return bool( AL_PAUSED == state );
   }

   return false;
}

/**
 * pause the sound, use unpause to return playback where you left off...
 */
void OpenALSoundImplementation::pause( const std::string& alias )
{
   vprASSERT(mContextId != NULL && mDev != NULL &&
             "startAPI must be called prior to this function");

   if (mBindLookup.count( alias ) > 0)
   {
      alSourcePause( mBindLookup[alias].source );
   }
}

/**
 * resume playback from a paused state.  does nothing if sound was not paused.
 */
void OpenALSoundImplementation::unpause( const std::string& alias )
{
   vprASSERT(mContextId != NULL && mDev != NULL &&
             "startAPI must be called prior to this function");

   if (mBindLookup.count( alias ) > 0)
   {
      if (this->isPaused( alias ))
      {
         alSourcePlay( mBindLookup[alias].source );
      }
   }
}

/**
 * @semantics stop the sound
 * @input alias of the sound to be stopped
 */
void OpenALSoundImplementation::stop( const std::string& alias )
{
   vprASSERT(mContextId != NULL && mDev != NULL &&
             "startAPI must be called prior to this function");

   snx::SoundImplementation::stop( alias );

   if (mBindLookup.count( alias ) > 0)
   {
      alSourceStop( mBindLookup[alias].source );
   }
}

/**
 * ambient or positional sound.
 * is the sound ambient - attached to the listener, doesn't change volume
 * when listener moves...
 * or is the sound positional - changes volume as listener nears or retreats..
 */
void OpenALSoundImplementation::setAmbient(const std::string& alias,
                                           bool ambient)
{
   snx::SoundImplementation::setAmbient( alias, ambient );
   if (mBindLookup.count( alias ) > 0 && mSounds.count( alias ) > 0)
   {
      // if positional
      if (ambient == false)
      {
         alSourcef( mBindLookup[alias].source, AL_SOURCE_RELATIVE, AL_FALSE );
      }
      // if ambient
      else
      {
         float pos[3] = { 0, 0, 0 };
         alSourcef( mBindLookup[alias].source, AL_SOURCE_RELATIVE, AL_TRUE );
         alSourcefv( mBindLookup[alias].source, AL_POSITION, pos );
      }
   }
}

/**
 * associate a name (alias) to the description
 * @preconditions provide an alias and a SoundInfo which describes the sound
 * @postconditions alias will point to loaded sound data
 * @semantics associate an alias to sound data.  later this alias can be used
 *            to operate on this sound data.
 */
void OpenALSoundImplementation::configure(const std::string& alias,
                                          const snx::SoundInfo& description)
{
   snx::SoundImplementation::configure( alias, description );
}

void OpenALSoundImplementation::remove(const std::string& alias)
{
   snx::SoundImplementation::remove( alias );
}

/**
 * set sound's 3D position
 */
void OpenALSoundImplementation::setPosition(const std::string& alias, float x,
                                            float y, float z)
{
   vprASSERT(mContextId != NULL && mDev != NULL &&
             "startAPI must be called prior to this function");
   snx::SoundImplementation::setPosition( alias, x, y, z );

   if (mBindLookup.count( alias ) > 0)
   {
      if (this->isAmbient( alias ) == false)
      {
         // @todo openal has a bug where SOURCE_RELATIVE set to true causes
         // GAIN_LINEAR_LOKI not to work at position == 0,0,0

         // @todo openal also has a bug where SOURCE_RELATIVE set to true
         // causes PITCH not to work at position != 0,0,0

         // this is not a good thing, since we can't write a hack that will
         // work!...

         float pos[3] = { x, y, z };
         alSourcefv( mBindLookup[alias].source, AL_POSITION, pos );
         // @todo: update AL_VELOCITY based on position over time...
      }
   }
}

/**
 * get sound's 3D position
 * @input alias is a name that has been associate()d with some sound data
 * @output x,y,z are returned in OpenGL coordinates.
 */
void OpenALSoundImplementation::getPosition(const std::string& alias,
                                            float& x, float& y, float& z) const
{
   snx::SoundImplementation::getPosition( alias, x, y, z );
}

/**
 * set the position of the listener
 */
void OpenALSoundImplementation::setListenerPosition(const gmtl::Matrix44f& mat)
{
   vprASSERT(mContextId != NULL && mDev != NULL &&
             "startAPI must be called prior to this function");

   snx::SoundImplementation::setListenerPosition( mat );

   // extract position from the matrix
   gmtl::Vec3f position;
   gmtl::setTrans( position, mat );

   // extract orientation from the matrix
   const gmtl::Vec3f forward( 0.0f, 0.0f, -1.0f );
   const gmtl::Vec3f up( 0.0f, 1.0f, 0.0f );
   gmtl::Vec3f forward_modified, up_modified;
   gmtl::xform( forward_modified, mat, forward );
   gmtl::xform( up_modified, mat, up );

   // openal wants a pair of 3 tuples: { forward, up }
   ALfloat orientation[] =
      { forward_modified[0], forward_modified[1], forward_modified[2],
        up_modified[0], up_modified[1], up_modified[2]
      };

   // set position
   alListenerfv( AL_POSITION, position.getData() );

   // set orientation
   alListenerfv( AL_ORIENTATION, orientation );
}

/**
 * get the position of the listener
 */
void
OpenALSoundImplementation::getListenerPosition(gmtl::Matrix44f& mat) const
{
   snx::SoundImplementation::getListenerPosition( mat );
}

/** 1 is no change.  2 is really high, 0 is really low. */
void OpenALSoundImplementation::setPitchBend(const std::string& alias,
                                             float amount)
{
   snx::SoundImplementation::setPitchBend( alias, amount );
   if (mBindLookup.count( alias ) > 0 && mSounds.count( alias ) > 0)
   {
      // zero or less is not legal.
      if (amount <= 0.0f)
      {
         amount = 0.001f;
      }

      alSourcef( mBindLookup[alias].source, AL_PITCH, amount );
   }
}

/** 0 - 1.  use 0 for mute, use 1 for unmute... */
void OpenALSoundImplementation::setVolume(const std::string& alias,
                                          float amount)
{
   snx::SoundImplementation::setVolume( alias, amount );
   if (mBindLookup.count( alias ) > 0 && mSounds.count( alias ) > 0)
   {
#if defined(AL_GAIN_LINEAR_LOKI)
      alSourcef( mBindLookup[alias].source, AL_GAIN_LINEAR_LOKI, amount );
#else
      alSourcef( mBindLookup[alias].source, AL_GAIN, amount );
#endif
   }
}

/** 1 is no change.  0 is total cutoff. */
void OpenALSoundImplementation::setCutoff(const std::string& alias,
                                          float amount)
{
   snx::SoundImplementation::setCutoff( alias, amount );
   if (mBindLookup.count( alias ) > 0 && mSounds.count( alias ) > 0)
   {
      // @todo: cutoff is not defined in openal, use gain instead... :(
#if defined(AL_GAIN_LINEAR_LOKI)
      alSourcef( mBindLookup[alias].source, AL_GAIN_LINEAR_LOKI, amount );
#else
      alSourcef( mBindLookup[alias].source, AL_GAIN, amount );
#endif
   }
}

/**
 * start the sound API, creating any contexts or other configurations at startup
 * @postconditions sound API is ready to go.
 *
 * @semantics this function should be called before using the other functions
 *            in the class.
 */
int OpenALSoundImplementation::startAPI()
{
   if (mContextId == NULL && mDev == NULL)
   {
      mDev = alcOpenDevice( NULL );
      if (mDev == NULL)
      {
         vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
            << clrOutNORM(clrYELLOW, "OpenAL| ERROR: Could not open device\n")
            << vprDEBUG_FLUSH;
         return 0;
      }

      // Initialize ALUT
      int attrlist[] = { ALC_FREQUENCY, 22050, ALC_INVALID };

#if defined(HAVE_ALUT_INIT_WITHOUT_CONTEXT)
      alutInitWithoutContext(NULL, NULL);
#endif

      // create context
      mContextId = alcCreateContext( mDev, attrlist );
      if (mContextId == NULL)
      {
         std::string err = (char*)alGetString( alcGetError( mDev ) );
         vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
            << clrOutNORM(clrYELLOW, "OpenAL| ERROR:")
            << "Could not open context: " << err << "\n" << vprDEBUG_FLUSH;
         return 0;
      }

      // make context active...
      alcMakeContextCurrent( mContextId );

      vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
         << clrOutNORM(clrYELLOW, "OpenAL| NOTICE:")
         << " OpenAL API started: [dev=0x" << std::hex << mDev
         << ",ctx=0x" << mContextId << std::dec << "]\n" << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
         << clrOutNORM(clrYELLOW, "OpenAL| WARNING: startAPI called when API is already started\n")
         << vprDEBUG_FLUSH;
   }

   // init the listener...
   this->setListenerPosition( mListenerPos );

   // ALfloat velocity[] = { 0.0f, 0.0f,  0.0f };
   // alListenerfv( AL_VELOCITY, velocity );
   return 1;
}

/**
 * kill the sound API, deallocating any sounds, etc...
 * @postconditions sound API is ready to go.
 * @semantics this function could be called any time, the function could be
 *            called multiple times, so it should be smart.
 */
void OpenALSoundImplementation::shutdownAPI()
{
   if (this->isStarted() == false)
   {
      vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
         << clrOutNORM(clrYELLOW, "OpenAL| WARNING:")
         << "API not started, nothing to shutdown [dev=0x" << std::hex
         << mDev << ",ctx=0x" << mContextId << std::dec << "]\n"
         << vprDEBUG_FLUSH;
      return;
   }

   this->unbindAll();

   alutExit();

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

   vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
      << clrOutNORM(clrYELLOW, "OpenAL| NOTICE:")
      << " OpenAL API closed: [dev=0x" << std::hex << mDev << ",ctx=0x"
      << mContextId << std::dec << "]\n" << vprDEBUG_FLUSH;
}

/**
 * clear all associate()tions.
 * @semantics any existing aliases will be stubbed. sounds will be unbind()ed
 */
void OpenALSoundImplementation::clear()
{
   snx::SoundImplementation::clear();
}

/**
 * load/allocate the sound data this alias refers to the sound API
 * @postconditions the sound API has the sound buffered.
 */
void OpenALSoundImplementation::bind( const std::string& alias )
{
   // need to initialize the get error thingie, otherwise it can misreport
   // errors...
   int err = alGetError();

   if (this->isStarted() == false)
   {
      vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
         << clrOutNORM(clrRED, "ERROR")
         << ": OpenAL| API not started, bind() failed\n" << vprDEBUG_FLUSH;
      return;
   }

   snx::SoundInfo& soundInfo = this->lookup( alias );

   // if alias is already bound, then unbind it...
   // TODO: we want a way to force a rebind, but do we _always_ want to force
   // it?
   if (mBindLookup.count( alias ) > 0)
   {
      this->unbind( alias );
   }

   // load the data
   switch (soundInfo.datasource)
   {
      default:
      case snx::SoundInfo::FILESYSTEM:
      {
         ALuint bufferID( 0 );
         ALuint sourceID( 0 );

         // Test for the existence of soundInfo.filename before proceeding
         // any further.
         try
         {
            boost::filesystem::path file_path(
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
               soundInfo.filename
#else
               soundInfo.filename, boost::filesystem::native
#endif
            );

            if ( ! boost::filesystem::exists(file_path) )
            {
               vprDEBUG(snxDBG, vprDBG_WARNING_LVL)
                  << clrOutNORM(clrRED, "ERROR") << ": OpenAL| alias '"
                  << alias << "',\n" << vprDEBUG_FLUSH;
               vprDEBUG_NEXT(snxDBG, vprDBG_WARNING_LVL)
                  << "file '" << soundInfo.filename << "'\n"
                  << vprDEBUG_FLUSH;
               vprDEBUG_NEXT(snxDBG, vprDBG_WARNING_LVL)
                  << "does not exist\n" << vprDEBUG_FLUSH;
               break;
            }
         }
         catch (boost::filesystem::filesystem_error& ex)
         {
            vprDEBUG(snxDBG, vprDBG_WARNING_LVL)
               << clrOutNORM(clrYELLOW, "WARNING")
               << ": OpenAL| File system exception while testing for the "
               << "existence of file '" << soundInfo.filename << "'"
               << std::endl << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(snxDBG, vprDBG_WARNING_LVL)
               << ex.what() << std::endl << vprDEBUG_FLUSH;
            break;
         }

         // read the data from the file.
         vpr::DebugOutputGuard output1(snxDBG, vprDBG_CONFIG_LVL,
                                       std::string("[snx]OpenAL| NOTIFY: loading: ") +
                                          soundInfo.filename +
                                          std::string("... \n"),
                                       std::string("\n"));

         ALenum format;
         ALvoid* data(NULL);
         ALsizei size;

         // alutLoadWAVFile() is deprecated, so we try to avoid using it if we
         // can.
#if defined(USE_ALUT_1_0)
         ALfloat freq;
         data = alutLoadMemoryFromFile(soundInfo.filename.c_str(), &format,
                                       &size, &freq);
#else
         ALsizei freq;
         // The Mac OS X version of alutLoadWAVFile() does not have the loop
         // parameter.
#  if defined(VPR_OS_Darwin)
         alutLoadWAVFile((ALbyte*) soundInfo.filename.c_str(), &format, &data,
                         &size, &freq);
#  else
         ALboolean loop;
         alutLoadWAVFile((ALbyte*) soundInfo.filename.c_str(), &format, &data,
                         &size, &freq, &loop);
#  endif
#endif  /* defined(USE_ALUT_1_0) */

         // XXX: This should probably throw an exception or something.
         if ( NULL == data )
         {
            vprDEBUG(snxDBG, vprDBG_CRITICAL_LVL)
               << clrOutNORM(clrRED, "ERROR")
               << ": OpenAL| Failed to load '" << soundInfo.filename << "'\n"
               << vprDEBUG_FLUSH;
#if defined(HAVE_ALUT_GET_ERROR)
            vprDEBUG_NEXT(snxDBG, vprDBG_CRITICAL_LVL)
               << alutGetErrorString(alutGetError()) << std::endl
               << vprDEBUG_FLUSH;
#endif
            return;
         }

         // Copy the memory in data into mBindLookup[alias].data.
         mBindLookup[alias].data.resize(size);
         memcpy(&mBindLookup[alias].data[0], data, size);

         // create a new buffer to put our loaded data into...
         alGenBuffers( 1, &bufferID );
         err = alGetError();
         if (err != AL_NO_ERROR)
         {
            vprDEBUG(snxDBG, vprDBG_WARNING_LVL)
               << clrOutNORM(clrYELLOW, "ERROR:")
               << "Could not gen a buffer [err=" << err << "]\n"
               << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(snxDBG, vprDBG_WARNING_LVL)
               << getAlErrorString(err) << std::endl << vprDEBUG_FLUSH;
         }

         // put the data into an OpenAL buffer
         alBufferData(bufferID, format, &(mBindLookup[alias].data[0]),
                      mBindLookup[alias].data.size(), freq);

#if defined(USE_ALUT_1_0)
         free(data);
#else
         alutUnloadWAV(format, data, size, freq);
#endif

         err = alGetError();
         if (err != AL_NO_ERROR)
         {
            vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
               << clrOutNORM(clrYELLOW, "OpenAL| ERROR:")
               << " Could not buffer data [bufferID="
               << bufferID << ",err=" << err << "]\n" << vprDEBUG_FLUSH;
            switch (err)
            {
#if defined(__APPLE__)
               case AL_INVALID_OPERATION:
#else
               case AL_ILLEGAL_COMMAND:
#endif
                  vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
                     <<  clrOutNORM(clrYELLOW, "Streaming buffers cannot use alBufferData\n")
                     << vprDEBUG_FLUSH;
                  break;
               case AL_INVALID_NAME:
                  vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
                     << clrOutNORM(clrYELLOW, "bufferID is not a valid buffer name\n")
                     << vprDEBUG_FLUSH;
                  break;
               //case AL_INVALID_ENUM:
               //   std::cout<<"       format is invalid\n"<<std::flush;
               //   break;
               case AL_OUT_OF_MEMORY:
                  vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
                     << clrOutNORM(clrYELLOW, "not enough memory is available to make a copy of this data\n")
                     << vprDEBUG_FLUSH;
                  break;
               default:
                  vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
                     << clrOutNORM(clrYELLOW, "       unknown error\n")
                     << vprDEBUG_FLUSH;
                  break;
            }
            alDeleteBuffers( 1, &bufferID );
            mBindLookup.erase( alias );
            break;
         }
         else
         {
            //std::cerr << "DEBUG: buffered data success [bufferID="
            //          << bufferID << "]\n";
         }

         // associate a source with the buffer
         alGenSources( 1, &sourceID );
         if (alGetError() != AL_NO_ERROR)
         {
            vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
               << clrOutNORM(clrYELLOW, "OpenAL| ERROR: Could not generate a source\n")
               << vprDEBUG_FLUSH;
            alDeleteBuffers( 1, &bufferID );
            mBindLookup.erase( alias );
            break;
         }
         alSourcei( sourceID, AL_BUFFER, bufferID );

         // store the resource IDs for later use...
         mBindLookup[alias].source = sourceID;
         mBindLookup[alias].buffer = bufferID;

         // set some defaults...
         alSourcei( sourceID, AL_LOOPING, AL_FALSE );
         alSourcef( sourceID, AL_MIN_GAIN, 0.0f ); // off
         alSourcef( sourceID, AL_MAX_GAIN, 1.0f ); // full

         this->setAmbient( alias, soundInfo.ambient );
         this->setPosition(alias, soundInfo.position[0],
                           soundInfo.position[1], soundInfo.position[2]);
         break;
      }
   }

   // was it playing?  if so, then start it up again...
   if (soundInfo.triggerOnNextBind == true)
   {
      soundInfo.triggerOnNextBind = false; // done...
      vpr::DebugOutputGuard output6(snxDBG, vprDBG_CONFIG_LVL,
                                    std::string("NOTIFY: triggering reconfigured sound\n"),
                                    std::string("\n"));
      this->trigger( alias, soundInfo.repeat );
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
      vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
         << clrOutNORM(clrYELLOW, "[snx]OpenAL| ERROR: API not started, unbind() failed\n")
         << vprDEBUG_FLUSH;
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
         vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
            << "ERROR: can't trigger on next bind. alias not registered "
            << "when it should be\n" << vprDEBUG_FLUSH;
      }
   }

   // if alias is bound, then unbind it...
   if (mBindLookup.count( alias ) > 0)
   {
      ALenum err = alGetError();
      alDeleteSources( 1, &mBindLookup[alias].source );
      err = alGetError();
      if (err != AL_NO_ERROR)
      {
         vprDEBUG(snxDBG, vprDBG_WARNING_LVL)
            << "ERROR: unbind() failed to delete source:\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(snxDBG, vprDBG_WARNING_LVL)
            << getAlErrorString(err) << std::endl << vprDEBUG_FLUSH;
      }
      alDeleteBuffers( 1, &mBindLookup[alias].buffer );
      err = alGetError();
      if (err != AL_NO_ERROR)
      {
         vprDEBUG(snxDBG, vprDBG_WARNING_LVL)
            << "ERROR: unbind() failed to delete buffer buffer:\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(snxDBG, vprDBG_WARNING_LVL)
            << getAlErrorString(err) << std::endl << vprDEBUG_FLUSH;
      }
      mBindLookup.erase( alias );
   }

   vprASSERT(mBindLookup.count(alias) == 0 && "unbind failed");
}

void OpenALSoundImplementation::step(const float timeElapsed)
{
   vprASSERT(mContextId != NULL && mDev != NULL &&
             "startAPI must be called prior to this function");

   snx::SoundImplementation::step(timeElapsed);
}

} // end namespace
