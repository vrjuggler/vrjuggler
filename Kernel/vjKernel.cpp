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


#include <vjConfig.h>
#include <vjParam.h>
#include <string.h>
#include <Kernel/vjKernel.h>
#include <Kernel/vjDebug.h>
#include <Config/vjChunkFactory.h>
#include <Kernel/vjConfigManager.h>
#include <Threads/vjThread.h>
#include <Environment/vjEnvironmentManager.h>

// Get the system factory we need
#if defined(VJ_OS_IRIX) || defined(VJ_OS_Linux) || defined(VJ_OS_AIX) ||   \
    defined(VJ_OS_Solaris) || defined(VJ_OS_FreeBSD) || defined(VJ_OS_HPUX)
#include <Kernel/vjSGISystemFactory.h>
#elif defined(VJ_OS_Win32)
#include <Kernel/vjWin32SystemFactory.h>
#endif

//vjKernel* vjKernel::_instance = NULL;
vjSingletonImp(vjKernel);

//: Start the Kernel loop running
int vjKernel::start()
{
   if(mControlThread != NULL) // Have already started
   {
      vjDEBUG(vjDBG_ERROR,0) << clrOutNORM(clrRED,"ERROR:") << "vjKernel::start called when kernel already running\n" << vjDEBUG_FLUSH;
      vjASSERT(false);
      exit(0);
   }

   // Create a new thread to handle the control
   vjThreadMemberFunctor<vjKernel>* memberFunctor =
   new vjThreadMemberFunctor<vjKernel>(this, &vjKernel::controlLoop, NULL);

   vjThread* new_thread;   // I set mControlThread in vjKernel::controlLoop
   new_thread = new vjThread(memberFunctor, 0);

   vjDEBUG(vjDBG_KERNEL,vjDBG_STATE_LVL) << "vjKernel::start: Just started control loop.  "
                                         << std::endl << vjDEBUG_FLUSH;

   return 1;
}

/// The Kernel loop
void vjKernel::controlLoop(void* nullParam)
{
   vjDEBUG(vjDBG_KERNEL,1) << "vjKernel::controlLoop: Started.\n" << vjDEBUG_FLUSH;

   while (0 == vjThread::self())
   {
      vjDEBUG(vjDBG_ALL,1) << "vjKernel: Waiting for (thread::self() != NULL)\n" << vjDEBUG_FLUSH;
      usleep(50);
   }
   mControlThread = (vjThread*) vjThread::self();

   vjTimeStamp::initialize();
   // Do any initial configuration
   initConfig();

   // setup performance buffer
   perfBuffer = new vjPerfDataBuffer ("Kernel loop", 500, 8);
   environmentManager->addPerfDataBuffer (perfBuffer);

   //while(!Exit)
   while (1)
   {
      // Iff we have an app and a draw manager
      if((mApp != NULL) && (mDrawManager != NULL))
      {
            vjDEBUG(vjDBG_KERNEL,5) << "vjKernel::controlLoop: mApp->preFrame()\n" << vjDEBUG_FLUSH;
         mApp->preFrame();         // PREFRAME: Do Any application pre-draw stuff
            perfBuffer->set (0);
            vjDEBUG(vjDBG_KERNEL,5) << "vjKernel::controlLoop: drawManager->draw()\n" << vjDEBUG_FLUSH;
         mDrawManager->draw();    // DRAW: Trigger the beginning of frame drawing
         vjASSERT(mSoundManager != NULL);
         mSoundManager->update();
            perfBuffer->set (1);
            vjDEBUG(vjDBG_KERNEL,5) << "vjKernel::controlLoop: mApp->intraFrame()\n" << vjDEBUG_FLUSH;
         mApp->intraFrame();        // INTRA FRAME: Do computations that can be done while drawing.  This should be for next frame.
         //usleep(15000);              // Generate a wait in critical section
            perfBuffer->set (2);
            vjDEBUG(vjDBG_KERNEL,5) << "vjKernel::controlLoop: drawManager->sync()\n" << vjDEBUG_FLUSH;
         mSoundManager->sync();
         mDrawManager->sync();    // SYNC: Block until drawing is done
            perfBuffer->set (3);
            vjDEBUG(vjDBG_KERNEL,5) << "vjKernel::controlLoop: mApp->postFrame()\n" << vjDEBUG_FLUSH;
         mApp->postFrame();        // POST FRAME: Do processing after drawing is complete
            perfBuffer->set (4);
      }
      else
      {
         // ??? Should we do this, or just grind up the CPU as fast as possible
         vjASSERT(NULL != mControlThread);      // If control thread is not set correctly, it will seg fault here
         mControlThread->yield();   // Give up CPU
      }

      //usleep(10000);
      checkForReconfig();        // Check for any reconfiguration that needs done

      perfBuffer->set(5);

         vjDEBUG(vjDBG_KERNEL,5) << "vjKernel::controlLoop: Update Trackers\n" << vjDEBUG_FLUSH;
      getInputManager()->updateAllData();    // Update the trackers
         perfBuffer->set(6);
         vjDEBUG(vjDBG_KERNEL,5) << "vjKernel::controlLoop: Update Projections\n" << vjDEBUG_FLUSH;
      updateFrameData();         // Update the projections, etc.
         perfBuffer->set(7);
   }
}

// Set the application to run
// XXX: Should have protection here
void vjKernel::setApplication(vjApp* _app)
{
   vjDEBUG(vjDBG_KERNEL,vjDBG_CONFIG_LVL) << "vjKernel::setApplication: New application set\n" << vjDEBUG_FLUSH;
   mNewApp = _app;
   mNewAppSet = true;
}



//: Checks to see if there is reconfiguration to be done
//! POST: Any reconfiguration needed has been completed
//! NOTE: Can only be called by the kernel thread
void vjKernel::checkForReconfig()
{
   vjASSERT(vjThread::self() == mControlThread);      // ASSERT: We are being called from kernel thread

   // ---- RECONFIGURATION --- //
   int total_chunks_processed(0);
   int local_chunks_processed(0);

   // This loop will keep processing the pending list
   // until there is an iteration where no chunks are processed.
   do
   {
      local_chunks_processed = configProcessPending();
      total_chunks_processed += local_chunks_processed;
   }
   while(local_chunks_processed > 0);

   // If we changed the active configuration, then the environment manager needs to refresh
   if((total_chunks_processed > 0) && (environmentManager != NULL))
   {
      environmentManager->sendRefresh();
   }

   // ---- APP SWITCH ---- //
   // check for a new applications
   if(mNewAppSet)
   {
      if(mNewApp->depSatisfied())
      {
         vjDEBUG(vjDBG_KERNEL,vjDBG_CONFIG_LVL) << "vjKernel: New application dependencies: Satisfied.\n" << vjDEBUG_FLUSH;
         mNewAppSet = false;
         changeApplication(mNewApp);
      }
      else
      {
         vjDEBUG(vjDBG_KERNEL,vjDBG_WARNING_LVL) << "vjKernel: New application dependencies: Not satisfied yet.\n" << vjDEBUG_FLUSH;
      }
   }
}

// Changes the application in use
//  If there is another app active, it has to stop that
//  application first then restart all API specific Managers.
//! ARGS: _app - If NULL, stops current application
//! NOTE: This can only be called from the kernel thread
// app = NULL ==> stop draw manager and null out app
// app != NULL ==>
//             Get the draw manager needed
//             Start it
//             Give it the application
void vjKernel::changeApplication(vjApp* _app)
{
   vjDEBUG(vjDBG_KERNEL,1) << "vjKernel::changeApplication: Changing to:"
                           << _app << std::endl << vjDEBUG_FLUSH;

   vjASSERT(vjThread::self() == mControlThread);      // ASSERT: We are being called from kernel thread

   // EXIT Previous application
   if(mApp != NULL)
      mApp->exit();

   // SET NEW APPLICATION
   if(_app != NULL)        // We were given an app
   {
      mApp = _app;
      vjDrawManager* new_draw_mgr = mApp->getDrawManager();
      vjASSERT(NULL != new_draw_mgr);

      if (new_draw_mgr != mDrawManager)      // Have NEW draw manager
      {
         stopDrawManager();                           // Stop old one
         mDrawManager = mApp->getDrawManager();       // Get the new one
         startDrawManager(true);                      // Start the new one
      }
      else     // SAME draw manager
      {
         startDrawManager(false);                     // Start new app
      }
   }
   else                 // No app, clear to NULL
   {
      stopDrawManager();
      mApp = NULL;
   }
}



//-----------------------------------------------
// Initialize Shared Memory
// Load config
// Setup Input, Display, and kernel
//!NOTE: Does initial configuration and then sends config file to configAdd
//!POST: Shared Memory Initialized
//----------------------------------------------
void vjKernel::initConfig()
{
   vjDEBUG_BEGIN(vjDBG_KERNEL,3) << "vjKernel::initConfig: Setting initial config.\n" << vjDEBUG_FLUSH;

   // --- CREATE SHARED MEMORY --- //
   vjSharedPool::init();         // Try to init the pool stuff
   sharedMemPool = new vjSharedPool(1024*1024);      // Create shared memory pool

   // ---- ALLOCATE MANAGERS --- //
   //initialSetupInputManager();
   mInputManager = new (sharedMemPool) vjInputManager;
   //initialSetupDisplayManager();
   mDisplayManager = vjDisplayManager::instance();  // Get display manager
   vjASSERT(mDisplayManager != NULL);                 // Did we get an object

   mSoundManager = vjSoundManager::instance();
   vjASSERT(mSoundManager != NULL);

   //setupEnvironmentManager();
   environmentManager = new vjEnvironmentManager();

   //??// processPending() // Should I do this here

#ifdef VJ_OS_IRIX
   mSysFactory = vjSGISystemFactory::instance(); // XXX: Should not be system specific
#elif defined(VJ_OS_Linux) || defined(VJ_OS_Solaris) || defined(VJ_OS_AIX) || \
      defined(VJ_OS_FreeBSD) || defined(VJ_OS_HPUX)
   mSysFactory = vjSGISystemFactory::instance(); // HACK - this could be trouble, using SGI factory
#elif defined(VJ_OS_Win32)
   mSysFactory = vjWin32SystemFactory::instance();
#else
   //vjDEBUG(0,0) << "ERROR!: Don't know how to create System Factory!\n" << vjDEBUG_FLUSH;
   vjASSERT(false);
#endif

   vjDEBUG_END(vjDBG_KERNEL,3) << "vjKernel::initConfig: Done.\n" << vjDEBUG_FLUSH;
}


void vjKernel::updateFrameData()
{
   // When we have a draw manager, tell it to update it's projections
   mDisplayManager->updateProjections();
}


// -------------------------------
// CHUNK Handler
// -------------------------------
//: Process any pending reconfiguration that we can deal with
//
//  For all dependant managers, call process pending.
//  and call it on our selves
int vjKernel::configProcessPending(bool lockIt)
{
   int chunks_processed(0);     // Needs to return this value

   vjConfigManager* cfg_mgr = vjConfigManager::instance();
   if(cfg_mgr->pendingNeedsChecked())
   {
      vjDEBUG_BEGIN(vjDBG_ALL,vjDBG_STATE_LVL) << "vjKernel::configProcessPending: Examining pending list.\n" << vjDEBUG_FLUSH;

      chunks_processed += vjConfigChunkHandler::configProcessPending(lockIt);      // Process kernels pending chunks
      chunks_processed += getInputManager()->configProcessPending(lockIt);
      chunks_processed += mDisplayManager->configProcessPending(lockIt);
      if(NULL != mSoundManager)
         chunks_processed += mSoundManager->configProcessPending(lockIt);
      if(NULL != mDrawManager)
         chunks_processed += mDrawManager->configProcessPending(lockIt);              // XXX: We should not necessarily do this for all draw mgrs
      if (NULL != environmentManager)
         chunks_processed += environmentManager->configProcessPending(lockIt);
      if(NULL != mApp)
         chunks_processed += mApp->configProcessPending(lockIt);

      vjDEBUG_CONT_END(vjDBG_ALL,vjDBG_CONFIG_LVL) << std::endl
                                                   << vjDEBUG_FLUSH;
   }
   return chunks_processed;
}


bool vjKernel::configCanHandle(vjConfigChunk* chunk)
{
   std::string chunk_type = (std::string)chunk->getType();

   if(std::string("JugglerUser") == chunk_type)
      return true;
   else
      return false;
}

bool vjKernel::configAdd(vjConfigChunk* chunk)
{
   std::string chunk_type = (std::string)chunk->getType();

   vjASSERT(configCanHandle(chunk));

   if(std::string("JugglerUser") == chunk_type)
   {
      return addUser(chunk);
   }
   else
      return false;
}

bool vjKernel::configRemove(vjConfigChunk* chunk)
{
   std::string chunk_type = (std::string)chunk->getType();

   vjASSERT(configCanHandle(chunk));

   if(std::string("JugglerUser") == chunk_type)
   {
      return removeUser(chunk);
   }
   else
      return false;
}

//: Add a new user to the kernel
bool vjKernel::addUser(vjConfigChunk* chunk)
{
   vjASSERT((std::string)chunk->getType() == std::string("JugglerUser"));

   vjUser* new_user = new vjUser;
   bool success = new_user->config(chunk);

   if(!success)
   {
      vjDEBUG(vjDBG_CONFIG,vjDBG_CRITICAL_LVL)
                     << clrOutNORM(clrRED,"ERROR:") << "Failed to add new vjUser: "
                     << chunk->getProperty("name") << std::endl
                     << vjDEBUG_FLUSH;
      delete new_user;
   }
   else
   {
      vjDEBUG(vjDBG_CONFIG,vjDBG_STATE_LVL)
                             << "vjKernel: Added new vjUser: "
                             << new_user->getName().c_str() << std::endl
                             << vjDEBUG_FLUSH;
      mUsers.push_back(new_user);
   }

   return success;
}

// XXX: Not implemented
bool vjKernel::removeUser(vjConfigChunk* chunk)
{
   return false;
}

// --- STARTUP ROUTINES --- //
void vjKernel::loadConfigFile(std::string filename)
{
   vjDEBUG(vjDBG_KERNEL,vjDBG_CONFIG_LVL) << "Loading config file: "
                           << filename << std::endl << vjDEBUG_FLUSH;

   vjConfigChunkDB* chunk_db = new vjConfigChunkDB;

   // ------- OPEN Program specified Config file ------ //
   if(filename.empty())   // We have a filename
     return;

   bool chunk_db_load_success = chunk_db->load(filename);
   if (!chunk_db_load_success)
   {
     vjDEBUG(vjDBG_ERROR,vjDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:")
        << "vjKernel::loadConfigFile: DB Load failed to load file: "
        << filename.c_str() << std::endl << vjDEBUG_FLUSH;
     exit(1);
   }

   // Put them all in pending
   vjConfigManager::instance()->addChunkDB(chunk_db);

   //vjDEBUG(vjDBG_KERNEL,5) << "------------  Loaded Config Chunks ----------" << vjDEBUG_FLUSH;
   //vjDEBUG(vjDBG_KERNEL,5) << (*mInitialChunkDB) << vjDEBUG_FLUSH;
}

//: Load a chunk description file
//! POST: The chunk factory can now manage chunks with the given types
void vjKernel::loadChunkDescFile(std::string filename)
{
   vjChunkFactory::instance()->loadDescs(filename);
}



// This starts up the draw manager given
//!POST: All processes and data should have been created by draw manager
void vjKernel::startDrawManager(bool newMgr)
{
   vjASSERT((mApp != NULL) && (mDrawManager != NULL) && (mDisplayManager != NULL));

   if(newMgr)
   {
      //mDrawManager->configInitial(mInitialChunkDB);     // Give it the chunk DB to extract API specific info
      mDrawManager->setDisplayManager(mDisplayManager);
      mDrawManager->configProcessPending(true);           // See if there are any config chunks for us
   }
   mDrawManager->setApp(mApp);

   mApp->init();                     // Init the app
   if(newMgr)
      mDrawManager->initAPI();       // Just sets up API type stuff, possibly starts new processes
   mApp->apiInit();                  // Have app do any app-init stuff
   if(newMgr)
   {
     mDisplayManager->setDrawManager(mDrawManager);      // This can trigger the update of windows to the draw manager
   }
}

// Stop the draw manager and close it's resources, then delete it
//! POST: draw mgr resources are closed
//+       draw mgr is deleted, display manger set to NULL draw mgr
void vjKernel::stopDrawManager()
{
   if(mDrawManager != NULL)
   {
      mDrawManager->closeAPI();
      delete mDrawManager;
      mDrawManager = NULL;
      mDisplayManager->setDrawManager(NULL);
   }
}


vjUser* vjKernel::getUser(std::string userName)
{
   for(unsigned int i=0;i<mUsers.size();i++)
      if(userName == mUsers[i]->getName())
         return mUsers[i];

   return NULL;
}

vjKernel::vjKernel()
{
   mApp = NULL;
   mNewApp = NULL;
   mNewAppSet = false;
   mControlThread = NULL;
   mSysFactory = NULL;
   mInputManager = NULL;
   mDrawManager = NULL;
   mDisplayManager = NULL;
   mSoundManager = NULL;

   environmentManager = NULL;
   perfBuffer = NULL;

   //mInitialChunkDB = NULL;
   //mChunkDB = NULL;

   sharedMemPool = NULL;

   // Print out the Juggler version number when the kernel is created.
   vjDEBUG(vjDBG_BASE, 0) << std::string(strlen(VJ_VERSION) + 12, '=')
                          << std::endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_BASE, 0) << clrOutNORM(clrGREEN, "VR Juggler: ")
                          << clrOutNORM(clrGREEN, VJ_VERSION) << clrRESET
                          << std::endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_BASE, 0) << std::string(strlen(VJ_VERSION) + 12, '=')
                          << std::endl << vjDEBUG_FLUSH;
}

