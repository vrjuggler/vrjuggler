/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef SOUND_HANDLE
#define SOUND_HANDLE

#include <snx/snxConfig.h>

#include <string>
#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>
#include <vpr/Util/Assert.h>

#include <snx/sonix.h>

/** @defgroup SonixAPI The Sonix API.
 *  The classes listed here define the sonix API.
 *  You shouldn't need to use anything else.
 */

namespace snx
{
   /** \class SoundHandle SoundHandle.h snx/SoundHandle.h
    *
    * A handle to a Sonix sound.
    * This is a convenient handle to a sound, and it is the interface to using
    * individual sounds in Sonix.  Use conifgure() and remove() as the new and
    * delete C++ operators would be used.  Memory leaks could occur if remove()
    * not called.  This is because there may be more than one handle to a
    * sound.
    *
    * @note You may have to call snx::sonix::instance()->changeAPI() to
    *       switch to the sound API you want to use (such as OpenAL or
    *       AudioWorks).
    *
    * @ingroup SonixAPI
    */
   class SNX_CLASS_API SoundHandle
   {
   public:
      /**
       * Default constructor.  You should call init to name this handle.
       *
       * @post This sound handle is not associated with any name.  init()
       *       must be called to assign a name to this handle.
       *
       * @see init
       */
      SoundHandle();

      /**
       * Name constructor.  This assigns the given name to this sound handle.
       * This name is that is used in all snx::sonix class calls.
       * (snx::SoundHandle is a wrapper for snx::sonix).
       *
       * @post This sound handle is associated with the given name, and there
       *       is no need to call init().
       */
      SoundHandle(const std::string& myUniqueName);

      /**
       * Initializes the name of this sound with the given name.  If this name
       * has not been conifgured before, then configure() must be invoked on
       * this handle after calling init().
       *
       * @example "Example of initializing a SoundHandle"
       * \code
       * snx::SoundHandle click_sound;
       * click_sound.init("click");
       * click_sound.configure(sound_info);
       * \endcode
       *
       * @see snx::SoundHandle::configure
       */
      void init( const std::string& myUniqueName )
      {
         mAlias = myUniqueName;
      }

      /** Gets the name of this handle. */
      std::string getName() const { return mAlias; }

      /** Virtual destructor. */
      virtual ~SoundHandle();

   public:

      /**
       * Triggers a sound.
       *
       * @pre Object does not have to be a valid sound (but it helps).
       * @post If it is, then the loaded sound is triggered.  If it isn't then
       *       nothing happens.
       *
       * @param repeat Number of times to play: -1 to repeat forever, 1
       *               (single shot) is default.
       */
      virtual void trigger( const int& repeat = 1 )
      {
         sonix::instance()->trigger( mAlias, repeat );
      }

      /**
       * Is the sound currently playing?
       */
      virtual bool isPlaying()
      {
         return sonix::instance()->isPlaying( mAlias );
      }

      /**
       * Specifies whether the sound retriggers from beginning when triggered
       * while playing.  In other words, when the sound is already playing and
       * trigger() is called, does the sound restart from the beginning?
       *
       * @param onOff A Boolean value enabling or disabling retriggering.
       */
      virtual void setRetriggerable( bool onOff )
      {
         sonix::instance()->setRetriggerable( mAlias, onOff );
      }

      /**
       * Is the sound retriggerable?
       */
      virtual bool isRetriggerable()
      {
         return sonix::instance()->isRetriggerable( mAlias );
      }

      /**
       * Stops the sound.
       */
      virtual void stop()
      {
         sonix::instance()->stop( mAlias );
      }

      /**
       * Pauses the sound.  Use unpause() to return playback from where the
       * sound was paused.
       */
      virtual void pause()
      {
         sonix::instance()->pause( mAlias );
      }

      /**
       * Resumes playback from a paused state.  This does nothing if the sound
       * was not paused.
       */
      virtual void unpause()
      {
         sonix::instance()->unpause( mAlias );
      }

      /** If the sound is paused, then return true. */
      virtual bool isPaused()
      {
         return sonix::instance()->isPaused( mAlias );
      }

      /**
       * Sets the sound as either ambient or positional depending on the value
       * of ambient.  If the sound is ambient, it is attached to the listener,
       * and its volume does not change when the listener moves.  If the sound
       * is positional, the volume changes when the listener moves.
       *
       * @param ambient A flag identifying whether this sound is ambient (true)
       *                or positional (false).  This parameter is optional,
       *                and it defaults to false (the sound is positional).
       */
      virtual void setAmbient(const bool ambient = false)
      {
         sonix::instance()->setAmbient(mAlias, ambient);
      }

      /** Is the sound ambient? */
      virtual bool isAmbient()
      {
         return sonix::instance()->isAmbient( mAlias );
      }

      /**
       * Alters the frequency of the sample.
       *
       * @param amount The value that determines the pitch bend.  1.0 means
       *               that there is no change.  A value less than 1.0 is low;
       *               a value greather than 1.0 is high.
       */
      virtual void setPitchBend( float amount )
      {
         sonix::instance()->setPitchBend( mAlias, amount );
      }

      /**
       * Sets the effect volume.  The value must be in the range [0,1].
       *
       * @param amount The value of the volume.  It must be between 0.0 and
       *               1.0 inclusive.
       */
      virtual void setVolume( float amount )
      {
         sonix::instance()->setVolume( mAlias, amount );
      }

      /**
       * Sets the effect cutoff.  The value must be in the range [0,1].
       *
       * @param amount The value of the cutoff.  1.0 is no change; 0.0 is
       *               total cutoff.
       */
      virtual void setCutoff( float amount )
      {
         sonix::instance()->setCutoff( mAlias, amount );
      }

      /**
       * Set sound's 3D position.
       *
       * @param x The X coordinate of the sound in 3D OpenGL coordinates.
       * @param y The Y coordinate of the sound in 3D OpenGL coordinates.
       * @param z The Z coordinate of the sound in 3D OpenGL coordinates.
       */
      virtual void setPosition( const float& x, const float& y, const float& z )
      {
         sonix::instance()->setPosition( mAlias, x, y, z );
      }

      /**
       * Gets the sound's 3D position.
       *
       * @param x Storage for the X coordinate of the sound in 3D OpenGL
       *          coordinates.
       * @param y Storage for the Y coordinate of the sound in 3D OpenGL
       *          coordinates.
       * @param z Storage for the Z coordinate of the sound in 3D OpenGL
       *          coordinates.
       */
      virtual void getPosition( float& x, float& y, float& z )
      {
         sonix::instance()->getPosition( mAlias, x, y, z );
      }

      /**
       * Sets the position of the listener.
       *
       * @param mat A transformation matrix representing the position of the
       *            listener.
       */
      virtual void setListenerPosition( const gmtl::Matrix44f& mat )
      {
         sonix::instance()->setListenerPosition( mat );
      }

      /**
       * Gets the position of the listener.
       *
       * @param mat Storage for returning the position of the listener.
       */
      virtual void getListenerPosition( gmtl::Matrix44f& mat )
      {
         sonix::instance()->getListenerPosition( mat );
      }

      /**
       * Configures/reconfigures this sound by associating sound data with this
       * handle.  Afterwards, this SoundHandle can be used to operate on this
       * sound data.
       *
       * Configure: associate a name (alias) to the description if not already
       * done.
       *
       * Reconfigure: change properties of the sound to the description
       * provided.
       *
       * @pre Provide a SoundInfo which describes the sound.
       * @post This handle will point to loaded sound data.
       *
       * @param description An object that describes the sound for which this
       *                    object will be a handle.
       */
      virtual void configure( const snx::SoundInfo& description )
      {
         vprASSERT(mAlias != mDefaultName && "you must call init to set your sound's name");
         sonix::instance()->configure( mAlias, description );
      }

      /**
        * Removes a configured sound.  Any future reference to the alias will
        * not cause an error, but it will not result in a rendered sound.
        */
      virtual void remove()
      {
         sonix::instance()->remove( mAlias );
      }

   private:
      std::string mDefaultName;
      std::string mAlias;
   };
}

#endif
