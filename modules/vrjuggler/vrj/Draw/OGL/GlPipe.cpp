/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <vrj/vrjConfig.h>

#include <vpr/vpr.h>

#ifdef VPR_OS_Darwin
#   include <OpenGL/gl.h>
#else
#   include <GL/gl.h>
#endif

#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/OGL/GlPipe.h>
#include <vrj/Draw/OGL/GlApp.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Guard.h>
#include <vrj/Util/Debug.h>

#include <vrj/Display/SurfaceViewport.h>
#include <vrj/Display/SimViewport.h>
#include <jccl/PerfMonitor/PerformanceMonitor.h>

namespace vrj
{


/**
 * Starts the pipe running.
 * @post The pipe has it's own thread of control and is ready to operate.
 */
int GlPipe::start()
{
    vprASSERT(mThreadRunning == false);        // We should not be running yet

    // Create a new thread to handle the control loop
    vpr::ThreadMemberFunctor<GlPipe>* memberFunctor =
         new vpr::ThreadMemberFunctor<GlPipe>(this, &GlPipe::controlLoop, NULL);

    mActiveThread = new vpr::Thread(memberFunctor);

    vprDEBUG(vrjDBG_DRAW_MGR,1) << "vjGlPipe::start: Started control loop. "
                              << mActiveThread << std::endl << vprDEBUG_FLUSH;
    return 1;
}

/**
 * Triggers rendering of the pipe to start.
 * @post The pipe has be told to start rendering.
 */
void GlPipe::triggerRender()
{
   //vprASSERT(mThreadRunning == true);      // We must be running
   while(!mThreadRunning)
   {
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_HVERB_LVL) << "Waiting in for thread to start triggerRender.\n" << vprDEBUG_FLUSH;
      vpr::Thread::yield();
   }

   renderTriggerSema.release();
}

/**
 * Blocks until rendering of the windows is completed.
 * @post The pipe has completed its rendering.
 */
void GlPipe::completeRender()
{
   vprASSERT(mThreadRunning == true);      // We must be running

   renderCompleteSema.acquire();
}

/** Triggers swapping of all pipe's windows. */
void GlPipe::triggerSwap()
{
   vprASSERT(mThreadRunning == true);
   swapTriggerSema.release();
}

/** Blocks until swapping of the windows is completed. */
void GlPipe::completeSwap()
{
   vprASSERT(mThreadRunning == true);
   swapCompleteSema.acquire();
}


/**
 * Adds a GLWindow to the window list.
 * Control loop must now open the window on the next frame.
 */
void GlPipe::addWindow(GlWindow* win)
{
   vpr::Guard<vpr::Mutex> guardNew(mNewWinLock);       // Protect the data
   vprDEBUG(vrjDBG_DRAW_MGR,3) << "vjGlPipe::addWindow: Pipe: " << mPipeNum
                             << " adding window (to new wins):\n" << win
                             << std::endl << vprDEBUG_FLUSH;
   mNewWins.push_back(win);
}

/**
 * Removes a GLWindow from the window list.
 * @note The window is not actually removed until the next draw trigger.
 */
void GlPipe::removeWindow(GlWindow* win)
{
   vpr::Guard<vpr::Mutex> guardClosing(mClosingWinLock);
   vprDEBUG(vrjDBG_DRAW_MGR,3) << "vjGlPipe:: removeWindow: Pipe: " << mPipeNum
                             << " window added to closingWins.\n" << win
                             << std::endl << vprDEBUG_FLUSH;
   mClosingWins.push_back(win);
}


// The main loop routine
// while running <br>
// - Check for windows to open/close <br>
// - Wait for render signal <br>
// - Render all the windows <br>
// - Signal render completed <br>
// - Wait for swap signal <br>
// - Swap all windows <br>
// - Signal swap completed <br>
//
void GlPipe::controlLoop(void* nullParam)
{
   mThreadRunning = true;     // We are running so set flag

   while (!controlExit)
   {
         jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/startframe");

      checkForWindowsToClose();  // Checks for closing windows
      checkForNewWindows();      // Checks for new windows to open

      // --- handle EVENTS for the windows --- //
      // XXX: This may have to be here because of need to get open window event (Win32)
      // otherwise I would like to move it to being after the swap to get better performance
      {
         for(unsigned int winId=0;winId<mOpenWins.size();winId++)
            mOpenWins[winId]->checkEvents();
      }

      // --- RENDER the windows ---- //
      {
         renderTriggerSema.acquire();

         GlApp* the_app = glManager->getApp();

            jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/window events and render sema");
         // --- pipe PRE-draw function ---- //
         the_app->pipePreDraw();      // Can't get a context since I may not be guaranteed a window
            jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/app/pipePreDraw()");

         // Render the windows
         for (unsigned int winId=0;winId < mOpenWins.size();winId++) {
            renderWindow(mOpenWins[winId]);
         }
         renderCompleteSema.release();
      }

      // ----- SWAP the windows ------ //
      {
         swapTriggerSema.acquire();

         // Swap all the windows
         for(unsigned int winId=0;winId < mOpenWins.size();winId++)
            swapWindowBuffers(mOpenWins[winId]);

         swapCompleteSema.release();
      }

      jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/finish swap");
   }

   mThreadRunning = false;     // We are not running

}

/**
 * Closes all the windows in the list of windows to close.
 *
 * @post The window to close is removed from the list of open windows
 *       and the list of mNewWins.
 */
void GlPipe::checkForWindowsToClose()
{
   if(mClosingWins.size() > 0)   // If there are windows to close
   {
      vpr::Guard<vpr::Mutex> guardClosing(mClosingWinLock);
      vpr::Guard<vpr::Mutex> guardNew(mNewWinLock);
      vpr::Guard<vpr::Mutex> guardOpen(mOpenWinLock);

      for(unsigned int i=0;i<mClosingWins.size();i++)
      {
         GlWindow* win = mClosingWins[i];

         // Call contextClose
         GlApp* the_app = glManager->getApp();               // Get application for easy access
         //Display* the_display = win->getDisplay();         // Get the display for easy access
         glManager->setCurrentContext(win->getId());        // Set TS data of context id
         glManager->currentUserData()->setUser(NULL);       // Set user data
         glManager->currentUserData()->setProjection(NULL);
         glManager->currentUserData()->setViewport(NULL);   // Set vp data
         glManager->currentUserData()->setGlWindow(win);    // Set the gl window

         win->makeCurrent();              // Make the context current
         the_app->contextClose();          // Call context close function

         // Close the window
         win->close();

         // Remove it from the lists
         mNewWins.erase(std::remove(mNewWins.begin(), mNewWins.end(), win), mNewWins.end());
         mOpenWins.erase(std::remove(mOpenWins.begin(), mOpenWins.end(), win), mOpenWins.end());

         // Delete the window
         delete win;
         mClosingWins[i] = NULL;
      }

      mClosingWins.erase(mClosingWins.begin(), mClosingWins.end());
      vprASSERT(mClosingWins.size() == 0);;
   }
}

/**
 *  Checks for any new windows to add to the pipe.
 * @post Any new windows will be opened and added to the pipe's rendering list.
 */
void GlPipe::checkForNewWindows()
{
   if (mNewWins.size() > 0)  // If there are new windows added
   {
      vpr::Guard<vpr::Mutex> guardNew(mNewWinLock);
      vpr::Guard<vpr::Mutex> guardOpen(mOpenWinLock);

      for (unsigned int winNum=0; winNum<mNewWins.size(); winNum++)
      {
          if (mNewWins[winNum]->open()) {
              mNewWins[winNum]->makeCurrent();
              vprDEBUG(vrjDBG_DRAW_MGR,1) << "vjGlPipe::checkForNewWindows: Just opened window: "
                                        << mNewWins[winNum]->getDisplay()->getName().c_str()
                                        << std::endl << vprDEBUG_FLUSH;
              mOpenWins.push_back(mNewWins[winNum]);
          }
          else {
              vprDEBUG(vprDBG_ALL,0) << clrOutBOLD(clrRED,"ERROR:") << "vjGlPipe::checkForNewWindows: Failed to open window: "
                                   << mNewWins[winNum]->getDisplay()->getName().c_str()
                                   << std::endl << vprDEBUG_FLUSH;
              // BUG!!
              // should we do something to tell the current config that it
              // didn't get enabled properly?
          }
      }

      mNewWins.erase(mNewWins.begin(), mNewWins.end());
      vprASSERT(mNewWins.size() == 0);
   }
}

/**
 * Renders the window using OpenGL.
 * @post win is rendered (In stereo if it is a stereo window).
 */
void GlPipe::renderWindow(GlWindow* win)
{
   float vp_ox, vp_oy, vp_sx, vp_sy;            // Viewport origin and size
   Viewport::View  view;                      // The view for the active viewport

   GlApp* the_app = glManager->getApp();       // Get application for easy access
   Display* the_display = win->getDisplay();   // Get the display for easy access
   
   // Update the projections for the display using the current app's scale factor
   // NOTE: This relies upon no other thread trying to update this display at the same time
   float scale_factor = the_app->getDrawScaleFactor();
   the_display->updateProjections(scale_factor);
   
   glManager->setCurrentContext(win->getId());     // Set TSS data of context id

   vprDEBUG(vrjDBG_DRAW_MGR,5) << "vjGlPipe::renderWindow: Set context to: "
                             << GlDrawManager::instance()->getCurrentContext()
                             << std::endl << vprDEBUG_FLUSH;

   // --- SET CONTEXT --- //
   win->makeCurrent();

   // VIEWPORT cleaning
   if(win->hasDirtyViewport())
      win->updateViewport();

   jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/renderWindow/setup");


   // CONTEXT INIT(): Check if we need to call contextInit()
   // - Must call when context is new OR application is new
   if(win->hasDirtyContext())
   {
         // Have dirty context
      glManager->currentUserData()->setUser(NULL);         // Set user data
      glManager->currentUserData()->setProjection(NULL);
      glManager->currentUserData()->setViewport(NULL);     // Set vp data
      glManager->currentUserData()->setGlWindow(win);      // Set the gl window

      the_app->contextInit();              // Call context init function
      win->setDirtyContext(false);        // All clean now
   }
   jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/renderWindow/context setup");

   // BUFFER PRE DRAW: Check if we need to clear stereo buffers
   if(win->isStereo())
   {
      win->setViewBuffer(Viewport::RIGHT_EYE);
      the_app->bufferPreDraw();
      win->setViewBuffer(Viewport::LEFT_EYE);
      the_app->bufferPreDraw();
   }
   else
      the_app->bufferPreDraw();

   jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/renderWindow/app/bufferPreDraw");

   the_app->contextPreDraw();                 // Do any context pre-drawing

   jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/renderWindow/app/contextPreDraw");


   // --- FOR EACH VIEWPORT -- //
   Viewport* viewport = NULL;
   unsigned num_vps = the_display->getNumViewports();
   for(unsigned vp_num=0; vp_num < num_vps; vp_num++)
   {
      viewport = the_display->getViewport(vp_num);

      // Should viewport be rendered???
      if(viewport->isActive())
      {
         viewport->recordLatency (0, 1);

         view = viewport->getView();

         // Set the glViewport to draw within
         viewport->getOriginAndSize(vp_ox, vp_oy, vp_sx, vp_sy);
         win->setViewport(vp_ox, vp_oy, vp_sx, vp_sy);

         // Set user information
         glManager->currentUserData()->setUser(viewport->getUser());       // Set user data
         glManager->currentUserData()->setViewport(viewport);              // Set the viewport
         glManager->currentUserData()->setGlWindow(win);                   // Set the gl window

         jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/renderWindow/set viewport and user");

         // ---- SURFACE & Simulator --- //
         // if (viewport->isSurface())
         {
            SimViewport* sim_vp(NULL);

            if(viewport->isSimulator())
            {
               sim_vp = dynamic_cast<SimViewport*>(viewport);
               vprASSERT(NULL != sim_vp);
            }

            if((Viewport::STEREO == view) || (Viewport::LEFT_EYE == view))      // LEFT EYE
            {
               win->setViewBuffer(Viewport::LEFT_EYE);
               win->setProjection(viewport->getLeftProj());
               glManager->currentUserData()->setProjection(viewport->getLeftProj());

               jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/renderWindow/set left buffer and projection");

               the_app->draw();
                              
               jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/renderWindow/app/left draw()");

               glManager->drawObjects();
               if(NULL != sim_vp)
               {
                  glManager->drawSimulator(sim_vp, scale_factor);
               }

               jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/renderWindow/left draw objects and sim");

            }
            if ((Viewport::STEREO == view) || (Viewport::RIGHT_EYE == view))    // RIGHT EYE
            {
               win->setViewBuffer(Viewport::RIGHT_EYE);
               win->setProjection(viewport->getRightProj());
               glManager->currentUserData()->setProjection(viewport->getRightProj());

               jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/renderWindow/set right buffer and projection");

               the_app->draw();
                              
               jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/renderWindow/app/right draw()");

               glManager->drawObjects();
               if(NULL != sim_vp)
               {
                  glManager->drawSimulator(sim_vp,scale_factor);
               }

               jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/renderWindow/right draw objects and sim");
            }
         }
      }  // should viewport be rendered
   }     // for each viewport

   // -- Post context stuff --- //
   the_app->contextPostDraw();

   jcclTIMESTAMP (jcclPERF_ALL, "GlPipe/renderWindow/app/contextPostDraw()");
}

/**
 * Swaps the buffers of the given window.
 * Make the context current, and swap the window.
 */
void GlPipe::swapWindowBuffers(GlWindow* win)
{
   win->makeCurrent();           // Set correct context
   win->swapBuffers();           // Implicitly calls a glFlush
//     vjDisplayWindow* the_display = win->getDisplay();   // Get the display for easy access
//     the_display->recordLatency (2, 3);
}

};

