/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 */

#ifndef _PERFORMER_JUGGLER_SOUND_NODE
#define _PERFORMER_JUGGLER_SOUND_NODE

#include <Performer/pr/pfLinMath.h>

#include <Math/vjCoord.h>
#include <Kernel/vjDebug.h>
#include <Kernel/Pf/vjPfUtil.h>
#include <Sound/vjSound.h> //juggler sound
#include <Performer/pf/pfDCS.h>
#include <Performer/pf/pfTraverser.h>

//: Performer-Juggler sound node.
//  this node automatically updates the Sound's position information.
//  you should keep a pointer to the Sound, so that you can trigger
//  and change other properties of it.
//  NOTE: This pfSoundNode does not trigger the Sound object, you must do that.
//  NOTE: you still need to use the SoundEngine::update function
class pfSoundNode : public pfDCS
{
public:
   // TODO: the constructor takes a vjSound, which does not
   //       mean that it was aquired with a getHandle function... fixme..
   //  who manages this memory?  should be sound manager... but...
   //  what if it wasn't created by the manager?
   pfSoundNode( vjSound* sound, bool isPositional = true );
   virtual ~pfSoundNode()
   {
      // vjSounds recieved with a sound handle are managed by
      // the sound manager.
      // TODO: the constructor takes a vjSound, which does not
      //       mean that it was aquired with a getHandle function... fixme..
      mSound = NULL;
   }

   float mX, mY, mZ;

   void setObs( float x, float y, float z )
   {
      mX = x; mY = y; mZ = z;
   }

   vjSound& sound()
   {
      vjASSERT( mSound != NULL );
      return *mSound;
   }

   void setSound( vjSound* sound )
   {
      mSound = sound;
   }

   void setPositional( bool isPositional)
   {
      mIsPositional = isPositional;
   }



public:  // APP traversal
   virtual int app( pfTraverser* );
   virtual int needsApp( void )
   {
      return TRUE;
   }


public:  // Required for Performer class
   static void init();
   static pfType* getClassType( void )
   {
      return classType;
   }

private:
   static pfType* classType;
   vjSound*         mSound;
   bool mIsPositional;
};



pfSoundNode::pfSoundNode( vjSound* sound, bool isPositional ) : mIsPositional( true )
{
   this->setPositional( isPositional );
   this->setSound( sound );
   this->setType(classType);  // Set the type
}


// app() - APP traversal function.  This overloads the standard pfDCS
// app() method, which will be called each frame during the APP
// traversal of the scene graph (*only if* needsApp() (below) returns
// TRUE).
// app() is called automatically by Performer; it is not called directly
// by a program.
int pfSoundNode::app(pfTraverser *trav)
{
   // update the sound attributes (position) based on the current
   // position of this node.
   // only do it if the sound is a positional sound.
   if (mSound != NULL && mIsPositional == true)
   {
      // get position of this sound in relation to the user's orientation and position
      // NOTE: the sound will change position if the user rotates without translation.
      //       it should orbit the user, so that the audio will pan correctly.
      pfMatrix matrix, traverserMatrix, dcsMatrix;
      matrix.makeIdent();

      //: take the sound from soundspace to userspace.

      // add in any offset due to this DCS node
      // (since the traverser hasn't yet added it to it's matrix stack.)
      // *NOTE: this is the matrix that takes the sound from local(sound)space to modelspace.
      this->getMat( dcsMatrix ); //TODO: don't call this.
      // ...to get the location of the sound in modelspace, you'd do an invertFull here, but we want it in user space..
      matrix.postMult( dcsMatrix );

      // add in any offset due to navigation and any other DCS xforms currently on the traverser's stack.
      // this is the position of the model's origin in relation to a user at position 0,0,0
      // this is just the nav matrix, which should be on the matrix stack right now...
      // *NOTE: this is the matrix that takes the sound from modelspace to userspace
      trav->getMat( traverserMatrix );
      // ...to get the location of the sound in modelspace, you'd do an invertFull here, but we want it in user space..
      matrix.postMult( traverserMatrix );

      pfCoord coord;
      matrix.getOrthoCoord( &coord );
      pfVec3 pf_soundPosition = coord.xyz;

      // set my sound's position.
      vjVec3 soundPosition = vjGetVjVec( pf_soundPosition );
      mSound->setPosition( soundPosition[0], soundPosition[1], soundPosition[2] );

      // Engine's update should be called by the app's frame process,
      // or in juggler's manager (not both, of course)...

      // use this for debugging the location of the sound reletive to the user.
      //std::cout<<"["<<mSound->getName()<<"] sound in userspace: "<<soundPosition[0]<<" "<<soundPosition[1]<<" "<<soundPosition[2]<<"\n"<<std::flush;
   }
   else
   {
      // redundant (fixme), but make sure it's 0.0f,0.0f,0.0f
      // this makes the sound the same as the observer.
      mSound->setPosition( 0.0f, 0.0f, 0.0f );
      //std::cout<<"["<<mSound->getName()<<"] No position\n"<<std::flush;
   }

   return pfDCS::app(trav);  // call the parent class's app()

}

//---------------------------------------------------------------------//
// Performer type data - this part is required for any class which
// is derived from a Performer class.  It creates a new pfType
// which identifies objects of this class.  All constructors for
// this class must then call setType(classType_).
pfType *pfSoundNode::classType = NULL;

void pfSoundNode::init(void)
{
   vjDEBUG(vjDBG_ALL,1)<<"[pfSoundNode] Registering sound node with performer.\n"<<vjDEBUG_FLUSH;

   if (classType == NULL)
   {
        pfDCS::init();           // Initialize my parent
        classType =  new pfType(pfDCS::getClassType(), "pfSoundNode");  // Create the new type
   }
}
//----------------------------------------------------------------------//

#endif
