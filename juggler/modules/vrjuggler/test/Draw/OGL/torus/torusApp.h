#ifndef _TORUS_APP
#define _TORUS_APP


#include <vjConfig.h>

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream.h>

#include <Kernel/GL/vjGlApp.h>
#include <Kernel/vjDebug.h>

#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>
#include <Math/vjQuat.h>
#include <Kernel/vjDebug.h>

#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjAnalogInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>


//--------------------------------------------------
// Torus test OpenGL application class
//
// This application simply renders a torus in the corner of a C2.
//---------------------------------------------------
class torusApp : public vjGlApp
{
public:
   torusApp(vjKernel* kern) : vjGlApp(kern)
   {;}

   // Execute any initialization needed before the API is started
   virtual void init()
   {
      mWand.init("VJWand");
   }

   // Execute any initialization needed <b>after</b> API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {;}

   // Called immediately upon opening a new OpenGL context
   virtual void contextInit()
   {
      initGLState();
   }

   /** Function to draw the scene
    * PRE: OpenGL state has correct transformation and buffer selected
    * POST: The current scene has been drawn
    */
   virtual void draw();

   /// Function called after tracker update but before start of drawing
   virtual void preFrame()
   {;}

   /// Function called after drawing has been triggered but BEFORE it completes
   virtual void intraFrame()
   {;}

   /// Function called before updating trackers but after the frame is drawn
   virtual void postFrame()
   {;}

private:
   void initGLState();

private:
   vjPosInterface    mWand;
};

#endif
