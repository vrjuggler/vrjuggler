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

#ifndef _WAND_APP_
#define _WAND_APP_

#include <vjConfig.h>

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <Kernel/GL/vjGlApp.h>
#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>

#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjGloveInterface.h>
#include <Input/InputManager/vjGestureInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>


//----------------------------------------------------
//: Demonstration OpenGL application class
//
// This application draws a glove and some rays and spheres
// on the glove
//----------------------------------------------------
class simpleGloveApp : public vjGlApp
{
public:
   simpleGloveApp(vjKernel* kern)
      : vjGlApp(kern)            // Initialize base class
   {
      mQuadObj = NULL;
   }

   // Execute any initialization needed before the API is started
   // Initialize interfaces
   virtual void init()
   {
      //cout<<"simpleGloveApp::init()"<<flush;
      
      // use this interface for the position of the glove.
      mGlove.init("VJGlove");
      
      // use this interface if you want to use *gestures*
      mGesture.init("VJGesture");
      
      //cout<<" --- done\n"<<flush;
   }

   // Execute any initialization needed after API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {
      //cout<<"simpleGloveApp::apiInit()\n"<<flush; 
   }

   //: Function to draw the scene
   //! PRE: OpenGL state has correct transformation and buffer selected
   //! POST: The current scene has been drawn
   virtual void draw()
   {
      //cout<<"simpleGloveApp::draw()\n"<<flush; 
      
      initGLState();    // This should really be in another function
      myDraw();
   }

   // ----- Drawing Loop Functions ------
   //
   //  The drawing loop will look similar to this:
   //
   //  while (drawing)
   //  {
   //        preFrame();
   //       draw();
   //        intraFrame();      // Drawing is happening while here
   //       sync();
   //        postFrame();      // Drawing is now done
   //
   //       UpdateTrackers();
   //  }
   //------------------------------------

   /// Function called before updating trackers but after the frame is drawn
   virtual void postFrame()
   {;}

   /// Function called after tracker update but before start of drawing
   virtual void preFrame()
   {
     //cout<<"simpleGloveApp::preFrame()\n"<<flush; 
      
       // what gesture is happening??
       int currentGesture = mGesture->getGesture();
       
       // lookup the name of that gesture, and output it.
       cout<<"gestureID["<<currentGesture<<"]:"<<mGesture->getGestureString( currentGesture )<<"\n"<<flush;
   
       if (mGesture->getGesture() == mGesture->getGestureIndex("Open Hand"))
       {
          // do openhand stuff.
          //cout<<"OpenHand\n"<<flush;
       }
       
       if (mGesture->getGesture() == mGesture->getGestureIndex("Closed Fist"))
       {
          // do closed fist stuff.
          //cout<<"Closed Fist\n"<<flush;
       }  
       
       if (mGesture->getGesture() == mGesture->getGestureIndex("Pointing"))
       {
          // do pointing stuff.
          //cout<<"Pointing\n"<<flush;
       }  
       
   }

   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void intraFrame()
   {;}

private:
   //-----------------------------------------------------
   //  Draw the scene.  Rays and spheres on glove fingers
   //------------------------------------------------------

   void myDraw();
   void initGLState();

protected:
   void drawLine(vjVec3& start, vjVec3& end)
   {
      glBegin(GL_LINES);
         glVertex3fv(start.vec);
         glVertex3fv(end.vec);
      glEnd();
   }

   void drawSphere(float radius, int slices, int stacks)
   {
      if (mQuadObj == NULL)
         mQuadObj = gluNewQuadric();

      gluQuadricDrawStyle(mQuadObj, (GLenum) GLU_FILL);
      gluQuadricNormals(mQuadObj, (GLenum) GLU_SMOOTH);
      gluSphere(mQuadObj, radius, slices, stacks);
   }

   GLUquadricObj* mQuadObj;

public:
      
   vjGestureInterface  mGesture;
   vjGloveInterface    mGlove;      // the glove
};


#endif
