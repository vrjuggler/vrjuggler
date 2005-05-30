/*********** <VRJ VNC auto-copyright.pl BEGIN do not edit this line> **********
 *
 * VRJ VNC is (C) Copyright 2003-2005 by Iowa State University
 *
 * Original Authors:
 *   Patrick Hartling, Allen Bierbaum
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this application; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ************ <VRJ VNC auto-copyright.pl END do not edit this line> **********/

#ifndef _VNCAPP_APP_
#define _VNCAPP_APP_

#include <vrj/vrjConfig.h>

#include <iostream>
#include <iomanip>
#include <GL/gl.h>

#include <vpr/Util/StatCollector.h>
#include <vrj/Draw/OGL/GlApp.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>

#include <gadget/InputManager.h>
#include <gadget/InputLogger.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/Coord.h>
#include <gmtl/Generate.h>


namespace vrjvnc
{

class VNCDesktop;

// Utility function to draw a cube
void drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
             GLdouble z0, GLdouble z1, GLenum type);

/**
 * This application demonstrates how to use VNC within a VR Juggler
 * application object to embed any 2D GUI in the 3D space.
 */
class vncApp : public vrj::GlApp
{
public:
   // Constructor.
   vncApp(vrj::Kernel* kern)
      : vrj::GlApp(kern)
      , mFrameRate(vpr::Interval(1, vpr::Interval::Sec))
      , mEmbeddedGUI(false)
   {
      ;
   }

   // Destructor.
   virtual ~vncApp()
   {
      /* Do nothing. */ ;
   }

   bool configCanHandle(jccl::ConfigElementPtr element);

   bool configAdd(jccl::ConfigElementPtr element);

   // Execute any initialization needed before the API is started<BR><BR>
   // Initialize VR Juggler device interfaces here.
   //
   // This is called once before OGL is initialized
   virtual void init()
   {
      vrj::GlApp::init();

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

      mLoggerPlayButton.init("LoggerPlayButton");

      mPrevFrameStartTime.setNow();
   }

   // Execute any initialization needed after API is started
   //  but before the drawManager starts the drawing loops.<BR><BR>
   //
   // This is called once after OGL is initialized
   virtual void apiInit()
   {
      /* Do nothing. */ ;
   }

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

   // Function called after tracker update but before start of drawing<BR><BR>
   // Called once before every frame.  Do calculations and state modifications
   // here.
   virtual void preFrame();

   virtual void bufferPreDraw();

   virtual void contextPreDraw();

   // Function to draw the scene.  Put OpenGL draw functions here.
   //
   // PRE: OpenGL state has correct transformation and buffer selected
   // POST: The current scene has been drawn<BR><BR>
   //
   // called [once * number of displays] per frame
   virtual void draw();

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
   void initGLState();

   void drawCube()
   {
      drawbox(-0.15, 0.15, -0.15, 0.15, -0.15, 0.15, GL_QUADS);
   }

   void drawFloor();

public:
   gadget::PositionInterface    mWand;      // the Wand
   gadget::PositionInterface    mHead;      // the head
   gadget::DigitalInterface     mButton0;
   gadget::DigitalInterface     mButton1;
   gadget::DigitalInterface     mButton2;
   gadget::DigitalInterface     mButton3;
   gadget::DigitalInterface     mButton4;
   gadget::DigitalInterface     mButton5;

   gadget::DigitalInterface     mLoggerPlayButton;   // Playback log file when pressed

   gmtl::Matrix44f              mNavMat;              // Navigation matrix. world_M_vworld

   vpr::Uint64                      mFrameCount;          // Count of the current frame
   vpr::StatCollector<float,false>  mFrameRate;           // Frame rate stats. (time per frame)
   vpr::Interval                    mPrevFrameStartTime;    // Start time of the previous frame

   // VNC Client info
   VNCDesktop*    mDesktop;         /**< The actual desktop object */
   bool           mEmbeddedGUI;     /**< If true, then we should embed the gui */
};

}


#endif
