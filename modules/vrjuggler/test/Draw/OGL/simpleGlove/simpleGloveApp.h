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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _SIMPLE_GLOVE_APP_
#define _SIMPLE_GLOVE_APP_

#include <vrj/vrjConfig.h>

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <gmtl/Point.h>
#include <vrj/Draw/OGL/GlApp.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/GloveInterface.h>
#include <gadget/Type/GestureInterface.h>


//----------------------------------------------------
//: Demonstration OpenGL application class
//
// This application draws a glove and some rays and spheres
// on the glove
//----------------------------------------------------
class simpleGloveApp : public vrj::GlApp
{
public:
   // Constructor.
   simpleGloveApp(vrj::Kernel* kern)
      : vrj::GlApp(kern)            // Initialize base class
   {
      mQuadObj = NULL;
   }

   // Destructor.
   virtual ~simpleGloveApp (void) {
      if ( mQuadObj != NULL ) {
         gluDeleteQuadric(mQuadObj);
         mQuadObj = NULL;
      }
   }

   // Execute any initialization needed before the API is started
   // Initialize VR Juggler device interfaces here.
   virtual void init()
   {
      //std::cout<<"simpleGloveApp::init()"<<std::flush;

      // use this interface for the position of the glove.
      mGlove.init("VJGlove");

      // use this interface if you want to use *gestures*
      mGesture.init("VJGesture");

      //std::cout<<" --- done\n"<<std::flush;
   }

   // Execute any initialization needed after API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {
      //std::cout<<"simpleGloveApp::apiInit()\n"<<std::flush;
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

   // Function called after tracker update but before start of drawing.  Do
   // calculations and state modifications here.
   virtual void preFrame();

   virtual void bufferPreDraw();

   //: Function to draw the scene.  Put OpenGL draw functions here.
   //
   //! PRE: OpenGL state has correct transformation and buffer selected
   //! POST: The current scene has been drawn
   virtual void draw()
   {
      //std::cout<<"simpleGloveApp::draw()\n"<<std::flush;

      initGLState();    // This should really be in another function
      myDraw();
   }

   // Function called after drawing has been triggered but BEFORE it completes
   virtual void intraFrame()
   {;}

   // Function called before updating trackers but after the frame is drawn
   virtual void postFrame()
   {;}

private:
   //-----------------------------------------------------
   //  Draw the scene.  Rays and spheres on glove fingers
   //------------------------------------------------------

   void myDraw();
   void initGLState();

protected:
   void drawLine(gmtl::Point3f& start, gmtl::Point3f& end)
   {
      glBegin(GL_LINES);
         glVertex3fv(start.mData);
         glVertex3fv(end.mData);
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

   gadget::GestureInterface  mGesture;
   gadget::GloveInterface    mGlove;      // the glove
};


#endif
