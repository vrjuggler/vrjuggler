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
      : vjGlApp(kern), clearColor(0.0f)            // Initialize base class
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

   //: Function to draw the scene
   //! PRE: OpenGL state has correct transformation and buffer selected
   //! POST: The current scene has been drawn
   virtual void draw()
   {
      initGLState();    // This should really be in another function
      myDraw();
   }

   // ----- Drawing Loop Functions ------
   //
   //  The drawing loop will look similar to this:
   //
   //  while (drawing)
   //  {
   //        preDraw();
   //	      draw();
   //	       postDraw();      // Drawing is happening while here
   //       sync();
   //        postSync();      // Drawing is now done
   //
   //	      UpdateTrackers();
   //  }
   //------------------------------------

   /// Function called before updating trackers but after the frame is drawn
   virtual void postSync()
   {
     // cout << "cubesApp::postSync\n";
   }

   /// Function called after tracker update but before start of drawing
   virtual void preDraw()
   {
      //cout << "cubesApp::preDraw()\n";
      static float direction = 1;
      const float incr(0.0025);

      clearColor += (direction*incr);

      if(clearColor == 1.0f)
         direction = -1.0f;
      if(clearColor == 0.0f)
         direction = 1.0f;
   }

   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void postDraw()
   {
       //cout << "cubesApp::postDraw()\n";
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
   vjPosInterface    mWand;      // the Wand
   vjPosInterface    mHead;      // the head
   float             clearColor;
};


#endif
