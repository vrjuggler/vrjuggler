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

#include <vrj/Draw/Direct3D/Config.h>

#include <stdlib.h>
#include <boost/bind.hpp>
#include <boost/concept_check.hpp>

#include <gmtl/Vec.h>
#include <gmtl/Output.h>

#include <jccl/Config/ConfigElement.h>

#include <cluster/ClusterManager.h>

#include <vrj/Display/DisplayManager.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Kernel/KernelExceptions.h>

#include <vrj/Display/Display.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Display/SimViewport.h>
#include <vrj/Display/SurfaceViewport.h>

#include <vrj/Draw/Direct3D/D3dApp.h>
#include <vrj/Draw/Direct3D/D3dWindow.h>
#include <vrj/Draw/Direct3D/D3dSimInterfaceFactory.h>
#include <vrj/Draw/Direct3D/D3dSimInterface.h>
#include <vrj/Util/Debug.h>

#include <vrj/Draw/Direct3D/D3dDrawManager.h>


namespace vrj
{
vprSingletonImp(D3dDrawManager);

D3dDrawManager::D3dDrawManager()
   : mApp(NULL)
   , drawTriggerSema(0)
   , drawDoneSema(0)
   , mRunning(false)
   , mControlThread(NULL)
{
}

D3dDrawManager::~D3dDrawManager()
{
   if ( mRunning )
   {
      closeAPI();
      mControlThread->join();
   }

   if ( NULL != mControlThread )
   {
      delete mControlThread;
      mControlThread = NULL;
   }
}

/** Sets the app the draw should interact with. */
void D3dDrawManager::setApp(App* _app)
{
   mApp = dynamic_cast<D3dApp*>(_app);

   // We have a new app, so the contexts must be re-initialized
   // so... dirty them all.
   dirtyAllWindows();

   if ( NULL == mApp )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR:")
         << " [vrj::D3dDrawManager::setApp()] Failed to downcast "
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << "application object from vrj::App to vrj::GlApp!" << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << "Type of object " << std::hex << _app << std::dec << " is "
         << typeid(_app).name() << std::endl << vprDEBUG_FLUSH;

      throw vrj::DrawMgrException("Object not of type vrj::GlApp",
                                  VPR_LOCATION);
   }
}

/** Returns the app we are rednering. */
D3dApp* D3dDrawManager::getApp()
{
   return mApp;
}

/** Starts the control loop. */
void D3dDrawManager::start()
{
   // --- Setup Multi-Process stuff --- //
   // Create a new thread to handle the control
   mRunning = true;

   // NOTE: Any exception thrown by spawning the thread will be propagated up
   // to the caller.
   mControlThread = new vpr::Thread(boost::bind(&D3dDrawManager::main, this));

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << "vrj::D3dDrawManager started (thread: " << mControlThread << ")\n"
      << vprDEBUG_FLUSH;
}


// Enable a frame to be drawn
// Trigger draw
void D3dDrawManager::draw()
{
   drawTriggerSema.release();
}


/**
 * Blocks until the end of the frame.
 * @post The frame has been drawn.
 */
void D3dDrawManager::sync()
{
   drawDoneSema.acquire();
}


/** This is the control loop for the manager. */
void D3dDrawManager::main()
{
   bool stop_requested(false);

   while ( ! stop_requested )
   {
      //**// Runtime config will happen sometime after
      // drawDoneSema.release() because the kernel is the only one that
      // can trigger it

      // Wait for trigger
      drawTriggerSema.acquire();

      // To properly stop this thread we must allow it to fall through the
      // semaphore and not execute drawAllPipes(). Test mRunning here only
      // and not outside of drawTriggerSema.acquire() / drawDoneSema.release()
      // so we know we will always reach drawTriggerSema.acquire() when
      // closing, since closeAPI() expects a final drawTriggerSema.acquire()
      // and drawDoneSema.release().
      if (mRunning)
      {
         // THEN --- Do Rendering --- //
         drawAllPipes();
      }
      else
      {
         stop_requested = true;
      }

      // -- Done rendering --- //
      drawDoneSema.release();
   }
}

/**
 * Renders the window using OpenGL.
 * @post win is rendered (In stereo if it is a stereo window).
 */
void D3dDrawManager::renderWindow(D3dWindow* win)
{
   float vp_ox, vp_oy, vp_sx, vp_sy;    // Viewport origin and size
   Viewport::View view;                 // The view for the active viewport

   DisplayPtr the_display = win->getDisplay();   // Get the display for easy access

   // Update the projections for the display using the current app's scale factor
   // NOTE: This relies upon no other thread trying to update this display at the same time
   float scale_factor = mApp->getDrawScaleFactor();
   the_display->updateProjections(scale_factor);

   //mGlDrawManager->setCurrentContext(win->getId());     // Set TSS data of context id
   //mGlDrawManager->currentUserData()->setGlWindow(win);

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
      //mGlDrawManager->currentUserData()->setUser(NULL);         // Set user data
      //mGlDrawManager->currentUserData()->setProjection(NULL);
      //mGlDrawManager->currentUserData()->setViewport(NULL);     // Set vp data
      //mGlDrawManager->currentUserData()->setGlWindow(win);      // Set the gl window

      mApp->contextInit(win->mRenderDevice);   // Call context init function
      win->setDirtyContext(false);        // All clean now
   }

   // BUFFER PRE DRAW: Check if we need to clear stereo buffers
   if (win->isStereo())
   {
      win->setViewBuffer(Viewport::RIGHT_EYE);
      mApp->bufferPreDraw();
      win->setViewBuffer(Viewport::LEFT_EYE);
      mApp->bufferPreDraw();
   }
   else
   {
      mApp->bufferPreDraw();
   }

   mApp->contextPreDraw();                 // Do any context pre-drawing

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
         //mGlDrawManager->currentUserData()->setUser(viewport->getUser());       // Set user data
         //mGlDrawManager->currentUserData()->setViewport(viewport);              // Set the viewport

         // ---- SURFACE & Simulator --- //
         // if (viewport->isSurface())
         {
            SimViewport*       sim_vp(NULL);
            D3dSimInterfacePtr draw_sim_i;

            if (viewport->isSimulator())
            {
               sim_vp = dynamic_cast<SimViewport*>(viewport);
               vprASSERT(NULL != sim_vp);
               if (NULL != sim_vp)
               {
                  draw_sim_i =
                     boost::dynamic_pointer_cast<D3dSimInterface>(
                        sim_vp->getDrawSimInterface()
                     );
               }
            }

            if ((Viewport::STEREO == view) || (Viewport::LEFT_EYE == view))      // LEFT EYE
            {
               win->setViewBuffer(Viewport::LEFT_EYE);
               win->setProjection(viewport->getLeftProj());
               //mGlDrawManager->currentUserData()->setProjection(viewport->getLeftProj());

               mApp->draw(win->mRenderDevice);

               if ( NULL != draw_sim_i.get() )
               {
                  draw_sim_i->draw(scale_factor);
               }
            }
            if ((Viewport::STEREO == view) || (Viewport::RIGHT_EYE == view))    // RIGHT EYE
            {
               win->setViewBuffer(Viewport::RIGHT_EYE);
               win->setProjection(viewport->getRightProj());
               //mGlDrawManager->currentUserData()->setProjection(viewport->getRightProj());

               mApp->draw(win->mRenderDevice);

               if ( NULL != draw_sim_i.get() )
               {
                  draw_sim_i->draw(scale_factor);
               }
            }
         }
      }  // should viewport be rendered
   }     // for each viewport

   // -- Post context stuff --- //
   mApp->contextPostDraw();
}

/**
 * Swaps the buffers of the given window.
 * Make the context current, and swap the window.
 */
void D3dDrawManager::swapWindowBuffers(D3dWindow* win)
{
   win->makeCurrent();           // Set correct context
   win->swapBuffers();           // Implicitly calls a glFlush
}

void D3dDrawManager::drawAllPipes()
{
   vprDEBUG_OutputGuard(vrjDBG_DRAW_MGR, vprDBG_HVERB_LVL,
                        "vrj::GLDrawManager::drawAllPipes()\n",
                        "vrj::GLDrawManager::drawAllPipes() done.\n");

   checkForNewWindows();
   for (unsigned int i = 0; i < mOpenWins.size(); i++)
   {
      D3dWindow* win = mOpenWins[i];
      win->checkEvents();
      renderWindow(win);
      swapWindowBuffers(win);
   }

}

/**
 * Initializes the drawing API (if not already running).
 * @post Control thread is started.
 */
void D3dDrawManager::initAPI()
{
   start();
}


/**
 * Callback when display is added to display manager.
 * @pre Must be in kernel controlling thread.
 * @note This function can only be called by the display manager
 *       functioning on behalf of a thread the holds the kernel
 *       reconfiguration lock.
 *       This guarantees that we are not rendering currently.
 *       We will most likely be waiting for a render trigger.
 */
void D3dDrawManager::addDisplay(DisplayPtr disp)
{
   vprASSERT(disp != NULL);    // Can't add a null display

   vprDEBUG(vrjDBG_DRAW_MGR, 0)
      << "========= vrj::D3dDrawManager::addDisplay: " << disp
      << std::endl << vprDEBUG_FLUSH;

   // -- Finish Simulator setup
   std::vector<vrj::Viewport*>::size_type num_vp(disp->getNumViewports());
   std::vector<vrj::Viewport*>::size_type i;

   for ( i = 0 ; i < num_vp ; ++i )
   {
      Viewport* vp = disp->getViewport(i);

      if (vp->isSimulator())
      {
         jccl::ConfigElementPtr vp_element = vp->getConfigElement();

         SimViewport* sim_vp(NULL);
         sim_vp = dynamic_cast<SimViewport*>(vp);
         vprASSERT(NULL != sim_vp);

         sim_vp->setDrawSimInterface(DrawSimInterfacePtr());

         // Create the simulator stuff
         vprASSERT(1 == vp_element->getNum("simulator_plugin") && "You must supply a simulator plugin.");

         // Create the simulator stuff
         jccl::ConfigElementPtr sim_element =
            vp_element->getProperty<jccl::ConfigElementPtr>("simulator_plugin");

         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
            << "D3dDrawManager::addDisplay() creating simulator of type '"
            << sim_element->getID() << "'\n" << vprDEBUG_FLUSH;

         DrawSimInterfacePtr new_sim_i(
            D3dSimInterfaceFactory::instance()->createObject(sim_element->getID())
         );

         // XXX: Change this to an error once the new simulator loading code is
         // more robust.  -PH (4/13/2003)
         vprASSERT(NULL != new_sim_i.get() &&
                   "Failed to create draw simulator");
         sim_vp->setDrawSimInterface(new_sim_i);
         new_sim_i->initialize(sim_vp);
         new_sim_i->config(sim_element);
      }
   }

   // -- Create a window for new display
   // -- Store the window in the wins vector
   // Create the gl window object.  NOTE: The glPipe actually "creates" the opengl window and context later
   D3dWindow* new_win = new D3dWindow();
   new_win->configWindow(disp);                                            // Configure it
   mNewWins.push_back(new_win);                                         // Add to our local window list

   //vprASSERT(isValidWindow(new_win));      // Make sure it was added to draw manager
}


/**
 * Callback when display is removed to display manager.
 * @pre disp must be a valid display that we have.
 * @post window for disp is removed from the draw manager and child pipes.
 */
void D3dDrawManager::removeDisplay(DisplayPtr disp)
{
   /*
   GlPipe* pipe;  pipe = NULL;
   GlWindow* win; win = NULL;     // Window to remove

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
      << "vrj::D3dDrawManager::removeDisplay: " << disp
      << std::endl << vprDEBUG_FLUSH;

   for (unsigned int i=0;i<mWins.size();i++)
   {
      if (mWins[i]->getDisplay() == disp)      // FOUND it
      {
         win = mWins[i];
         pipe = pipes[win->getDisplay()->getPipe()];
      }
   }

   // Remove the window from the pipe and our local list
   if (win != NULL)
   {
      vprASSERT(pipe != NULL);
      vprASSERT(isValidWindow(win));
      pipe->removeWindow(win);                                                   // Remove from pipe
      mWins.erase(std::remove(mWins.begin(),mWins.end(),win), mWins.end());      // Remove from draw manager
      vprASSERT(!isValidWindow(win));
   }
   else
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:")
         << "vrj::D3dDrawManager::removeDisplay: Attempted to remove a display that was not found.\n"
         << vprDEBUG_FLUSH;
      vprASSERT(false);
   }
   */
}

/**
 *  Checks for any new windows to add to the pipe.
 * @post Any new windows will be opened and added to the pipe's rendering list.
 */
void D3dDrawManager::checkForNewWindows()
{
   if (mNewWins.size() > 0)  // If there are new windows added
   {
      // Cross-pipe lock to prevent a window from opening on another pipe
      // while we are opening ours.
      //vpr::Guard<vpr::Mutex> guard_draw_mgr(*mDrawMgrWinLock);
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
              mNewWins[winNum]->makeCurrent();
              vprDEBUG(vrjDBG_DRAW_MGR, 0)
                 << "[vrj::GlPipe::checkForNewWindows()] Just opened window: "
                 << mNewWins[winNum]->getDisplay()->getName() << std::endl
                 << *(mNewWins[winNum]) << std::endl << vprDEBUG_FLUSH;
              mNewWins[winNum]->finishSetup();        // Complete any window open stuff
              mOpenWins.push_back(mNewWins[winNum]);
          }
          else
          {
              vprDEBUG(vprDBG_ALL, 0)
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

/** Shutdown the drawing API */
void D3dDrawManager::closeAPI()
{
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL) << "vrj::D3dDrawManager::closeAPI\n" << vprDEBUG_FLUSH;

   mRunning = false;
   
   // We must allow our control thread to fall through and die naturally.
   drawTriggerSema.release(); 	 
   drawDoneSema.acquire();
   mControlThread->join();

   for (unsigned int i = 0; i < mOpenWins.size(); i++)
   {
      D3dWindow* win = mOpenWins[i];
      win->close();
      delete win;
      mOpenWins[i] = NULL;
   }
   mOpenWins.clear();
}

/**
 * Adds the element to the draw manager config.
 * @pre configCanHandle(element) == true.
 * @post The elements have reconfigured the system.
 */
bool D3dDrawManager::configAdd(jccl::ConfigElementPtr element)
{
   boost::ignore_unused_variable_warning(element);
   return false;
}

/**
 * Removes the element from the current configuration.
 * @pre configCanHandle(element) == true.
 * @return success.
 */
bool D3dDrawManager::configRemove(jccl::ConfigElementPtr element)
{
   boost::ignore_unused_variable_warning(element);
   return false;
}

/**
 * Can the handler handle the given element?
 * @return false: We can't handle anything.
 */
bool D3dDrawManager::configCanHandle(jccl::ConfigElementPtr element)
{
   boost::ignore_unused_variable_warning(element);
   return false;
}

/**
 * Sets the dirty bits off all the gl windows.
 * Dirty all the window contexts.
 */
void D3dDrawManager::dirtyAllWindows()
{
   // Create Pipes & Add all windows to the correct pipe
   for (unsigned int winId=0;winId<mWins.size();winId++)   // For each window we created
   {
      mWins[winId]->setDirtyContext(true);
   }
}

bool D3dDrawManager::isValidWindow(D3dWindow* win)
{
   bool ret_val = false;
   for (unsigned int i=0;i<mWins.size();i++)
   {
      if (mWins[i] == win)
      {
         ret_val = true;
      }
   }

   return ret_val;
}

/// dumps the object's internal state
void D3dDrawManager::outStream(std::ostream& out)
{
   out << clrSetNORM(clrGREEN)
       << "========== D3dDrawManager: " << (void*)this << " ========="
       << clrRESET << std::endl
       << clrOutNORM(clrCYAN,"\tapp: ") << (void*)mApp << std::endl
       << clrOutNORM(clrCYAN,"\tWindow count: ") << mOpenWins.size()
       << std::endl << std::flush;

   for (unsigned int i = 0; i < mOpenWins.size(); i++)
   {
      vprASSERT(mOpenWins[i] != NULL);
      out << clrOutNORM(clrCYAN,"\tD3dWindow:\n") << *(mOpenWins[i]) << std::endl;
   }
   out << "=======================================" << std::endl;
}

} // end vrj namespace

namespace vrj
{

} // end vrj namespace
