/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001, 2002
 *   by Iowa State University
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

#ifndef _PF_NAV_DCS_H_
#define _PF_NAV_DCS_H_

#include <Performer/pf/pfDCS.h>
#include <Performer/pr/pfLinMath.h>
#include <navigator.h>


class pfNavDCS : public pfDCS
{
public:
   pfNavDCS();

   navigator* getNavigator()
   { return mNaver; }

   void setNavigator(navigator* nav)
   {
      mNaver = nav;
      mNaver->setActive(mActive);
   }

   bool isActive() { return mActive;}

   // Set the active state of the nav DCS
   // NOTE: This is done by setting the state of the navigator
   void setActive(bool state = true)
   {
      mActive = state;
      mNaver->setActive(state);
   }

   // Get the current position from the navigator
   // and set the DCS with it
   void updateTransformMatrix();

   // Reset the navigation DCS to the navigators "home" position
   void reset()
   {
      vjASSERT(mNaver != NULL);
      mNaver->reset();
      updateTransformMatrix();
   }

private:
   bool                 mActive;     // Are we active
   navigator*           mNaver;      // My navigator

   vjPosInterface       mWand;
   vjDigitalInterface   mButton0;
   vjDigitalInterface   mButton1;
   vjDigitalInterface   mButton2;

public:  // APP traversal
   virtual int app(pfTraverser*);
   virtual int needsApp(void) {return TRUE;}

   // Required for Performer class
public:
   static void init();
   static pfType* getClassType(void){ return classType;}
private:
   static pfType* classType;
};


#endif
