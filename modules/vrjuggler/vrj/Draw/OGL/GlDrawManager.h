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

#ifndef _VRJ_GL_DRAW_MANAGER_H_
#define _VRJ_GL_DRAW_MANAGER_H_

#include <vrj/vrjConfig.h>

#include <vpr/vpr.h>
//#include <vpr/Sync/CondVar.h>
#include <vpr/Sync/Semaphore.h>
#include <vpr/Util/Singleton.h>
#include <vpr/Thread/TSObjectProxy.h>

#ifdef VPR_OS_Darwin
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif

#include <vrj/Draw/DrawManager.h>
//#include <vrj/Draw/OGL/GlApp.h>
#include <vrj/Draw/OGL/GlWindow.h>
#include <vrj/Draw/OGL/GlPipe.h>
#include <vrj/Draw/OGL/GlUserData.h>

namespace jccl {
    class ConfigChunkDB;
};

namespace gadget {
   class GloveProxy;
}

namespace vrj
{
   class GlApp;
   class SimViewport;

//-----------------------------------------------
//: Concrete Singleton Class for OpenGL drawing
//
//    Responsible for all OGL based rendering.
//
//  GlDrawManager is an active object.  It manages
// ogl pipes and windows.  In addition, it triggers
// rendering, swapping, and syncing of the windows
// under it's control.
//
// All access to the ogl rendering structures has
// to happen from the control thread or in the case
// of context sensitive functions, from the control
// thread of the managed pipes. Because of this,
// the object uses queues to hold new windows.
//
// @author Allen Bierbaum
//  Date: 1-7-98
//------------------------------------------------
//! PUBLIC_API:
class VJ_CLASS_API GlDrawManager : public DrawManager
{
public:
   friend class GlPipe;
   friend class GlContextDataBase;

    //: Function to config API specific stuff.
    // Takes a chunkDB and extracts API specific stuff
   //**//virtual void configInitial(jccl::ConfigChunkDB*  chunkDB);

   //: Start the control loop
   virtual void start();

   //: Enable a frame to be drawn
   virtual void draw();

    //: Blocks until the end of the frame
    //! POST: The frame has been drawn
   virtual void sync();

   //: Control loop for the manager
   void main(void* nullParam);

   //: Initialize the drawing API (if not already running)
   virtual void initAPI();

   //: Callback when display is added to display manager
   //! NOTE: This function can only be called by the display manager
   //+      functioning in the kernel thread to signal a new display added
   //+      This guarantees that we are not rendering currently.
   //+      We will most likely be waiting for a render trigger.
   virtual void addDisplay(Display* disp);

   //: Callback when display is removed to display manager
   virtual void removeDisplay(Display* disp);

   //: Shutdown the drawing API
   virtual void closeAPI();

   //: Output some debug info
   virtual void outStream(std::ostream& out);

   //: Draw all the ogl pipes/windows
   void drawAllPipes();

   //: Set the app the draw should interact with.
   virtual void setApp(App* _app);

   //: Return the app we are rendering
   GlApp* getApp();

   //void setDisplayManager(DisplayManager* _dispMgr);

public: // Chunk handlers
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! RETURNS: success
   virtual bool configAdd(jccl::ConfigChunk* chunk);

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //!RETURNS: success
   virtual bool configRemove(jccl::ConfigChunk* chunk);

   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   virtual bool configCanHandle(jccl::ConfigChunk* chunk);


public:  // Drawing functions used by library
   //: Draw any objects that the manager needs to display
   // i.e. Gloves, etc
   void drawObjects();

   //: Draw projections in Opengl
   void drawProjections(bool drawFrustum, Vec3 surfColor);

   //: Draw a simulator using OpenGL commands
   //! NOTE: This is called internally by the library
   void drawSimulator(SimViewport* sim);

public:
   //: Get ptr to the current user data
   // Should be used in the draw function
   //! NOTE: This user data is valid ONLY
   //+ in draw().  It is not valid anywhere else.
   GlUserData* currentUserData()
   { return &(*mUserData); }

   //: Returns a unique identifier for the current context
   //! NOTE: This id is ONLY valid in
   //+ contextInit() and draw()
   int getCurrentContext()
   { return (*mContextId); }

protected:     // --- Geom helpers --- //
   void initQuadObj();
   void drawLine(Vec3& start, Vec3& end);
   void drawSphere(float radius, int slices, int stacks);
   void drawCone(float base, float height, int slices, int stacks);
   void drawBox(float size, GLenum type);
   void drawWireCube(float size);
   void drawSolidCube(float size);
   void drawGlove(gadget::GloveProxy* gloveProxy);

   GLUquadricObj* mQuadObj;

protected:
   //: Factory function to get system specific OpenGL window
    //! POST: Returns an OpenGL window for the current system
   GlWindow* getGLWindow();

   void setCurrentContext(int val)
   { (*mContextId) = val; }

   //: Set the dirty bits off all the gl windows
   void dirtyAllWindows();

   //: Is the window a valid window for the draw manager
   bool isValidWindow(GlWindow* win);


protected:
   // --- Config Data --- //
   int      numPipes;     //: The number of pipes in the system

   // --- API data --- //
   GlApp*                 mApp;      //: The OpenGL application
   std::vector<GlWindow*> mWins;     //: A list of the windows in the system
   std::vector<GlPipe*>   pipes;    //: A list of the pipes in the system

   // --- Helper field data --- //
   vpr::TSObjectProxy<int>             mContextId;    //: TS Data for context id
   vpr::TSObjectProxy<GlUserData>    mUserData;     //: User data for draw func

   // --- MP Stuff -- //
   vpr::Semaphore    drawTriggerSema;  // Semaphore for draw trigger
   vpr::Semaphore    drawDoneSema;     // Semaphore for drawing done
   vpr::Semaphore    mRuntimeConfigSema;  //: Protects run-time config.  Only when this semaphore
                                       //+ is acquired can run-time config occur

protected:
   GlDrawManager() : drawTriggerSema(0), drawDoneSema(0), mRuntimeConfigSema(0)
   {
      mQuadObj = NULL;
   }

   virtual ~GlDrawManager() {}

   GlDrawManager(const GlDrawManager& o) {;}
   void operator=(const GlDrawManager& o) {;}

   vprSingletonHeader(GlDrawManager);
};

} // end namespace

#endif
