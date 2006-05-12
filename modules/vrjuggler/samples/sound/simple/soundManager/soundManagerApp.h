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

#ifndef _SOUND_MANAGER_APP_
#define _SOUND_MANAGER_APP_

#include <vrj/vrjConfig.h>

#include <iostream>
#include <iomanip>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vrj/Draw/OGL/GlApp.h>

#include <snx/SoundHandle.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>

namespace vrjTest
{

// Utility function to draw a cube
void drawbox( GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
                GLdouble z0, GLdouble z1, GLenum type );

//----------------------------------------------------
//: Demonstration OpenGL application class
//
// This application simply renders a box on the wand
//----------------------------------------------------
class soundManagerApp : public vrj::GlApp
{
public:
   // Constructor.
   soundManagerApp(vrj::Kernel* kern)
      : vrj::GlApp(kern)         // Initialize base class
   {;}

   // Destructor.
   virtual ~soundManagerApp (void) {
      /* Do nothing. */ ;
   }

   // Execute any initialization needed before the API is started<BR><BR>
   // Initialize VR Juggler device interfaces here.
   //
   // This is called once before OGL is initialized
   virtual void init()
   {
      //std::cout << "---------- App:init() ---------------" << std::endl;
      // Initialize devices
      mWand.init("VJWand");
      mHead.init("VJHead");
      mButton0.init("VJButton0");
      mButton1.init("VJButton1");
      mButton2.init("VJButton2");
      mButton3.init("VJButton3");
      mButton4.init("VJButton4");
      mButton5.init("VJButton5");

      mSound.init("bump");
   }

   // Execute any initialization needed after API is started
   //  but before the drawManager starts the drawing loops.<BR><BR>
   //
   // This is called once after OGL is initialized
   virtual void apiInit()
   {;}

   // Function that is called immediately after a new OGL context is created.
   // This is called for every display window that is opened.  Initialize GL
   // state here. Also used to create context specific information<BR>
   //
   // This is called once for each display (twice for each stereo display)
   virtual void contextInit()
   {
      // Initialize the GL state information. (lights, shading, etc)
      initGLState();
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

   // Function called after tracker update but before start of drawing.
   // Called once before every frame.  Do calculations and state modifications
   // here.
   virtual void preFrame();

   virtual void bufferPreDraw();

   // Function to draw the scene.  Put OpenGL draw functions here.
   //
   // PRE: OpenGL state has correct transformation and buffer selected
   // POST: The current scene has been drawn<BR><BR>
   //
   // called [once * number of displays] per frame
   virtual void draw()
   {
      //call your openGL drawing code here.
      myDraw();
   }

   // Function called after drawing has been triggered but BEFORE it
   // completes<BR><BR>
   //
   // called once during each frame
   virtual void intraFrame()
   {
      // Put your intra frame computations here.
   }

   // Function called before updating trackers but after the frame is
   // drawn<BR><BR>
   //
   // Called once after every frame.  Do calculations here.
   virtual void postFrame()
   {
      // Put your post frame computations here.
   }

private:
   //------------------------------------------------
   // Draw the scene.  A box on the end of the wand.
   //------------------------------------------------
   void myDraw();
   void initGLState();

   void drawCube()
   {
      drawbox(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5, GL_QUADS);
   }

public:
   gadget::PositionInterface    mWand;      // the Wand
   gadget::PositionInterface    mHead;      // the head
   gadget::DigitalInterface     mButton0;
   gadget::DigitalInterface     mButton1;
   gadget::DigitalInterface     mButton2;
   gadget::DigitalInterface     mButton3;
   gadget::DigitalInterface     mButton4;
   gadget::DigitalInterface     mButton5;

   snx::SoundHandle mSound;
};

}


#endif
