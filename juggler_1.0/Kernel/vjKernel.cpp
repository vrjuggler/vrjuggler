#include <vjConfig.h>
#include <string.h>
#include <Kernel/vjKernel.h>
#include <Config/vjChunkFactory.h>

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
//!NOTE: Does initial configuration and then sends config file to configAdd
//!POST: Shared Memory Initialized
//----------------------------------------------
void vjKernel::initConfig()
{
      vjDEBUG_BEGIN(0) << "vjKernel::initConfig: Entered.\n" << vjDEBUG_FLUSH;

   vjSharedPool::init();         // Try to init the pool stuff
   sharedMemPool = new vjSharedPool(1024*1024);      // XXX: should not be system specific

   // --- Read default config file --- //
   loadConfigFile();

   // Setup initial environments.
   initialSetupInputManager();
   initialSetupDisplayManager();
   setupEnvironmentManager();

   configAdd(mChunkDB);       // Setup the configuration

   //apiFactory = app->api.getAPIFactory();
   sysFactory = vjSGISystemFactory::instance(); // XXX: Should not be system specific

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

   // setup performance buffer
   performanceEnabled = 0;
   vector<vjConfigChunk*>* perfchunks = mChunkDB->getMatching ("PerfMeasure");
   if (perfchunks->size() > 0) {
       vjConfigChunk* perfchunk = (*perfchunks)[0];
       performanceEnabled = (*perfchunks)[0]->getProperty ("KernelEnabled");
       delete perfchunks;
   }
   perfBuffer = new vjPerfDataBuffer ("Kernel loop",
				      500, 7, performanceEnabled);
   environmentManager->addPerfDataBuffer (perfBuffer);


   //while(!Exit)
   while (1)
   {
       // perfBuffer->set (0);
         vjDEBUG(3) << "vjKernel::controlLoop: app->preDraw()\n" << vjDEBUG_FLUSH;
      app->preDraw();         // Do Any application pre-draw stuff
      perfBuffer->set (0);
         vjDEBUG(3) << "vjKernel::controlLoop: drawManager->draw()\n" << vjDEBUG_FLUSH;
      drawManager->draw();    // Trigger the beginning of frame drawing
      perfBuffer->set (1);
         vjDEBUG(3) << "vjKernel::controlLoop: app->postDraw\n" << vjDEBUG_FLUSH;
      app->postDraw();        // Do computations that can be done while drawing.  This should be for next frame.
      perfBuffer->set (2);
         vjDEBUG(3) << "vjKernel::controlLoop: drawManager->sync()\n" << vjDEBUG_FLUSH;
      drawManager->sync();    // Block until drawing is done
      perfBuffer->set (3);
         vjDEBUG(3) << "vjKernel::controlLoop: app->postSync()\n" << vjDEBUG_FLUSH;
      app->postSync();        // Do processing after drawing is complete
      perfBuffer->set (4);
      // Sync should be here for Kernel changes from
      // the environment manager

      //Tell trackers to swap buffers;
         vjDEBUG(3) << "vjKernel::controlLoop: Update Trackers\n" << vjDEBUG_FLUSH;
      data.inputManager->UpdateAllData();
      perfBuffer->set(5);
         vjDEBUG(3) << "vjKernel::controlLoop: Update Projections\n" << vjDEBUG_FLUSH;
      drawManager->updateProjections();
      perfBuffer->set(6);
   }
}

void vjKernel::updateFrameData()
{
   drawManager->updateProjections();
}

//---------------------------------
// Config routines
//---------------------------------
void vjKernel::configAdd(vjConfigChunkDB* chunkDB)
{
   // Dependency sort the items
   int dep_result = chunkDB->dependencySort();

   // If sort fails, exit with error
   if(dep_result == -1)
   {
      vjDEBUG(0) << "vjKernel::configAdd: ERROR: Dependency sort failed. Aborting add.\n" << vjDEBUG_FLUSH;
      return;
   }

   // Get sorted list
   vector<vjConfigChunk*> chunks = chunkDB->getChunks();

   // For each element
   for(int i=0;i<chunks.size();i++)
   {
      bool added_chunk = false;

      if(getInputManager()->configCanHandle(chunks[i]))  // inputMgr
         added_chunk = getInputManager()->configAdd(chunks[i]);
      if(displayManager->configCanHandle(chunks[i]))     // displayMgr
         added_chunk = displayManager->configAdd(chunks[i]);
      // drawMgr
      // app

      // --- Check for adding to active config --- //
      if(added_chunk)      // if added: add to config database
      {}
      else                 // Else: Give unrecognized error
      {
         vjDEBUG(0) << "vjKernel::configAdd: Unrecognized chunk.\n"
                    << "   type: " << (char*)chunks[i]->getType() << endl << vjDEBUG_FLUSH;
      }
   }

   // Dump status
   getInputManager()->DumpStatus();

}

void vjKernel::configRemove(vjConfigChunkDB* chunkDB)
{
   ;
}

// --- STARTUP ROUTINES --- //
void vjKernel::loadConfigFile()
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
   vjChunkFactory::setChunkDescDB (configDesc);
   if (!configDesc->load(chunk_desc_file))
   {
      cerr << "vjKernel::loadConfig: Config Desc failed to load file: " << endl;
      exit(1);
   }

   mChunkDB = new vjConfigChunkDB(configDesc);      // Create config database

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
      if (!mChunkDB->load(mProgramConfigFile.c_str()))
      {
         cerr << "vjKernel::loadConfig: DB Load failed to load file: " << mProgramConfigFile << endl;
         exit(1);
      }
   }

   vjDEBUG(2) << "------------  Config Chunks ----------" << vjDEBUG_FLUSH;
   vjDEBUG(2) << (*mChunkDB) << vjDEBUG_FLUSH;

}


void vjKernel::initialSetupInputManager()
{
   vjDEBUG(0) << "   vjKernel::initialSetupInputManager\n" << vjDEBUG_FLUSH;
   data.inputManager = new (sharedMemPool) vjInputManager;
   data.inputManager->ConfigureInitial(mChunkDB);
}


void vjKernel::initialSetupDisplayManager()
{
   vjDEBUG_BEGIN(0) << "------- vjKernel::initialSetupDisplayManager\n -------" << vjDEBUG_FLUSH;

   // Get display manager
   displayManager = vjDisplayManager::instance();
}

void vjKernel::setupDrawManager()
{
   vjDEBUG_BEGIN(0) << "   vjKernel::setupDrawManager" << endl << vjDEBUG_FLUSH;

   //drawManager = apiFactory->getDrawManager();
   drawManager = app->getDrawManager();
   drawManager->config(mChunkDB);     // Give it the chunk DB to extract API specific info
   drawManager->setApp(app);
   drawManager->setDisplayManager(displayManager);

   app->init();                     // Init the app
   drawManager->initAPI();          // Just sets up API type stuff
   app->apiInit();                  // Have app do any app-init stuff
   drawManager->initDrawing();      // Configs and Starts drawing procs

   vjDEBUG_END(0) << "   vjKernel::setupDrawManager: Exiting." << endl << vjDEBUG_FLUSH;
}


void vjKernel::setupEnvironmentManager() {
   vjDEBUG(0) << "   vjKernel::setupEnvironmentManager\n"
              << vjDEBUG_FLUSH;
   vjTimeStamp::initialize();
   environmentManager = new vjEnvironmentManager();
   environmentManager->activate();
   vjDEBUG(0) << "      Environment Manager running\n" << vjDEBUG_FLUSH;
}
