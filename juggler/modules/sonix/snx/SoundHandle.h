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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
   /**
    * A handle to a sonix sound.
    * This is a convenient handle to your sound and is your interface to
    * using individual sounds in sonix use configure, and remove as you
    * would new and delete.
    * That is, you could leak, if you don't call remove.  This is because
    * there may be more than one handle to a sound.
    * @note You may have to sonix::instance()->changeAPI() to the API
    *       you want to use (usually OpenAL or AudioWorks).
    * @ingroup SonixAPI
    */
   class SNX_CLASS_API SoundHandle
   {
   public:
      /**
       * Default constructor.  You should call init to name this handle.
       *
       * @see init
       */
      SoundHandle();

      /**
       * Name constructor.  This gives the handle your chosen name.
       * This name is what is used in all sonix:: class calls (SoundHandle is
       * a wrapper for sonix::).
       *
       * @post You do not need to call init if you use the name constructor.
       *
       * @see init
       */
      SoundHandle(const std::string& myUniqueName);

      /**
       * Initialize the name of this sound with your chosen name.
       * Important: you must configure() your sound after calling init(), if
       * this name hasn't been configured before...
       *
       * @example "Example of initializing a SoundHandle"
       * \code
       *    snx::SoundHandle click_sound;
       *    click_sound.init( "click" );
       *    click_sound.configure( sound_info );
       * \endcode
       *
       * @see snx::SoundHandle::configure
       */
      void init( const std::string& myUniqueName )
      {
         mAlias = myUniqueName;
      }

      /** Gets the handle name. */
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
       * @param repeat Number of times to play: -1 to repeat infinately, 1
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
       * Specifies whether sound retriggers from beginning when triggered
       * while playing.  When sound is already playing then you call trigger,
       * does the sound restart from beginning?
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
       * Pause the sound.  Use unpause to return playback where you left off.
       */
      virtual void pause()
      {
         sonix::instance()->pause( mAlias );
      }

      /**
       * Resumes playback from a paused state.  Does nothing if sound was not
       * paused.
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
       * Ambient or positional sound.
       * Is the sound ambient: attached to the listener, doesn't change volume
       * when listener moves.
       * Or is the sound positional: changes volume as listener nears or
       * retreats.
       */
      virtual void setAmbient( bool setting = false )
      {
         sonix::instance()->setAmbient( mAlias, setting );
      }

      /** Is the sound ambient? */
      virtual bool isAmbient()
      {
         return sonix::instance()->isAmbient( mAlias );
      }

      /**
       * Alters the frequency of the sample.
       * 1 is no change.
       * < 1 is low.
       * > 1 is high.
       */
      virtual void setPitchBend( float amount )
      {
         sonix::instance()->setPitchBend( mAlias, amount );
      }

      /** Sets the effect volume.  set to a value between [0..1]. */
      virtual void setVolume( float amount )
      {
         sonix::instance()->setVolume( mAlias, amount );
      }

      /**
       * Sets the effect cutoff.
       * Set to a value between [0..1]... 1 is no change.  0 is total cutoff.
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
       */
      virtual void setListenerPosition( const gmtl::Matrix44f& mat )
      {
         sonix::instance()->setListenerPosition( mat );
      }

      /**
       * Gets the position of the listener.
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
       * Reconfigure: change properties of the sound to the descriptino
       * provided.
       *
       * @pre Provide a SoundInfo which describes the sound.
       * @post This handle will point to loaded sound data.
       */
      virtual void configure( const snx::SoundInfo& description )
      {
         vprASSERT(mAlias != mDefaultName && "you must call init to set your sound's name");
         sonix::instance()->configure( mAlias, description );
      }

      /**
        * Removes a configured sound.  Any future reference to the mAlias will
        * not cause an error, but will not result in a rendered sound.
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
