#ifndef _WAND_APP_
#define _WAND_APP_

#include <iostream.h>
#include <iomanip.h>

#include <Kernel/GL/vjGlApp.h>
#include <Math/vjMatrix.h>

#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjAnalogInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>

// Utility function to draw a cube
void drawbox(GLdouble x0, GLdouble x1, GLdouble y0, GLdouble y1,
                GLdouble z0, GLdouble z1, GLenum type);

//----------------------------------------------------
//: Demonstration OpenGL application class
//
// This application simply renders a box on the wand
//----------------------------------------------------
class wandApp : public vjGlApp
{
public:
   wandApp(vjKernel* kern)
      : vjGlApp(kern)         // Initialize base class
   {;}

   // Execute any initialization needed before the API is started
   virtual void init()
   {
      //cout << "---------- App:init() ---------------" << endl;
      // Initialize devices
      mWand.init("VJWand");
      mHead.init("VJHead");
   }

   // Execute any initialization needed after API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {;}

   //: Function that is called immediately after a new OGL context is created
   // Initialize GL state here. Also used to create context specific information
   virtual void contextInit()
   {
      initGLState();       // Initialize the GL state information. (lights, shading, etc)
   }

   //: Function to draw the scene
   //! PRE: OpenGL state has correct transformation and buffer selected
   //! POST: The current scene has been drawn
   virtual void draw()
   {
      myDraw();
   }

   // ----- Drawing Loop Functions ------
   //
   //  The drawing loop will look similar to this:
   //
   //  while (drawing)
   //  {
   //        preDraw();
   //       draw();
   //        postDraw();      // Drawing is happening while here
   //       sync();
   //        postSync();      // Drawing is now done
   //
   //       UpdateTrackers();
   //  }
   //------------------------------------

   //: Function called before updating trackers but after the frame is drawn
   virtual void postSync()
   {;}

   //: Function called after tracker update but before start of drawing
   virtual void preDraw()
   {;}

   //: Function called after drawing has been triggered but BEFORE it completes
   virtual void postDraw()
   {;}

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
   vjPosInterface    mWand;      // the Wand
   vjPosInterface    mHead;      // the head
};


#endif
