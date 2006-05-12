/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <vrj/vrjConfig.h>

#include <vpr/vpr.h>

#ifdef VPR_OS_Darwin
#   include <OpenGL/gl.h>
#else
#   include <GL/gl.h>
#endif

#include <boost/bind.hpp>

#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/OGL/GlPipe.h>
#include <vrj/Draw/OGL/GlApp.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Guard.h>
#include <vrj/Util/Debug.h>

#include <vrj/Display/SurfaceViewport.h>
#include <vrj/Display/SimViewport.h>
#include <vrj/Draw/OGL/GlSimInterface.h>


namespace vrj
{

GlPipe::~GlPipe()
{
   if ( NULL != mActiveThread )
   {
      delete mActiveThread;
      mActiveThread = NULL;
   }
}

/**
 * Starts the pipe running.
 * @post The pipe has it's own thread of control and is ready to operate.
 */
int GlPipe::start()
{
    vprASSERT(mThreadRunning == false);        // We should not be running yet

    // Create a new thread to call the control loop
    mActiveThread = new vpr::Thread(boost::bind(&GlPipe::controlLoop, this));

    vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
       << "[vrj::GlPipe::start()] Started control loop. " << mActiveThread
       << std::endl << vprDEBUG_FLUSH;
    return 1;        // XXX: Is this always true?
}

/**
 * Triggers rendering of the pipe to start.
 * @post The pipe has be told to start rendering.
 */
void GlPipe::triggerRender()
{
   //vprASSERT(mThreadRunning == true);      // We must be running
   while (!mThreadRunning)
   {
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_HVERB_LVL) << "Waiting in for thread to start triggerRender.\n" << vprDEBUG_FLUSH;
      vpr::Thread::yield();
   }

   mRenderTriggerSema.release();
}

/**
 * Blocks until rendering of the windows is completed.
 * @post The pipe has completed its rendering.
 */
void GlPipe::completeRender()
{
   vprASSERT(mThreadRunning == true);      // We must be running

   mRenderCompleteSema.acquire();
}

/** Triggers swapping of all pipe's windows. */
void GlPipe::triggerSwap()
{
   vprASSERT(mThreadRunning == true);
   mSwapTriggerSema.release();
}

/** Blocks until swapping of the windows is completed. */
void GlPipe::completeSwap()
{
   vprASSERT(mThreadRunning == true);
   mSwapCompleteSema.acquire();
}


/**
 * Adds a GLWindow to the window list.
 * Control loop must now open the window on the next frame.
 */
void GlPipe::addWindow(GlWindow* win)
{
   vpr::Guard<vpr::Mutex> guardNew(mNewWinLock);       // Protect the data
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
      << "[vrj::GlPipe::addWindow()] Pipe: " << mPipeNum
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
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
      << "[vrj::GlPipe::removeWindow()] Pipe: " << mPipeNum
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
void GlPipe::controlLoop()
{
   mThreadRunning = true;     // We are running so set flag
   // Loop until flag set
   while (!mControlExit)
   {
      // --- handle EVENTS for the windows --- //
      // XXX: This may have to be here because of need to get open window event (Win32)
      // otherwise I would like to move it to being after the swap to get better performance
      {
         for (unsigned int winId=0;winId<mOpenWins.size();winId++)
         {
            mOpenWins[winId]->checkEvents();
         }
      }

      // --- RENDER the windows ---- //
      {
         mRenderTriggerSema.acquire();

         GlApp* the_app = mGlDrawManager->getApp();

         // --- pipe PRE-draw function ---- //
         the_app->pipePreDraw();      // Can't get a context since I may not be guaranteed a window

         // Render the windows
         for (unsigned int winId = 0 ; winId < mOpenWins.size() ; winId++)
         {
            renderWindow(mOpenWins[winId]);
         }
         mRenderCompleteSema.release();
      }

      // ----- SWAP the windows ------ //
      {
         mSwapTriggerSema.acquire();

         // Swap all the windows
         for (unsigned int winId = 0 ; winId < mOpenWins.size() ; winId++)
         {
            swapWindowBuffers(mOpenWins[winId]);
         }

         mSwapCompleteSema.release();
      }
      checkForWindowsToClose();  // Checks for closing windows
      checkForNewWindows();      // Checks for new windows to open
   }

   mThreadRunning = false;     // We are not running
}

void GlPipe::stop()
{
   // Close all open windows/contexts.
   std::vector<GlWindow*> windows = getOpenWindows();
   for ( std::vector<GlWindow*>::iterator itr = windows.begin();
         itr != windows.end();
         ++itr )
   {
      removeWindow(*itr);
   }

   mControlExit = 1;     // Set the control loop exit flag
   
   // We don't actually need to call completeRender() or completeSwap()
   // since we don't care about when they complete. We only care about
   // joining the thread
   triggerRender();
   //completeRender();
   triggerSwap();
   //completeSwap();

   mActiveThread->join();
}

/**
 * Closes all the windows in the list of windows to close.
 *
 * @post The window to close is removed from the list of open windows
 *       and the list of mNewWins.
 */
void GlPipe::checkForWindowsToClose()
{
   if (mClosingWins.size() > 0)   // If there are windows to close
   {
      vpr::Guard<vpr::Mutex> guardClosing(mClosingWinLock);
      vpr::Guard<vpr::Mutex> guardNew(mNewWinLock);
      vpr::Guard<vpr::Mutex> guardOpen(mOpenWinLock);

      for (unsigned int i=0;i<mClosingWins.size();i++)
      {
         GlWindow* win = mClosingWins[i];
        
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
            << "[vrj::GlPipe::checkForNewWindows()] Just closed window: "
            << mClosingWins[i]->getDisplay()->getName() << std::endl
            << *(mClosingWins[i]) << std::endl << vprDEBUG_FLUSH;

         // Call contextClose
         GlApp* the_app = mGlDrawManager->getApp();               // Get application for easy access
         //Display* the_display = win->getDisplay();         // Get the display for easy access
         mGlDrawManager->setCurrentContext(win->getId());        // Set TS data of context id
         mGlDrawManager->currentUserData()->setUser(NULL);       // Set user data
         mGlDrawManager->currentUserData()->setProjection(NULL);
         mGlDrawManager->currentUserData()->setViewport(NULL);   // Set vp data
         mGlDrawManager->currentUserData()->setGlWindow(win);    // Set the gl window

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
          if (mNewWins[winNum]->open())
          {
              mNewWins[winNum]->makeCurrent();
              vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
                 << "[vrj::GlPipe::checkForNewWindows()] Just opened window: "
                 << mNewWins[winNum]->getDisplay()->getName() << std::endl
                 << *(mNewWins[winNum]) << std::endl << vprDEBUG_FLUSH;
              mNewWins[winNum]->finishSetup();        // Complete any window open stuff
              mOpenWins.push_back(mNewWins[winNum]);
          }
          else
          {
              vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                 << clrOutBOLD(clrRED,"ERROR:")
                 << " vrj::GlPipe::checkForNewWindows(): Failed to open window: "
                 << mNewWins[winNum]->getDisplay()->getName().c_str()
                 << std::endl << vprDEBUG_FLUSH;
              // XXX: We should handle this error more gracefully
          }   // Done
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

   GlApp* the_app = mGlDrawManager->getApp();       // Get application for easy access
   Display* the_display = win->getDisplay();   // Get the display for easy access

   // Update the projections for the display using the current app's scale factor
   // NOTE: This relies upon no other thread trying to update this display at the same time
   float scale_factor = the_app->getDrawScaleFactor();
   the_display->updateProjections(scale_factor);

   mGlDrawManager->setCurrentContext(win->getId());     // Set TSS data of context id
   mGlDrawManager->currentUserData()->setGlWindow(win);

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_HVERB_LVL)
      << "[vrj::GlPipe::renderWindow()] Set context to: "
      << GlDrawManager::instance()->getCurrentContext()
      << std::endl << vprDEBUG_FLUSH;

   // --- SET CONTEXT --- //
   win->makeCurrent();
 

   // VIEWPORT cleaning
   if (win->hasDirtyViewport())
   {
      win->updateViewport();
   }

   // CONTEXT INIT(): Check if we need to call contextInit()
   // - Must call when context is new OR application is new
   if (win->hasDirtyContext())
   {
      // Have dirty context
      mGlDrawManager->currentUserData()->setUser(NULL);         // Set user data
      mGlDrawManager->currentUserData()->setProjection(NULL);
      mGlDrawManager->currentUserData()->setViewport(NULL);     // Set vp data
      mGlDrawManager->currentUserData()->setGlWindow(win);      // Set the gl window

      the_app->contextInit();              // Call context init function
      win->setDirtyContext(false);        // All clean now
   }

   // BUFFER PRE DRAW: Check if we need to clear stereo buffers
   if (win->isStereo())
   {
      win->setViewBuffer(Viewport::RIGHT_EYE);
      the_app->bufferPreDraw();
      win->setViewBuffer(Viewport::LEFT_EYE);
      the_app->bufferPreDraw();
   }
   else
   {
      the_app->bufferPreDraw();
   }

   the_app->contextPreDraw();                 // Do any context pre-drawing

   // --- FOR EACH VIEWPORT -- //
   Viewport* viewport = NULL;
   size_t num_vps = the_display->getNumViewports();
   for ( size_t vp_num = 0; vp_num < num_vps; ++vp_num )
   {
      viewport = the_display->getViewport(vp_num);

      // Should viewport be rendered???
      if (viewport->isActive())
      {
         view = viewport->getView();

         // Set the glViewport to draw within
         viewport->getOriginAndSize(vp_ox, vp_oy, vp_sx, vp_sy);
         win->setViewport(vp_ox, vp_oy, vp_sx, vp_sy);

         // Set user information
         mGlDrawManager->currentUserData()->setUser(viewport->getUser());       // Set user data
         mGlDrawManager->currentUserData()->setViewport(viewport);              // Set the viewport

         // ---- SURFACE & Simulator --- //
         // if (viewport->isSurface())
         {
            SimViewport*      sim_vp(NULL);
            GlSimInterface*   draw_sim_i(NULL);

            if (viewport->isSimulator())
            {
               sim_vp = dynamic_cast<SimViewport*>(viewport);
               vprASSERT(NULL != sim_vp);
               if (NULL != sim_vp)
               {
                  draw_sim_i = dynamic_cast<GlSimInterface*>(sim_vp->getDrawSimInterface());
               }
            }

            if ((Viewport::STEREO == view) || (Viewport::LEFT_EYE == view))      // LEFT EYE
            {
               win->setViewBuffer(Viewport::LEFT_EYE);
               win->setProjection(viewport->getLeftProj());
               mGlDrawManager->currentUserData()->setProjection(viewport->getLeftProj());

               the_app->draw();

               if (NULL != draw_sim_i)
               {
                  draw_sim_i->draw(scale_factor);
               }
            }
            if ((Viewport::STEREO == view) || (Viewport::RIGHT_EYE == view))    // RIGHT EYE
            {
               win->setViewBuffer(Viewport::RIGHT_EYE);
               win->setProjection(viewport->getRightProj());
               mGlDrawManager->currentUserData()->setProjection(viewport->getRightProj());

               the_app->draw();

               if (NULL != draw_sim_i)
               {
                  draw_sim_i->draw(scale_factor);
               }
            }
         }
      }  // should viewport be rendered
   }     // for each viewport

   // -- Post context stuff --- //
   the_app->contextPostDraw();
}

/**
 * Swaps the buffers of the given window.
 * Make the context current, and swap the window.
 */
void GlPipe::swapWindowBuffers(GlWindow* win)
{
   win->makeCurrent();           // Set correct context
   win->swapBuffers();           // Implicitly calls a glFlush
}

}
