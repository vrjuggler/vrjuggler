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
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef SNXSOUNDIMPLEMENTATION_INTERFACE_H
#define SNXSOUNDIMPLEMENTATION_INTERFACE_H

#include <snx/snxConfig.h>

#include <string>
#include <gmtl/Matrix.h>
#include <vpr/vpr.h>

#include "snx/SoundInfo.h"
#include "snx/SoundAPIInfo.h"

namespace snx
{

/** \class ISoundImplementation ISoundImplementation.h snx/ISoundImplementation.h
 *
 * API-specific sound implementation interface.
 */
class ISoundImplementation
{
public:
   /** Default constructor. */
   ISoundImplementation() {}

   /**
    * Every implementation can return a new copy of itself.
    */
   virtual void clone( ISoundImplementation* &newCopy ) = 0;

   /**
    * Default virtual destructor.
    */
   virtual ~ISoundImplementation() {}

public:

   /**
    * Triggers a sound.
    *
    * @pre alias does not have to be associated with a loaded sound.
    * @post If alias is associated with a loaded sound, then the loaded sound
    *       is triggered.  If it isn't, then nothing happens.
    *
    * @param alias Alias of the sound to trigger, and number of times to play:
    *              -1 is repeat infinitely.
    */
   virtual void trigger(const std::string& alias, const int repeat = 1) = 0;

   /**
    * Is the sound currently playing?
    */
   virtual bool isPlaying( const std::string& alias ) = 0;

   /**
    * When sound is already playing then you call trigger,
    * does the sound restart from beginning?
    */
   virtual void setRetriggerable( const std::string& alias, bool onOff ) = 0;

   /**
    * Is the sound retriggerable?
    */
   virtual bool isRetriggerable( const std::string& alias ) = 0;

   /**
    * Stops the sound.
    * @param alias Alias of the sound to be stopped.
    */
   virtual void stop( const std::string& alias ) = 0;

   /**
    * Pauses the sound, use unpause to return playback where you left off.
    */
   virtual void pause( const std::string& alias ) = 0;

   /**
    * Resumes playback from a paused state.  Does nothing if sound was not
    * paused.
    */
   virtual void unpause( const std::string& alias ) = 0;

   /** If the sound is paused, then return true. */
   virtual bool isPaused( const std::string& alias ) = 0;

   /**
    * Ambient or positional sound.
    * Is the sound ambient: attached to the listener, doesn't change volume
    * when listener moves.
    * Or is the sound positional: changes volume as listener nears or retreats.
    */
   virtual void setAmbient( const std::string& alias, bool ambient = false ) = 0;

   /**
    * Is the sound ambient: attached to the listener, doesn't change volume
    * when listener moves.
    */
   virtual bool isAmbient( const std::string& alias ) = 0;

   /** Bends the pitch of the sound. */
   virtual void setPitchBend( const std::string& alias, float amount ) = 0;

   /** Sets the effect volume.  Set to a value between [0..1]. */
   virtual void setVolume( const std::string& alias, float amount ) = 0;

   /**
    * Sets the effect cutoff.
    * Set to a value between [0..1]... 1 is no change.  0 is total cutoff.
    */
   virtual void setCutoff( const std::string& alias, float amount ) = 0;

   /**
    * Sets sound's 3D position.
    */
   virtual void setPosition( const std::string& alias, float x, float y, float z ) = 0;

   /**
    * Gets sound's 3D position.
    *
    * @param alias A that has been associated with some sound data.
    * @param x     Storage for the X coordinate of the sound's position (in
    *              OpenGL coordinates).
    * @param y     Storage for the Y coordinate of the sound's position (in
    *              OpenGL coordinates).
    * @param z     Storage for the Z coordinate of the sound's position (in
    *              OpenGL coordinates).
    */
   virtual void getPosition( const std::string& alias, float& x, float& y, float& z ) = 0;

   /**
    * Sets the position of the listener.
    */
   virtual void setListenerPosition( const gmtl::Matrix44f& mat ) = 0;

   /**
    * Gets the position of the listener.
    */
   virtual void getListenerPosition( gmtl::Matrix44f& mat ) = 0;

   /**
    * Starts the sound API, creating any contexts or other configurations at
    * startup.  This function should be called before using the other functions
    * in the class.
    *
    * @post Sound API is ready to go.
    *
    * @return 1 if successful, 0 otherwise.
    */
   virtual int startAPI() = 0;

   /**
    * Queries whether the API has been started or not.
    *
    * @return true if API has been started, false otherwise.
    */
   virtual bool isStarted() const = 0;

   /**
    * Kills the sound API, deallocating any sounds, etc.  This function could
    * be called any time.  The function could be called multiple times, so it
    * should be smart.
    *
    * @post Sound API is ready to go.
    */
   virtual void shutdownAPI() = 0;

   /**
    * Configures/reconfigures the sound API global settings.
    */
   virtual void configure( const snx::SoundAPIInfo& sai ) = 0;

   /**
    * Configures/reconfigures a sound by associating an alias to sound data.
    * Later, this alias can be used to operate on this sound data.
    *
    * Configure: associate a name (alias) to the description if not already
    * done.
    * Reconfigure: change properties of the sound to the description provided.
    *
    * @pre Provide an alias and a SoundInfo which describes the sound.
    * @post Alias will point to loaded sound data.
    */
   virtual void configure( const std::string& alias, const snx::SoundInfo& description ) = 0;

   /**
    * Removes a configured sound.  Any future reference to the alias will not
    * cause an error, but will not result in a rendered sound.
    */
   virtual void remove(const std::string& alias) = 0;

   /**
    * Call once per sound frame (doesn't have to be same as your graphics
    * frame).
    *
    * @param timeElapsed The time elapsed since the last sound frame.
    */
   virtual void step(const float timeElapsed) = 0;

   /**
    * Clears all associations.
    *
    * @post Any existing aliases will be stubbed. Sounds will be unbound.
    */
   virtual void clear() = 0;

   /**
    * Bind: load (or reload) all associated sounds.
    *
    * @post All sound associations are buffered by the sound API.
    */
   virtual void bindAll() = 0;

   /**
    * Unbind: unload/deallocate all associated sounds.
    *
    * @post All sound associations are unbuffered by the sound API.
    */
   virtual void unbindAll() = 0;

   /**
    * Loads/allocates the sound data this alias refers to the sound API.
    *
    * @post The sound API has the sound buffered.
    */
   virtual void bind( const std::string& alias ) = 0;

   /**
    * Unloads/deallocates the sound data this alias refers from the sound API.
    *
    * @post The sound API no longer has the sound buffered.
    */
   virtual void unbind( const std::string& alias ) = 0;

   virtual snx::SoundInfo& lookup( const std::string& alias ) = 0;

   virtual void setName( const std::string& name ) = 0;

   virtual std::string& name() = 0;

#ifdef VPR_OS_Windows
   /**
    * Overlaod delete so that we can delete our memory correctly.  This is
    * necessary for DLLs on Windows to release memory from the correct memory
    * space.  All subclasses must overload delete similarly.
    */
   void operator delete(void* p)
   {
      if ( NULL != p )
      {
         ISoundImplementation* sound_ptr = static_cast<ISoundImplementation*>(p);
         sound_ptr->destroy();
      }
   }
#endif

protected:
   /**
    * Subclasses must implement this so that dynamically loaded plug-ins
    * delete themselves in the correct memory space.  This uses a template
    * method pattern.
    */
   virtual void destroy() = 0;
};

} // end namespace

#endif //SNXSOUNDIMPLEMENTATION_INTERFACE_H
