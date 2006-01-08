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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _PERFORMER_JUGGLER_SOUND_REPLACE_TRAV_H_
#define _PERFORMER_JUGGLER_SOUND_REPLACE_TRAV_H_

#include <string>
#include <Performer/pf.h>
#include <Performer/pfutil.h>


/**
 * This is a collection of Performer traversers...
 *
 * REQUIRED!!! You must call pfSoundTraverser::preForkInit() in your Juggler
 *             app's preForkInit() member function.  Your app is likely to
 *             crash otherwise!!
 */
class pfSoundTraverser
{
public:
   /**
    * This function must be called pfSoundTraverser::preForkInit() in your
    * VR Juggler app's preForkInit() member function.  Your app is likely to
    * crash otherwise!!
    */
   static void preForkInit();

   /**
    * This is a Performer traverser that will replace any node ending in
    * keyName in your scene graph with a pfSoundNode.
    * For example, if mKeyName == _Sound_, and your nodename == Gong_Sound_
    * then that node will be replaced with a new pfSoundNode of a sound
    * called "Gong"
    */
   static void replace(pfNode* node, const std::string& keyName = "_Sound_");

   /**
    * Enables the sound, keeping the state whether it is triggered or stopped.
    */
   static void enable(pfNode* node);

   /**
    * Disables the sound, keeping the state whether it is triggered or stopped.
    * after this, an enable would respect the previous triggered/stopped state.
    */
   static void disable(pfNode* node);

   /** Triggers the sound. */
   static void trigger(pfNode* node);

   /** Stops the sound. */
   static void stop(pfNode* node);

protected:
   /** Triggers every Sound in the graph. */
   static int soundNodesTrigger(pfuTraverser* trav);

   /** Stops every Sound in the graph. */
   static int soundNodesStop(pfuTraverser* trav);

   /** Enables every Sound in the graph. */
   static int soundNodesEnable(pfuTraverser* trav);

   /** Disables every Sound in the graph. */
   static int soundNodesDisable(pfuTraverser* trav);

   /** Used to traverse - don't call. */
   static int nodesReplaceWithSound(pfuTraverser* trav);
};

#endif   /* _PERFORMER_JUGGLER_SOUND_REPLACE_TRAV_H_ */
