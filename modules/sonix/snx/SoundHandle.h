
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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
#include <snx/sonix.h>

namespace snx
{
   /** A proxy (handle) to a sonix sound 
    *  this is a convenient handle to your sound and is your interface to
    *   using individual sounds in sonix
    *  use configure, and remove as you would new and delete... 
    *  i.e. you could leak, if you don't call remove - this is because there may be more than one handle to a sound...
    * NOTE: you may have to sonix::instance()->changeAPI() to the api 
    *       you want to use (usually OpenAL, or AudioWorks)
    */
   class SoundHandle
   {
   public:
      
      /**  default constructor
       *   @semantics you should call init to name this handle...
       */
      SoundHandle() : mAlias( "unnamed" )
      {
      }
      
      /**  constructor
       *   @semantics gives the handle your chosen name.
       *   this name is what is used in all sonix:: class calls (SoundHandle is a wrapper for sonix::)
       */
      SoundHandle( const std::string& myUniqueName ) : mAlias( myUniqueName )
      {
      }

      /**  init
       *   @semantics initialize the handle your chosen name.
       *   this name is what is used in all sonix:: class calls (SoundHandle is a wrapper for sonix::)
       * important: you must configure() your sound, if this name hasn't been configured before...
       */
      void init( const std::string& myUniqueName )
      {
         mAlias = myUniqueName;
      }
      
      //: virtual destructor
      virtual ~SoundHandle()
      {
         
      }

   public:

      /**
       * @input number of times to play
       * @preconditions object does not have to be a valid sound (but it helps :)
       * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
       * @semantics Triggers a sound
       */
      virtual void trigger( const unsigned int& repeat = 1 )
      {
         sonix::instance()->trigger( mAlias, repeat );
      }

      /*
       * when sound is already playing then you call trigger,
       * does the sound restart from beginning?
       */
      virtual void setRetriggerable( bool onOff )
      {
         sonix::instance()->setRetriggerable( mAlias, onOff );
      }

      /**
       * ambient or positional sound.
       * is the sound ambient - attached to the listener, doesn't change volume
       * when listener moves...
       * or is the sound positional - changes volume as listener nears or retreats..
       */
      virtual void setAmbient( bool setting = false )
      {
         sonix::instance()->setAmbient( mAlias, setting );
      }

      /**
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

      /**
       * mute, sound continues to play, but you can't hear it...
       */
      virtual void mute()
      {
         sonix::instance()->mute( mAlias );
      }

      /**
       * unmute, let the muted-playing sound be heard again
       */
      virtual void unmute()
      {
         sonix::instance()->unmute( mAlias );
      }

      /**
       * set sound's 3D position 
       * @input x,y,z are in OpenGL coordinates (right handed sys, x right, y up, z towards you)
       */
      virtual void setPosition( const float& x, const float& y, const float& z )
      {
         sonix::instance()->setPosition( mAlias, x, y, z );
      }

      /**
       * get sound's 3D position
       * @output x,y,z are returned in OpenGL coordinates.
       */
      virtual void getPosition( float& x, float& y, float& z )
      {
         sonix::instance()->getPosition( mAlias, x, y, z );
      }

      /**
       * set the position of the listener
       */
      virtual void setListenerPosition( const vrj::Matrix& mat )
      {
         sonix::instance()->setListenerPosition( mat );
      }

      /**
       * get the position of the listener
       */
      virtual void getListenerPosition( vrj::Matrix& mat )
      {
         sonix::instance()->getListenerPosition( mat );
      }

      /**
        * configure/reconfigure this sound
        * configure: associate a name (alias) to the description if not already done
        * reconfigure: change properties of the sound to the descriptino provided.
        * @preconditions provide a SoundInfo which describes the sound
        * @postconditions this handle will point to loaded sound data
        * @semantics associate sound data to this handle.  afterwards, this SoundHandle can be used to operate on this sound data.
        */
      virtual void configure( const snx::SoundInfo& description )
      {
         sonix::instance()->configure( mAlias, description );
      }

      /**
        * remove a configured sound, any future reference to the mAlias will not
        * cause an error, but will not result in a rendered sound
        */
      virtual void remove()
      {
         sonix::instance()->remove( mAlias );
      }   

      /**
        * @semantics call once per sound frame (doesn't have to be same as your graphics frame)
        * @input time elapsed since last frame
        */
      virtual void step( const float& timeElapsed )
      {
         sonix::instance()->step( timeElapsed );
      }
      
   private:
      std::string mAlias;
   };
}

#endif
