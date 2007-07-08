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

#ifndef _PF_NAV_DCS_H_
#define _PF_NAV_DCS_H_

#include <Performer/pf/pfDCS.h>
#include <Performer/pr/pfLinMath.h>

#include <gmtl/Coord.h>

#include <vrj/Kernel/Kernel.h>
#include <vrj/Util/Debug.h>
#include <vrj/Draw/Pf/Util.h>
#include <gadget/Type/PositionInterface.h>

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
      vprASSERT(mNaver != NULL);
      mNaver->reset();
      updateTransformMatrix();
   }

private:
   bool                 mActive;     // Are we active
   navigator*           mNaver;      // My navigator

   gadget::PositionInterface  mWand;
   gadget::DigitalInterface   mButton0;
   gadget::DigitalInterface   mButton1;
   gadget::DigitalInterface   mButton2;

public:  // APP traversal
   virtual int app(pfTraverser*);
   virtual int needsApp(void) {return TRUE;}

   // Required for Performer class
public:
   static void init();
   static pfType* getClassType(void){ return classType();}
private:
   // Performer type data - this part is required for any class which
   // is derived from a Performer class.  It creates a new pfType
   // which identifies objects of this class.  All constructors for
   // this class must then call this->setType( classType() ).
   static pfType* &classType() { static pfType* ct = NULL; return ct; }
};



inline pfNavDCS::pfNavDCS() : pfDCS()
{
   assert( pfNavDCS::classType() != NULL && "you must call pfNavDCS::init() before performer inits" );
   setType( pfNavDCS::classType() );  // Set the type
   mActive = true;
   mNaver = NULL;
}


// Get the current position from the navigator
// and set the DCS with it
inline void pfNavDCS::updateTransformMatrix()
{
   vprASSERT( mNaver != NULL );     // We mut have a naver

   // Set the navigation DCS to the new navigation matrix
   // cur_pos = modelspace_M_user
   gmtl::Matrix44f cur_pos_inv, cur_pos;
   cur_pos = mNaver->getCurPos();
   gmtl::invert( cur_pos_inv, cur_pos );
   pfMatrix model_move = vrj::pf::GetPfMatrix(cur_pos_inv);
   this->setMat( model_move );
}

// app() - APP traversal function.  This overloads the standard pfNode
// app() method, which will be called each frame during the APP
// traversal of the scene graph (*only if* needsApp() (below) returns
// TRUE).
// app() is called automatically by Performer; it is not called directly
// by a program.
inline int pfNavDCS::app(pfTraverser *trav)
{
   if(mNaver != NULL)
   {
      if(isActive())
      {
         // Get input and update the state baed upon that input
         mNaver->updateInteraction();

         // Update the state of the navigation
         mNaver->update();

         updateTransformMatrix();
      }
   }
   else
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "pfNavDCS::app: No naver set\n" << vprDEBUG_FLUSH;
   }

   return pfDCS::app(trav);        /* Finish by calling the parent class's app() */
}

inline void pfNavDCS::init(void)
{
   if (pfNavDCS::classType() == NULL)
   {
        pfDCS::init();           // Initialize my parent
        pfNavDCS::classType() =  new pfType(pfDCS::getClassType(), "pfNavDCS");  // Create the new type
   }
}
//----------------------------------------------------------------------//

#endif
