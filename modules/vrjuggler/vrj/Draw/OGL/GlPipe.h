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

#ifndef _VRJ_GL_PIPE_H_
#define _VRJ_GL_PIPE_H_

#include <vrj/vjConfig.h>

#include <vrj/Draw/OGL/GlWindow.h>
#include <vrj/Draw/OGL/GlDrawManager.h>
#include <vpr/Sync/CondVar.h>
#include <vpr/Sync/Semaphore.h>

#include <vrj/Performance/PerfDataBuffer.h>

namespace vrj
{
   class GlDrawManager;

//--------------------------------------------------------------------------------------
//: Handles the rendering on multiple GLWindows in a single process.
//
//    This class can be used to group glWindows to render efficiently
//    on one pipe (SGI terminology).
//
// @author Allen Bierbaum
//  Date: 1-12-98
//---------------------------------------------------------------------------------------
class VJ_CLASS_API GlPipe
{
public:
      //: Constructor
      //!ARGS: _num - The id number of the pipe.
      //!NOTE: All contained windows SHOULD have the same pipe number
   GlPipe(int num, GlDrawManager* glMgr)
      : mActiveThread(NULL), mPipeNum(num), controlExit(0), glManager(glMgr),
        renderTriggerSema(0), renderCompleteSema(0), swapTriggerSema(0), swapCompleteSema(0)
   {
      mThreadRunning = false;
      char namebuf[42];  // careful with that buffer, eugene

      sprintf( namebuf, "vjGlPipe %d", mPipeNum );

      // we need to check if we should be enabled... It looks like GlPipe
      // is gonna need a configure method, though...
      mPerfBuffer = new PerfDataBuffer( namebuf, 500, 40 );

    }

      //: Start the pipe running
      //! PRE: The pipe should not have a thread of control yet
      //!POST: The pipe has it's own thread of control and is ready to operate
      //+       The Thread of control is running controlLoop
      //! NOTE: The pipe does NOT have to have any windows in order to run
      //+        that way we can add windows to pipes at run-time
   int start();

      //: The main loop routine
      // -Checks for new windows <br>
      // -renders all windows when triggered <br>
      // -
   void controlLoop(void* nullParam);


      //: Stop the pipe
      //!POST: Flag is set to tell pipe to stop rendering
   void stop()
   { controlExit = 1; }     // Set the control loop exit flag

public:     // --------- Triggering functions ------ //
   //: Trigger rendering of the pipe to start
   //! PRE: The pipe must have a thread of control
   //!POST: The pipe has be told to start rendering
   void triggerRender();

   //: Blocks until rendering of the windows is completed
   //! PRE: The pipe must have a thread of control
   //!POST: The pipe has completed its rendering
   void completeRender();

   //: Trigger swapping of all pipe's windows
   //! PRE: The pipe must have a thread of control
   //! POST: The pipe has be told to swap
   void triggerSwap();

   //: Blocks until swapping of the windows is completed
   //! PRE: The pipe must have a thread of control
   //!POST: The pipe has completed its rendering
   void completeSwap();

public: // --- Window Management ----- //

   //: Add a GLWindow to the new window list
   // Control loop must now open the window on the next frame
   void addWindow(GlWindow* win);

   //: Remove a GLWindow from the window list
   //! NOTE: The window is not actually removed until the next draw trigger
   void removeWindow(GlWindow* win);


   //: Returns true if pipe has any windows
   int hasWindows()
   { return ( (newWins.size() > 0) || (openWins.size() > 0)); }


   //: Return a list of open windows
   std::vector<GlWindow*> getOpenWindows()
   { return openWins; }

private:
      //:  Checks for any new windows to add to the pipe
      //! POST: Any new windows will be opened and added to the pipe's rendering list
   void checkForNewWindows();

   //: Check for any windows that need closed
   //! POST: Any windows to close will be closed and removed from the pipe's rendering list
   void checkForWindowsToClose();

   //:   Renders the window using OpenGL
   //!POST: win is rendered (In stereo if it is a stereo window)
   void renderWindow(GlWindow* win);

   //: Swaps the buffers of the given window
   void swapWindowBuffers(GlWindow* win);

   GlPipe(const GlPipe& o) {;}
   void operator=(const GlPipe& o) {;}

private:
   vpr::Thread*   mActiveThread;      //: The thread running this object
   bool        mThreadRunning;      //: Do we have a running thread?

   int   mPipeNum;                     //: The id of the pipe

   std::vector<GlWindow*> newWins;  //: List of windows still to be opened on current pipe
   vpr::Mutex newWinLock;                //: Lock for accessing the newWin list

   std::vector<GlWindow*> openWins; //: List of current open windows to render
   vpr::Mutex openWinLock;               //: Lock for accessing the openWinList

   std::vector<GlWindow*> mClosingWins; //: List of windows to close
   vpr::Mutex mClosingWinLock;               //: Lock for access the windows to close

   int         controlExit;         //: Flag for when to exit the control loop

   GlDrawManager*    glManager;    //: The openlGL manager that we are rendering for
                                     //: Needed to get app, etc.

   vpr::Semaphore    renderTriggerSema;   //: Signals render trigger
   vpr::Semaphore    renderCompleteSema;  //: signals render completed
   vpr::Semaphore    swapTriggerSema;     //: Signals a swap to happen
   vpr::Semaphore    swapCompleteSema;    //: Signals a swap has been completed

    PerfDataBuffer* mPerfBuffer;  //: Performance data for this pipe
    int mPerfPhase;                 //: utility var for perf measurement
};

};

#endif
