/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _VRJ_GL_PIPE_H_
#define _VRJ_GL_PIPE_H_

#include <vrj/Draw/OGL/Config.h>
#include <vector>

#include <vrj/Draw/OGL/GlWindow.h>
#include <vrj/Draw/OGL/GlDrawManager.h>
#include <vpr/Sync/CondVar.h>
#include <vpr/Sync/Semaphore.h>

namespace vrj
{
   class GlDrawManager;

/** \class GlPipe GlPipe.h vrj/Draw/OGL/GlPipe.h
 *
 * Handles the rendering on multiple GLWindows in a single process.
 *
 * This class can be used to group glWindows to render efficiently
 * on one pipe (SGI terminology).
 *
 * @date 1-12-98
 */
class VJ_OGL_CLASS_API GlPipe
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
   GlPipe(size_t num, GlDrawManager* glMgr, vpr::Mutex* drawMgrWinLock)
      : mControlFunctor(NULL)
      , mActiveThread(NULL)
      , mPipeNum(num)
      , mControlExit(0)
      , mGlDrawManager(glMgr)
      , mDrawMgrWinLock(drawMgrWinLock)
      , mRenderTriggerSema(0)
      , mRenderCompleteSema(0)
      , mSwapTriggerSema(0)
      , mSwapCompleteSema(0)
   {
      mThreadRunning = false;
   }

   ~GlPipe();

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
   void controlLoop(void* nullParam);

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
   void addWindow(GlWindow* win);

   /**
    * Removes the given GLWindow from the window list.
    * @note The window is not actually removed until the next draw trigger.
    */
   void removeWindow(GlWindow* win);

   /** Returns true if pipe has any windows. */
   int hasWindows()
   {
      return ( (mNewWins.size() > 0) || (mOpenWins.size() > 0));
   }

   /** Return a list of open windows. */
   const std::vector<GlWindow*>& getOpenWindows()
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
   void renderWindow(GlWindow* win);

   /** Swaps the buffers of the given window. */
   void swapWindowBuffers(GlWindow* win);

   /**
    * Do any other window setup stuff that is needed after the window is open.
    * @pre Window is open and is current.
    */
   void finishWindowSetup(GlWindow* win);

   GlPipe(const GlPipe&) {;}
   void operator=(const GlPipe&) {;}

private:
   vpr::ThreadMemberFunctor<GlPipe>* mControlFunctor;
   vpr::Thread*   mActiveThread;    /**< The thread running this object */
   bool           mThreadRunning;   /**< Do we have a running thread? */

   size_t         mPipeNum;         /**< The id of the pipe */

   std::vector<GlWindow*>  mNewWins;         /**< List of windows still to be opened on current pipe */
   vpr::Mutex              mNewWinLock;      /**< Lock for accessing the newWin list */

   std::vector<GlWindow*>  mOpenWins;        /**< List of current open windows to render */
   vpr::Mutex              mOpenWinLock;     /**< Lock for accessing the openWinList */

   std::vector<GlWindow*>  mClosingWins;     /**< List of windows to close */
   vpr::Mutex              mClosingWinLock;  /**< Lock for access the windows to close */

   int                     mControlExit;     /**< Flag for when to exit the control loop */

   GlDrawManager*          mGlDrawManager;   /**< The openlGL manager that we are rendering for.
                                                  Needed to get app, etc. */
   vpr::Mutex*             mDrawMgrWinLock;  /**< Cross-pipe lock associaed
                                                  with the OpenGL Draw Manager */

   vpr::Semaphore    mRenderTriggerSema;  /**< Signals render trigger */
   vpr::Semaphore    mRenderCompleteSema; /**< Signals render completed */
   vpr::Semaphore    mSwapTriggerSema;    /**< Signals a swap to happen */
   vpr::Semaphore    mSwapCompleteSema;   /**< Signals a swap has been completed */
};

}

#endif
