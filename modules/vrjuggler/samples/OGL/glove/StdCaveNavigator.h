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


//#include <Kernel/vjKernel.h>
#include <Math/vjCoord.h>
#include <Kernel/vjDebug.h>

#include <velocityNav.h>

//: This is a "cave style" navigation matrix
//  Provided that you call update once per frame, it should 
//  provide back to you a matrix that can be used to navigate a scene.
//  The matrix is in the OpenGL form, 
//  For performer, convert this matrix, 
//  and copy it to a DCS node once per frame as well.
class vjStdCaveNavigator : public vjMatrix
{
public:
   vjStdCaveNavigator();

   velocityNav* getNavigator()
   { return &mVNav; }
   
   //: call once per frame (pre or post frame, not intra frame)
   virtual int update();
   
   //: tell the navigator to rotate
   //  Rotation is based on the current setMatrix call
   virtual void rotate( const bool& state = true )
   {
      mNowRotating = state;
   }   
   
   //: tell the navigator to accelerate
   //  Acceleration is based on the current setMatrix call
   virtual void accelerate( const bool& state = true )
   {
      mNowTranslating = state;
   }
   
   //: tell the navigator what the pointing device's matrix is.
   //  you can usually do this to get that matrix
   //  vjMatrix* wand_mat = mWand->getData();
   virtual void setMatrix( const vjMatrix& matrix )
   {
      mDeviceMatrix = matrix;
   }   
   
private:
   velocityNav          mVNav;      // My navigator
   //vjKernel* mKern;        // The Kernel

protected:
   vjMatrix mDeviceMatrix;
   bool     mNowTranslating;
   bool     mNowRotating;
};



vjStdCaveNavigator::vjStdCaveNavigator() : vjMatrix(), mNowTranslating( false ), mNowRotating( false )
{
   //mKern = vjKernel::instance();    // Store the kernel
   //mWand.init("VJWand");
   //mButton0.init("VJButton0");
   //mButton1.init("VJButton1");
   //mButton2.init("VJButton2");
   mVNav.setRotAxis(false, true, false);
   mVNav.setMode( velocityNav::FLY );
}


// app() - APP traversal function.  This overloads the standard pfNode
// app() method, which will be called each frame during the APP
// traversal of the scene graph (*only if* needsApp() (below) returns
// TRUE).
// app() is called automatically by Performer; it is not called directly
// by a program.
int vjStdCaveNavigator::update()
{
   //bool nowTranslating = mButton0->getData();
   //bool nowRotating = mButton1->getData();
   //int button2_state = mButton2->getData();

   //vjMatrix* mDeviceMatrix = mWand->getData();

   vjDEBUG(vjDBG_ALL, vjDBG_VERB_LVL) << "b0: " << mNowTranslating
                         << "b1: " << mNowRotating
                         //<< "b2: " << button2_state
                         << "\t" << vjDEBUG_FLUSH;

   mVNav.update( mDeviceMatrix, mNowTranslating, mNowRotating);    // mat, trans, rot

   if(true == mNowTranslating)     // Translate
      mVNav.incTransVelocity();
   else
      mVNav.zeroTransVelocity();       // Set velocity to 0

   // Set the matrix to the navigation matrix
   vjMatrix cur_pos,world_pos;
   cur_pos = mVNav.getCurPos();  // Invert because we want to move the world
   //cerr << "Set Pos: " << vjCoord(cur_pos).pos << endl;
   world_pos.invert( cur_pos );
   this->copy( world_pos );


   return 1;        /* Finish by calling the parent class's app() */
}

#endif
