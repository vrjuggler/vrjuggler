/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _VRJ_D3D_DRAW_MANAGER_H_
#define _VRJ_D3D_DRAW_MANAGER_H_

#include <vrj/Draw/Direct3D/Config.h>
#include <vector>

#include <vpr/vpr.h>
//#include <vpr/Sync/CondVar.h>
#include <vpr/Sync/Semaphore.h>
#include <vpr/Util/Singleton.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/TSObjectProxy.h>

#include <jccl/Config/ConfigElementPtr.h>

#include <vrj/Draw/DrawManager.h>

namespace vrj
{
   class D3dApp;
   class D3dWindow;

/** \class D3dDrawManager D3dDrawManager.h vrj/Draw/Direct3D/D3dDrawManager.h
 *
 * Concrete Singleton Class for Direct3D drawing.  Responsible for all
 * Direct3D-based rendering.
 *
 * vrj::D3dDrawManager is an active object.  It manages Direct3D pipes and
 * windows.  In addition, it triggers rendering, swapping, and syncing of the
 * windows under its control.
 *
 * All access to the Direct3D rendering structures has to happen from the
 * control thread or in the case of context sensitive functions, from the
 * control thread of the managed pipes.  Because of this, the object uses
 * queues to hold new windows.
 *
 * @since 2.1.16
 */
class VJ_D3D_CLASS_API D3dDrawManager : public DrawManager
{
public:
   //friend class GlPipe;
   //friend class GlContextDataBase;

   /** Starts the control loop. */
   virtual void start();

   /** Enables a frame to be drawn. */
   virtual void draw();

   /**
    * Blocks until the end of the frame.
    *
    * @post The frame has been drawn.
    */
   virtual void sync();

   /** Control loop for the manager. */
   void main();

   /** Initializes the drawing API (if not already running). */
   virtual void initAPI();

   /**
    * Callback when display is added to the Display Manager.
    *
    * @note This function can only be called by the display manager
    *       functioning in the kernel thread to signal a new display added
    *       This guarantees that we are not rendering currently.
    *       We will most likely be waiting for a render trigger.
    */
   virtual void addDisplay(Display* disp);

   /** Callback when display is removed from the Display Manager. */
   virtual void removeDisplay(Display* disp);

   /** Shutdown the drawing API. */
   virtual void closeAPI();

   /** Outputs some debug info. */
   virtual void outStream(std::ostream& out);

   /** Draws all the ogl pipes/windows. */
   void drawAllPipes();

   /** Set the app the draw should interact with. */
   virtual void setApp(App* _app);

   /** Returns the app we are rendering. */
   D3dApp* getApp();

   //void setDisplayManager(DisplayManager* _dispMgr);

public:
   /** @name Config element handler implementation */
   //@{
   /**
    * Adds the element to the configuration.
    *
    * @pre configCanHandle(element) == true
    * @return success
    */
   virtual bool configAdd(jccl::ConfigElementPtr element);

   /**
    * Removes the element from the current configuration.
    *
    * @pre configCanHandle(element) == true
    * @return success
    */
   virtual bool configRemove(jccl::ConfigElementPtr element);

   /**
    * Can the handler handle the given config element?
    *
    * @return true if we can handle it; false if we can't.
    */
   virtual bool configCanHandle(jccl::ConfigElementPtr element);
   //@}

public:
   /**
    * Gets pointer to the current user data.  Should be used in the draw
    * function.
    *
    * @note This user data is valid ONLY in draw().  It is not valid anywhere
    *       else.
    */
   //GlUserData* currentUserData();

   /**
    * Returns a unique identifier for the current context.
    *
    * @note This id is ONLY valid in contextInit() and draw().
    */
   //int getCurrentContext();

protected:
   /**
    * Factory function to get system specific OpenGL window.
    *
    * @post Returns an OpenGL window for the current system.
    */
   //GlWindow* getGLWindow();

   void setCurrentContext(int val);

   /** Sets the dirty bits off all the GL windows. */
   void dirtyAllWindows();

   /** Is the window a valid window for the draw manager? */
   bool isValidWindow(D3dWindow* win);
   void renderWindow(D3dWindow* win);
   void swapWindowBuffers(D3dWindow* win);
   void checkForNewWindows();


protected:
   /** @name Config Data */
   //@{
   int      numPipes;     /**<  The number of pipes in the system */
   //@}

   /** @name API data */
   //@{
   D3dApp*                 mApp;   /**< The OpenGL application */
   std::vector<D3dWindow*> mWins;  /**< A list of the windows in the system */
   std::vector<D3dWindow*>  mNewWins;         /**< List of windows still to be opened on current pipe */
   vpr::Mutex               mNewWinLock;      /**< Lock for accessing the newWin list */
   //@}

   std::vector<D3dWindow*>  mOpenWins;        /**< List of current open windows to render */
   vpr::Mutex               mOpenWinLock;     /**< Lock for accessing the openWinList */

   /** @name MP Stuff */
   //@{
   vpr::Semaphore    drawTriggerSema;  /**< Semaphore for draw trigger */
   vpr::Semaphore    drawDoneSema;     /**< Semaphore for drawing done */

   /**
    * Protects run-time config.  Only when this semaphore is acquired can
    * run-time config occur.
    */
   bool              mRunning;         /**< Used to stop the drawing thread. */

   vpr::Thread* mControlThread;
   vpr::Mutex   mCreateWindowMutex;    /**< Mutex for opening windows across
                                            pipes. */
   //@}

protected:
   D3dDrawManager();

   virtual ~D3dDrawManager();

   D3dDrawManager(const D3dDrawManager& o)
      : DrawManager(o)
   {;}

   void operator=(const D3dDrawManager&) {;}

   vprSingletonHeader(D3dDrawManager);
};

} // end namespace

#endif
