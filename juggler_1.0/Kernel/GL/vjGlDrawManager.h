#ifndef _VJ_GL_DRAW_MANAGER_
#define _VJ_GL_DRAW_MANAGER_

#include <config.h>

#ifdef VJ_OS_HPUX
#   include <vector>
#else
#   if defined(VJ_USE_PTHREADS) && ! defined(_PTHREADS)
#       define _PTHREADS
#   endif

#   include <vector.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <Kernel/vjDrawManager.h>
#include <Kernel/GL/vjGlApp.h>
#include <Kernel/GL/vjGlWindow.h>
#include <Kernel/GL/vjGlPipe.h>

class vjGlPipe;
class vjConfigChunkDB;
class vjSimulator;

#include <Sync/vjCond.h>

//-----------------------------------------------
//: Concrete Singleton Class for OpenGL drawing
// 
//    Responsible for all OGL based rendering. 
//
// @author Allen Bierbaum
//  Date: 1-7-98
//------------------------------------------------
class vjGlDrawManager : public vjDrawManager
{
public:
    //: Function to config API specific stuff.
    // Takes a chunkDB and extracts API specific stuff
   virtual void config(vjConfigChunkDB*  chunkDB);

   //: Enable a frame to be drawn
   virtual void draw();

    //: Blocks until the end of the frame
    //! POST: The frame has been drawn
   virtual void sync();

   //: Control loop for the manager
   void main(void* nullParam);

   //: Initialize the drawing API (if not already running)
   virtual void initAPI();

   //: Initialize the drawing state for the API based on 
   // the data in the display manager.
   //
   //! PRE: API is running (initAPI has been called)
   //! POST: API is ready do draw <br>
   //+	 Process model is configured <br>
   //	 Multi-pipe data is set <br>
   //	 Window list is correct
   virtual void initDrawing();

   //: Shutdown the drawing API
   virtual void closeAPI();

   //: Draw all the ogl pipes/windows
   void drawAllPipes();

   //: Draw a simulator using OpenGL commands
   //! NOTE: This is called internally by the library
   void drawSimulator(vjSimulator* sim);

   //: Set the app the draw should interact with.
   virtual void setApp(vjApp* _app)
   { app = dynamic_cast<vjGlApp*>(_app);}

   //: Return the app we are rednering
   vjGlApp* getApp()
   { return app; }

   void setDisplayManager(vjDisplayManager* _dispMgr);

   void debugDump();

protected:     // --- Geom helpers --- //
   void initQuadObj();
   void drawSphere(float radius, int slices, int stacks);
   void drawCone(float base, float height, int slices, int stacks);
   void drawBox(float size, GLenum type);
   void drawWireCube(float size);
   void drawSolidCube(float size);

   GLUquadricObj* mQuadObj;

protected:
   // --- Config Data --- //
   int      numPipes;     //: The number of pipes in the system

   // --- API data --- //
   vjGlApp*             app;        //: The OpenGL application
   vector<vjGlWindow*>  wins;       //: A list of the windows in the system
   vector<vjGlPipe*>    pipes;      //: A list of the pipes in the system

   // --- MP Stuff -- //
   vjCond     syncCond;   //: Condition var for syncing
   int        rendering;  //: Are we rendering


   // --- Singleton Stuff --- //
public:
   static vjGlDrawManager* instance()
   {
      if (_instance == NULL)
         _instance = new vjGlDrawManager();
      return _instance;
   }
protected:
   vjGlDrawManager() : rendering(0)
   { ;}
private:
   static vjGlDrawManager* _instance;          
};

#endif
