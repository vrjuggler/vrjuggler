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

   mControlThread = new vjThread(memberFunctor, 0);

   vjDEBUG(0) << "vjKernel::start: Just started control loop.  "
              << mControlThread << endl << vjDEBUG_FLUSH;

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
      vjDEBUG_BEGIN(0) << "vjKernel::initConfig: Entered.\n" << vjDEBUG_FLUSH;

   vjSharedPool::init();         // Try to init the pool stuff
   sharedMemPool = new vjSharedPool(1024*1024);      // XXX: should not be system specific

   // --- Read config stuff --- //
   loadConfig();

   setupInputManager();

   //apiFactory = app->api.getAPIFactory();
   sysFactory = vjSGISystemFactory::instance(); // XXX: Should not be system specific

      vjDEBUG(0) << "vjKernel::initConfig: Calling setupDisplayManager.\n" << vjDEBUG_FLUSH;
   setupDisplayManager();
      vjDEBUG(0) << "vjKernel::initConfig: Calling setupDrawManager.\n" << vjDEBUG_FLUSH;
   setupDrawManager();
   displayManager->setDrawManager(drawManager);
      vjDEBUG_END(0) << "vjKernel::initConfig: Exiting.\n" << vjDEBUG_FLUSH;
}

/// The Kernel loop
void vjKernel::controlLoop(void* nullParam)
{
   vjDEBUG(0) << "vjKernel::controlLoop: Entered.\n" << vjDEBUG_FLUSH;

   initConfig();

   //while(!Exit)
   while (1)
   {
         vjDEBUG(3) << "vjKernel::controlLoop: app->preDraw()\n" << vjDEBUG_FLUSH;
      app->preDraw();         // Do Any application pre-draw stuff
         vjDEBUG(3) << "vjKernel::controlLoop: drawManager->draw()\n" << vjDEBUG_FLUSH;
      drawManager->draw();    // Trigger the beginning of frame drawing
         vjDEBUG(3) << "vjKernel::controlLoop: app->postDraw\n" << vjDEBUG_FLUSH;
      app->postDraw();        // Do computations that can be done while drawing.  This should be for next frame.
         vjDEBUG(3) << "vjKernel::controlLoop: drawManager->sync()\n" << vjDEBUG_FLUSH;
      drawManager->sync();    // Block until drawing is done
         vjDEBUG(3) << "vjKernel::controlLoop: app->postSync()\n" << vjDEBUG_FLUSH;
      app->postSync();        // Do processing after drawing is complete

      // Sync should be here for Kernel changes from
      // the environment manager

      //Tell trackers to swap buffers;
         vjDEBUG(3) << "vjKernel::controlLoop: Update Trackers\n" << vjDEBUG_FLUSH;
      data.inputManager->UpdateAllData();
         vjDEBUG(3) << "vjKernel::controlLoop: Update Projections\n" << vjDEBUG_FLUSH;
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
   vjDEBUG(5) << "   vjKernel::loadConfig:\n" << vjDEBUG_FLUSH;

   // ------ OPENG chunksDesc file ----- //
   char* vj_base_dir = getenv("VJ_BASE_DIR");
   if(vj_base_dir == NULL)
   {
      cerr << "vjKernel::loadConfig: Env var VJ_BASE_DIR not defined." << endl;
      exit(1);
   }

   char chunk_desc_file[250];
   strcpy(chunk_desc_file, vj_base_dir);
   strcat(chunk_desc_file, "/Data/chunksDesc");

   configDesc = new vjChunkDescDB;
   if (!configDesc->load(chunk_desc_file))
   {
      cerr << "vjKernel::loadConfig: Config Desc failed to load file: " << endl;
      exit(1);
   }

   chunkDB = new vjConfigChunkDB(configDesc);      // Create config database

   // ------- OPEN User Config file ----------- //
      /*
   char  configFile[250];
   char* homeDir = getenv("HOME");     // Get users home directory
   strcpy(configFile, homeDir);
   strcat(configFile, "/.vjconfig/activeConfig");

   if (!chunkDB->load(configFile))
   {
      cerr << "vjKernel::loadConfig: DB Load failed to load file: " << configFile << endl;
      exit(1);
   }
   */
   // ------- OPEN Program specified Config file ------ //
   if(!mProgramConfigFile.empty())   // We have a filename
   {
      if (!chunkDB->load(mProgramConfigFile.c_str()))
      {
         cerr << "vjKernel::loadConfig: DB Load failed to load file: " << mProgramConfigFile << endl;
         exit(1);
      }
   }

   vjDEBUG(2) << "------------  Config Chunks ----------" << vjDEBUG_FLUSH;
   vjDEBUG(2) << (*chunkDB) << vjDEBUG_FLUSH;

}

void vjKernel::setupInputManager()
{
   vjDEBUG(0) << "   vjKernel::setupInputManager\n" << vjDEBUG_FLUSH;
   data.inputManager = new (sharedMemPool) vjInputManager;
   data.inputManager->FNewInput(chunkDB);

   vjDEBUG(0) << "      Input manager has passed. (Andy did good)" << endl << vjDEBUG_FLUSH;

   data.inputManager->UpdateAllData();

   vjDEBUG(0) << "      First Update trackers succeeded..." << endl << vjDEBUG_FLUSH;
}

void vjKernel::setupDisplayManager()
{
   vjDEBUG_BEGIN(0) << "------- vjKernel::setupDisplayManager\n -------" << vjDEBUG_FLUSH;

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

      vjDEBUG(0) << "Display: " << i << endl
                 << *newDisp << endl << flush << vjDEBUG_FLUSH;
   }

      // Tell Display manager to look for head
   displayManager->setupHeadIndices();

   vjDEBUG_END(0) << "------- vjKernel::setupDisplayManager --------\n" << vjDEBUG_FLUSH;
}

void vjKernel::setupDrawManager()
{
   vjDEBUG_BEGIN(0) << "   vjKernel::setupDrawManager" << endl << vjDEBUG_FLUSH;

   //drawManager = apiFactory->getDrawManager();
   drawManager = app->getDrawManager();
   drawManager->config(chunkDB);     // Give it the chunk DB to extract API specific info
   drawManager->setApp(app);
   drawManager->setDisplayManager(displayManager);

   app->init();                     // Init the app
   drawManager->initAPI();          // Just sets up API type stuff
   app->apiInit();                  // Have app do any app-init stuff
   drawManager->initDrawing();      // Configs and Starts drawing procs

   vjDEBUG_END(0) << "   vjKernel::setupDrawManager: Exiting." << endl << vjDEBUG_FLUSH;
}

