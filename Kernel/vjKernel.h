/*************** <auto-copyright.pl BEGIN do not edit this line> ************** *
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


#ifndef _VJ_KERNEL_
#define _VJ_KERNEL_
//#pragma once

#include <vjConfig.h>
#include <Kernel/vjApp.h>
class vjApp;
#include <Kernel/vjSystemFactory.h>
//#include <Kernel/vjAPIFactory.h>
#include <Kernel/vjDisplayManager.h>
#include <Kernel/vjDrawManager.h>
#include <Threads/vjThread.h>
#include <Kernel/vjDebug.h>
#include <Input/InputManager/vjInputManager.h>
#include <Kernel/vjSystemData.h>
#include <Performance/vjPerfDataBuffer.h>
#include <Kernel/vjUser.h>
#include <Sound/vjSoundManager.h>

    // Config stuff
//#include <Config/vjConfigChunkDB.h>
//#include <Config/vjChunkDescDB.h>
// #include <Kernel/vjQueuedConfigChunkHandler.h>
#include <Kernel/vjConfigChunkHandler.h>
#include <Utils/vjSingleton.h>

class vjEnvironmentManager;

//-------------------------------------------------------
//: Main control class for all vj applications.
//
// Takes care of all initialization and object creation
// for the system.
// This class is the only class that MUST be instantiated
// for all applications.
//
// @author  Allen Bierbaum
//
// Date: 9-7-97
//-------------------------------------------------------
//!PUBLIC_API:
class vjKernel : public vjConfigChunkHandler
{
public:

   //: Start the Kernel running
   // Spawn kernel thread
   int start();

   //: Load initial configuration data for the managers
   //! POST: InputManager, DisplayManager, and kernel
   //+       Config files loaded and handed to configAdd
   void initConfig();

   //: The Kernel loop
   void controlLoop(void* nullParam);

   // Set the application object for the Kernel to deal with
   //  If there is another app active, it has to stop that
   //  application first then restart all API specific Managers.
   //! ARGS: _app - If NULL, stops current application
   void setApplication(vjApp* _app);

   //: Load configuration data for Kernel
   //! POST: Config data has been read into initial buffer
   void loadConfigFile(std::string filename);

   //: Load a chunk description file
   //! POST: The chunk factory can now manage chunks with the given types
   void loadChunkDescFile(std::string filename);


protected:  // -- CHUNK HANDLER
   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   virtual bool configCanHandle(vjConfigChunk* chunk);

   //: Process any pending reconfiguration that we can deal with
   //
   //  Process pending reconfiguration of the kernel and
   //  it's dependant managers (basically all of them
   //  that don't have a control thread)
   //
   // It just calls process pending for dependant processes
   virtual int configProcessPending(bool lockIt = true);

protected:  // -- CHUNK HANDLER
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! RETURNS: success
   virtual bool configAdd(vjConfigChunk* chunk);

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //!RETURNS: success
   virtual bool configRemove(vjConfigChunk* chunk);

protected:  // Local config functions
   //: Add a vjUser to the system
   bool addUser(vjConfigChunk* chunk);

   //: Remove a vjUser from the system
   //! NOTE: Currently not implemented
   bool removeUser(vjConfigChunk* chunk);

protected:
   //: Updates any data that needs updated once a frame (Trackers, etc.)
   //! POST: All tracker data is ready for next frame
   void updateFrameData();

   //: Checks to see if there is reconfiguration to be done
   //! POST: Any reconfiguration needed has been completed
   //! NOTE: Can only be called by the kernel thread
   void checkForReconfig();

   // Changes the application in use
   //  If there is another app active, it has to stop that
   //  application first then restart all API specific Managers.
   //! ARGS: _app - If NULL, stops current application
   //! NOTE: This can only be called from the kernel thread
   void changeApplication(vjApp* _app);

protected:      // --- DRAW MGR ROUTINES --- //
   // Starts the draw manager running
   // Calls the app callbacks for the draw manager
   //! ARGS: newMgr - Is this a new manager, or the same one
   void startDrawManager(bool newMgr);

   // Stop the draw manager and close it's resources, then delete it
   //! POST: draw mgr resources are closed
   //+       draw mgr is deleted, display manger set to NULL draw mgr
   void stopDrawManager();

public:      // Global "get" interface

      //: Get the system Factory
   vjSystemFactory* getSysFactory()
   { return mSysFactory; }

      //: Get the input manager
   vjInputManager* getInputManager()
   { return mInputManager; }

    //: Get the Environment Manager

    vjEnvironmentManager* getEnvironmentManager()
    { return environmentManager; }

    vjSoundManager* getSoundManager()
    { return mSoundManager; }

   //: Get the user associated with given name
   //! RETURNS: NULL - Not found
   vjUser*  getUser(std::string userName);

   //: Get a list of the users back
   std::vector<vjUser*> getUsers()
   { return mUsers; }

   const vjBaseThread* getThread()
   { return mControlThread; }

private:
   vjSystemData    data;   //: Global system data

protected:
   vjApp*      mApp;                        //: The app object
   vjApp*      mNewApp;                      //: New application to set
   bool        mNewAppSet;                   //: Flag to notify that a new application should be set

   vjBaseThread*   mControlThread;             //: The thread in control of me.

   /// Factories and Managers
   vjSystemFactory*  mSysFactory;          //: The current System factory
   vjInputManager*   mInputManager;       //: The input manager for the system
   vjDrawManager*    mDrawManager;         //: The Draw Manager we are currently using
   vjDisplayManager* mDisplayManager;      //: The Display Manager we are currently using
   vjEnvironmentManager* environmentManager; //: The Environment Manager object
   vjSoundManager*   mSoundManager;

   /// Performance information
   vjPerfDataBuffer* perfBuffer;          //: store perfdata for kernel main
   bool              performanceEnabled;

   /// Config Stuff
   //**//vjChunkDescDB*    mConfigDesc;
   //**//vjConfigChunkDB*  mChunkDB;            //: The current chunk db for the system
   //**//vjConfigChunkDB*  mInitialChunkDB;     //: Initial chunks added to system before it is started
   //vjSemaphore       mRuntimeConfigSema;  //: Protects run-time config.  Only when this semaphore
                                          //+ is acquired can run-time config occur
   /// Shared Memory stuff
   vjMemPool*       sharedMemPool;

   /// Multi-user information
   std::vector<vjUser*>   mUsers;         //: A list of user objects in system

   // ----------------------- //
   // --- SINGLETON STUFF --- //
   // ----------------------- //
protected:
   //: Constructor:  Hidden, so no instantiation is allowed
   vjKernel();

   virtual ~vjKernel()
   {;}

   vjSingletonHeader( vjKernel );
};


#endif
