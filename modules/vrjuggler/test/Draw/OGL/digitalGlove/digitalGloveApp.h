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

#include <vrj/vrjConfig.h>

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vrj/Draw/OGL/GlApp.h>
#include <vrj/Math/Vec3.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/GloveInterface.h>
#include <gadget/Type/DigitalInterface.h>


//----------------------------------------------------
//: Demonstration OpenGL application class
//
// This application draws a glove and some rays and spheres
// on the glove
//----------------------------------------------------
class digitalGloveApp : public vrj::GlApp
{
public:
   // Constructor.
   digitalGloveApp(vrj::Kernel* kern)
      : vrj::GlApp(kern)            // Initialize base class
   {
      mQuadObj = NULL;
   }

   // Destructor.
   virtual ~digitalGloveApp (void) {
      if ( mQuadObj != NULL ) {
         gluDeleteQuadric(mQuadObj);
         mQuadObj = NULL;
      }
   }

   // Execute any initialization needed before the API is started.
   // Initialize VR Juggler device interfaces here.
   virtual void init();

   // Execute any initialization needed after API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {
      //std::cout<<"digitalGloveApp::apiInit()\n"<<std::flush;
   }

   // Function called after tracker update but before start of drawing.  Do
   // calculations and state modifications here.
   virtual void preFrame();

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

   //: Function to draw the scene.  Put OpenGL draw functions here.
   //
   //! PRE: OpenGL state has correct transformation and buffer selected
   //! POST: The current scene has been drawn
   virtual void draw()
   {
      //std::cout<<"digitalGloveApp::draw()\n"<<std::flush;

      initGLState();    // This should really be in another function
      myDraw();
   }

   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void intraFrame()
   {;}

   // Function called before updating trackers but after the frame is drawn.
   // Do calculations here.
   virtual void postFrame()
   {;}

private:
   //-----------------------------------------------------
   //  Draw the scene.  Rays and spheres on glove fingers
   //------------------------------------------------------

   void myDraw();
   void initGLState();

protected:
   void drawLine(vrj::Vec3& start, vrj::Vec3& end)
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
   gadget::GloveInterface    mGlove;      // the glove

   // for the glove fingers
   gadget::DigitalInterface  mLeftThumb;
   gadget::DigitalInterface  mLeftIndex;
   gadget::DigitalInterface  mLeftMiddle;
   gadget::DigitalInterface  mLeftRing;
   gadget::DigitalInterface  mLeftPinky;
   gadget::DigitalInterface  mRightThumb;
   gadget::DigitalInterface  mRightIndex;
   gadget::DigitalInterface  mRightMiddle;
   gadget::DigitalInterface  mRightRing;
   gadget::DigitalInterface  mRightPinky;
};


#endif
