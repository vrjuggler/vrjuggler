/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <SNX heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <snx/PluginConfig.h>

#include <string>
#include <iostream>
#include <boost/concept_check.hpp>

#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>

#include <snx/SoundImplementation.h>
#include <snx/SoundInfo.h>
#include <snx/SoundFactory.h>
#include <snx/Util/Debug.h>

#include "AudiereSoundImplementation.h"

/////////////////////////
// plugin API:
#ifdef NO_SELF_REGISTER
extern "C"
{

SNX_PLUGIN_EXPORT(const char*) getVersion()
{
   return "sonix xx.xx.xx";
}

SNX_PLUGIN_EXPORT(const char*) getName()
{
   return "Audiere";
}

SNX_PLUGIN_EXPORT(snx::ISoundImplementation*) newPlugin()
{
   return new snx::AudiereSoundImplementation;
}

}
#endif
/////////////////////////

namespace snx
{
#ifndef NO_SELF_REGISTER
snx::SoundFactoryReg<AudiereSoundImplementation> audiereRegistrator("audiere");
#endif

void AudiereSoundImplementation::step( const float & timeElapsed )
{
   vprASSERT(mDev.get() != NULL &&
             "startAPI must be called prior to this function");

   snx::SoundImplementation::step( timeElapsed );
}


void AudiereSoundImplementation::remove( const std::string& alias )
{
   snx::SoundImplementation::remove( alias );
}

AudiereSoundImplementation::AudiereSoundImplementation()
   : snx::SoundImplementation()
   , mDev(NULL)
{
   // TODO: set up the defaults for audiere...
   //mSoundAPIInfo.
}

AudiereSoundImplementation::~AudiereSoundImplementation()
{
   this->shutdownAPI();
}

/**
 * @input alias of the sound to trigger, and number of times to play
 * @preconditions alias does not have to be associated with a loaded sound.
 * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
 * @semantics Triggers a sound
 */
void AudiereSoundImplementation::trigger(const std::string& alias,
                                         const int& looping)
{
   vprASSERT(mDev.get() != NULL &&
             "startAPI must be called prior to this function");

   vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
      << clrOutNORM(clrYELLOW, "Audiere| playing sound\n") << vprDEBUG_FLUSH;

   snx::SoundImplementation::trigger( alias, looping );

   snx::SoundInfo si = this->lookup(alias);

   if ( si.streaming )
   {
      this->bind(alias);
 //     mCurrentTrack->play();
      // Set audiere to loop indefinly if that is requested other wise play
      // only once.
      if ( trackMap.count(alias) > 0 )
      {
         trackMap[alias]->setRepeat(looping == -1);
         trackMap[alias]->play();
      }
   }
   else
   {
      // if the sound is already bound then play it.
      if ( effectMap.count(alias) > 0 )
      {
         effectMap[alias]->play();
      }
   }
}

bool AudiereSoundImplementation::isPlaying( const std::string& alias )
{
   vprASSERT(mDev.get() != NULL &&
             "startAPI must be called prior to this function");
   bool is_playing(false);

   snx::SoundInfo si = this->lookup(alias);

   if ( si.streaming )
   {
      if ( trackMap.count(alias) > 0 )
      {
         is_playing = trackMap[alias]->isPlaying();
      }
   }
   else
   {
      is_playing = false;
      /*
      if ( effectMap.count(alias) > 0 )
      {
      }
      */
   }

   return is_playing;
}

/** if the sound is paused, then return true. */
bool AudiereSoundImplementation::isPaused( const std::string& alias )
{
   vprASSERT(mDev.get() != NULL &&
             "startAPI must be called prior to this function");
   boost::ignore_unused_variable_warning(alias);
   return false;
}

/**
 * pause the sound, use unpause to return playback where you left off...
 */
void AudiereSoundImplementation::pause( const std::string& alias )
{
   vprASSERT(mDev.get() != NULL &&
             "startAPI must be called prior to this function");
   boost::ignore_unused_variable_warning(alias);
}

/**
 * resume playback from a paused state.  does nothing if sound was not paused.
 */
void AudiereSoundImplementation::unpause( const std::string& alias )
{
   vprASSERT(mDev.get() != NULL &&
             "startAPI must be called prior to this function");
   boost::ignore_unused_variable_warning(alias);
}

/**
 * @semantics stop the sound
 * @input alias of the sound to be stopped
 */
void AudiereSoundImplementation::stop( const std::string& alias )
{
   vprASSERT(mDev.get() != NULL &&
             "startAPI must be called prior to this function");

   if ( this->lookup(alias).streaming )
   {
      if ( trackMap.count(alias) > 0 )
      {
         trackMap[alias]->stop();
      }
   }
   else
   {
      if ( effectMap.count(alias) > 0 )
      {
         effectMap[alias]->stop();
      }
   }
}

/**
 * ambient or positional sound.
 * is the sound ambient - attached to the listener, doesn't change volume
 * when listener moves...
 * or is the sound positional - changes volume as listener nears or retreats..
 */
void AudiereSoundImplementation::setAmbient(const std::string& alias,
                                            bool ambient)
{
   snx::SoundImplementation::setAmbient(alias, ambient);
}

/**
 * associate a name (alias) to the description
 * @preconditions provide an alias and a SoundInfo which describes the sound
 * @postconditions alias will point to loaded sound data
 * @semantics associate an alias to sound data.  later this alias can be used to operate on this sound data.
 */
void AudiereSoundImplementation::configure(const std::string& alias,
                                           const snx::SoundInfo& description)
{
   snx::SoundImplementation::configure(alias, description);
}

/**
 * set sound's 3D position
 */
void AudiereSoundImplementation::setPosition(const std::string& alias,
                                             float x, float y, float z)
{
   vprASSERT(mDev.get() != NULL &&
             "startAPI must be called prior to this function");
   snx::SoundImplementation::setPosition( alias, x, y, z );

}

/**
 * get sound's 3D position
 * @input alias is a name that has been associate()d with some sound data
 * @output x,y,z are returned in OpenGL coordinates.
 */
void AudiereSoundImplementation::getPosition(const std::string& alias,
                                             float& x, float& y, float& z)
{
   snx::SoundImplementation::getPosition( alias, x, y, z );
}

/**
 * set the position of the listener
 */
void AudiereSoundImplementation::setListenerPosition(const gmtl::Matrix44f& mat)
{
   vprASSERT(mDev.get() != NULL &&
             "startAPI must be called prior to this function");

   snx::SoundImplementation::setListenerPosition( mat );
}

/**
 * get the position of the listener
 */
void AudiereSoundImplementation::getListenerPosition( gmtl::Matrix44f& mat )
{
   snx::SoundImplementation::getListenerPosition( mat );
}

/** 1 is no change.  2 is really high, 0 is really low. */
void AudiereSoundImplementation::setPitchBend(const std::string& alias,
                                              float amount)
{
   snx::SoundImplementation::setPitchBend( alias, amount );
}

/** 0 - 1.  use 0 for mute, use 1 for unmute... */
void AudiereSoundImplementation::setVolume(const std::string& alias,
                                           float amount)
{
   snx::SoundImplementation::setVolume(alias, amount);

   snx::SoundInfo si = this->lookup(alias);

   if ( si.streaming )
   {
      if ( trackMap.count(alias) > 0 )
      {
         trackMap[alias]->setVolume(amount);
      }
   }
   else
   {
      if ( effectMap.count(alias) > 0 )
      {
         effectMap[alias]->setVolume(amount);
      }
   }
}

/** 1 is no change.  0 is total cutoff. */
void AudiereSoundImplementation::setCutoff(const std::string& alias,
                                           float amount)
{
   snx::SoundImplementation::setCutoff( alias, amount );
}


/**
 * start the sound API, creating any contexts or other configurations at startup
 * @postconditions sound API is ready to go.
 *
 * @semantics this function should be called before using the other functions
 *            in the class.
 */
int AudiereSoundImplementation::startAPI()
{
   if ( mDev.get() == NULL )
   {
      // open the device for output
      mDev = audiere::OpenDevice();
      if ( ! mDev.get() )
      {
         vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
            << clrOutNORM(clrYELLOW, "Audiere| ERROR: Could not open device\n")
            << vprDEBUG_FLUSH;
         return 0;
      }

      vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
         << clrOutNORM(clrYELLOW, "Audiere| NOTICE:")
         << " Audiere API started: [dev=" << (int) mDev << "]\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
         << clrOutNORM(clrYELLOW, "Audiere| WARNING: startAPI called when API is already started\n")
         << vprDEBUG_FLUSH;
   }

   return 1;
}

/**
 * kill the sound API, deallocating any sounds, etc...
 * @postconditions sound API is ready to go.
 * @semantics this function could be called any time, the function could be
 *            called multiple times, so it should be smart.
 */
void AudiereSoundImplementation::shutdownAPI()
{
   if ( this->isStarted() == false )
   {
      vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
         << clrOutNORM(clrYELLOW, "Auidere| WARNING:")
         << "API not started, nothing to shutdown [dev="
         << std::hex << mDev.get() << std::dec << "]\n" << vprDEBUG_FLUSH;
      return;
   }

   this->unbindAll();

   mDev = NULL;

   vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
      << clrOutNORM(clrYELLOW, "Audiere| NOTICE:")
      << " Audiere API closed: [dev="
      << std::hex << mDev.get() << std::dec << "]\n" << vprDEBUG_FLUSH;
}

/**
 * clear all associate()tions.
 * @semantics any existing aliases will be stubbed. sounds will be unbind()ed
 */
void AudiereSoundImplementation::clear()
{
   snx::SoundImplementation::clear();
}

/**
 * load/allocate the sound data this alias refers to the sound API
 * @postconditions the sound API has the sound buffered or ready to stream.
 */
void AudiereSoundImplementation::bind( const std::string& alias )
{
   // need to initialize the get error thingie, otherwise it can misreport
   // errors...
   if ( this->isStarted() == false )
   {
      vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
         << clrOutNORM(clrYELLOW, "[snx]Audiere| ERROR: API not started, bind() failed\n")
         << vprDEBUG_FLUSH;
      return;
   }

   snx::SoundInfo& soundInfo = this->lookup( alias );

   // are we streaming this sound from disk?
   if ( soundInfo.streaming )
   {
      audiere::OutputStream* stream =
         audiere::OpenSound(mDev.get(), soundInfo.filename.c_str());

      if ( NULL != stream )
      {
         trackMap[alias] = stream;
      }
      else
      {
         vprDEBUG(snxDBG, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": [AudiereSoundImplementation::bind()] Failed to load";
         vprDEBUG_NEXTnl(snxDBG, vprDBG_WARNING_LVL)
            << "'" << soundInfo.filename << "'\n" << vprDEBUG_FLUSH;
      }
   }
   else
   {
      audiere::SoundEffect* effect =
         audiere::OpenSoundEffect(mDev.get(), soundInfo.filename.c_str(),
                                  audiere::SINGLE);

      if ( NULL != effect )
      {
         effectMap[alias] = effect;
      }
      else
      {
         vprDEBUG(snxDBG, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": [AudiereSoundImplementation::bind()] Failed to load";
         vprDEBUG_NEXTnl(snxDBG, vprDBG_WARNING_LVL)
            << "'" << soundInfo.filename << "'\n" << vprDEBUG_FLUSH;
      }
   }

   // was it playing?  if so, then start it up again...
   if ( soundInfo.triggerOnNextBind == true )
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
void AudiereSoundImplementation::unbind( const std::string& alias )
{
   if ( this->isStarted() == false )
   {
      vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
         << clrOutNORM(clrYELLOW, "[snx]Audiere| ERROR: API not started, unbind() failed\n")
         << vprDEBUG_FLUSH;
      return;
   }

   // is it currently playing?  if so, stop it
   if ( this->isPlaying(alias) == true )
   {
      this->stop( alias );

      // should trigger next time, since we just stopped it
      if (mSounds.count( alias ) > 0)
      {
         mSounds[alias].triggerOnNextBind = true;
      }
      else
      {
         vpr::DebugOutputGuard output7(snxDBG, vprDBG_CONFIG_LVL,
                                       std::string("ERROR: can't trigger on next bind. alias not registered when it should be\n"),
                                       std::string("\n"));
      }
   }
}

} // end namespace
