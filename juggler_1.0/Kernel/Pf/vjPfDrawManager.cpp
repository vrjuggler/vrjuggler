#include <vjConfig.h>
#include <Kernel/Pf/vjPfDrawManager.h>
#include <Kernel/Pf/vjPfApp.h>
#include <Kernel/vjDebug.h>
#include <Kernel/vjDisplayManager.h>
#include <Kernel/vjProjection.h>
#include <Config/vjConfigChunk.h>
#include <Kernel/vjSimulator.h>

#include <Performer/pfdu.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pf/pfScene.h>
#include <Performer/pf/pfChannel.h>

vjPfDrawManager* vjPfDrawManager::_instance = NULL;

void vjPfDrawManager::config(vjConfigChunkDB*  chunkDB)
{
   vector<vjConfigChunk*>* sgiChunks;
   sgiChunks = chunkDB->getMatching("displaySystem");       // Get the chunk to config system

   if (sgiChunks->size() <= 0)
      cerr << "vjPfDrawManager::config: ERROR: Chunks not found. " << endl;

   vjConfigChunk* sgiChunk = (*sgiChunks)[0];

   vjDEBUG_BEGIN(0) << "------------- vjPfDrawManager::config ----------------" << endl << vjDEBUG_FLUSH;
   numPipes = sgiChunk->getProperty("numpipes");

   vjDEBUG(0) << "NumPipes: " << numPipes << endl << vjDEBUG_FLUSH;
   for (int i=0;i<numPipes;i++)
   {
      pipeStrs.push_back(sgiChunk->getProperty("xpipes", i));
      if(strcmp(pipeStrs[i], "-1") == 0)    // Use display env
      {
         char* display_env = getenv("DISPLAY");
         char* xpipe_name  = new char[strlen(display_env)+1];
         strcpy(xpipe_name, display_env);
         pipeStrs[i] = xpipe_name;
      }
      vjDEBUG(0) << "Pipe:" << i << ": " << pipeStrs[i] << endl << vjDEBUG_FLUSH;
   }

      // --- Get simulator model info --- //
   vector<vjConfigChunk*>* perf_chunks;
   perf_chunks = chunkDB->getMatching("apiPerformer");      // Get the performer API chunks

   if(perf_chunks->size() > 0)
   {
      vjConfigChunk* perf_chunk = (*perf_chunks)[0];

      char* head_file = perf_chunk->getProperty("simHeadModel");
      char* wand_file = perf_chunk->getProperty("simWandModel");
      if(head_file == NULL)
         vjDEBUG(0) << "vjPfDrawManager::config: simHeadModel not set." << endl << vjDEBUG_FLUSH;
      if(wand_file == NULL)
         vjDEBUG(0) << "vjPfDrawManager::config: simWandModel not set." << endl << vjDEBUG_FLUSH;

      mHeadModel = string(head_file);
      mWandModel = string(wand_file);
   }

   vjDEBUG(0) << "Head Model: " << mHeadModel << endl
              << "Wand Model: " << mWandModel << endl << vjDEBUG_FLUSH;
   vjDEBUG_END(0) << "-----------------------------------------------------" << endl << vjDEBUG_FLUSH;
}


void vjPfDrawManager::sync()
{
   vjDEBUG(2) << "vjPfDrawManager::sync\n" << vjDEBUG_FLUSH;
   pfSync();
}

//! POST: Calls pfFrame()
void vjPfDrawManager::draw()
{
   vjDEBUG(2) << "vjPfDrawManager::draw\n" << vjDEBUG_FLUSH;

   pfFrame();
}

//: Set the app the draw whould interact with.
//! PRE: none
//! POST: dynamic_cast of the app to a pf app
void vjPfDrawManager::setApp(vjApp* _app)
{ app = dynamic_cast<vjPfApp*>(_app);}

//! POST: Calls pfInit()
void vjPfDrawManager::initAPI()
{
   pfInit();
}

void vjPfDrawManager::initDrawing()
{
   vjDEBUG_BEGIN(0) << "vjPfDrawManager::initDrawing: Entering." << endl << vjDEBUG_FLUSH;

   // Set params for Multi-pipe and Multiprocess
   pfMultipipe(numPipes);
   pfMultiprocess(PFMP_APP_CULL_DRAW);

   initLoaders();          // Must call before pfConfig

   // --- FORKS HERE --- //
   pfConfig();

   // ------ OPEN/ALLOCATE Pipes ------- //
   for (int pipeNum = 0; pipeNum < numPipes; pipeNum++)
   {
      vjDEBUG(0) << "vjPfDrawManager::initDrawing: Opening Pipe." << endl << vjDEBUG_FLUSH;
      vjDEBUG(0) << "\tpipe:" << pipeNum << ": " << pipeStrs[pipeNum] << endl << vjDEBUG_FLUSH;

      if(pipes.size() < pipeNum)    // Make sure there is room for the pipe
         pipes.push_back(NULL);
      pipes[pipeNum] = pfGetPipe(pipeNum);
      pipes[pipeNum]->setWSConnectionName(pipeStrs[pipeNum]);
      pipes[pipeNum]->setScreen(pipeNum);
   }

   // pipeWindow FrameBuffer config
   int stereo_win_attrs[] = { PFFB_DOUBLEBUFFER, PFFB_RGBA,
      PFFB_RED_SIZE, 1, PFFB_GREEN_SIZE, 1, PFFB_BLUE_SIZE, 1,
      PFFB_ALPHA_SIZE, 1, PFFB_DEPTH_SIZE, 1,
      PFFB_STEREO, None};

   int norm_win_attrs[] = { PFFB_DOUBLEBUFFER, PFFB_RGBA,
      PFFB_RED_SIZE, 1, PFFB_GREEN_SIZE, 1, PFFB_BLUE_SIZE, 1,
      PFFB_ALPHA_SIZE, 1, PFFB_DEPTH_SIZE, 1, None};

   //  For each display:
   //	    -Create a pWin for it
   //	    -Create the channels to put it the pWin
   vector<vjDisplay*> displays = displayManager->getDisplays();
   for (vector<vjDisplay*>::iterator dispIter = displays.begin();
       dispIter != displays.end(); dispIter++)
   {
      vjDEBUG(0) << "------- Opening new Display --------" << endl << vjDEBUG_FLUSH;

      pfDisp tempPfDisp;
      tempPfDisp.disp = (*dispIter);

      vjDEBUG(0) << "\tDisplay is:" << (void*)(*dispIter) << endl << vjDEBUG_FLUSH;
      vjDEBUG(0) << "\tvjPfDrawManager::initDrawing: Got Display:\n" << *(*dispIter) << vjDEBUG_FLUSH;

      int xo, yo, xs, ys;
      pfPipe* localPipe = pipes[tempPfDisp.disp->pipe()];
      vjASSERT(NULL != localPipe);        // Make sure we have a good pipe

      // --- Setup pWin --- //
      tempPfDisp.pWin = new pfPipeWindow(localPipe);
      tempPfDisp.disp->originAndSize(xo, yo, xs, ys);
      tempPfDisp.pWin->setOriginSize(xo, yo, xs, ys);

         // Setup window border
      if (tempPfDisp.disp->drawBorder())
         tempPfDisp.pWin->setName(tempPfDisp.disp->getName()); // Give the window a name
      else
         tempPfDisp.pWin->setMode(PFWIN_NOBORDER, 1);          // Get rid of that border

         // Setup Frame Buffer config
      if (tempPfDisp.disp->inStereo())                            // If we need stereo
      	tempPfDisp.pWin->setFBConfigAttrs(stereo_win_attrs);     // Configure framebuffer for stereo
      else
         tempPfDisp.pWin->setFBConfigAttrs(norm_win_attrs);       // Configure a "norm" window

      tempPfDisp.pWin->setConfigFunc(vjPFconfigPWin); // Set config function
      tempPfDisp.pWin->config();                      // Next pfFrame, config Func will be called

      // --- Setup Channels --- //
      // Left
      tempPfDisp.chans[pfDisp::LEFT] = new pfChannel(localPipe);
      tempPfDisp.chans[pfDisp::LEFT]->setViewport(0.0f, 1.0f, 0.0f, 1.0f);
      tempPfDisp.pWin->addChan(tempPfDisp.chans[pfDisp::LEFT]);

      // Right
      if(tempPfDisp.disp->inStereo())
      {
         tempPfDisp.chans[pfDisp::RIGHT] = new pfChannel(localPipe);
         tempPfDisp.chans[pfDisp::RIGHT]->setViewport(0.0f, 1.0f, 0.0f, 1.0f);
         tempPfDisp.pWin->addChan(tempPfDisp.chans[pfDisp::RIGHT]);
      }

      // -- Add new pfDisp to disp Vector -- //
      disps.push_back(tempPfDisp);      // This should make a COPY
   }

   initSimulator();        // Call here to load the scene graph
   initPerformerApp();

   // ----- SETUP MASTER CHANNEL ----- //
   pfChannel* masterChan = disps[0].chans[0];

   // Setup all Shared properties
   masterChan->setTravFunc(PFTRAV_DRAW, vjPfDrawFunc);
   masterChan->setNearFar(0.05, 10000.0f);

   if (masterChan == NULL)
      cerr << "vjPfDrawManager::initDrawing: MasterChan is NULL!!\n";

   masterChan->setShare(PFCHAN_NEARFAR | PFCHAN_EARTHSKY |
                        PFCHAN_STRESS | PFCHAN_LOD | PFCHAN_SWAPBUFFERS |
                        PFCHAN_APPFUNC | PFCHAN_CULLFUNC | PFCHAN_DRAWFUNC);

   // ----- SETUP CHANNEL GROUP ---- //
   for (int dispIndex=0; dispIndex<disps.size(); dispIndex++)
   {
      if (dispIndex != 0)
         masterChan->attach(disps[dispIndex].chans[pfDisp::LEFT]);
      if(disps[dispIndex].chans[pfDisp::RIGHT] != NULL)
      	masterChan->attach(disps[dispIndex].chans[pfDisp::RIGHT]);
   }

   // --- Setup channel's scene --- //
   // Channels that are drawing a simulator get a scene graph with
   // the simulator stuff in it.
   for(dispIndex=0;dispIndex<disps.size();dispIndex++)
   {
      if(disps[dispIndex].disp->isSimulator())
      {
         if (disps[dispIndex].chans[pfDisp::LEFT] != NULL)
            disps[dispIndex].chans[pfDisp::LEFT]->setScene(mRootWithSim);
      } else {
         if (disps[dispIndex].chans[pfDisp::LEFT] != NULL)
            disps[dispIndex].chans[pfDisp::LEFT]->setScene(sceneRoot);
         if (disps[dispIndex].chans[pfDisp::RIGHT] != NULL)
            disps[dispIndex].chans[pfDisp::RIGHT]->setScene(sceneRoot);
      }
   }

   pfFrame();

   // Dump the state
   debugDump();

   vjDEBUG_END(0) << "vjPfDrawManager::initDrawing: Exiting." << endl << vjDEBUG_FLUSH;
}


// Initializes the application's scene <br>
// Set's the sceneRoot
void vjPfDrawManager::initPerformerApp()
{
   app->initScene();
   sceneRoot = app->getScene();
   mRootWithSim->addChild(sceneRoot);
}

void vjPfDrawManager::initLoaders()
{
   pfdInitConverter(mHeadModel.c_str());
   pfdInitConverter(mWandModel.c_str());
}

void vjPfDrawManager::initSimulator()
{
   pfNode* head_node = pfdLoadFile(mHeadModel.c_str());
   pfNode* wand_node = pfdLoadFile(mWandModel.c_str());
   mSimTree = new pfGroup;
   mRootWithSim = new pfScene;
   mHeadDCS = new pfDCS;
   mWandDCS = new pfDCS;
   mSimTree->addChild(mHeadDCS);
   mSimTree->addChild(mWandDCS);
   mHeadDCS->addChild(head_node);
   mWandDCS->addChild(wand_node);
   mRootWithSim->addChild(mSimTree);
}

void vjPfDrawManager::updateSimulator(vjSimulator* sim)
{
   pfMatrix head_mat = vjGetPfMatrix(sim->getHeadPos());
   pfMatrix wand_mat = vjGetPfMatrix(sim->getWandPos());
   mHeadDCS->setMat(head_mat);
   mWandDCS->setMat(wand_mat);
}

void vjPfDrawManager::closeAPI()
{
   pfExit();
}


void vjPfDrawManager::updateProjections()
{
   vjDEBUG(2) << "vjPfDrawManager::updateProjections: Entering." << endl << vjDEBUG_FLUSH;

   vjDrawManager::updateProjections();

   // --- Update the channel projections --- //
   //for(each pfDisp)
   //    update Performer specific stuff.
   for (vector<pfDisp>::iterator i = disps.begin(); i != disps.end(); i++)
   {
      if(!((*i).disp->isSimulator()))
      {   // Update the projections
         updatePfProjection((*i).chans[pfDisp::LEFT], (*i).disp->leftProj);
         if((*i).disp->inStereo())
            updatePfProjection((*i).chans[pfDisp::RIGHT], (*i).disp->rightProj);
      } else {
         updateSimulator((*i).disp->mSim);
         updatePfProjection((*i).chans[pfDisp::LEFT], (*i).disp->cameraProj, true);
      }
   }
}

//! POST: chan has it's view matrix set to the Performer
//+        equivalent of proj's projection data.
void vjPfDrawManager::updatePfProjection(pfChannel* chan, vjProjection* proj, bool simulator)
{
   vjDEBUG_BEGIN(6) << "vjPfDrawManager::updatePfProjection: Entering. viewMat:\n"
                    << proj->viewMat << endl << vjDEBUG_FLUSH;

   pfMatrix pfViewMat;
   pfViewMat.set(proj->viewMat.getFloatPtr());      // Hmm...

      // The following is scary.  Don't break or you die!!!
      // Basically, Performer does a Rotate of 90 around X
      // first thing in modelview.  So, we have to undo that, put
      // our OGL matrix on, and then redo the Performer thing.
      // The matrix inversion is because Performer specifies
      // Their viewing params in modelling coords instead
      // of camera coords.
   pfViewMat.invertAff(pfViewMat);
   pfViewMat.preRot(-90, 1, 0, 0, pfViewMat);
   pfViewMat.postRot(pfViewMat, 90, 1, 0, 0);

   chan->setViewMat(pfViewMat);

   if(!simulator)
   {
      chan->setAutoAspect(PFFRUST_CALC_NONE);         // No auto aspect
      chan->setNearFar(proj->frustum[vjFrustum::NEAR], proj->frustum[vjFrustum::FAR]);
      chan->makePersp(proj->frustum[vjFrustum::LEFT], proj->frustum[vjFrustum::RIGHT],
                      proj->frustum[vjFrustum::BOTTOM], proj->frustum[vjFrustum::TOP]);
   } else {
      chan->setAutoAspect(PFFRUST_CALC_VERT);
      chan->setNearFar(0.1, 1000);
      chan->setFOV(80.0f, 0.0f);
   }

   vjDEBUG(7) << "Frustum: l:" << proj->frustum[vjFrustum::LEFT]
              << "   r: " << proj->frustum[vjFrustum::RIGHT]
              << "   b: " << proj->frustum[vjFrustum::BOTTOM]
              << "   t: " << proj->frustum[vjFrustum::TOP] << endl << vjDEBUG_FLUSH;

   vjDEBUG_END(6) << "vjPfDrawManager::updatePfProjection: Exiting.\n" << vjDEBUG_FLUSH;
}



void vjPfDrawManager::debugDump()
{
   vjDEBUG_BEGIN(0) << "-- DEBUG DUMP --------- vjPfDrawManager: " << (void*)this << " ------------" << endl << vjDEBUG_FLUSH;
   vjDEBUG(0)       << "app:" << (void*)app << endl << vjDEBUG_FLUSH;
   vjDEBUG(0)       << "scene:" << (void*)sceneRoot << endl << vjDEBUG_FLUSH;
   vjDEBUG(0)       << "Disps:" << disps.size() << endl << vjDEBUG_FLUSH;

   for (vector<pfDisp>::iterator i = disps.begin(); i != disps.end(); i++)
   {
      vjDEBUG(0) << "\n\tDisplay:" << (void*)(i->disp) << endl << vjDEBUG_FLUSH;
      vjDEBUG(0) << "\tpWin:" << (void*)(i->pWin) << endl << vjDEBUG_FLUSH;
      vjDEBUG(0) << "\t\tvis id:" << hex << i->pWin->getFBConfigId() << vjDEBUG_FLUSH;
      vjDEBUG(0) << "\tchans: L:" << (void*)(i->chans[pfDisp::LEFT]) << "\tR:" << (void*)(i->chans[pfDisp::RIGHT]) << endl << vjDEBUG_FLUSH;
   }
   vjDEBUG_END(0) << "-------- Dump end ----\n" << vjDEBUG_FLUSH;
}


// Config function called in draw proc after window is set up
void vjPFconfigPWin(pfPipeWindow* pWin)
{
   vjPfDrawManager* dm = vjPfDrawManager::instance();

   // Init the vj monitor modes
   pWin->open();

   // Call user config function
   dm->app->initPWin(pWin);
}


// --- Traversal functions --- //
void vjPfDrawFunc(pfChannel *chan, void* chandata)
{
   vjPfDrawManager* dm = vjPfDrawManager::instance();
   vjPfDrawManager::pfDisp* cur_pf_disp = dm->getPfDisp(chan);

      // Select correct buffer to draw to
      // If we are in stereo and not a simulator
   if((cur_pf_disp->disp->inStereo()) && (!cur_pf_disp->disp->isSimulator()))
   {
      if(cur_pf_disp->chans[vjPfDrawManager::pfDisp::LEFT] == chan)
         glDrawBuffer(GL_BACK_LEFT);
      else
         glDrawBuffer(GL_BACK_RIGHT);
   }
   else                                // No Stereo or have sim, so just go to back buffer
   {
      glDrawBuffer(GL_BACK);
   }

      // -- Configure buffers for correct eye/stereo etc -- //
   dm->app->drawChan(chan, chandata);     // Draw the channel
                                          // Note: This function calls pfDraw and clears

      // Should we draw the simulator
   if(cur_pf_disp->disp->isSimulator())
   {

   }
}
