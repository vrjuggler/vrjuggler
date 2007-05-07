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

#include <vrj/Draw/OGL/Config.h>

#include <stdlib.h>
#include <boost/bind.hpp>
#include <boost/concept_check.hpp>

#include <jccl/Config/ConfigElement.h>

#include <cluster/ClusterManager.h>

#include <vrj/Display/DisplayManager.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Kernel/KernelExceptions.h>

#include <vrj/Display/Display.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Display/SimViewport.h>
#include <vrj/Display/SurfaceViewport.h>

#include <vrj/Draw/OGL/GlApp.h>

#include <vrj/Draw/OGL/GlPipe.h>
#include <vrj/Draw/OGL/GlWindow.h>
#include <vrj/Draw/OGL/GlSimInterfaceFactory.h>

#include <gmtl/Vec.h>
#include <gmtl/Output.h>

//#include <gadget/Type/Glove.h>
//#include <gadget/Type/GloveProxy.h>

#include <vrj/Draw/OGL/GlDrawManager.h>


namespace vrj
{

vprSingletonImp(GlDrawManager);

GlDrawManager::GlDrawManager()
   : mApp(NULL)
   , drawTriggerSema(0)
   , drawDoneSema(0)
   , mRunning(false)
   , mControlThread(NULL)
{
}

GlDrawManager::~GlDrawManager()
{
   if ( mRunning )
   {
      // This will wait on mControlThread to exit.
      closeAPI();
   }

   if ( NULL != mControlThread )
   {
      delete mControlThread;
      mControlThread = NULL;
   }
}

/** Sets the app the draw should interact with. */
void GlDrawManager::setApp(App* _app)
{
   mApp = dynamic_cast<GlApp*>(_app);

   // We have a new app, so the contexts must be re-initialized
   // so... dirty them all.
   dirtyAllWindows();

   if ( NULL == mApp )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR:")
         << " [vrj::GlDrawManager::setApp()] Failed to downcast "
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
GlApp* GlDrawManager::getApp()
{
   return mApp;
}

/** Starts the control loop. */
void GlDrawManager::start()
{
   // --- Setup Multi-Process stuff --- //
   // Create a new thread to handle the control
   mRunning = true;

   // NOTE: Any exception thrown by spawning the thread will be propagated up
   // to the caller.
   mControlThread = new vpr::Thread(boost::bind(&GlDrawManager::main, this));

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << "vrj::GlDrawManager started (thread: " << mControlThread << ")\n"
      << vprDEBUG_FLUSH;
}


    // Enable a frame to be drawn
    // Trigger draw
void GlDrawManager::draw()
{
   drawTriggerSema.release();
}


/**
 * Blocks until the end of the frame.
 * @post The frame has been drawn.
 */
void GlDrawManager::sync()
{
   drawDoneSema.acquire();
}


/** This is the control loop for the manager. */
void GlDrawManager::main()
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

void GlDrawManager::drawAllPipes()
{
   vprDEBUG_OutputGuard(vrjDBG_DRAW_MGR, vprDBG_HVERB_LVL,
                        "vrj::GLDrawManager::drawAllPipes()\n",
                        "vrj::GLDrawManager::drawAllPipes() done.\n");

   // RENDER
   // Start rendering all the pipes
   for ( unsigned int pipe_num = 0; pipe_num < pipes.size(); ++pipe_num )
   {
      pipes[pipe_num]->triggerRender();
   }

      // Wait for rendering to finish on all the pipes
   for ( unsigned int pipe_num = 0; pipe_num < pipes.size(); ++pipe_num )
   {
      pipes[pipe_num]->completeRender();
   }

   // Barrier for Cluster
   //vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) <<  "BARRIER: Going to sleep for: " << num << std::endl << vprDEBUG_FLUSH;
   cluster::ClusterManager::instance()->swapBarrier();
   //vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) <<  "BARRIER: IS DONE" << std::endl << vprDEBUG_FLUSH;


   // SWAP
   // Start swapping all the pipes
   for ( unsigned int pipe_num = 0; pipe_num < pipes.size(); ++pipe_num )
   {
      pipes[pipe_num]->triggerSwap();
   }

   // Wait for swapping to finish on all the pipes
   for ( unsigned int pipe_num = 0; pipe_num < pipes.size(); ++pipe_num )
   {
      pipes[pipe_num]->completeSwap();
   }
}

/**
 * Initializes the drawing API (if not already running).
 * @post Control thread is started.
 */
void GlDrawManager::initAPI()
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
void GlDrawManager::addDisplay(DisplayPtr disp)
{
   vprASSERT(disp.get() != NULL);    // Can't add a null display

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
      << "[vrj::GlDrawManager::addDisplay()] " << disp
      << std::endl << vprDEBUG_FLUSH;

   // -- Finish Simulator setup
   std::vector<vrj::ViewportPtr>::size_type num_vp(disp->getNumViewports());

   for ( std::vector<vrj::ViewportPtr>::size_type i = 0 ; i < num_vp ; ++i )
   {
      ViewportPtr vp = disp->getViewport(i);

      if (vp->isSimulator())
      {
         jccl::ConfigElementPtr vp_element = vp->getConfigElement();

         SimViewportPtr sim_vp = boost::dynamic_pointer_cast<SimViewport>(vp);
         vprASSERT(NULL != sim_vp.get());

         sim_vp->setDrawSimInterface(DrawSimInterfacePtr());

         // Create the simulator stuff
         vprASSERT(1 == vp_element->getNum("simulator_plugin") &&
                   "You must supply a simulator plugin.");

         // Create the simulator stuff
         jccl::ConfigElementPtr sim_element =
            vp_element->getProperty<jccl::ConfigElementPtr>("simulator_plugin");

         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
            << "[vrj::GlDrawManager::addDisplay()] Creating simulator of type '"
            << sim_element->getID() << "'\n" << vprDEBUG_FLUSH;

         DrawSimInterfacePtr new_sim_i(
            GlSimInterfaceFactory::instance()->createObject(sim_element->getID())
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
   GlWindowPtr new_win = getGLWindow();
   new_win->configWindow(disp);                                            // Configure it
   mWins.push_back(new_win);                                         // Add to our local window list

   // -- Create any needed Pipes & Start them
   unsigned int pipe_num = new_win->getDisplay()->getPipe();    // Find pipe to add it too

   if (pipes.size() < (pipe_num+1))           // ASSERT: Max index of pipes is < our pipe
   {                                         // +1 because if pipeNum = 0, I still need size() == 1
      while (pipes.size() < (pipe_num+1))     // While we need more pipes
      {
         GlPipe* new_pipe = new GlPipe(pipes.size(), this,
                                       &mCreateWindowMutex);  // Create a new pipe to use
         pipes.push_back(new_pipe);                          // Add the pipe
         new_pipe->start();                                  // Start the pipe running
                                                             // NOTE: Run pipe even if no windows.  Then it waits for windows.
      }
   }

   // -- Add window to the correct pipe
   GlPipe* pipe;                           // The pipe to assign it to
   pipe = pipes[pipe_num];                 // ASSERT: pipeNum is in the valid range
   pipe->addWindow(new_win);               // Window has been added

   vprASSERT(isValidWindow(new_win));      // Make sure it was added to draw manager
}

/**
 * Callback when display is removed to display manager.
 * @pre disp must be a valid display that we have.
 * @post window for disp is removed from the draw manager and child pipes.
 */
void GlDrawManager::removeDisplay(DisplayPtr disp)
{
   GlPipe* pipe(NULL);
   GlWindowPtr win;     // Window to remove

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
      << "[vrj::GlDrawManager::removeDisplay()] " << disp
      << std::endl << vprDEBUG_FLUSH;

   for ( unsigned int i = 0; i < mWins.size(); ++i )
   {
      if (mWins[i]->getDisplay() == disp)      // FOUND it
      {
         win = mWins[i];
         pipe = pipes[win->getDisplay()->getPipe()];
      }
   }

   // Remove the window from the pipe and our local list
   if ( win.get() != NULL )
   {
      vprASSERT(pipe != NULL);
      vprASSERT(isValidWindow(win));
      pipe->removeWindow(win);                                                   // Remove from pipe
      mWins.erase(std::remove(mWins.begin(),mWins.end(),win), mWins.end());      // Remove from draw manager
      vprASSERT(!isValidWindow(win));
   }
   else
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR")
         << ": [vrj::GlDrawManager::removeDisplay()] Attempted to remove a "
         << "display that was not found.\n" << vprDEBUG_FLUSH;
      vprASSERT(false);
   }
}

/** Shutdown the drawing API */
void GlDrawManager::closeAPI()
{
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
      << "[vrj::GlDrawManager::closeAPI()]\n" << vprDEBUG_FLUSH;

   mRunning = false;
   
   // We must allow our control thread to fall through and die naturally.
   drawTriggerSema.release(); 	 
   drawDoneSema.acquire();
   mControlThread->join();

   // Stop and delete all pipes. This causes the GL windows to be closed and
   // deleted.
   typedef std::vector<GlPipe*>::iterator iter_t;
   for ( iter_t p = pipes.begin(); p != pipes.end(); ++p )
   {
      (*p)->stop();
      delete *p;
   }

   pipes.clear();
   mWins.clear();
}

/**
 * Adds the element to the draw manager config.
 * @pre configCanHandle(element) == true.
 * @post The elements have reconfigured the system.
 */
bool GlDrawManager::configAdd(jccl::ConfigElementPtr element)
{
   boost::ignore_unused_variable_warning(element);
   return false;
}

/**
 * Removes the element from the current configuration.
 * @pre configCanHandle(element) == true.
 * @return success.
 */
bool GlDrawManager::configRemove(jccl::ConfigElementPtr element)
{
   boost::ignore_unused_variable_warning(element);
   return false;
}

/**
 * Can the handler handle the given element?
 * @return false: We can't handle anything.
 */
bool GlDrawManager::configCanHandle(jccl::ConfigElementPtr element)
{
   boost::ignore_unused_variable_warning(element);
   return false;
}

GlUserData* GlDrawManager::currentUserData()
{
   return &(*mUserData);
}

int GlDrawManager::getCurrentContext()
{
   return (*mContextId);
}

void GlDrawManager::setCurrentContext(int val)
{
   (*mContextId) = val;
}

/**
 * Sets the dirty bits off all the gl windows.
 * Dirty all the window contexts.
 */
void GlDrawManager::dirtyAllWindows()
{
    // Create Pipes & Add all windows to the correct pipe
   for ( unsigned int winId = 0; winId < mWins.size(); ++winId )   // For each window we created
   {
      mWins[winId]->setDirtyContext(true);
   }
}

bool GlDrawManager::isValidWindow(GlWindowPtr win)
{
   bool ret_val = false;
   for ( unsigned int i = 0; i < mWins.size(); ++i )
   {
      if (mWins[i] == win)
      {
         ret_val = true;
      }
   }

   return ret_val;
}

/// dumps the object's internal state
void GlDrawManager::outStream(std::ostream& out)
{
    out     << clrSetNORM(clrGREEN)
            << "========== GlDrawManager: " << (void*)this << " ========="
            << clrRESET << std::endl
            << clrOutNORM(clrCYAN,"\tapp: ") << (void*)mApp << std::endl
            << clrOutNORM(clrCYAN,"\tWindow count: ") << mWins.size()
            << std::endl << std::flush;

    for ( unsigned int i = 0; i < mWins.size(); ++i )
    {
       vprASSERT(mWins[i] != NULL);
       out << clrOutNORM(clrCYAN,"\tGlWindow:\n") << *(mWins[i]) << std::endl;
    }
    out << "=======================================" << std::endl;
}

} // end vrj namespace


#if  defined(VPR_OS_Windows)
#  include <vrj/Draw/OGL/GlWindowWin32.h>
#elif defined(VPR_OS_Darwin) && ! defined(VRJ_USE_X11)
#  include <vrj/Draw/OGL/GlWindowCocoa.h>
#else
#  include <vrj/Draw/OGL/GlWindowXWin.h>
#endif

namespace vrj
{

vrj::GlWindowPtr GlDrawManager::getGLWindow()
{
#if  defined(VPR_OS_Windows)
   return GlWindowPtr(new vrj::GlWindowWin32);
#elif defined(VPR_OS_Darwin) && ! defined(VRJ_USE_X11)
   return GlWindowPtr(new vrj::GlWindowCocoa);
#else
   return GlWindowPtr(new vrj::GlWindowXWin);
#endif
}

} // end vrj namespace
