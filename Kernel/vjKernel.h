#ifndef _VJ_KERNEL_
#define _VJ_KERNEL_

#include <config.h>
#include <iostream.h>
#include <Kernel/vjApp.h>
#include <Kernel/vjSystemFactory.h>
#include <Kernel/vjAPIFactory.h>
#include <Kernel/vjDisplayManager.h>
#include <Kernel/vjDrawManager.h>
#include <Threads/vjThread.h>
#include <Kernel/vjDebug.h>
#include <Input/InputManager/vjInputManager.h>
#include <Kernel/vjSystemData.h>

	// --- HACK --- //
	// Is this really needed??
#include <Kernel/vjSGISystemFactory.h>

    // Config stuff
#include <Config/vjConfigChunkDB.h>


//-------------------------------------------------------
//: Main control class for all vj applications.
//
//	Takes care of all initialization and object creation
//	for the system.
//	This class is the only class that MUST be instantiated
//	for all applications.
//
// @author  Allen Bierbaum
//
// Date: 9-7-97
//------------------------------------------------------- 
class vjKernel
{
public:

   //: Start the Kernel running
   int start();
      
   //: Setup all the managers and load the config information
   void initConfig();

   //: The Kernel loop
   void controlLoop(void* nullParam);

   // Set the application object for the Kernel to deal with
   //  If there is another app active, it has to stop that
   //  application first then restart all API specific Managers. 
   void setApplication(vjApp* _app)
   { app = _app; }

   // Stops the current application but leaves the kernel running.
   // It closes all API specific stuff (DrawManager,  etc.)
   int stopApplication()
   {
      return 1;
   }

protected:
   //: Updates any data that needs updated once a frame (Trackers, etc.)
   //! POST: All tracker data is ready for next frame
   void updateFrameData();

protected:      // --- STARTUP ROUTINES --- //
   /* Load configuration d;ata for Kernel
    * POST: Config data has been read in
    *       All Managers are configured
    */
   void loadConfig();

      // --- Manager setup functions ---- //
   void setupInputManager();
   void setupDisplayManager();
   void setupDrawManager();

public:      // Global "get" interface
      //: Get the global config database
   vjConfigChunkDB* getChunkDB()
   { return chunkDB;}

      //: Get the Factories
   vjAPIFactory* getAPIFactory()
   { return apiFactory; }

      //: Get the system Factory
   vjSystemFactory* getSysFactory()
   { return sysFactory; }

      //: Get the input manager
   vjInputManager* getInputManager()
   { return data.inputManager; }

public:
   vjSystemData    data;   //: Global system data

protected:
   
   vjApp* app;                         //: The app object
   vjThreadId* controlPid;             //: The pid for the thread in control of me.
   
   /// Factories and Managers
   vjAPIFactory*     apiFactory;          //: The Current API Factory
   vjSystemFactory*  sysFactory;          //: The current System factory
   vjDrawManager*    drawManager;         //: The Draw Manager we are currently using
   vjDisplayManager* displayManager;      //: The Display Manager we are currently using
   
   /// Config Stuff
   vjChunkDescDB*    configDesc;
   vjConfigChunkDB*  chunkDB;

   /// Shared Memory stuff
   vjMemPool*       sharedMemPool;   

   // ----------------------- //
   // --- SINGLETON STUFF --- //
   // ----------------------- //
protected:
   //: Constructor:  Hidden, so no instantiation is allowed
   vjKernel()
   { app = NULL;}   

public:
   //: Get instance of singleton object
   static vjKernel* instance()
   {
      if (_instance == NULL)
         _instance = new vjKernel;
      return _instance;
   }

private:  
   static vjKernel* _instance;   //: The instance
};


#endif
