/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#ifndef _DIGITAL_GLOVE_APP_
#define _DIGITAL_GLOVE_APP_

#include <vjConfig.h>

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <Kernel/GL/vjGlApp.h>
#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>

#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjGloveInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>


//----------------------------------------------------
//: Demonstration OpenGL application class
//
// This application draws a glove and some rays and spheres
// on the glove
//----------------------------------------------------
class digitalGloveApp : public vjGlApp
{
public:
   digitalGloveApp(vjKernel* kern)
      : vjGlApp(kern)            // Initialize base class
   {
      mQuadObj = NULL;
   }

   virtual ~digitalGloveApp (void) {
      if ( mQuadObj != NULL ) {
         gluDeleteQuadric(mQuadObj);
         mQuadObj = NULL;
      }
   }

   // Execute any initialization needed before the API is started
   // Initialize interfaces
   virtual void init()
   {
      //std::cout<<"digitalGloveApp::init()"<<std::flush;
      
      mGlove.init("VJGlove");
      
      // for the digital glove fingers.
      mLeftThumb.init("LeftThumb");
      mLeftIndex.init("LeftIndex");
      mLeftMiddle.init("LeftMiddle");
      mLeftRing.init("LeftRing");
      mLeftPinky.init("LeftPinky");
      mRightThumb.init("RightThumb");
      mRightIndex.init("RightIndex");
      mRightMiddle.init("RightMiddle");
      mRightRing.init("RightRing");
      mRightPinky.init("RightPinky");
      
      //std::cout<<" --- done\n"<<std::flush;
   }

   // Execute any initialization needed after API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {
      //std::cout<<"digitalGloveApp::apiInit()\n"<<std::flush; 
   }

   //: Function to draw the scene
   //! PRE: OpenGL state has correct transformation and buffer selected
   //! POST: The current scene has been drawn
   virtual void draw()
   {
      //std::cout<<"digitalGloveApp::draw()\n"<<std::flush; 
      
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
     //std::cout<<"digitalGloveApp::preFrame()\n"<<std::flush; 
      
       std::cout<<mLeftThumb->getData()
                <<mLeftIndex->getData()
                <<mLeftMiddle->getData()
                <<mLeftRing->getData()
                <<mLeftPinky->getData()<<"."
                <<mRightThumb->getData()
                <<mRightIndex->getData()
                <<mRightMiddle->getData()
                <<mRightRing->getData()
                <<mRightPinky->getData()<<"\n"<<std::flush;
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
   vjGloveInterface    mGlove;      // the glove

   // for the glove fingers
   vjDigitalInterface  mLeftThumb;
   vjDigitalInterface  mLeftIndex;
   vjDigitalInterface  mLeftMiddle;
   vjDigitalInterface  mLeftRing;
   vjDigitalInterface  mLeftPinky;
   vjDigitalInterface  mRightThumb;
   vjDigitalInterface  mRightIndex;
   vjDigitalInterface  mRightMiddle;
   vjDigitalInterface  mRightRing;
   vjDigitalInterface  mRightPinky;
};


#endif
