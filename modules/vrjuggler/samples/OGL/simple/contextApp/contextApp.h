/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _SIMPLE_APP
#define _SIMPLE_APP

#include <vrj/vrjConfig.h>

#include <iostream>
#include <iomanip>

#include <vrj/Draw/OpenGL/App.h>
#include <vrj/Draw/OpenGL/ContextData.h>

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>

using namespace gmtl;
using namespace vrj;

// Utility function to draw a cube
void drawbox( GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
              GLdouble z0, GLdouble z1, GLenum type );

/**
 * Simple OGL Demonstration application.
 *
 * This application intialized and recieves positional
 * and digital intput from the wand.
 *
 * It also has basic code that draws a box centered on
 * the origin
 */
class contextApp : public opengl::App
{
public:
   contextApp()
   {;}

   virtual ~contextApp (void) {;}

public: // ---- INITIALIZATION FUNCTIONS ---- //
   /**
    * Executes any initialization needed before the API is started.
    *
    * @post Device interfaces are initialized with the device names
    *       we want to use.
    * @note This is called once before OpenGL is initialized.
    */
   virtual void init()
   {
      std::cout << "---------- App:init() ---------------" << std::endl;
      // Initialize devices
      mWand.init("VJWand");
      mHead.init("VJHead");
      mButton0.init("VJButton0");
      mButton1.init("VJButton1");
   }

   /**
    * Executes any initialization needed after API is started but before the
    * drawManager starts the drawing loops.
    *
    * This is called once after OGL is initialized.
    */
   virtual void apiInit()
   {;}

public:
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
   //       UpdateDevices();
   //  }
   //------------------------------------

   /**
    * Function that is called upon entry into a buffer of an OpenGL context
    * (window).
    *
    * @note This function is designed to be used when you want to do something
    *       only once per buffer (ie.once for left buffer, once for right
    *       buffer).
    */
   virtual void bufferPreDraw();

   /** Called before start of frame
    *
    * @note Function called after device updates but before start of drawing.
    */
   virtual void preFrame()
   {
      if (mButton0->getData())
      {
         std::cout << "Button 0 pressed" << std::endl;
      }

      if (mButton1->getData())
      {
         std::cout << "Button 1 pressed" << std::endl;
      }
   }

   /**
    * Called during the Frame.
    *
    * @note Function called after drawing has been triggered but BEFORE it
    *       completes.
    */
   virtual void intraFrame() {;}

   /**
    * Called at end of frame.
    *
    * @note Function called before updating trackers but after the frame is
    *       drawn.
    */
   virtual void postFrame() {;}

public: // ----- OpenGL FUNCTIONS ---- //
   /**
    * Function that is called immediately after a new OGL context is created.
    * Initialize GL state here. Also used to create context-specific
    * information.
    *
    * This is called once for each context
    */
   virtual void contextInit();

   /**
    * Function to draw the scene.
    *
    * @pre OpenGL state has correct transformation and buffer selected.
    * @post The current scene has been drawn.
    *
    * @note Called 1 or more times per frame.
    */
   virtual void draw();

private:
   void initGLState();

   /// Draws a cube
   void drawCube()
   {
      drawbox(-0.5, 0.5, -0.5, 0.5, -0.5, 0.5, GL_QUADS);
   }

public:
   opengl::ContextData<GLuint> mCubeDlId; /**< ID of the cube display list */

   gadget::PositionInterface  mWand;    /**< Positional interface for Wand position */
   gadget::PositionInterface  mHead;    /**< Positional interface for Head position */
   gadget::DigitalInterface   mButton0; /**< Digital interface for button 0 */
   gadget::DigitalInterface   mButton1; /**< Digital interface for button 1v */
};


#endif
