#include <vjConfig.h>

#include <Kernel/GL/vjGlPipe.h>
#include <Kernel/GL/vjGlApp.h>
#include <Threads/vjThread.h>
#include <Sync/vjGuard.h>
#include <Kernel/vjDebug.h>

#include <Kernel/vjSurfaceDisplay.h>
#include <Kernel/vjSimDisplay.h>

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

//: Trigger swapping of all pipe's windows
void vjGlPipe::triggerSwap()
{
   vjASSERT(mThreadRunning == true);
   swapTriggerSema.release();
}

//: Blocks until swapping of the windows is completed
void vjGlPipe::completeSwap()
{
   vjASSERT(mThreadRunning == true);
   swapCompleteSema.acquire();
}


/// Add a GLWindow to the window list
// Control loop must now open the window on the next frame
void vjGlPipe::addWindow(vjGlWindow* win)
{
   vjGuard<vjMutex> guardNew(newWinLock);       // Protect the data
   newWins.push_back(win);
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
void vjGlPipe::controlLoop(void* nullParam)
{
   mThreadRunning = true;     // We are running so set flag

   while (!controlExit)
   {
      checkForNewWindows();      // Checks for new windows to open
      checkForWindowsToClose();  // Checks for closing windows


      // --- RENDER the windows ---- //
      {
         renderTriggerSema.acquire();

            // --- Call the pipe pre-draw function --- //
         vjGlApp* theApp = glManager->getApp();

            // Can't get a context since I may not be guaranteed a window
         theApp->pipePreDraw();                    // Call pipe pre-draw function

         // Render the windows
         for (int winId=0;winId < openWins.size();winId++)
            renderWindow(openWins[winId]);

         renderCompleteSema.release();
      }

      // ----- SWAP the windows ------ //
      {
         swapTriggerSema.acquire();

         // Swap all the windows
         for(int winId=0;winId < openWins.size();winId++)
            swapWindowBuffers(openWins[winId]);

         swapCompleteSema.release();
      }
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
         vjGlDrawManager* dMgr = vjGlDrawManager::instance();
         vjDisplay* theDisplay = newWins[winNum]->getDisplay();   // Get the display for easy access
         dMgr->setCurrentContext(newWins[winNum]->getId());
         dMgr->currentUserData()->setUser(theDisplay->getUser());         // Set user data
         dMgr->currentUserData()->setProjection(NULL);

         theApp->contextInit();              // Call context init function
      }

      newWins.erase(newWins.begin(), newWins.end());
   }
}

//: Renders the window using OpenGL
//! POST: win is rendered (In stereo if it is a stereo window)
void vjGlPipe::renderWindow(vjGlWindow* win)
{
   vjGlApp* theApp = glManager->getApp();       // Get application for easy access
   vjDisplay* theDisplay = win->getDisplay();   // Get the display for easy access

   vjGlDrawManager::instance()->setCurrentContext(win->getId());     // Set TSS data of context id
   vjDEBUG(4) << "vjGlPipe::renderWindow: Set context to: " << vjGlDrawManager::instance()->getCurrentContext() << endl << vjDEBUG_FLUSH;

   win->makeCurrent();                       // Set correct context
   theApp->contextPreDraw();                 // Do any context pre-drawing

   if (theDisplay->isSurface())        // Surface display
   {
      vjSurfaceDisplay* surface_disp = dynamic_cast<vjSurfaceDisplay*>(theDisplay);

      win->setLeftEyeProjection();
      glManager->currentUserData()->setUser(surface_disp->getUser());         // Set user data
      glManager->currentUserData()->setProjection(surface_disp->getLeftProj());

      theApp->draw();
      glManager->drawObjects();

      if (win->isStereo())
      {
         win->setRightEyeProjection();
         glManager->currentUserData()->setUser(surface_disp->getUser());         // Set user data
         glManager->currentUserData()->setProjection(surface_disp->getRightProj());

         theApp->draw();
         glManager->drawObjects();
      }
   }
   else if(theDisplay->isSimulator())                                  // SIMULATOR
   {
      vjSimDisplay* sim_disp = dynamic_cast<vjSimDisplay*>(theDisplay);

      win->setCameraProjection();
      glManager->currentUserData()->setUser(sim_disp->getUser());         // Set user data
      glManager->currentUserData()->setProjection(sim_disp->getCameraProj());

      theApp->draw();

      glManager->drawObjects();
      glManager->drawSimulator(sim_disp);
   }
}

//: Swaps the buffers of the given window
// Make the context current, and swap the window
void vjGlPipe::swapWindowBuffers(vjGlWindow* win)
{
   win->makeCurrent();           // Set correct context
   win->swapBuffers();           // Implicitly calls a glFlush
}
