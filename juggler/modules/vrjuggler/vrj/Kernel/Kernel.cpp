#include <config.h>
#include <string.h>
#include <Kernel/vjKernel.h>

// Initailize Statics
vjKernel* vjKernel::_instance = NULL;

/// Start the Kernel running
int vjKernel::start()
{
   if (app == NULL)
   {
      cerr << "vj ERROR: Can't start Kernel until setting App" << endl;
      return -1;
   }

   // Create a new thread to handle the control
   vjThreadMemberFunctor<vjKernel>* memberFunctor = 
   new vjThreadMemberFunctor<vjKernel>(this, &vjKernel::controlLoop, NULL);

   controlPid = vjThread::spawn(memberFunctor, 0);

   cerr << "vjKernel::start: Just started control loop.  "
        << *controlPid << endl;

   return 1;
}

//-----------------------------------------------
// Initialize Shared Memory
// Load config
// Setup Input, Display, and then Draw Managers
//!POST: Shared Memory Initialized
//----------------------------------------------
void vjKernel::initConfig()
{
   vjDEBUG(0) << "vjKernel::initConfig: Entered.\n";

   vjSharedPool::init();         // Try to init the pool stuff
   sharedMemPool = new vjSharedPool(1024*1024);      // XXX: should not be system specific

   // --- Read config stuff --- //
   loadConfig();

   setupInputManager();

   apiFactory = app->api.getAPIFactory();
   sysFactory = vjSGISystemFactory::instance(); // XXX: Should not be system specific

   vjDEBUG(0) << "vjKernel::initConfig: Calling setupDisplayManager.\n";
   setupDisplayManager();
   vjDEBUG(0) << "vjKernel::initConfig: Calling setupDrawManager.\n";  
   setupDrawManager();
   displayManager->setDrawManager(drawManager);
   vjDEBUG(0) << "vjKernel::initConfig: Exiting.\n";
}

/// The Kernel loop
void vjKernel::controlLoop(void* nullParam)
{
   cerr << "vjKernel::controlLoop: Entered.\n";

   initConfig();

   //while(!Exit)
   while (1)
   {
      app->preDraw();         // Do Any application pre-draw stuff
      drawManager->draw();    // Trigger the beginning of frame drawing
      app->postDraw();        // Do computations that can be done while drawing.  This should be for next frame.
      drawManager->sync();    // Block until drawing is done
      app->postSync();        // Do processing after drawing is complete

      // Sync should be here for Kernel changes from
      // the environment manager

      //Tell trackers to swap buffers;
      data.inputManager->UpdateAllData();
      drawManager->updateProjections(); 
   }
}

void vjKernel::updateFrameData()
{
   drawManager->updateProjections();
}

// --- STARTUP ROUTINES --- //
void vjKernel::loadConfig()
{
   vjDEBUG(5) << "   vjKernel::loadConfig:\n";
   configDesc = new vjChunkDescDB;
   if (!configDesc->load("/home/users/allenb/Source/juggler/Data/chunksDesc"))
   {
      cerr << "vjKernel::loadConfig: Config Desc failed to load file: " << endl;
      exit(1);
   }

   char  configFile[250];
   char* homeDir = getenv("HOME");     // Get users home directory
   strcpy(configFile, homeDir);
   strcat(configFile, "/.vjconfig/activeConfig");
   
   chunkDB = new vjConfigChunkDB(configDesc);
   if (!chunkDB->load(configFile))
   {
      cerr << "vjKernel::loadConfig: DB Load failed to load file: " << configFile << endl;
      exit(1);
   }

   vjDEBUG(2) << "------------  Config Chunks ----------";
   vjDEBUG(2) << (*chunkDB);

   /*  XXX: I think this code was just here to display config data.  It didn't do anything??
   vjDEBUG(2) << "----- Displays -------";
   vector<vjConfigChunk*>* displayChunks;
   displayChunks = chunkDB->getMatching("display");

   for (int i = 0; i < displayChunks->size(); i++)
   {
      vjConfigChunk* chunk = (*displayChunks)[i];

      vjDEBUG(2) << "\n\nDisplay: " << i << endl;
      vjDEBUG(2) << "Name:"
                  << (char*)chunk->getProperty("name") << endl;
      vjDEBUG(2) << "Proj:"
                  << (char*)chunk->getProperty("projectiontype") << endl;
      vjDEBUG(2) << "Origin:"
                  << (int)chunk->getProperty("origin", 0) << " "
                  << (int)chunk->getProperty("origin", 1) << endl;
      vjDEBUG(2) << "Size:"
                  << (int)chunk->getProperty("size", 0) << " "
                  << (int)chunk->getProperty("size", 1) << endl;
   }
   */
}

void vjKernel::setupInputManager()
{
   vjDEBUG(0) << "   vjKernel::setupInputManager\n";
   data.inputManager = new (sharedMemPool) vjInputManager;
   data.inputManager->FNewInput(chunkDB);

   vjDEBUG(0) << "      Input manager has passed. (Andy did good)" << endl;

   data.inputManager->UpdateAllData();
   
   vjDEBUG(0) << "      First Update trackers succeeded..." << endl;
}

void vjKernel::setupDisplayManager()
{
   vjDEBUG(0) << "   vjKernel::setupDisplayManager\n";
   // Setup displays
   displayManager = vjDisplayManager::instance();

   // Get the vector of display chunks
   vector<vjConfigChunk*>* displayChunks;
   displayChunks = chunkDB->getMatching("display");

   // For each display in vector,
   //	   -- Create display
   //	   -- Have it config itself.
   //	   -- Add it to the display Manager
   for (int i = 0; i < displayChunks->size(); i++)
   {
      vjConfigChunk* chunk = (*displayChunks)[i];
      vjDisplay* newDisp = new vjDisplay();
      newDisp->config(chunk);
      displayManager->addDisplay(newDisp);

      vjDEBUG(0) << "--- New Display ----" << endl
              << *newDisp << endl << flush;
   }

      // Tell Display manager to look for head
   displayManager->setupHeadIndices();
}

void vjKernel::setupDrawManager()
{
   vjDEBUG(0) << "   vjKernel::setupDrawManager" << endl;

   drawManager = apiFactory->getDrawManager();
   drawManager->config(chunkDB);     // Give it the chunk DB to extract API specific info
   drawManager->setApp(app);
   drawManager->setDisplayManager(displayManager);

   app->init();                     // Init the app
   drawManager->initAPI();          // Just sets up API type stuff
   app->apiInit();                  // Have app do any app-init stuff
   drawManager->initDrawing();      // Configs and Starts drawing procs

   vjDEBUG(0) << "   vjKernel::setupDrawManager: Exiting." << endl << flush;
}

