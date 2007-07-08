/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _PERFORMER_JUGGLER_SOUND_NODE
#define _PERFORMER_JUGGLER_SOUND_NODE

#include <vrj/vrjConfig.h>

#include <Performer/pr/pfLinMath.h>

#include <vrj/Util/Debug.h>
#include <vrj/Draw/Pf/Util.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pf/pfTraverser.h>

#include <snx/sonix.h>

/**
 * Performer-Juggler sound node.
 * This node automatically updates the Sound's position information.
 * You should keep a pointer to the Sound, so that you can trigger
 * and change other properties of it.
 *
 * @note This pfSoundNode does not trigger the Sound object, you must do that.
 */
class pfSoundNode : public pfDCS
{
public:
   /**
    * @todo the constructor takes a Sound, which does not
    *       mean that it was aquired with a getHandle function... fixme..
    *  who manages this memory?  should be sound manager... but...
    *  what if it wasn't created by the manager?
    */
   pfSoundNode(const std::string& soundHandle, bool isPositional = true);

   virtual ~pfSoundNode()
   {
   }

   pfSoundNode& operator=(const pfSoundNode& rhs)
   {
      pfDCS::operator=(rhs);
      mX = rhs.mX;
      mY = rhs.mY;
      mZ = rhs.mZ;
      mSound = rhs.mSound;
      mIsPositional = rhs.mIsPositional;

      return *this;
   }

   /** Sets the listener. */
   void setObs(const float x, const float y, const float z)
   {
      mX = x;
      mY = y;
      mZ = z;
   }

   const std::string& sound() const
   {
      return mSound;
   }

   void setSound(const std::string& sound)
   {
      mSound = sound;
   }

   void setPositional(const bool isPositional)
   {
      mIsPositional = isPositional;
   }



public:  // APP traversal
   virtual int app(pfTraverser*);

   virtual int needsApp()
   {
      return TRUE;
   }

protected:
   float mX, mY, mZ;
   std::string mSound;
   bool        mIsPositional;


public:  // Required for Performer class
   static void init();

   static pfType* getClassType()
   {
      return classType;
   }

private:
   static pfType* classType;
};


#endif
