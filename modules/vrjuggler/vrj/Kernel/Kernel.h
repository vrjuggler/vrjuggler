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

#ifndef _VRJ_KERNEL_H_
#define _VRJ_KERNEL_H_
//#pragma once

#include <vrj/vrjConfig.h>

#include <vpr/Util/Singleton.h>

#include <vrj/Kernel/SystemFactory.h>
#include <jccl/Plugins/PerformanceMonitor/PerfDataBuffer.h>
#include <vrj/Environment/EnvironmentManager.h>

#include <gadget/InputManager.h>

//class gadget::InputManager;

namespace vpr
{
   class BaseThread;
};

namespace vrj
{
class DisplayManager;
class DrawManager;
class SoundManager;
class User;
class App;


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
class VJ_CLASS_API Kernel : public jccl::ConfigChunkHandler
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
   void setApplication(vrj::App* _app);

   //: Load configuration data for Kernel
   //! POST: Config data has been read into initial buffer
   void loadConfigFile(const char* filename)
   {
      std::string filename_str = std::string(filename);
      loadConfigFile(filename_str);
   }

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
   virtual bool configCanHandle(jccl::ConfigChunkPtr chunk);

   //: Process any pending reconfiguration that we can deal with
   //
   //  Process pending reconfiguration of the kernel and
   //  it's dependant managers (basically all of them
   //  that don't have a control thread)
   //
//     // It just calls process pending for dependant processes
//     virtual int configProcessPending(bool lockIt = true);

protected:  // -- CHUNK HANDLER
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! RETURNS: success
   virtual bool configAdd(jccl::ConfigChunkPtr chunk);

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //!RETURNS: success
   virtual bool configRemove(jccl::ConfigChunkPtr chunk);

protected:  // Local config functions
   //: Add a User to the system
   bool addUser(jccl::ConfigChunkPtr chunk);

   //: Remove a User from the system
   //! NOTE: Currently not implemented
   bool removeUser(jccl::ConfigChunkPtr chunk);

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
   void changeApplication(vrj::App* _app);

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
   vrj::SystemFactory* getSysFactory()
   { return mSysFactory; }

   //: Get the input manager
   gadget::InputManager* getInputManager();

    //: Get the Environment Manager
   vrj::EnvironmentManager* getEnvironmentManager()
    { return environmentManager; }

   //: Get the user associated with given name
   //! RETURNS: NULL - Not found
   vrj::User*  getUser(std::string userName);

   //: Get a list of the users back
   std::vector<vrj::User*> getUsers()
   { return mUsers; }

   const vpr::BaseThread* getThread()
   { return mControlThread; }

protected:
   vrj::App*      mApp;                        //: The app object
   vrj::App*      mNewApp;                      //: New application to set
   bool        mNewAppSet;                   //: Flag to notify that a new application should be set

   vpr::BaseThread*   mControlThread;             //: The thread in control of me.

   /// Factories and Managers
   vrj::SystemFactory*        mSysFactory;            //: The current System factory
   gadget::InputManager*      mInputManager;          //: The input manager for the system
   DrawManager*               mDrawManager;           //: The Draw Manager we are currently using
   SoundManager*              mSoundManager;          //: The Audio Manager we are currently using
   DisplayManager*            mDisplayManager;        //: The Display Manager we are currently using
   EnvironmentManager*        environmentManager;     //: The Environment Manager object

   /// Performance information
   jccl::PerfDataBuffer* perfBuffer;          //: store perfdata for kernel main
   bool              performanceEnabled;

   /// Multi-user information
   std::vector<vrj::User*>   mUsers;         //: A list of user objects in system

   // ----------------------- //
   // --- SINGLETON STUFF --- //
   // ----------------------- //
protected:
   //: Constructor:  Hidden, so no instantiation is allowed
   Kernel();

   virtual ~Kernel()
   {;}

   vprSingletonHeader( Kernel );
};


} // end namespace
#endif
