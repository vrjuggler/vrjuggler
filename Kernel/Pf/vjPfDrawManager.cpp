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
#define PF_C_API 1

#include <vjConfig.h>

#include <Performer/pf.h>
#include <Performer/pfdu.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pf/pfScene.h>
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pfdu.h>
#include <Performer/pf/pfTraverser.h>

#include <Kernel/Pf/vjPfDrawManager.h>
#include <Kernel/Pf/vjPfApp.h>
#include <Kernel/vjDebug.h>
#include <Kernel/vjDisplayManager.h>
#include <Kernel/vjProjection.h>
#include <Kernel/vjCameraProjection.h>
#include <Config/vjConfigChunk.h>
#include <Kernel/vjSimDisplay.h>
#include <Kernel/vjSurfaceDisplay.h>
#include <Utils/vjFileIO.h>

// Draw Callbacks
void vjPfDrawFuncStereoLeft(pfChannel *chan, void* chandata);
void vjPfDrawFuncStereoRight(pfChannel *chan, void* chandata);
void vjPfDrawFuncMonoBackbuffer(pfChannel *chan, void* chandata);
void vjPfDrawFuncSimulator(pfChannel* chan, void* chandata);

//void vjPfAppFunc(pfChannel *chan, void* chandata);

//vjPfDrawManager* vjPfDrawManager::_instance = NULL;
vjSingletonImp(vjPfDrawManager);

//: Can the handler handle the given chunk?
//! RETURNS: true - Can handle it
//+          false - Can't handle it
bool vjPfDrawManager::configCanHandle(vjConfigChunk* chunk)
{
   std::string chunk_type = chunk->getType();
   return ( chunk_type == std::string("apiPerformer"));
}

//: Add the chunk to the configuration
//! PRE: configCanHandle(chunk) == true
//! RETURNS: success
bool vjPfDrawManager::configAdd(vjConfigChunk* chunk)
{
   vjASSERT(configCanHandle(chunk));

   std::string chunk_type = chunk->getType();

   if(chunk_type == std::string("apiPerformer"))
   {
      return configPerformerAPI(chunk);
   }

   return false;
}


bool vjPfDrawManager::configDisplaySystem(vjConfigChunk* chunk)
{
   vjASSERT(chunk != NULL);
   vjASSERT((std::string)chunk->getType() == std::string("displaySystem"));

   // ---- SETUP PipeStr's ---- //
   vjDEBUG_BEGIN(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL)
      << "------------- vjPfDrawManager::config ----------------" << std::endl
      << vjDEBUG_FLUSH;
   mNumPipes = (unsigned int)(int)chunk->getProperty("numpipes");

   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "NumPipes: " << mNumPipes
                                            << std::endl << vjDEBUG_FLUSH;
   for (unsigned int i=0;i<mNumPipes;i++)
   {
      char cur_disp_name[] = "-1";

      mPipeStrs.push_back(chunk->getProperty("xpipes", i).cstring());

      if(strcmp(mPipeStrs[i], cur_disp_name) == 0)    // Use display env
      {
	 char env_var[] = "DISPLAY";
         char* display_env = getenv(env_var);
         char* xpipe_name  = new char[strlen(display_env)+1];
         strcpy(xpipe_name, display_env);
         mPipeStrs[i] = xpipe_name;
      }
      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "Pipe:" << i << ": "
                                               << mPipeStrs[i] << std::endl
                                               << vjDEBUG_FLUSH;
   }
   return true;
}

//: Configure the performer api stuff
bool vjPfDrawManager::configPerformerAPI(vjConfigChunk* chunk)
{
   vjASSERT((std::string)chunk->getType() == std::string("apiPerformer"));

   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << clrOutNORM(clrRED,"vjPfDrawManager::configPerformerAPI:")
                                            << " Configuring Performer\n" << vjDEBUG_FLUSH;

   // --- Get simulator model info --- //
   char* head_file = chunk->getProperty("simHeadModel").cstring();
   char* wand_file = chunk->getProperty("simWandModel").cstring();
   if(head_file == NULL)
      vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL)
         << "WARNING: vjPfDrawManager::config: simHeadModel not set."
         << std::endl << vjDEBUG_FLUSH;
   if(wand_file == NULL)
      vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL)
         << "WARNING: vjPfDrawManager::config: simWandModel not set."
         << std::endl << vjDEBUG_FLUSH;

   mHeadModel = vjFileIO::replaceEnvVars(head_file);
   mWandModel = vjFileIO::replaceEnvVars(wand_file);

   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL)
      << "Head Model: " << mHeadModel.c_str() << std::endl
      << "Wand Model: " << mWandModel.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_END(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL)
      << "-----------------------------------------------------" << std::endl
      << vjDEBUG_FLUSH;
   return true;
}


void vjPfDrawManager::sync()
{
   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << "vjPfDrawManager::sync\n" << vjDEBUG_FLUSH;
   pfSync();
}

//! POST: Calls pfFrame()
void vjPfDrawManager::draw()
{
   vjDEBUG(vjDBG_ALL,vjDBG_VERB_LVL) << "vjPfDrawManager::calling appChanFuncs\n" << vjDEBUG_FLUSH;
   callAppChanFuncs();

   vjDEBUG(vjDBG_ALL,vjDBG_VERB_LVL) << "vjPfDrawManager::draw\n" << vjDEBUG_FLUSH;
   //vjDEBUG(vjDBG_ALL,0) << "vjPfDrawManager::draw\n" << vjDEBUG_FLUSH;

   pfFrame();
}

//:
// XXX: Hack for now
void vjPfDrawManager::callAppChanFuncs()
{
   for(unsigned int dispIndex=0;dispIndex<mSurfDisps.size();dispIndex++)
   {
      if (mSurfDisps[dispIndex].chans[pfDisp::PRIMARY] != NULL)
         app->appChanFunc(mSurfDisps[dispIndex].chans[pfDisp::PRIMARY]);
      if (mSurfDisps[dispIndex].chans[pfDisp::SECONDARY] != NULL)
         app->appChanFunc(mSurfDisps[dispIndex].chans[pfDisp::SECONDARY]);
   }

   for(unsigned int dispIndex=0;dispIndex<mSimDisps.size();dispIndex++)
   {
      if (mSimDisps[dispIndex].chans[pfDisp::PRIMARY] != NULL)
         app->appChanFunc(mSimDisps[dispIndex].chans[pfDisp::PRIMARY]);
      if (mSimDisps[dispIndex].chans[pfDisp::SECONDARY] != NULL)
         app->appChanFunc(mSimDisps[dispIndex].chans[pfDisp::SECONDARY]);
   }
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

   // XXX: This call should really be triggered by a change in draw manager or something
   vjASSERT(mDisplayManager != NULL);
   configDisplaySystem(mDisplayManager->getDisplaySystemChunk());    // Configure all the display system stuff

   app->preForkInit();

   initDrawing();
}

void vjPfDrawManager::initDrawing()
{
   vjDEBUG_BEGIN(vjDBG_DRAW_MGR,vjDBG_STATE_LVL) << "vjPfDrawManager::initDrawing: Entering." << std::endl << vjDEBUG_FLUSH;

   // Set params for Multi-pipe and Multiprocess
   pfMultipipe(mNumPipes);
   pfMultiprocess(PFMP_APP_CULL_DRAW);
   //pfMultiprocess(PFMP_APPCULLDRAW);

   initLoaders();          // Must call before pfConfig

   // --- FORKS HERE --- //
   pfConfig();

   initSimulator();        // Call here to load the scene graph
   initPerformerApp();

   // ------ OPEN/ALLOCATE Pipes ------- //
   for (unsigned int pipeNum = 0; pipeNum < mNumPipes; pipeNum++)
   {
      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager::initDrawing: Opening Pipe." << std::endl << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "\tpipe:" << pipeNum << ": " << mPipeStrs[pipeNum] << std::endl << vjDEBUG_FLUSH;

      // Make sure there is room for the pipe.  The check must be made using
      // less than or equal because pipe numbering may start from 0, and an
      // empty vector has size 0 but still needs room for a pipe entry.
      while(pipes.size() <= pipeNum)
         pipes.push_back(NULL);

      pipes[pipeNum] = pfGetPipe(pipeNum);
      pipes[pipeNum]->setWSConnectionName(mPipeStrs[pipeNum]);
      pipes[pipeNum]->setScreen(pipeNum);
   }

   // Get frame buffer config
   std::vector<int> stereo_fb_config = getStereoFBConfig();
   std::vector<int> mono_fb_config = getMonoFBConfig();

   // ouput debug info about the frame buffer config recieved
   unsigned int i;
   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << "vjPfDrawManager::initDrawing: Got Stereo FB config\n" << vjDEBUG_FLUSH;
   for(i=0;i<stereo_fb_config.size();i++)
      vjDEBUG_CONT(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << "  " << stereo_fb_config[i] << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << "\nvjPfDrawManager::initDrawing: Got Mono FB config\n" << vjDEBUG_FLUSH;
   for(i=0;i<mono_fb_config.size();i++)
      vjDEBUG_CONT(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << "  " << mono_fb_config[i] << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << std::endl << vjDEBUG_FLUSH;

   //  For each display:
   //     -Create a pWin for it
   //     -Create the channels to put it the pWin
   std::vector<vjDisplay*> displays = mDisplayManager->getActiveDisplays();
   for (std::vector<vjDisplay*>::iterator dispIter = displays.begin();
       dispIter != displays.end(); dispIter++)
   {
      vjDEBUG_BEGIN(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager: ---- Opening new Display --------" << std::endl << vjDEBUG_FLUSH;

      pfDisp tempPfDisp;
      tempPfDisp.disp = (*dispIter);

      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "\tDisplay is:" << (void*)(*dispIter) << std::endl << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "\tvjPfDrawManager::initDrawing: Got Display:\n" << (*dispIter) << vjDEBUG_FLUSH;

      int xo, yo, xs, ys;
      pfPipe* localPipe = pipes[tempPfDisp.disp->getPipe()];
      vjASSERT(NULL != localPipe);        // Make sure we have a good pipe

      // --- Setup pWin --- //
      tempPfDisp.pWin = new pfPipeWindow(localPipe);
      vjASSERT(NULL != tempPfDisp.pWin);

      tempPfDisp.disp->getOriginAndSize(xo, yo, xs, ys);
      tempPfDisp.pWin->setOriginSize(xo, yo, xs, ys);

      // Setup window border
      if (tempPfDisp.disp->shouldDrawBorder())
         tempPfDisp.pWin->setName(tempPfDisp.disp->getName().c_str()); // Give the window a name
      else
         tempPfDisp.pWin->setMode(PFWIN_NOBORDER, 1);          // Get rid of that border

      // Setup Frame Buffer config
      if (tempPfDisp.disp->inStereo())                            // If we need stereo
      {
         vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager::initDrawing: Configuring stereo window attribs.\n" << vjDEBUG_FLUSH;
         tempPfDisp.pWin->setFBConfigAttrs(&(stereo_fb_config[0]));     // Configure framebuffer for stereo
      }
      else
      {
         vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager::initDrawing: Configuring mono window attribs.\n" << vjDEBUG_FLUSH;
         tempPfDisp.pWin->setFBConfigAttrs(&(mono_fb_config[0]));       // Configure a "norm" window
      }

      // -- Set pwin config info -- //
      tempPfDisp.pWin->setConfigFunc(vjPFconfigPWin); // Set config function
      tempPfDisp.pWin->config();                      // Next pfFrame, config Func will be called

      // --- Setup Channels --- //
      // Primary channel - (Left in stereo)
      tempPfDisp.chans[pfDisp::PRIMARY] = new pfChannel(localPipe);
      tempPfDisp.chans[pfDisp::PRIMARY]->setViewport(0.0f, 1.0f, 0.0f, 1.0f);
      tempPfDisp.pWin->addChan(tempPfDisp.chans[pfDisp::PRIMARY]);

      // Secondary channel - (Right in stereo)
      if(tempPfDisp.disp->inStereo())
      {
         tempPfDisp.chans[pfDisp::SECONDARY] = new pfChannel(localPipe);
         tempPfDisp.chans[pfDisp::SECONDARY]->setViewport(0.0f, 1.0f, 0.0f, 1.0f);
         tempPfDisp.pWin->addChan(tempPfDisp.chans[pfDisp::SECONDARY]);
      }
      else
      {
         tempPfDisp.chans[pfDisp::SECONDARY] = NULL;
      }

      // ----- SET DRAW FUNC ------ //
      if(tempPfDisp.disp->inStereo() && (!tempPfDisp.disp->isSimulator()))
      {
         tempPfDisp.chans[pfDisp::PRIMARY]->setTravFunc(PFTRAV_DRAW, vjPfDrawFuncStereoLeft);
         tempPfDisp.chans[pfDisp::SECONDARY]->setTravFunc(PFTRAV_DRAW, vjPfDrawFuncStereoRight);
      }
      else if(tempPfDisp.disp->isSimulator())
      {
         tempPfDisp.chans[pfDisp::PRIMARY]->setTravFunc(PFTRAV_DRAW, vjPfDrawFuncSimulator);
      }
      else
      {
         tempPfDisp.chans[pfDisp::PRIMARY]->setTravFunc(PFTRAV_DRAW, vjPfDrawFuncMonoBackbuffer);
      }

      // -- Add new pfDisp to disp Vector -- //
      // This should make a COPY
      if(tempPfDisp.disp->isSimulator())
         mSimDisps.push_back(tempPfDisp);
      else
         mSurfDisps.push_back(tempPfDisp);

      vjDEBUG_END(vjDBG_DRAW_MGR,vjDBG_STATE_LVL) << "---- Display Open (done) --------" << std::endl << vjDEBUG_FLUSH;
   }

   // ----- SETUP Surface MASTER CHANNEL ----- //
   if(mSurfDisps.size() > 0)
   {
      mSurfMasterChan = mSurfDisps[0].chans[pfDisp::PRIMARY];
      vjASSERT(mSurfMasterChan != NULL);

      mSurfMasterChan->setScene(mSceneRoot);       // Set the shared "normal" scene

      initChanGroupAttribs(mSurfMasterChan);       // Setup the channel group attribs

            // Attach channel groups
      for (unsigned dispIndex=0; dispIndex<mSurfDisps.size(); dispIndex++)
      {
         pfChannel* primary_ch = mSurfDisps[dispIndex].chans[pfDisp::PRIMARY];
         pfChannel* secondary_ch = mSurfDisps[dispIndex].chans[pfDisp::SECONDARY];
         vjASSERT(NULL != primary_ch);

         if (dispIndex != 0)                    // XXX: Assumes that all displays will have a valid primary channel
            mSurfMasterChan->attach(primary_ch);
         if(secondary_ch != NULL)
            mSurfMasterChan->attach(secondary_ch);
      }
   }
   else
      mSurfMasterChan = NULL;

   // ----- SETUP Sim MASTER CHANNEL ----- //
   if(mSimDisps.size() > 0)
   {
      mSimMasterChan = mSimDisps[0].chans[pfDisp::PRIMARY];
      vjASSERT(mSimMasterChan != NULL);

      mSimMasterChan->setScene(mRootWithSim);       // Set the shared "normal" scene
      initChanGroupAttribs(mSimMasterChan);       // Setup the channel group attribs

            // Attach channel groups
      for (unsigned dispIndex=0; dispIndex<mSimDisps.size(); dispIndex++)
      {
         pfChannel* primary_ch = mSimDisps[dispIndex].chans[pfDisp::PRIMARY];
         pfChannel* secondary_ch = mSimDisps[dispIndex].chans[pfDisp::SECONDARY];
         vjASSERT(NULL != primary_ch);

         if (dispIndex != 0)                    // XXX: Assumes that all displays will have a valid primary channel
            mSimMasterChan->attach(primary_ch);
         if(secondary_ch != NULL)
            mSimMasterChan->attach(secondary_ch);
      }
   }
   else
      mSimMasterChan = NULL;


   vjASSERT(mSceneRoot != NULL && "We have a NULL root scene in vjPfDrawManager");
   vjASSERT(mRootWithSim != NULL && "We have a NULL sim root scene in vjPfDrawManager");

   //pfFrame();

   // Dump the state
   debugDump(vjDBG_CONFIG_LVL);

   vjDEBUG_END(vjDBG_DRAW_MGR,vjDBG_STATE_LVL) << "vjPfDrawManager::initDrawing: Exiting." << std::endl << vjDEBUG_FLUSH;
}

// Initialize the parameters of the master channel
// Sets the attribs to share
void vjPfDrawManager::initChanGroupAttribs(pfChannel* masterChan)
{
   //masterChan->setNearFar(0.05, 10000.0f);      // XXX: Look here near far information

   vjASSERT(masterChan != NULL);

   /*  Original code for reference
    * masterChan->setShare(PFCHAN_NEARFAR | PFCHAN_EARTHSKY |
    *                    PFCHAN_STRESS | PFCHAN_LOD | PFCHAN_SWAPBUFFERS |
    *                    PFCHAN_APPFUNC | PFCHAN_SCENE | PFCHAN_CULLFUNC | PFCHAN_STATS_DRAWMODE );
    */

   //
   // Set the Channel attribs based on cur settings
   //
   unsigned cur_share = masterChan->getShare();          // Get current setting, and OR the new stuff on
   unsigned turn_on = ( PFCHAN_NEARFAR | PFCHAN_EARTHSKY |
                        PFCHAN_STRESS | PFCHAN_LOD | PFCHAN_SWAPBUFFERS |
                        PFCHAN_APPFUNC | PFCHAN_SCENE | PFCHAN_CULLFUNC |
                        PFCHAN_STATS_DRAWMODE);
   unsigned turn_off = (   PFCHAN_FOV |
                           PFCHAN_VIEW |
                           PFCHAN_VIEW_OFFSETS |
                           PFCHAN_DRAWFUNC );

   masterChan->setShare((cur_share | turn_on) &(~turn_off));

   //masterChan->setTravFunc(PFTRAV_APP, vjPfAppFunc);
   //masterChan->setShare(PFCHAN_NEARFAR | PFCHAN_EARTHSKY |
   //                     PFCHAN_STRESS | PFCHAN_SWAPBUFFERS |
   //                     PFCHAN_APPFUNC | PFCHAN_CULLFUNC );
}


//: Return the needed mono frame buffer config
std::vector<int> vjPfDrawManager::getMonoFBConfig()
{
   std::vector<int> mono_fb;
   mono_fb.push_back(PFFB_DOUBLEBUFFER);
   mono_fb.push_back(PFFB_RGBA);
   mono_fb.push_back(PFFB_RED_SIZE); mono_fb.push_back(8);
   mono_fb.push_back(PFFB_GREEN_SIZE); mono_fb.push_back(8);
   mono_fb.push_back(PFFB_BLUE_SIZE); mono_fb.push_back(8);
   mono_fb.push_back(PFFB_ALPHA_SIZE); mono_fb.push_back(8);
   mono_fb.push_back(PFFB_DEPTH_SIZE); mono_fb.push_back(16);

   // Add application requests
   std::vector<int> app_fb = app->getFrameBufferAttrs();
   mono_fb.insert(mono_fb.end(), app_fb.begin(), app_fb.end());

   mono_fb.push_back(None);

   return mono_fb;
}

//: Return the needed stereo frame buffer config
std::vector<int> vjPfDrawManager::getStereoFBConfig()
{
   std::vector<int> stereo_fb;
   stereo_fb.push_back(PFFB_DOUBLEBUFFER);
   stereo_fb.push_back(PFFB_RGBA);
   stereo_fb.push_back(PFFB_STEREO);
   stereo_fb.push_back(PFFB_RED_SIZE); stereo_fb.push_back(8);
   stereo_fb.push_back(PFFB_GREEN_SIZE); stereo_fb.push_back(8);
   stereo_fb.push_back(PFFB_BLUE_SIZE); stereo_fb.push_back(8);
   stereo_fb.push_back(PFFB_ALPHA_SIZE); stereo_fb.push_back(8);
   stereo_fb.push_back(PFFB_DEPTH_SIZE); stereo_fb.push_back(16);

   // Add application requests
   std::vector<int> app_fb = app->getFrameBufferAttrs();
   stereo_fb.insert(stereo_fb.end(), app_fb.begin(), app_fb.end());

   stereo_fb.push_back(None);
   return stereo_fb;
}



//: Callback when display is added to display manager
void vjPfDrawManager::addDisplay(vjDisplay* disp)
{
   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager:addDisplay\n" << disp << std::endl << vjDEBUG_FLUSH;
}


// Initializes the application's scene <br>
// Set's the sceneRoot
void vjPfDrawManager::initPerformerApp()
{
   app->initScene();
   mSceneRoot = new pfScene;
   mSceneGroup = app->getScene();
   mSceneRoot->addChild(mSceneGroup);        // Create the base scene without sim
   mRootWithSim->addChild(mSceneGroup);      // Create base scene with sim
}

void vjPfDrawManager::initLoaders()
{
   if(!mHeadModel.empty())
      pfdInitConverter(mHeadModel.c_str());
   if(!mWandModel.empty())
      pfdInitConverter(mWandModel.c_str());
}

void vjPfDrawManager::initSimulator()
{
   pfNode* head_node(NULL);
   pfNode* wand_node(NULL);

   if(!mHeadModel.empty())
   {
      head_node = pfdLoadFile(mHeadModel.c_str());     // Load head model
      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager: Loaded head model: " << mHeadModel.c_str() << std::endl << vjDEBUG_FLUSH;
   } else {
      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager: No wand head specified.\n" << vjDEBUG_FLUSH;
   }

   if(!mWandModel.empty())
   {
      wand_node = pfdLoadFile(mWandModel.c_str());     // Load wand model
      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager: Loaded wand model: " << mWandModel.c_str() << std::endl << vjDEBUG_FLUSH;
   } else {
      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager: No wand model specified.\n" << vjDEBUG_FLUSH;
   }

   mSimTree = new pfGroup;
   mRootWithSim = new pfScene;
   mHeadDCS = new pfDCS;
   mWandDCS = new pfDCS;
   mSimTree->addChild(mHeadDCS);
   mSimTree->addChild(mWandDCS);
   if(NULL != head_node)
   {
      mHeadDCS->addChild(head_node);
   }
   if(NULL != wand_node)
   {
      mWandDCS->addChild(wand_node);
   }

   if((head_node != NULL) && (wand_node != NULL))
   {
      mRootWithSim->addChild(mSimTree);      // Put sim stuff in the graph
   }
}

void vjPfDrawManager::updateSimulator(vjSimDisplay* sim)
{
   vjMatrix vj_head_mat = sim->getHeadPos();          // Get Juggler matrices
   vjMatrix vj_wand_mat = sim->getWandPos();
   pfMatrix head_mat = vjGetPfMatrix(vj_head_mat);    // Convert to Performer
   pfMatrix wand_mat = vjGetPfMatrix(vj_wand_mat);
   mHeadDCS->setMat(head_mat);                        // Set the DCS nodes
   mWandDCS->setMat(wand_mat);
}

void vjPfDrawManager::closeAPI()
{
   pfExit();
}


void vjPfDrawManager::updateProjections()
{
   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << "vjPfDrawManager::updateProjections: Entering." << std::endl << vjDEBUG_FLUSH;

   // --- Update the channel projections --- //
   //for(each pfDisp)
   //    update Performer specific stuff.
   for (std::vector<pfDisp>::iterator i = mSurfDisps.begin(); i != mSurfDisps.end(); i++)
   {
      vjSurfaceDisplay* surf_disp = dynamic_cast<vjSurfaceDisplay*>((*i).disp);
      vjASSERT(surf_disp != NULL && "Could not cast supposedly surface display to vjSurfaceDisplay.");
      vjDisplay::DisplayView view = surf_disp->getView();

      if(vjDisplay::LEFT_EYE == view)
      {
         updatePfProjection((*i).chans[pfDisp::PRIMARY], surf_disp->getLeftProj(),false);
      }
      else if(vjDisplay::RIGHT_EYE == view)
      {
         updatePfProjection((*i).chans[pfDisp::PRIMARY], surf_disp->getRightProj(),false);
      }
      else if(vjDisplay::STEREO == view)
      {
         updatePfProjection((*i).chans[pfDisp::PRIMARY], surf_disp->getLeftProj(),false);
         updatePfProjection((*i).chans[pfDisp::SECONDARY], surf_disp->getRightProj(),false);
      }
      else
      {
         vjASSERT(false && "vjPfDrawManager::updateProjections(): We don't have a valid display type, don't know what to do");
      }
   }

   for(std::vector<pfDisp>::iterator i = mSimDisps.begin(); i != mSimDisps.end(); i++)
   {
      vjSimDisplay* sim_disp = dynamic_cast<vjSimDisplay*>((*i).disp);
      vjASSERT(sim_disp != NULL && "Could not cast supposedly simulator display to vjSimDisplay.");

      updateSimulator(sim_disp);
      updatePfProjection((*i).chans[pfDisp::PRIMARY], sim_disp->getCameraProj(), true);
   }

}

//! POST: chan has it's view matrix set to the Performer
//+        equivalent of proj's projection data.
void vjPfDrawManager::updatePfProjection(pfChannel* chan, vjProjection* proj, bool simulator)
{
   vjDEBUG_BEGIN(vjDBG_DRAW_MGR,vjDBG_HVERB_LVL) << "vjPfDrawManager::updatePfProjection: Entering. viewMat:\n"
                    << proj->mViewMat << std::endl << vjDEBUG_FLUSH;

   pfMatrix pfViewMat;
   pfViewMat.set(proj->mViewMat.getFloatPtr());      // Hmm...

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
      chan->setNearFar(proj->mFrustum[vjFrustum::VJ_NEAR], proj->mFrustum[vjFrustum::VJ_FAR]);
      chan->makePersp(proj->mFrustum[vjFrustum::VJ_LEFT], proj->mFrustum[vjFrustum::VJ_RIGHT],
                      proj->mFrustum[vjFrustum::VJ_BOTTOM], proj->mFrustum[vjFrustum::VJ_TOP]);
   } else {
      vjCameraProjection* cam_proj = dynamic_cast<vjCameraProjection*>(proj);
      vjASSERT(cam_proj != NULL && "Trying to use non-camera projection for simulator");
      chan->setAutoAspect(PFFRUST_CALC_HORIZ);
      chan->setNearFar(proj->mFrustum[vjFrustum::VJ_NEAR], proj->mFrustum[vjFrustum::VJ_FAR]);
      chan->setFOV(0.0f, cam_proj->mVertFOV);
   }

   vjDEBUG(vjDBG_DRAW_MGR,7) << "Frustum: l:" << proj->mFrustum[vjFrustum::VJ_LEFT]
              << "   r: " << proj->mFrustum[vjFrustum::VJ_RIGHT]
              << "   b: " << proj->mFrustum[vjFrustum::VJ_BOTTOM]
              << "   t: " << proj->mFrustum[vjFrustum::VJ_TOP] << std::endl << vjDEBUG_FLUSH;

   vjDEBUG_END(vjDBG_DRAW_MGR,6) << "vjPfDrawManager::updatePfProjection: Exiting.\n" << vjDEBUG_FLUSH;
}


//: Helper function that finds the pfDisp given a channel
// This function just loops through all the entries in the disps variable,
// looking for one that contains the channel.  When it is found, it is
// returned.
// NOTE: The "cool" STL functor search didn't work for some reason
vjPfDrawManager::pfDisp* vjPfDrawManager::getPfDisp(pfChannel* chan)
{
   // Search surface displays
   for(unsigned int i=0;i<mSurfDisps.size();i++)
   {
      pfChannel* primary_chan = mSurfDisps[i].chans[pfDisp::PRIMARY];
      pfChannel* secondary_chan = mSurfDisps[i].chans[pfDisp::SECONDARY];
      if((chan == primary_chan) || (chan == secondary_chan))
         return &(mSurfDisps[i]);
   }

   // Search simulator displays
   for(unsigned int i=0;i<mSimDisps.size();i++)
   {
      pfChannel* primary_chan = mSimDisps[i].chans[pfDisp::PRIMARY];
      pfChannel* secondary_chan = mSimDisps[i].chans[pfDisp::SECONDARY];
      if((chan == primary_chan) || (chan == secondary_chan))
         return &(mSimDisps[i]);
   }

   return NULL;
}

void vjPfDrawManager::debugDump(int debugLevel)
{
   vjDEBUG_BEGIN(vjDBG_DRAW_MGR,debugLevel) << "-- DEBUG DUMP --------- " << clrOutNORM(clrCYAN,"vjPfDrawManager:") << (void*)this << " ------------" << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)       << "app:" << (void*)app << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)       << "scene:" << (void*)mSceneRoot << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)       << "sim scene:" << (void*)mRootWithSim << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)       << "Surf Disps:" << mSurfDisps.size() << std::endl << vjDEBUG_FLUSH;
   for (std::vector<pfDisp>::iterator i = mSurfDisps.begin(); i != mSurfDisps.end(); i++)
   {
      debugDumpPfDisp(&(*i),debugLevel);
   }
   vjDEBUG(vjDBG_DRAW_MGR,debugLevel)       << "Sim Disps:" << mSimDisps.size() << std::endl << vjDEBUG_FLUSH;
   for (std::vector<pfDisp>::iterator i = mSimDisps.begin(); i != mSimDisps.end(); i++)
   {
      debugDumpPfDisp(&(*i),debugLevel);
   }

   vjDEBUG_END(vjDBG_DRAW_MGR,debugLevel) << "-------- Dump end ----\n" << vjDEBUG_FLUSH;
}


void vjPfDrawManager::debugDumpPfDisp(pfDisp* pf_disp, int debugLevel)
{
   vjDEBUG_BEGIN(vjDBG_DRAW_MGR,debugLevel) << "Display:" << (void*)(pf_disp->disp) << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)  << "pWin:" << (void*)(pf_disp->pWin) << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)  << "vis id:" << std::hex << pf_disp->pWin->getFBConfigId() << std::dec << std::endl << vjDEBUG_FLUSH;

   pfChannel* l_chan = (pf_disp->chans[pfDisp::PRIMARY]);
   pfChannel* r_chan = (pf_disp->chans[pfDisp::SECONDARY]);
   unsigned lc_mask,rc_mask;
   if (l_chan != NULL)
      lc_mask = l_chan->getShare();
   if (r_chan != NULL)
      rc_mask = r_chan->getShare();

   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)
         << "chans: L:" << (void*)l_chan
         << "  shared: FOV:" << (lc_mask & PFCHAN_FOV)
         << " Scene:" << (lc_mask & PFCHAN_SCENE)
         << " AppFunc:" << (lc_mask & PFCHAN_APPFUNC)
         << " SwapBuff:" << (lc_mask & PFCHAN_SWAPBUFFERS)
         << " SwapBuff-HW:" << (lc_mask & PFCHAN_SWAPBUFFERS_HW)
         << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)
         << "       R:" << (void*)r_chan
         << "  shared: FOV:" << (rc_mask & PFCHAN_FOV)
         << " Scene:" << (rc_mask & PFCHAN_SCENE)
         << " AppFunc:" << (rc_mask & PFCHAN_APPFUNC)
         << " SwapBuff:" << (rc_mask & PFCHAN_SWAPBUFFERS)
         << " SwapBuff-HW:" << (rc_mask & PFCHAN_SWAPBUFFERS_HW)
         << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT_END(vjDBG_DRAW_MGR,debugLevel) << vjDEBUG_FLUSH;
}



// Config function called in draw proc after window is set up
void vjPFconfigPWin(pfPipeWindow* pWin)
{
   vjPfDrawManager* dm = vjPfDrawManager::instance();

   // Init the vj monitor modes
   pWin->open();
   pfInitGfx();

   // Call user config function
   dm->app->configPWin(pWin);

   // Ouput the visual id
   int fb_id = pWin->getFBConfigId();
   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL)
      << "vjPFConfigPWin: framebuffer id: x" << std::hex << fb_id << std::dec
      << std::endl << vjDEBUG_FLUSH;
}


/*
void vjPfAppFunc(pfChannel *chan, void* chandata)
{
   vjDEBUG(vjDBG_DRAW_MGR, 0) << "--- vjPfAppFunc: Enter ---.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR, 0) << "chan: " << chan << std::endl << vjDEBUG_FLUSH;

   pfChannel* master_chan = vjPfDrawManager::instance()->mMasterChan;
   if(master_chan == chan)
   {
      vjDEBUG(vjDBG_DRAW_MGR,0) << "I am the master of the house:\n" << vjDEBUG_FLUSH;
      if(vjPfDrawManager::instance()->mPfAppCalled == false)      // Haven't called it yet
      {
         vjDEBUG(vjDBG_DRAW_MGR,0) << "pfApp has not been called yet.  Let me do it...\n" << vjDEBUG_FLUSH;
         vjPfDrawManager::instance()->mPfAppCalled = true;
         pfApp();
      }
   }
   else
   {
      vjDEBUG(vjDBG_DRAW_MGR,0) << "I am not master. I can't do anything.\n" << vjDEBUG_FLUSH;
   }
}
*/


// --- Traversal functions --- //
// This function is the performer draw callback function
// - Sets up the correct OGL drawing buffer
// - Calls the app draw chan function
//template <bool left_eye, bool right_eye, bool stereo, bool simulator>
void vjPfDrawFunc(pfChannel *chan, void* chandata,bool left_eye, bool right_eye, bool stereo, bool simulator)
{
   vjDEBUG_BEGIN(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "--- vjPfDrawFunc: Enter ---.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "chan: " << chan << std::endl << vjDEBUG_FLUSH;

      // Select correct buffer to draw to
      // If we are in stereo and not a simulator
   if(stereo && !simulator)
   {
      vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "vjPfDrawFunc: Drawing stereo win\n" << vjDEBUG_FLUSH;
      if(left_eye)
      {
         glDrawBuffer(GL_BACK_LEFT);
         vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "vjPfDrawFunc: Set to BACK_LEFT\n" << vjDEBUG_FLUSH;
      }
      else
      {
         glDrawBuffer(GL_BACK_RIGHT);
         vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "vjPfDrawFunc: Set to BACK_RIGHT\n" << vjDEBUG_FLUSH;
      }
   }
   else                                // No Stereo or have sim, so just go to back buffer
   {
      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << "vjPfDrawFunc: Drawing mono window\n" << vjDEBUG_FLUSH;
      glDrawBuffer(GL_BACK);
   }

   // -- Configure buffers for correct eye/stereo etc -- //
   vjPfDrawManager::instance()->app->drawChan(chan, chandata);     // Draw the channel
                                          // Note: This function calls pfDraw and clears

   // How should we draw the simulator
   /*
   if(cur_pf_disp->disp->isSimulator())
   {

   }
   */

   vjDEBUG_END(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "--- vjPfDrawFunc: Exit ---.\n" << vjDEBUG_FLUSH;
}

/*********************************************************************
*  Draw traversal functions.
*
*  These functions are just place holders for the drawing function
*  They call the drawing function with specific parameters
***********************************************************************/
void vjPfDrawFuncStereoLeft(pfChannel *chan, void* chandata)
{
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "--- vjPfDrawFuncStereoLeft: Enter ---.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "chan: " << chan << std::endl << vjDEBUG_FLUSH;
   vjPfDrawFunc(chan,chandata,true,false,true,false);
}

void vjPfDrawFuncStereoRight(pfChannel *chan, void* chandata)
{
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "--- vjPfDrawFuncStereoRight: Enter ---.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "chan: " << chan << std::endl << vjDEBUG_FLUSH;
   vjPfDrawFunc(chan,chandata,false,true,true,false);
}

void vjPfDrawFuncMonoBackbuffer(pfChannel *chan, void* chandata)
{
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "--- vjPfDrawFuncMonoBackbuffer: Enter ---.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "chan: " << chan << std::endl << vjDEBUG_FLUSH;
   vjPfDrawFunc(chan,chandata,false,false,false,false);
}

void vjPfDrawFuncSimulator(pfChannel* chan, void* chandata)
{
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "--- vjPfDrawFuncSimulator: Enter ---.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "chan: " << chan << std::endl << vjDEBUG_FLUSH;
   vjPfDrawFunc(chan,chandata,false,false,false,true);
}

