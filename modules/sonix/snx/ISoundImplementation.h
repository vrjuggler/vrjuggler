
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

#ifndef SNXSOUNDIMPLEMENTATION_INTERFACE_H
#define SNXSOUNDIMPLEMENTATION_INTERFACE_H
#include <string>
#include <map>
#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>

#include "snx/SoundInfo.h"
#include "snx/SoundAPIInfo.h"

namespace snx
{

class ISoundImplementation
{
public:
   /** default constructor 
    */
   ISoundImplementation() {}

   /**
     * every implementation can return a new copy of itself
     */
   virtual void clone( ISoundImplementation* &newCopy ) = 0; 
   
   /**
    * @semantics destructor 
    */
   virtual ~ISoundImplementation() {}

public:

   /** trigger a sound.
    * @input alias of the sound to trigger, and number of times to play, -1 is repeat infinately
    * @preconditions alias does not have to be associated with a loaded sound.
    * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
    * @semantics Triggers a sound
    */
   virtual void trigger( const std::string & alias, const int & repeat = 1 ) = 0;

   /**
     * is the sound currently playing?
     */
   virtual bool isPlaying( const std::string& alias ) = 0;
   
   /**
    * when sound is already playing then you call trigger,
    * does the sound restart from beginning?
    * (if a tree falls and no one is around to hear it, does it make sound?)
    */
   virtual void setRetriggerable( const std::string& alias, bool onOff ) = 0;
   
   /**
    * is the sound retriggerable?
    */
   virtual bool isRetriggerable( const std::string& alias ) = 0;

   /**
    * @semantics stop the sound
    * @input alias of the sound to be stopped
    */
   virtual void stop( const std::string& alias ) = 0;

   /**
    * pause the sound, use unpause to return playback where you left off...
    */
   virtual void pause( const std::string& alias ) = 0;

   /**
    * resume playback from a paused state.  does nothing if sound was not paused.
    */
   virtual void unpause( const std::string& alias ) = 0;

   /** if the sound is paused, then return true. */
   virtual bool isPaused( const std::string& alias ) = 0;
   
   /**
    * ambient or positional sound.
    * is the sound ambient - attached to the listener, doesn't change volume
    * when listener moves...
    * or is the sound positional - changes volume as listener nears or retreats..
    */
   virtual void setAmbient( const std::string& alias, bool ambient = false ) = 0;
   
   /**
    * is the sound ambient - attached to the listener, doesn't change volume
    * when listener moves...
    */
   virtual bool isAmbient( const std::string& alias ) = 0;

   /** bend the pitch of the sound. */
   virtual void setPitchBend( const std::string& alias, float amount ) = 0;
   
   /** affect volume.  set to a value between [0..1]. */
   virtual void setVolume( const std::string& alias, float amount ) = 0;
   
   /** affect cutoff. 
    * set to a value between [0..1]... 1 is no change.  0 is total cutoff. 
    */
   virtual void setCutoff( const std::string& alias, float amount ) = 0;
   
   /**
    * set sound's 3D position 
    */
   virtual void setPosition( const std::string& alias, float x, float y, float z ) = 0;

   /**
    * get sound's 3D position
    * @input alias is a name that has been associate()d with some sound data
    * @output x,y,z are returned in OpenGL coordinates.
    */
   virtual void getPosition( const std::string& alias, float& x, float& y, float& z ) = 0;

   /**
    * set the position of the listener
    */
   virtual void setListenerPosition( const gmtl::Matrix44f& mat ) = 0;

   /**
    * get the position of the listener
    */
   virtual void getListenerPosition( gmtl::Matrix44f& mat ) = 0;

   /**
    * start the sound API, creating any contexts or other configurations at startup
    * @postconditions sound API is ready to go.
    * @semantics this function should be called before using the other functions in the class.
    * @return values: return 1 if successful, 0 otherwise
    */
   virtual int startAPI() = 0;

   /**
    * query whether the API has been started or not
    * @semantics return true if api has been started, false otherwise.
    */
   virtual bool isStarted() const = 0;   
   
   /**
    * kill the sound API, deallocating any sounds, etc...
    * @postconditions sound API is ready to go.
    * @semantics this function could be called any time, the function could be called multiple times, so it should be smart.
    */
   virtual void shutdownAPI() = 0;

   /*
    * configure/reconfigure the sound API global settings
    */
   virtual void configure( const snx::SoundAPIInfo& sai ) = 0;

   /**
     * configure/reconfigure a sound
     * configure: associate a name (alias) to the description if not already done
     * reconfigure: change properties of the sound to the descriptino provided.
     * @preconditions provide an alias and a SoundInfo which describes the sound
     * @postconditions alias will point to loaded sound data
     * @semantics associate an alias to sound data.  later this alias can be used to operate on this sound data.
     */
   virtual void configure( const std::string& alias, const snx::SoundInfo& description ) = 0;

   /**
    * remove a configured sound, any future reference to the alias will not
    * cause an error, but will not result in a rendered sound
    */
   virtual void remove(const std::string& alias) = 0;

   /**
    * @semantics call once per sound frame (doesn't have to be same as your graphics frame)
    * @input time elapsed since last frame
    */
   virtual void step( const float& timeElapsed ) = 0;

   /**
    * clear all associate()tions.
    * @semantics any existing aliases will be stubbed. sounds will be unbound
    */
   virtual void clear() = 0;

   /**
    * bind: load (or reload) all associate()d sounds
    * @postconditions all sound associations are buffered by the sound API
    */
   virtual void bindAll() = 0;

   /**
    * unbind: unload/deallocate all associate()d sounds.
    * @postconditions all sound associations are unbuffered by the sound API
    */
   virtual void unbindAll() = 0;

   /**
    * load/allocate the sound data this alias refers to the sound API
    * @postconditions the sound API has the sound buffered.
    */
   virtual void bind( const std::string& alias ) = 0;

   /**
    * unload/deallocate the sound data this alias refers from the sound API
    * @postconditions the sound API no longer has the sound buffered.
    */
   virtual void unbind( const std::string& alias ) = 0;

   virtual snx::SoundInfo& lookup( const std::string& alias ) = 0;

   virtual void setName( const std::string& name ) = 0;
   
   virtual std::string& name() = 0;
};

}; // end namespace

#endif //SNXSOUNDIMPLEMENTATION_INTERFACE_H
