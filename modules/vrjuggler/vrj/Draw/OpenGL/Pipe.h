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

#ifndef _VRJ_OPENGL_PIPE_H_
#define _VRJ_OPENGL_PIPE_H_

#include <vrj/Draw/OpenGL/Config.h>

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <vpr/Sync/Semaphore.h>
#include <vrj/Draw/OpenGL/Window.h>
#include <vrj/Draw/OpenGL/DrawManager.h>


namespace vrj
{

namespace opengl
{

class DrawManager;

/** \class vrj::opengl::Pipe Pipe.h vrj/Draw/OpenGL/Pipe.h
 *
 * Handles the rendering on multiple GLWindows in a single process.
 *
 * This class can be used to group OpenGL windows to render efficiently
 * on one pipe (SGI terminology).
 *
 * @date 1-12-98
 *
 * @note This class was renamed from vrj::GlPipe in VR Juggler 2.3.11.
 */
class VJ_OGL_CLASS_API Pipe : boost::noncopyable
{
public:
   /**
    * Constructor.
    *
    * @param num            The ID number of the pipe.
    * @param glMgr          The OpenGL Draw Manager instance associated with
    *                       this pipe.
    * @param drawMgrWinLock Cross-pipe lock associated with the OpenGL Draw
    *                       Manager to prevent windows from opening
    *                       simultaneously on different pipes.
    *
    * @note All contained windows SHOULD have the same pipe number.
    */
   Pipe(const size_t num, vrj::opengl::DrawManager* glMgr,
        vpr::Mutex* drawMgrWinLock)
      : mActiveThread(NULL)
      , mPipeNum(num)
      , mControlExit(false)
      , mGlDrawManager(glMgr)
      , mDrawMgrWinLock(drawMgrWinLock)
      , mRenderTriggerSema(0)
      , mRenderCompleteSema(0)
      , mSwapTriggerSema(0)
      , mSwapCompleteSema(0)
   {
      mThreadRunning = false;
   }

   ~Pipe();

   /**
    * Starts the pipe running.
    *
    * @pre The pipe should not have a thread of control yet.
    * @post The pipe has it's own thread of control and is ready to operate
    *       The Thread of control is running controlLoop.
    * @note The pipe does NOT have to have any windows in order to run
    *       that way we can add windows to pipes at run-time.
    */
   int start();

   /**
    * The main loop routine.
    * -Checks for new windows <br>
    * -renders all windows when triggered <br>
    */
   void controlLoop();

   /**
    * Stops the pipe.
    * @post Flag is set to tell pipe to stop rendering.
    */
   void stop();

public:     // --------- Triggering functions ------ //
   /**
    * Triggers rendering of the pipe to start.
    * @pre The pipe must have a thread of control.
    * @post The pipe has be told to start rendering.
    */
   void triggerRender();

   /**
    * Blocks until rendering of the windows is completed.
    * @pre The pipe must have a thread of control.
    * @post The pipe has completed its rendering.
    */
   void completeRender();

   /**
    * Triggers swapping of all pipe's windows.
    * @pre The pipe must have a thread of control.
    * @post The pipe has be told to swap.
    */
   void triggerSwap();

   /**
    * Blocks until swapping of the windows is completed.
    * @pre The pipe must have a thread of control.
    * @post The pipe has completed its rendering.
    */
   void completeSwap();

public: // --- Window Management ----- //

   /**
    * Adds the given GLWindow to the new window list.
    * Control loop must now open the window on the next frame.
    */
   void addWindow(vrj::opengl::WindowPtr win);

   /**
    * Removes the given GLWindow from the window list.
    * @note The window is not actually removed until the next draw trigger.
    */
   void removeWindow(vrj::opengl::WindowPtr win);

   /** Returns true if pipe has any windows. */
   bool hasWindows() const
   {
      return mNewWins.size() > 0 || mOpenWins.size() > 0;
   }

   /** Return a list of open windows. */
   const std::vector<vrj::opengl::WindowPtr>& getOpenWindows()
   {
      return mOpenWins;
   }

private:
   /**
    * Checks for any new windows to add to the pipe.
    * @post Any new windows will be opened and added to the pipe's rendering
    *       list.
    */
   void checkForNewWindows();

   /**
    * Checks for any windows that need closed.
    * @post Any windows to close will be closed and removed from the pipe's
    *       rendering list.
    */
   void checkForWindowsToClose();

   /**
    * Renders the window using OpenGL.
    * @post win is rendered (In stereo if it is a stereo window).
    */
   void renderWindow(vrj::opengl::WindowPtr win);

   /** Swaps the buffers of the given window. */
   void swapWindowBuffers(vrj::opengl::WindowPtr win);

   /**
    * Do any other window setup stuff that is needed after the window is open.
    * @pre Window is open and is current.
    */
   void finishWindowSetup(vrj::opengl::WindowPtr win);

private:
   vpr::Thread*   mActiveThread;    /**< The thread running this object */
   bool           mThreadRunning;   /**< Do we have a running thread? */

   size_t         mPipeNum;         /**< The id of the pipe */

   /** @name Window Management */
   //@{
   /** List of windows still to be opened on current pipe. */
   std::vector<vrj::opengl::WindowPtr> mNewWins;
   /** Lock for accessing the newWin list. */
   vpr::Mutex mNewWinLock;

   /** List of current open windows to render. */
   std::vector<vrj::opengl::WindowPtr> mOpenWins;
   /** Lock for accessing \c mOpenWins. */
   vpr::Mutex mOpenWinLock;

   /** List of windows to close. */
   std::vector<vrj::opengl::WindowPtr> mClosingWins;
   /** Lock for access the windows to close. */
   vpr::Mutex mClosingWinLock;
   //@}

   /** Flag for when to exit the control loop */
   bool mControlExit;

   /** @name Draw Manager */
   //@{
   /**
    * The OpenGL Draw Manager for which this pipe renders. Needed to get the
    * application object and other information.
    */
   vrj::opengl::DrawManager* mGlDrawManager;

   /** Cross-pipe lock associaed with the OpenGL Draw Manager. */
   vpr::Mutex* mDrawMgrWinLock;
   //@}

   /** @name Synchronization */
   //@{
   vpr::Semaphore mRenderTriggerSema;  /**< Signals render trigger */
   vpr::Semaphore mRenderCompleteSema; /**< Signals render completed */
   vpr::Semaphore mSwapTriggerSema;    /**< Signals a swap to happen */
   vpr::Semaphore mSwapCompleteSema;   /**< Signals a swap has been completed */
   //@}
};

}

}


#endif
