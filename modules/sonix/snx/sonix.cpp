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

#include <snx/snxConfig.h>

#include <stdlib.h>
#include <gmtl/Math.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>

#include <snx/SoundFactory.h>
#include <snx/sonix.h>

//:  destructor
sonix::~sonix()
{
   // release the implementation
   if (mImplementation != NULL)
   {
      // unload all sound data
      mImplementation->unbindAll();
      
      // shutdown old api if exists
      mImplementation->shutdownAPI();
      delete mImplementation;
      mImplementation = NULL;
   }
}

/**
 * @input alias of the sound to trigger, and number of times to play, -1 to 
 *        repeat infinately, 1 (single shot) is default.
 * @preconditions alias does not have to be associated with a loaded sound.
 * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
 * @semantics Triggers a sound
 */
void sonix::trigger( const std::string& alias, const int& repeat )
{
   this->impl().trigger( alias, repeat );
}

/**
  * is the sound currently playing?
  */
bool sonix::isPlaying( const std::string& alias )
{
   return this->impl().isPlaying( alias );
}  

/*
 * when sound is already playing then you call trigger,
 * does the sound restart from beginning?
 */
void sonix::setRetriggerable( const std::string& alias, bool onOff )
{
   this->impl().setRetriggerable( alias, onOff );
}

/**
 * is the sound retriggerable?
 */
bool sonix::isRetriggerable( const std::string& alias )
{
   return this->impl().isRetriggerable( alias );
}

/**
 * @semantics stop the sound
 * @input alias of the sound to be stopped
 */
void sonix::stop( const std::string& alias )
{
   this->impl().stop( alias );
}

/**
 * pause the sound, use unpause to return playback where you left off...
 */
void sonix::pause( const std::string& alias )
{
   this->impl().pause( alias );
}

/**
 * resume playback from a paused state.  does nothing if sound was not paused.
 */
 void sonix::unpause( const std::string& alias )
{
   this->impl().unpause( alias );
}

/** if the sound is paused, then return true. */
 bool sonix::isPaused( const std::string& alias )
{
   return this->impl().isPaused( alias );
} 

/**
 * ambient or positional sound.
 * is the sound ambient - attached to the listener, doesn't change volume
 * when listener moves...
 * or is the sound positional - changes volume as listener nears or retreats..
 */
 void sonix::setAmbient( const std::string& alias, bool setting )
{
   this->impl().setAmbient( alias, setting );
}

/** is the sound ambient? */
 bool sonix::isAmbient( const std::string& alias )
{
   return this->impl().isAmbient( alias );
}

/** alters the frequency of the sample.
 *  1 is no change
 *  < 1 is low
 *  > 1 is high.
 */
 void sonix::setPitchBend( const std::string& alias, float amount )
{
   this->impl().setPitchBend( alias, amount );
}

/** affect volume.  set to a value between [0..1]. */
 void sonix::setVolume( const std::string& alias, float amount )
{
   this->impl().setVolume( alias, amount );
}

/** affect cutoff.
 *  set to a value between [0..1]... 1 is no change.  0 is total cutoff.
 */
 void sonix::setCutoff( const std::string& alias, float amount )
{
   this->impl().setCutoff( alias, amount );
}

/**
 * set sound's 3D position 
 * @input x,y,z are in OpenGL coordinates.  alias is a name that has been associate()d with some sound data
 */
 void sonix::setPosition( const std::string& alias, const float& x, const float& y, const float& z )
{
   this->impl().setPosition( alias, x, y, z );
}

/**
 * get sound's 3D position
 * @input alias is a name that has been associate()d with some sound data
 * @output x,y,z are returned in OpenGL coordinates.
 */
 void sonix::getPosition( const std::string& alias, float& x, float& y, float& z )
{
   this->impl().getPosition( alias, x, y, z );
}

/**
 * set the position of the listener
 */
 void sonix::setListenerPosition( const gmtl::Matrix44f& mat )
{
   this->impl().setListenerPosition( mat );
}

/**
 * get the position of the listener
 */
 void sonix::getListenerPosition( gmtl::Matrix44f& mat )
{
   this->impl().getListenerPosition( mat );
}


/**
 * change the underlying sound API to something else.
 * @input usually a name of a valid registered sound API implementation
 * @preconditions sound implementation should be registered
 * @postconditions underlying API is changed to the requested API name.   if apiName's implementation is not registered, then underlying API is changed to the stub version.
 * @semantics function is safe: always returns a valid implementation.
 * @time O(1)
 * @output a valid sound API.  if apiName is invalid, then a stub implementation is returned.
 */
 void sonix::changeAPI( const std::string& apiName )
{
   snx::ISoundImplementation& oldImpl = this->impl();
   assert( &oldImpl != NULL && "this->impl() should ensure that oldImpl is non-NULL" );
   
   std::cout<<"[snx] NOTIFY: Changing API from "<<oldImpl.name();
   
   // change the current api to the newly requested one.
   snx::SoundFactory::instance()->createImplementation( apiName, mImplementation );

   std::cout<<" to "<<mImplementation->name()<<".\n"<<std::flush;
   
   // copy sound state from old to current (doesn't do binding yet)
   snx::SoundImplementation* si = dynamic_cast<snx::SoundImplementation*>( mImplementation );
   assert( NULL != si && "implementation is not of type SoundImplementation, cast fails" );
   snx::SoundImplementation& old_si = dynamic_cast<snx::SoundImplementation&>( oldImpl );
   assert( NULL != &old_si && "implementation is not of type SoundImplementation, cast fails" );
   si->copy( old_si );

      // unload all sound data
      oldImpl.unbindAll();

      // shutdown old api if exists
      oldImpl.shutdownAPI();

      // delete old api, we're done with it...
      delete &oldImpl;
   
   // startup the new API
   if(!mImplementation->startAPI()) // if it fails to start then we revert back to stub
   {
      std::cout <<"[snx] NOTIFY: startAPI return 0, changing API back to stub" << std::endl;
      changeAPI("stub");
   }

   // load all sound data
   mImplementation->bindAll();
}

/*
 * configure the sound API global settings
 */
 void sonix::configure( const snx::SoundAPIInfo& sai )
{
   this->impl().configure( sai );
}   

/**
  * configure/reconfigure a sound
  * configure: associate a name (alias) to the description if not already done
  * reconfigure: change properties of the sound to the descriptino provided.
  * @preconditions provide an alias and a SoundInfo which describes the sound
  * @postconditions alias will point to loaded sound data
  * @semantics associate an alias to sound data.  later this alias can be used to operate on this sound data.
  */
 void sonix::configure( const std::string& alias, const snx::SoundInfo& description )
{
   this->impl().configure( alias, description );
}   

/**
  * remove a configured sound, any future reference to the alias will not
  * cause an error, but will not result in a rendered sound
  */
 void sonix::remove( const std::string alias )
{
   this->impl().remove( alias );
}   

/**
  * @semantics call once per sound frame (doesn't have to be same as your graphics frame)
  * @input time elapsed since last frame
  */
 void sonix::step( const float& timeElapsed )
{
   this->impl().step( timeElapsed );
}

snx::ISoundImplementation& sonix::impl()
{
   if (mImplementation == NULL)
   {
      snx::SoundFactory::instance()->createImplementation( "stub", mImplementation );
      mImplementation->startAPI();
      mImplementation->bindAll();
   }
   return *mImplementation;
}
