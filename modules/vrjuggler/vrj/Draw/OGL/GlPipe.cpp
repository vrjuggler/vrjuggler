#include <config.h>

#ifdef VJ_OS_HPUX
#   include <algorithm>
#else
#   if defined(VJ_USE_PTHREADS) && ! defined(_PTHREADS)
#       define _PTHREADS
#   endif

#   include <algo.h>
#endif

#include <Kernel/GL/vjGlPipe.h>
#include <Threads/vjThread.h>
#include <Sync/vjGuard.h>
#include <Kernel/vjDebug.h>
#include <GL/gl.h>


/** Start the pipe running
 * POST: The pipe has it's own thread of control and is ready to operate
 */
int vjGlPipe::start()
{
   vjASSERT(mThreadRunning == false);        // We should not be running yet

   // Create a new thread to handle the control loop
   vjThreadMemberFunctor<vjGlPipe>* memberFunctor =
   new vjThreadMemberFunctor<vjGlPipe>(this, &vjGlPipe::controlLoop, NULL);

   mActiveThread = new vjThread(memberFunctor, 0);

   vjDEBUG(0) << "vjGlPipe::start: Started control loop.  " << mActiveThread << endl << vjDEBUG_FLUSH;
   return 1;
}

//: Trigger rendering of the pipe to start
//! POST: The pipe has be told to start rendering
void vjGlPipe::triggerRender()
{
   vjASSERT(mThreadRunning == true);      // We must be running

   renderTriggerSema.release();
}

//: Blocks until rendering of the windows is completed
//! POST: The pipe has completed its rendering
void vjGlPipe::completeRender()
{
   vjASSERT(mThreadRunning == true);      // We must be running

   renderCompleteSema.acquire();
}

/// Add a GLWindow to the window list
// Control loop must now open the window on the next frame
void vjGlPipe::addWindow(vjGlWindow* win)
{
   vjGuard<vjMutex> guardNew(newWinLock);       // Protect the data
   newWins.push_back(win);
}


// The main loop routine
void vjGlPipe::controlLoop(void* nullParam)
{
   mThreadRunning = true;     // We are running

   while (!controlExit)
   {
      checkForNewWindows();      // Checks for new windows to open
      checkForWindowsToClose();  // Checks for closing windows

      // wait for render signal
      renderTriggerSema.acquire();

         // --- Call the pipe pre-draw function --- //
      vjGlApp* theApp = glManager->getApp();

         // Should I get into a context
      theApp->pipePreDraw();                    // Call pipe pre-draw function

      for (int winId=0;winId < openWins.size();winId++)
         renderWindow(openWins[winId]);

      // Done rendering. Tell people that we are waiting now.
      renderCompleteSema.release();
   }

   mThreadRunning = false;     // We are not running
}

/**  Checks for any new windows to add to the pipe
 * POST: Any new windows will be opened and added to the pipe's rendering list
 */
void vjGlPipe::checkForNewWindows()
{
   vjGlApp* theApp = glManager->getApp();    // Get the application

   if (newWins.size() > 0)  // If there are new windows added
   {
      vjGuard<vjMutex> guardNew(newWinLock);
      vjGuard<vjMutex> guardOpen(openWinLock);

      for (int winNum=0; winNum<newWins.size(); winNum++)
      {
         newWins[winNum]->open();
         newWins[winNum]->makeCurrent();
         vjDEBUG(2) << "vjGlPipe::checkForNewWindows: Just opened window:\n" << *(newWins[winNum]) << endl << vjDEBUG_FLUSH;
         openWins.push_back(newWins[winNum]);

         // Opened new window, call init function for the context
         vjGlDrawManager::instance()->currentContext() = newWins[winNum]->getId();
         theApp->contextInit();              // Call context init function
      }

      newWins.erase(newWins.begin(), newWins.end());
   }
}

/**   Renders the window using OpenGL
 * POST: win is rendered (In stereo if it is a stereo window)
 */
void vjGlPipe::renderWindow(vjGlWindow* win)
{
   vjGlApp* theApp = glManager->getApp();       // Get application for easy access
   vjGlDrawManager::instance()->currentContext() = win->getId();
   vjDEBUG(1) << "vjGlPipe::renderWindow: Set context to: " << vjGlDrawManager::instance()->currentContext() << endl << vjDEBUG_FLUSH;

   win->makeCurrent();

   if (!win->getDisplay()->isSimulator())      // NON-SIMULATOR
   {
      win->setLeftEye();
      theApp->draw();

      if (win->isStereo())
      {
         win->setRightEye();
         theApp->draw();
      }
   }
   else                                   // SIMULATOR
   {
      win->setCameraEye();
      theApp->draw();
      glManager->drawSimulator(win->getDisplay()->mSim);
   }

   glFlush();              // Flush the OpenGL commands
   win->swapBuffers();     // Swap those buffers
}
