/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#include <vrj/Draw/OpenGL/Config.h>

#include <vpr/vpr.h>

#ifdef VPR_OS_Darwin
#   include <OpenGL/gl.h>
#else
#   include <GL/gl.h>
#endif

#include <boost/bind.hpp>

#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Guard.h>

#include <cluster/ClusterManager.h>

#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/OpenGL/App.h>
#include <vrj/Util/Debug.h>
#include <vrj/Display/SurfaceViewport.h>
#include <vrj/Display/SimViewport.h>
#include <vrj/Draw/OpenGL/SimInterface.h>

#if defined(VRJ_USE_COCOA)
#  include <vrj/Draw/OpenGL/WindowCocoa.h>
#endif

#include <vrj/Draw/OpenGL/Pipe.h>


namespace vrj
{

namespace opengl
{

Pipe::Pipe(const size_t num, vrj::opengl::DrawManager* glMgr,
           vpr::Mutex* drawMgrWinLock)
   : mActiveThread(NULL)
   , mThreadRunning(false)
   , mPipeNum(num)
   , mControlExit(false)
   , mGlDrawManager(glMgr)
   , mDrawMgrWinLock(drawMgrWinLock)
   , mRenderTriggerSema(0)
   , mRenderCompleteSema(0)
   , mSwapTriggerSema(0)
   , mSwapCompleteSema(0)
{
   /* Do nothihg. */ ;
}

Pipe::~Pipe()
{
   if ( NULL != mActiveThread )
   {
      delete mActiveThread;
      mActiveThread = NULL;
   }
}

// Starts the pipe running.
int Pipe::start(const int cpuAffinity)
{
   vprASSERT(mThreadRunning == false);        // We should not be running yet

   int started(0);

   // Create a new thread to call the control loop
   try
   {
      // mActiveThread is assigned at the start of controlLoop().
      vpr::Thread* thread =
         new vpr::Thread(boost::bind(&Pipe::controlLoop, this, cpuAffinity));
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << "[vrj::opengl::Pipe::start()] Started control loop. "
         << thread<< std::endl << vprDEBUG_FLUSH;
      started = 1;
   }
   catch (vpr::Exception& ex)
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": Failed to spawn thread for OpenGL pipe!\n"
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL)
         << ex.what() << std::endl << vprDEBUG_FLUSH;
   }

   return started;
}

//* Triggers rendering of the pipe to start.
void Pipe::triggerRender()
{
   //vprASSERT(mThreadRunning == true);      // We must be running
   while (!mThreadRunning)
   {
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_HVERB_LVL)
         << "Waiting in for thread to start triggerRender.\n"
         << vprDEBUG_FLUSH;
      vpr::Thread::yield();
   }

   mRenderTriggerSema.release();
}

// Blocks until rendering of the windows is completed.
void Pipe::completeRender()
{
   vprASSERT(mThreadRunning == true);      // We must be running

   mRenderCompleteSema.acquire();
}

// Triggers swapping of all pipe's windows.
void Pipe::triggerSwap()
{
   vprASSERT(mThreadRunning == true);
   mSwapTriggerSema.release();
}

// Blocks until swapping of the windows is completed.
void Pipe::completeSwap()
{
   vprASSERT(mThreadRunning == true);
   mSwapCompleteSema.acquire();
}

// Adds a vrj::opengl::Window to the window list.
// Control loop must now open the window on the next frame.
void Pipe::addWindow(vrj::opengl::WindowPtr win)
{
   vpr::Guard<vpr::Mutex> guardNew(mNewWinLock);       // Protect the data
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
      << "[vrj::opengl::Pipe::addWindow()] Pipe: " << mPipeNum
      << " adding window (to new wins):\n" << win
      << std::endl << vprDEBUG_FLUSH;
   mNewWins.push_back(win);
}

// Removes a GLWindow from the window list.
void Pipe::removeWindow(vrj::opengl::WindowPtr win)
{
   vpr::Guard<vpr::Mutex> guardClosing(mClosingWinLock);
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
      << "[vrj::opengl::Pipe::removeWindow()] Pipe: " << mPipeNum
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
void Pipe::controlLoop(const int cpuAffinity)
{
   vprASSERT(NULL != vpr::Thread::self());
   mActiveThread = vpr::Thread::self();

   if ( cpuAffinity >= 0 )
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
         << "[vrj::opengl::Pipe::controlLoop()] Setting CPU affinity for "
         << "pipe " << mPipeNum << " to " << cpuAffinity << std::endl
         << vprDEBUG_FLUSH;

      try
      {
         mActiveThread->setRunOn(cpuAffinity);
      }
      catch (vpr::Exception& ex)
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Failed to set draw thread affinity in vrj::opengl::Pipe:\n"
            << ex.what() << std::endl << vprDEBUG_FLUSH;
      }
   }

   mThreadRunning = true;     // We are running so set flag
   // Loop until flag set
   while ( ! mControlExit )
   {
      bool cluster(true);
      if (cluster::ClusterManager::instance()->isClusterActive())
      {
         cluster = cluster::ClusterManager::instance()->isClusterReady();
      }

      if (cluster)
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

            vrj::opengl::App* the_app = mGlDrawManager->getApp();

            // --- pipe PRE-draw function ---- //
            the_app->pipePreDraw();      // Can't get a context since I may not be guaranteed a window

#if defined(VRJ_USE_COCOA)
        boost::shared_ptr<vrj::opengl::WindowCocoa> cocoa_window;
#endif
            // Render the windows
            for (unsigned int winId = 0 ; winId < mOpenWins.size() ; winId++)
            {

#if defined(VRJ_USE_COCOA)
               cocoa_window = boost::static_pointer_cast<vrj::opengl::WindowCocoa>(mOpenWins[winId]);
               cocoa_window->acquireRenderLock();
#endif
               renderWindow(mOpenWins[winId]);
#if defined(VRJ_USE_COCOA)
               cocoa_window->releaseRenderLock();
#endif
            }
            mRenderCompleteSema.release();
         }

         // ----- SWAP the windows ------ //
         {
            mSwapTriggerSema.acquire();

#if defined(VRJ_USE_COCOA)
         boost::shared_ptr<vrj::opengl::WindowCocoa> cocoa_window;
#endif
         // Swap all the windows
            for (unsigned int winId = 0 ; winId < mOpenWins.size() ; winId++)
            {
#if defined(VRJ_USE_COCOA)
               cocoa_window = boost::static_pointer_cast<vrj::opengl::WindowCocoa>(mOpenWins[winId]);
               cocoa_window->acquireRenderLock();
#endif
               swapWindowBuffers(mOpenWins[winId]);
#if defined(VRJ_USE_COCOA)
               cocoa_window->releaseRenderLock();
#endif
            }

            mSwapCompleteSema.release();
         }
      }
      checkForWindowsToClose();  // Checks for closing windows
      checkForNewWindows();      // Checks for new windows to open
   }

   mThreadRunning = false;     // We are not running
}

void Pipe::stop()
{
   // Close all open windows/contexts.
   std::vector<vrj::opengl::WindowPtr> windows = getOpenWindows();
   typedef std::vector<vrj::opengl::WindowPtr>::iterator iter_t;
   for ( iter_t itr = windows.begin(); itr != windows.end(); ++itr )
   {
      removeWindow(*itr);
   }

   mControlExit = true;     // Set the control loop exit flag
   
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
void Pipe::checkForWindowsToClose()
{
   if (mClosingWins.size() > 0)   // If there are windows to close
   {
      vpr::Guard<vpr::Mutex> guardClosing(mClosingWinLock);
      vpr::Guard<vpr::Mutex> guardNew(mNewWinLock);
      vpr::Guard<vpr::Mutex> guardOpen(mOpenWinLock);

      for (unsigned int i=0;i<mClosingWins.size();i++)
      {
         vrj::opengl::WindowPtr win = mClosingWins[i];
        
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
            << "[vrj::opengl::Pipe::checkForWindowsToClose()] Just closed "
            << "window: " << mClosingWins[i]->getDisplay()->getName()
            << std::endl << *(mClosingWins[i]) << std::endl << vprDEBUG_FLUSH;

         // Call contextClose
         vrj::opengl::App* the_app = mGlDrawManager->getApp();   // Get application for easy access
         //DisplayPtr the_display = win->getDisplay();           // Get the display for easy access
         mGlDrawManager->setCurrentContext(win->getId());        // Set TS data of context id
         mGlDrawManager->currentUserData()->setUser(UserPtr());  // Set user data
         mGlDrawManager->currentUserData()->setProjection(ProjectionPtr());
         mGlDrawManager->currentUserData()->setViewport(ViewportPtr()); // Set vp data
         mGlDrawManager->currentUserData()->setGlWindow(win);    // Set the gl window

         win->makeCurrent();              // Make the context current
         the_app->contextClose();          // Call context close function

         // Close the window
         win->close();

         // Remove it from the lists
         mNewWins.erase(std::remove(mNewWins.begin(), mNewWins.end(), win), mNewWins.end());
         mOpenWins.erase(std::remove(mOpenWins.begin(), mOpenWins.end(), win), mOpenWins.end());
      }

      mClosingWins.clear();
      vprASSERT(mClosingWins.size() == 0);;
   }
}

/**
 *  Checks for any new windows to add to the pipe.
 * @post Any new windows will be opened and added to the pipe's rendering list.
 */
void Pipe::checkForNewWindows()
{
   if (mNewWins.size() > 0)  // If there are new windows added
   {
      // Cross-pipe lock to prevent a window from opening on another pipe
      // while we are opening ours.
      vpr::Guard<vpr::Mutex> guard_draw_mgr(*mDrawMgrWinLock);
      // Pipe-specific lock to prevent mNewWins from being modified while
      // the new windows are being opened.
      vpr::Guard<vpr::Mutex> guard_new(mNewWinLock);
      // Pipe-specific lock to prevent checkForWindowsToClose() from doing
      // any work while new windows are being opened.
      vpr::Guard<vpr::Mutex> guard_open(mOpenWinLock);

      for (unsigned int winNum=0; winNum<mNewWins.size(); winNum++)
      {
         if (mNewWins[winNum]->open())
         {
            // Inform the ClusterManager that a window opened on this node.
            cluster::ClusterManager::instance()->windowOpened();

            mNewWins[winNum]->makeCurrent();
            vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
               << "[vrj::opengl::Pipe::checkForNewWindows()] Just opened "
               << "window: " << mNewWins[winNum]->getDisplay()->getName()
               << std::endl << *(mNewWins[winNum]) << std::endl
               << vprDEBUG_FLUSH;
            mNewWins[winNum]->finishSetup();        // Complete any window open stuff
            mOpenWins.push_back(mNewWins[winNum]);
         }
         else
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << clrOutBOLD(clrRED,"ERROR:")
               << " [vrj::opengl::Pipe::checkForNewWindows()] Failed to open "
               << "window: " << mNewWins[winNum]->getDisplay()->getName()
               << std::endl << vprDEBUG_FLUSH;
            // XXX: We should handle this error more gracefully
         } // Done
      }

      mNewWins.erase(mNewWins.begin(), mNewWins.end());
      vprASSERT(mNewWins.size() == 0);
   }
}

// Renders the window using OpenGL.
void Pipe::renderWindow(vrj::opengl::WindowPtr win)
{
   float vp_ox, vp_oy, vp_sx, vp_sy;        // Viewport origin and size
   Viewport::View  view;                    // The view for the active viewport
   bool vp_dirty;                           // True if viewport is dirty

   vrj::opengl::App* the_app = mGlDrawManager->getApp(); // Get application for easy access
   DisplayPtr the_display = win->getDisplay();   // Get the display for easy access

   // Update the projections for the display using the current app's scale factor
   // NOTE: This relies upon no other thread trying to update this display at the same time
   const float scale_factor = the_app->getDrawScaleFactor();
   the_display->updateProjections(scale_factor);

   mGlDrawManager->setCurrentContext(win->getId());     // Set TSS data of context id
   mGlDrawManager->currentUserData()->setGlWindow(win);

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_HVERB_LVL)
      << "[vrj::opengl::Pipe::renderWindow()] Set context to: "
      << vrj::opengl::DrawManager::instance()->getCurrentContext()
      << std::endl << vprDEBUG_FLUSH;

   // --- SET CONTEXT --- //
   win->makeCurrent();
 

   // VIEWPORT cleaning
   vp_dirty = win->hasDirtyViewport();
   if (vp_dirty)
   {
      win->updateViewport();
   }

   const size_t num_vps = the_display->getNumViewports();

   // CONTEXT INIT(): Check if we need to call contextInit()
   // - Must call when context is new OR application is new
   if (win->hasDirtyContext())
   {
      // Have dirty context
      mGlDrawManager->currentUserData()->setUser(UserPtr());    // Set user data
      mGlDrawManager->currentUserData()->setProjection(ProjectionPtr());
      mGlDrawManager->currentUserData()->setViewport(ViewportPtr()); // Set vp data
      mGlDrawManager->currentUserData()->setGlWindow(win);      // Set the gl window

      the_app->contextInit();              // Call context init function

      // Invoke vrj::opengl::SimInterface::contextInit() for all simulator
      // viewports.
      for ( size_t vp_num = 0; vp_num < num_vps; ++vp_num )
      {
         ViewportPtr viewport = the_display->getViewport(vp_num);

         if ( viewport->isActive() && viewport->isSimulator() )
         {
            SimViewportPtr sim_vp =
               boost::dynamic_pointer_cast<SimViewport>(viewport);
            vprASSERT(NULL != sim_vp.get());

            vrj::opengl::SimInterfacePtr draw_sim_i =
               boost::dynamic_pointer_cast<vrj::opengl::SimInterface>(
                  sim_vp->getDrawSimInterface()
               );

            if ( NULL != draw_sim_i.get() )
            {
               draw_sim_i->contextInit();
            }
         }
      }

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
   for ( size_t vp_num = 0; vp_num < num_vps; ++vp_num )
   {
      ViewportPtr viewport = the_display->getViewport(vp_num);

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
         vrj::opengl::SimInterfacePtr draw_sim_i;
         
         // Note: static casts below could fail if type enum does not 
         // match the type of the cast.
         if (viewport->isSimulator())
         {
            SimViewportPtr sim_vp =
               boost::static_pointer_cast<SimViewport>(viewport);
            draw_sim_i =
               boost::dynamic_pointer_cast<vrj::opengl::SimInterface>(
                  sim_vp->getDrawSimInterface());
         }
         else if ( vp_dirty && viewport->isSurface() )
         {
            SurfaceViewportPtr surf_vp = 
              boost::static_pointer_cast<SurfaceViewport>(viewport);
            surf_vp->updateCorners();
         }

         if ((Viewport::STEREO == view) || (Viewport::LEFT_EYE == view))      // LEFT EYE
         {
            win->setViewBuffer(Viewport::LEFT_EYE);
            win->setProjection(viewport->getLeftProj());
            mGlDrawManager->currentUserData()->setProjection(viewport->getLeftProj());

            the_app->draw();

            if ( NULL != draw_sim_i.get() )
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

            if ( NULL != draw_sim_i.get() )
            {
               draw_sim_i->draw(scale_factor);
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
void Pipe::swapWindowBuffers(vrj::opengl::WindowPtr win)
{
   win->makeCurrent();           // Set correct context
   win->swapBuffers();           // Implicitly calls a glFlush
}

}

}
