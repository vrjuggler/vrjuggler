#include <vjConfig.h>

#include <Kernel/GL/vjGlPipe.h>
#include <Kernel/GL/vjGlApp.h>
#include <Threads/vjThread.h>
#include <Sync/vjGuard.h>
#include <Kernel/vjDebug.h>

#include <Kernel/vjSurfaceDisplay.h>
#include <Kernel/vjSimDisplay.h>
#include <Environment/vjEnvironmentManager.h>

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

    vjDEBUG(vjDBG_ALL,0) << "vjGlPipe::start: Started control loop.  " << mActiveThread << endl << vjDEBUG_FLUSH;
    return 1;
}

//: Trigger rendering of the pipe to start
//! POST: The pipe has be told to start rendering
void vjGlPipe::triggerRender()
{
   //vjASSERT(mThreadRunning == true);      // We must be running
   while(!mThreadRunning)
   {
      vjDEBUG(vjDBG_ALL,1) << "Waiting in triggerRender.\n" << vjDEBUG_FLUSH;
   }

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
   vjDEBUG(vjDBG_ALL,1) << "vjGlPipe::addWindow: Pipe: " << mPipeNum << " adding window (to new wins):\n" << win << endl << vjDEBUG_FLUSH;
   newWins.push_back(win);
}

//: Remove a GLWindow from the window list
//! NOTE: The window is not actually removed until the next draw trigger
void vjGlPipe::removeWindow(vjGlWindow* win)
{
   vjGuard<vjMutex> guardClosing(mClosingWinLock);
   vjDEBUG(vjDBG_ALL,1) << "vjGlPipe:: removeWindow: Pipe: " << mPipeNum << " window added to closingWins.\n" << win << endl << vjDEBUG_FLUSH;
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
void vjGlPipe::controlLoop(void* nullParam)
{
   mThreadRunning = true;     // We are running so set flag
   // this should really not be here...
   vjKernel::instance()->getEnvironmentManager()->addPerfDataBuffer (mPerfBuffer);

   while (!controlExit)
   {
      checkForWindowsToClose();  // Checks for closing windows
      checkForNewWindows();      // Checks for new windows to open

      // --- RENDER the windows ---- //
      {
         renderTriggerSema.acquire();

            // --- Call the pipe pre-draw function --- //
         vjGlApp* theApp = glManager->getApp();

	 mPerfBuffer->set(mPerfPhase = 0);
            // Can't get a context since I may not be guaranteed a window
         theApp->pipePreDraw();                    // Call pipe pre-draw function
	 mPerfBuffer->set(++mPerfPhase);

         // Render the windows
         for (int winId=0;winId < openWins.size();winId++)
            renderWindow(openWins[winId]);

         renderCompleteSema.release();
	 mPerfBuffer->set(35);
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

// Closes all the windows in the list of windows to close
void vjGlPipe::checkForWindowsToClose()
{
   if(mClosingWins.size() > 0)   // If there are windows to close
   {
      vjGuard<vjMutex> guardClosing(mClosingWinLock);
      vjGuard<vjMutex> guardNew(newWinLock);
      vjGuard<vjMutex> guardOpen(openWinLock);

      for(int i=0;i<mClosingWins.size();i++)
      {
         vjGlWindow* win = mClosingWins[i];

         // Call contextClose
         vjGlApp* theApp = glManager->getApp();       // Get application for easy access
         vjDisplay* theDisplay = win->getDisplay();   // Get the display for easy access

         glManager->setCurrentContext(win->getId());     // Set TS data of context id
         glManager->currentUserData()->setUser(theDisplay->getUser());         // Set user data
         glManager->currentUserData()->setProjection(NULL);

         win->makeCurrent();              // Make the context current
         theApp->contextClose();          // Call context close function

         // Close the window
         win->close();

         // Remove it from the lists
         newWins.erase(std::remove(newWins.begin(), newWins.end(), win), newWins.end());
         openWins.erase(std::remove(openWins.begin(), openWins.end(), win), openWins.end());

         // Delete the window
         delete win;
         mClosingWins[i] = NULL;
      }

      mClosingWins.erase(mClosingWins.begin(), mClosingWins.end());
   }
}

/**  Checks for any new windows to add to the pipe
 * POST: Any new windows will be opened and added to the pipe's rendering list
 */
void vjGlPipe::checkForNewWindows()
{
   //vjGlApp* theApp = glManager->getApp();    // Get the application

   if (newWins.size() > 0)  // If there are new windows added
   {
      vjGuard<vjMutex> guardNew(newWinLock);
      vjGuard<vjMutex> guardOpen(openWinLock);

      for (int winNum=0; winNum<newWins.size(); winNum++)
      {
         newWins[winNum]->open();
         newWins[winNum]->makeCurrent();
         vjDEBUG(vjDBG_ALL,1) << "vjGlPipe::checkForNewWindows: Just opened window:\n" << newWins[winNum] << endl << vjDEBUG_FLUSH;
         openWins.push_back(newWins[winNum]);
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

   glManager->setCurrentContext(win->getId());     // Set TSS data of context id

   vjDEBUG(vjDBG_ALL,4) << "vjGlPipe::renderWindow: Set context to: " << vjGlDrawManager::instance()->getCurrentContext() << endl << vjDEBUG_FLUSH;

   mPerfBuffer->set(++mPerfPhase);


   // --- SET CONTEXT --- //
   win->makeCurrent();


   // CONTEXT INIT(): Check if we need to call contextInit()
   if(win->hasDirtyContext())
   {
         // Have dirty context
      glManager->currentUserData()->setUser(theDisplay->getUser());         // Set user data
      glManager->currentUserData()->setProjection(NULL);

      theApp->contextInit();              // Call context init function
      win->setDirtyContext(false);        // All clean now
   }

   mPerfBuffer->set(++mPerfPhase);

   theApp->contextPreDraw();                 // Do any context pre-drawing

   mPerfBuffer->set(++mPerfPhase);


   // ---- SURFACE --- //
   if (theDisplay->isSurface())
   {
      vjSurfaceDisplay* surface_disp = dynamic_cast<vjSurfaceDisplay*>(theDisplay);
      vjDisplay::DisplayView view = theDisplay->getView();        // Get the view we are rendering

      if((vjDisplay::STEREO == view) || (vjDisplay::LEFT_EYE == view))
      {
         win->setLeftEyeProjection();
            mPerfBuffer->set(++mPerfPhase);
         glManager->currentUserData()->setUser(surface_disp->getUser());         // Set user data
         glManager->currentUserData()->setProjection(surface_disp->getLeftProj());

            mPerfBuffer->set(++mPerfPhase);

         theApp->draw();
            mPerfBuffer->set(4);
         glManager->drawObjects();
            mPerfBuffer->set(5);
      }
      if ((vjDisplay::STEREO == view) || (vjDisplay::RIGHT_EYE == theDisplay->getView()))
      {
         win->setRightEyeProjection();
         glManager->currentUserData()->setUser(surface_disp->getUser());         // Set user data
         glManager->currentUserData()->setProjection(surface_disp->getRightProj());

	 mPerfBuffer->set(++mPerfPhase);

         theApp->draw();

	 mPerfBuffer->set(++mPerfPhase);

         glManager->drawObjects();
      }
      else
	  mPerfPhase += 2;
   }
   // ---- SIMULATOR ---------- //
   else if(theDisplay->isSimulator())
   {
      vjSimDisplay* sim_disp = dynamic_cast<vjSimDisplay*>(theDisplay);

      win->setCameraProjection();
      glManager->currentUserData()->setUser(sim_disp->getUser());         // Set user data
      glManager->currentUserData()->setProjection(sim_disp->getCameraProj());

      mPerfBuffer->set(++mPerfPhase);

      theApp->draw();

      mPerfBuffer->set(++mPerfPhase);
      mPerfPhase += 2;

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
   //vjTimeStamp* stamp = win->getDisplay()->getUser()->getHeadUpdateTime();
   //cout << "timestamp time is: " << stamp->usecs() << " usecs" << endl;

}
