#ifndef _VJ_GL_DRAW_MANAGER_
#define _VJ_GL_DRAW_MANAGER_

#include <vjConfig.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <Kernel/vjDrawManager.h>
//#include <Kernel/GL/vjGlApp.h>
class vjGlApp;
#include <Kernel/GL/vjGlWindow.h>
#include <Kernel/GL/vjGlPipe.h>
#include <Threads/vjTSObjectProxy.h>
#include <Kernel/GL/vjGlUserData.h>

class vjConfigChunkDB;
class vjSimulator;
class vjGloveProxy;

//#include <Sync/vjCond.h>
#include <Sync/vjSemaphore.h>

//-----------------------------------------------
//: Concrete Singleton Class for OpenGL drawing
//
//    Responsible for all OGL based rendering.
//
// @author Allen Bierbaum
//  Date: 1-7-98
//------------------------------------------------
//! PUBLIC_API:
class vjGlDrawManager : public vjDrawManager
{
public:
   friend class vjGlPipe;
   friend class vjGlContextDataBase;

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

   //: Draw any objects that the manager needs to display
   // i.e. Gloves, etc
   void drawObjects();

   //: Draw a simulator using OpenGL commands
   //! NOTE: This is called internally by the library
   void drawSimulator(vjSimulator* sim);

   //: Set the app the draw should interact with.
   virtual void setApp(vjApp* _app);

   //: Return the app we are rednering
   vjGlApp* getApp();

   void setDisplayManager(vjDisplayManager* _dispMgr);

   void debugDump();

protected:     // --- Geom helpers --- //
   void initQuadObj();
   void drawLine(vjVec3& start, vjVec3& end);
   void drawSphere(float radius, int slices, int stacks);
   void drawCone(float base, float height, int slices, int stacks);
   void drawBox(float size, GLenum type);
   void drawWireCube(float size);
   void drawSolidCube(float size);
   void drawGlove(vjGloveProxy* gloveProxy);

   GLUquadricObj* mQuadObj;

public:
   //: Get ptr to the current user data
   // Should be used in the draw function
   //! NOTE: This user data is valid ONLY
   //+ in contextInit() and draw()
   vjGlUserData* currentUserData()
   { return &(*mUserData); }

   //: Returns a unique identifier for the current context
   //! NOTE: This id is ONLY valid in
   //+ contextInit() and draw()
   int getCurrentContext()
   { return (*mContextId); }

protected:
   void setCurrentContext(int val)
   { (*mContextId) = val; }


protected:
   // --- Config Data --- //
   int      numPipes;     //: The number of pipes in the system

   // --- API data --- //
   vjGlApp*                 app;      //: The OpenGL application
   std::vector<vjGlWindow*> wins;     //: A list of the windows in the system
   std::vector<vjGlPipe*>   pipes;    //: A list of the pipes in the system

   // --- Helper field data --- //
   vjTSObjectProxy<int>             mContextId;    //: TS Data for context id
   vjTSObjectProxy<vjGlUserData>    mUserData;     //: User data for draw func

   // --- MP Stuff -- //
   vjSemaphore    drawTriggerSema;  // Semaphore for draw trigger
   vjSemaphore    drawDoneSema;     // Semaphore for drawing done

   // --- Singleton Stuff --- //
public:
   static vjGlDrawManager* instance()
   {
      if (_instance == NULL)
         _instance = new vjGlDrawManager();
      return _instance;
   }
protected:
   vjGlDrawManager() : drawTriggerSema(0), drawDoneSema(0)
   { ;}
private:
   static vjGlDrawManager* _instance;
};

#endif
