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

#include <snx/sonix.h>

#include <vrj/vrjConfig.h>
#include <pfSoundNode.h>


pfSoundNode::pfSoundNode(const std::string& sound, bool isPositional)
   : mIsPositional(true)
{
   this->setPositional(isPositional);
   this->setSound(sound);
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
   if (mSound != "" && mIsPositional == true)
   {
      // Get position of this sound in relation to the user's orientation and
      // position.
      // NOTE: The sound will change position if the user rotates without
      //       translation.  It should orbit the user, so that the audio will
      //       pan correctly.
      pfMatrix matrix, traverserMatrix, dcsMatrix;
      matrix.makeIdent();

      // Take the sound from soundspace to userspace.

      // Add in any offset due to this DCS node.
      // (Since the traverser hasn't yet added it to it's matrix stack.)
      // *NOTE: this is the matrix that takes the sound from local(sound)space
      // to modelspace.
      this->getMat(dcsMatrix); //TODO: don't call this.
      // ...to get the location of the sound in modelspace, you'd do an
      // invertFull here, but we want it in user space..
      matrix.postMult(dcsMatrix);

      // Add in any offset due to navigation and any other DCS xforms currently
      // on the traverser's stack.  This is the position of the model's origin
      // in relation to a user at position 0,0,0.
      // This is just the nav matrix, which should be on the matrix stack right
      // now...
      // *NOTE: this is the matrix that takes the sound from modelspace to
      // userspace
      trav->getMat(traverserMatrix);
      // ...to get the location of the sound in modelspace, you'd do an
      // invertFull here, but we want it in user space..
      matrix.postMult(traverserMatrix);

      pfCoord coord;
      matrix.getOrthoCoord(&coord);

      // set my sound's position.
      pfVec3 pf_soundPosition = coord.xyz;
      gmtl::Vec3f soundPosition = vrj::pf::GetVjVec(pf_soundPosition);
      snx::sonix::instance()->setPosition(mSound, soundPosition[0],
                                          soundPosition[1], soundPosition[2]);

      // Engine's update should be called by the app's frame process,
      // or in juggler's manager (not both, of course)...

      // use this for debugging the location of the sound reletive to the user.
      //std::cout<<"["<<mSound->getName()<<"] sound in userspace: "<<soundPosition[0]<<" "<<soundPosition[1]<<" "<<soundPosition[2]<<"\n"<<std::flush;
   }
   else
   {
      // redundant (fixme), but make sure it's 0.0f,0.0f,0.0f
      // this makes the sound the same as the observer.
      snx::sonix::instance()->setPosition(mSound, 0.0f, 0.0f, 0.0f);
   }

   return pfDCS::app(trav);  // call the parent class's app()
}

//---------------------------------------------------------------------//
// Performer type data - this part is required for any class which
// is derived from a Performer class.  It creates a new pfType
// which identifies objects of this class.  All constructors for
// this class must then call setType(classType_).
pfType* pfSoundNode::classType = NULL;

void pfSoundNode::init()
{
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "[pfSoundNode] Registering sound node with performer.\n"
      << vprDEBUG_FLUSH;

   if (classType == NULL)
   {
        pfDCS::init();           // Initialize my parent
        classType =  new pfType(pfDCS::getClassType(), "pfSoundNode");  // Create the new type
   }
}
//----------------------------------------------------------------------//
