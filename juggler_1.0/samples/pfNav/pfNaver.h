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

#ifndef _PF_NAVER_H_
#define _PF_NAVER_H_

#include <Performer/pf/pfDCS.h>
#include <Performer/pr/pfLinMath.h>

#include <Kernel/vjKernel.h>
#include <Math/vjCoord.h>
#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>
#include <Kernel/vjDebug.h>
#include <Kernel/Pf/vjPfUtil.h>

#include <velocityNav.h>

class pfNaver : public pfDCS
{
public:
   pfNaver();

   velocityNav* getNavigator()
   { return &mVNav; }

private:
   velocityNav          mVNav;      // My navigator

   vjPosInterface       mWand;
   vjDigitalInterface   mButton0;
   vjDigitalInterface   mButton1;
   vjDigitalInterface   mButton2;

   vjKernel* mKern;        // The Kernel


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



pfNaver::pfNaver() : pfDCS()
{
   setType(classType);  // Set the type
   mKern = vjKernel::instance();    // Store the kernel
   mWand.init("VJWand");
   mButton0.init("VJButton0");
   mButton1.init("VJButton1");
   mButton2.init("VJButton2");
   mVNav.setRotAxis(false, true, false);
}


// app() - APP traversal function.  This overloads the standard pfNode
// app() method, which will be called each frame during the APP
// traversal of the scene graph (*only if* needsApp() (below) returns
// TRUE).
// app() is called automatically by Performer; it is not called directly
// by a program.
int pfNaver::app(pfTraverser *trav)
{
   int button0_state = mButton0->getData();
   int button1_state = mButton1->getData();
   int button2_state = mButton2->getData();

   vjMatrix* wand_mat = mWand->getData();

   vjDEBUG(vjDBG_ALL, vjDBG_VERB_LVL) << "b0: " << button0_state
                         << "b1: " << button1_state
                         << "b2: " << button2_state
                         << "\t" << vjDEBUG_FLUSH;

   mVNav.update(*wand_mat, button0_state, button1_state);    // mat, trans, rot

   if(1 == button0_state)     // Translate
      mVNav.incTransVelocity();
   else
      mVNav.zeroTransVelocity();       // Set velocity to 0

   // Set the DCS to the navigation matrix
   vjMatrix cur_pos,world_pos;
   cur_pos = mVNav.getCurPos();  // Invert because we want to move the world
   //cerr << "Set Pos: " << vjCoord(cur_pos).pos << endl;
   world_pos.invert(cur_pos);
   pfMatrix pf_world_pos;
   pf_world_pos = vjGetPfMatrix(world_pos);
   setMat(pf_world_pos);


   return pfDCS::app(trav);        /* Finish by calling the parent class's app() */
}



//---------------------------------------------------------------------//
// Performer type data - this part is required for any class which
// is derived from a Performer class.  It creates a new pfType
// which identifies objects of this class.  All constructors for
// this class must then call setType(classType_).
pfType *pfNaver::classType = NULL;

void pfNaver::init(void)
{
 if (classType == NULL)
   {
        pfDCS::init();           // Initialize my parent
        classType =  new pfType(pfDCS::getClassType(), "pfNaver");  // Create the new type
   }
}
//----------------------------------------------------------------------//

#endif
