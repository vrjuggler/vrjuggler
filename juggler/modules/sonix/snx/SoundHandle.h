
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

#include <string>
#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>

#include <snx/sonix.h>

/** @defgroup SonixAPI The Sonix API.
 *  The classes listed here define the sonix API.
 *  You shouldn't need to use anything else.
 */

namespace snx
{
   /**
    * A handle to a sonix sound.
    *  This is a convenient handle to your sound and is your interface to
    *   using individual sounds in sonix use configure, and remove as you 
    *  would new and delete... 
    *  i.e. you could leak, if you don't call remove - this is because there may be more than one handle to a sound...
    * NOTE: you may have to sonix::instance()->changeAPI() to the api 
    *       you want to use (usually OpenAL, or AudioWorks)
    * @ingroup SonixAPI
    */
   class SoundHandle
   {
   public:
      
      /**  default constructor
       *   @semantics you should call init to name this handle...
       *   @see init
       */
      SoundHandle() : mDefaultName( "unnamed_sound_handle" ), mAlias( mDefaultName )
      {
      }
      
      /**  name constructor.
       *   @semantics gives the handle your chosen name.
       *   this name is what is used in all sonix:: class calls (SoundHandle is a wrapper for sonix::)
       *   @post you do not need to call init if you use the name constructor.
       *   @see init
       */
      SoundHandle( const std::string& myUniqueName ) : mAlias( myUniqueName )
      {
      }
      
      /**  initialize the name of this sound.
       *   @semantics initialize the handle your chosen name.
       *   important: you must configure() your sound after calling init(), if this name hasn't been configured before...
       * 
       * <h3> "Example:" </h3>
       * \code
       *    snx::SoundHandle click_sound;
       *    click_sound.init( "click" );
       *    click_sound.configure( sound_info );
       * \endcode
       * 
       *  @see snx::SoundHandle::configure
       */
      void init( const std::string& myUniqueName )
      {
         mAlias = myUniqueName;
      }
      
      /** get the handle name */
      std::string getName() const { return mAlias; }
      
      //: virtual destructor
      virtual ~SoundHandle()
      {
         mAlias = "deleted";
      }

   public:

      /** trigger a sound.
       * @input number of times to play, -1 to repeat infinately, 1 (single shot) is default.
       * @preconditions object does not have to be a valid sound (but it helps :).
       * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
       * @semantics Triggers a sound.
       */
      virtual void trigger( const int& repeat = 1 )
      {
         sonix::instance()->trigger( mAlias, repeat );
      }
      
      /**
        * is the sound currently playing?
        */
      virtual bool isPlaying()
      {
         return sonix::instance()->isPlaying( mAlias );
      }  

      /* specify whether sound retriggers from beginning when triggered while playing.
       * when sound is already playing then you call trigger,
       * does the sound restart from beginning?
       */
      virtual void setRetriggerable( bool onOff )
      {
         sonix::instance()->setRetriggerable( mAlias, onOff );
      }
      
      /**
       * is the sound retriggerable?
       */
      virtual bool isRetriggerable()
      {
         return sonix::instance()->isRetriggerable( mAlias );
      }

      /** 
       * stop the sound.
       * @semantics stop the sound
       * @input
       */
      virtual void stop()
      {
         sonix::instance()->stop( mAlias );
      }

      /**
       * pause the sound, use unpause to return playback where you left off...
       */
      virtual void pause()
      {
         sonix::instance()->pause( mAlias );
      }

      /**
       * resume playback from a paused state.  does nothing if sound was not paused.
       */
      virtual void unpause()
      {
         sonix::instance()->unpause( mAlias );
      }
      
      /** if the sound is paused, then return true. */
      virtual bool isPaused()
      {
         return sonix::instance()->isPaused( mAlias );
      } 

      /** ambient or positional sound.
       * is the sound ambient - attached to the listener, doesn't change volume
       * when listener moves...
       * or is the sound positional - changes volume as listener nears or retreats..
       */
      virtual void setAmbient( bool setting = false )
      {
         sonix::instance()->setAmbient( mAlias, setting );
      }
      
      /** is the sound ambient? */
      virtual bool isAmbient()
      {
         return sonix::instance()->isAmbient( mAlias );
      }

      /** alters the frequency of the sample.
       *  1 is no change
       *  < 1 is low
       *  > 1 is high.
       */
      virtual void setPitchBend( float amount )
      {
         sonix::instance()->setPitchBend( mAlias, amount );
      }
      
      /** affect volume.  set to a value between [0..1]. */
      virtual void setVolume( float amount )
      {
         sonix::instance()->setVolume( mAlias, amount );
      }

      /** affect cutoff.
       *  set to a value between [0..1]... 1 is no change.  0 is total cutoff.
       */
      virtual void setCutoff( float amount )
      {
         sonix::instance()->setCutoff( mAlias, amount );
      }
      
      /**
       * set sound's 3D position .
       * @input x,y,z are in OpenGL coordinates (right handed sys, x right, y up, z towards you)
       */
      virtual void setPosition( const float& x, const float& y, const float& z )
      {
         sonix::instance()->setPosition( mAlias, x, y, z );
      }

      /**
       * get sound's 3D position.
       * @output x,y,z are returned in OpenGL coordinates.
       */
      virtual void getPosition( float& x, float& y, float& z )
      {
         sonix::instance()->getPosition( mAlias, x, y, z );
      }

      /**
       * set the position of the listener.
       */
      virtual void setListenerPosition( const gmtl::Matrix44f& mat )
      {
         sonix::instance()->setListenerPosition( mat );
      }

      /**
       * get the position of the listener.
       */
      virtual void getListenerPosition( gmtl::Matrix44f& mat )
      {
         sonix::instance()->getListenerPosition( mat );
      }

      /**
        * configure/reconfigure this sound.
        * configure: associate a name (alias) to the description if not already done
        * reconfigure: change properties of the sound to the descriptino provided.
        * @preconditions provide a SoundInfo which describes the sound
        * @postconditions this handle will point to loaded sound data
        * @semantics associate sound data to this handle.  afterwards, this SoundHandle can be used to operate on this sound data.
        */
      virtual void configure( const snx::SoundInfo& description )
      {
         assert( mAlias != mDefaultName && "you must call init to set your sound's name" );
         sonix::instance()->configure( mAlias, description );
      }

      /**
        * remove a configured sound. any future reference to the mAlias will not
        * cause an error, but will not result in a rendered sound
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
