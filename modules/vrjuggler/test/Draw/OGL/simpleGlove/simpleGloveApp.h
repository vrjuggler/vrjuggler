#ifndef _WAND_APP_
#define _WAND_APP_

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <Kernel/GL/vjGlApp.h>
#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>

#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjGloveInterface.h>

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
      mGlove.init("VJGlove");
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
   {;}

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

      gluQuadricDrawStyle(mQuadObj, GLU_FILL);
      gluQuadricNormals(mQuadObj, GLU_SMOOTH);
      gluSphere(mQuadObj, radius, slices, stacks);
   }

   GLUquadricObj* mQuadObj;

public:
   vjGloveInterface    mGlove;      // the glove
};


#endif
