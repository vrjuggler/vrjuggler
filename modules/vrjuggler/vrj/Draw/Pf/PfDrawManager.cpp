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

#include <Kernel/vjDisplayManager.h>
#include <Kernel/Pf/vjPfDrawManager.h>
#include <Kernel/Pf/vjPfApp.h>
#include <Utils/vjDebug.h>

#include <Kernel/vjProjection.h>
#include <Kernel/vjCameraProjection.h>

#include <Kernel/vjSimViewport.h>
#include <Kernel/vjSurfaceViewport.h>

#include <Config/vjConfigChunk.h>

#include <Utils/vjFileIO.h>

namespace vrj
{
   
// Draw Callbacks
void PfDrawFuncStereoLeft(pfChannel *chan, void* chandata);
void PfDrawFuncStereoRight(pfChannel *chan, void* chandata);
void PfDrawFuncMonoBackbuffer(pfChannel *chan, void* chandata);
void PfDrawFuncSimulator(pfChannel* chan, void* chandata);
//void PfAppFunc(pfChannel *chan, void* chandata);

//vjPfDrawManager* PfDrawManager::_instance = NULL;
vprSingletonImp(PfDrawManager);


//: Can the handler handle the given chunk?
//! RETURNS: true - Can handle it
//+          false - Can't handle it
bool PfDrawManager::configCanHandle(ConfigChunk* chunk)
{
   std::string chunk_type = chunk->getType();
   return ( chunk_type == std::string("apiPerformer"));
}

//: Add the chunk to the configuration
//! PRE: configCanHandle(chunk) == true
//! RETURNS: success
bool PfDrawManager::configAdd(ConfigChunk* chunk)
{
   vprASSERT(configCanHandle(chunk));

   std::string chunk_type = chunk->getType();

   if(chunk_type == std::string("apiPerformer"))
   {
      return configPerformerAPI(chunk);
   }

   return false;
}


// Configure the Performer display settings that are needed
// - Number of pipes to allow
// - The xpipe strings to use
bool PfDrawManager::configDisplaySystem(ConfigChunk* chunk)
{
   vprASSERT(chunk != NULL);
   vprASSERT((std::string)chunk->getType() == std::string("displaySystem"));

   // ---- SETUP PipeStr's ---- //
   vjDEBUG_BEGIN(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL)
      << "------------- PfDrawManager::config ----------------" << std::endl
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
bool PfDrawManager::configPerformerAPI(ConfigChunk* chunk)
{
   vprASSERT((std::string)chunk->getType() == std::string("apiPerformer"));

   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager::configPerformerAPI:"
                                            << " Configuring Performer\n" << vjDEBUG_FLUSH;

   // --- Get simulator model info --- //
   char* head_file = chunk->getProperty("simHeadModel").cstring();
   char* wand_file = chunk->getProperty("simWandModel").cstring();
   if(head_file == NULL)
      vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL)
         << "WARNING: PfDrawManager::config: simHeadModel not set."
         << std::endl << vjDEBUG_FLUSH;
   if(wand_file == NULL)
      vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL)
         << "WARNING: PfDrawManager::config: simWandModel not set."
         << std::endl << vjDEBUG_FLUSH;

   mHeadModel = FileIO::replaceEnvVars(head_file);
   mWandModel = FileIO::replaceEnvVars(wand_file);

   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL)
      << "Head Model: " << mHeadModel.c_str() << std::endl
      << "Wand Model: " << mWandModel.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_END(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL)
      << "-----------------------------------------------------" << std::endl
      << vjDEBUG_FLUSH;
   return true;
}


void PfDrawManager::sync()
{
   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << "vjPfDrawManager::sync\n" << vjDEBUG_FLUSH;
   pfSync();
}

//! POST: Calls pfFrame()
void PfDrawManager::draw()
{
   vjDEBUG(vjDBG_ALL,vjDBG_VERB_LVL) << "vjPfDrawManager::calling appChanFuncs\n" << vjDEBUG_FLUSH;
   callAppChanFuncs();

   vjDEBUG(vjDBG_ALL,vjDBG_VERB_LVL) << "vjPfDrawManager::draw\n" << vjDEBUG_FLUSH;
   //vjDEBUG(vjDBG_ALL,0) << "vjPfDrawManager::draw\n" << vjDEBUG_FLUSH;

   pfFrame();
}

//:
// XXX: Hack for now
void PfDrawManager::callAppChanFuncs()
{
   // for(each display)
   //    for(each viewport)
   //       for(each channel)
   for(unsigned int dispIndex=0;dispIndex<mDisplays.size();dispIndex++)
   {
      for(unsigned vp=0;vp<mDisplays[dispIndex].viewports.size();vp++)
      {
         for(unsigned ch=0;ch<2;ch++)
         {
            if(mDisplays[dispIndex].viewports[vp].chans[ch] != NULL)
               app->appChanFunc(mDisplays[dispIndex].viewports[vp].chans[ch]);
         }
      }
   }
}


//: Set the app the draw whould interact with.
//! PRE: none
//! POST: dynamic_cast of the app to a pf app
void PfDrawManager::setApp(App* _app)
{
   //vprASSERT(app != NULL);
   app = dynamic_cast<PfApp*>(_app);
   if(mPfHasForked)
      initAppGraph();         // If pf is already started, then intialize the app scene graph

}

//! POST: Calls pfInit() and sets up the system
// - Configures multiprocessing mode
// - Configures number of pipes
// - Forks off the processes
void PfDrawManager::initAPI()
{
   pfInit();

   // XXX: This call should really be triggered by a change in draw manager or something
   vprASSERT(mDisplayManager != NULL);
   configDisplaySystem(mDisplayManager->getDisplaySystemChunk());    // Configure all the display system stuff

   app->preForkInit();

   vjDEBUG_BEGIN(vjDBG_DRAW_MGR,vjDBG_STATE_LVL) << "vjPfDrawManager::initAPI: Entering." << std::endl << vjDEBUG_FLUSH;

   // Set params for Multi-pipe and Multiprocess
   pfMultipipe(mNumPipes);
   pfMultiprocess(PFMP_APP_CULL_DRAW);
   //pfMultiprocess(PFMP_APPCULLDRAW);

   initLoaders();          // Must call before pfConfig

   // --- FORKS HERE --- //
   pfConfig();
   mPfHasForked = true;

   // Initialize the pipes that the system may need
   // If we don't do this, then pf automatically give each pipe a big black channel
   initPipes();

   initSimulatorGraph();        // Create the simulator scene graph nodes
   initPerformerGraph();        // Create the other scene graph nodes
   if(app != NULL)
      initAppGraph();           // App was already set, but pf was not loaded.  So load graph now


   vprASSERT(mRoot != NULL && "We have a NULL root in PfDrawManager");
   vprASSERT(mRootWithSim != NULL && "We have a NULL sim root scene in PfDrawManager");
   vprASSERT(mSceneRoot != NULL && "We have a NULL root scene in PfDrawManager");

   //pfFrame();

   // Dump the state
//   debugDump(vjDBG_CONFIG_LVL);

   vjDEBUG_END(vjDBG_DRAW_MGR,vjDBG_STATE_LVL) << "vjPfDrawManager::initAPI: Exiting." << std::endl << vjDEBUG_FLUSH;
}

// Get a performer pipe
//! PRE: pipe_num < mNumPipes
//       Fork must have happend
pfPipe* PfDrawManager::getPfPipe(unsigned pipe_num)
{
   vprASSERT((mPfHasForked) && "Tried to get pipe before forking happened");
   vprASSERT((pipe_num < mNumPipes) && "Tried to request out of bounds pipe");
   vprASSERT((pipe_num < mPipes.size()) && "Tried to get out of range pipe");

   // Return the actual pipe
   vprASSERT((NULL != mPipes[pipe_num]) && "Have NULL pipe");
   return mPipes[pipe_num];
}

// Initialize all the pipes that the system may need to use
// XXX: If performer were more flexible, then this would actually be
//      done on demand while the application is running :(
void PfDrawManager::initPipes()
{
   mPipes.resize(mNumPipes, NULL);     // Resize the vector

   for(unsigned pipe_num=0; pipe_num<mNumPipes; pipe_num++)
   {
      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager::initPipes: Opening Pipe." << std::endl << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "\tpipe:" << pipe_num << ": " << mPipeStrs[pipe_num] << std::endl << vjDEBUG_FLUSH;

      mPipes[pipe_num] = pfGetPipe(pipe_num);
      mPipes[pipe_num]->setWSConnectionName(mPipeStrs[pipe_num]);
      mPipes[pipe_num]->setScreen(pipe_num);

      pfPipeWindow* pw = allocatePipeWin(pipe_num);   // new pfPipeWindow(mPipes[pipe_num]);
      pw->setOriginSize(0,0,1,1);
   }
}


//: Callback when display is added to display manager
//! PRE: Must be in kernel controlling thread
//  PRE: Must have already initialized performer
void PfDrawManager::addDisplay(Display* disp)
{
   vprASSERT(disp != NULL);    // Can't add a null display
   vprASSERT((true == mPfHasForked) && "Trying to add display when performer has not been initialized");

   // Get frame buffer config
   std::vector<int> stereo_fb_config = getStereoFBConfig();
   std::vector<int> mono_fb_config = getMonoFBConfig();

   // ouput debug info about the frame buffer config recieved
   unsigned int i;
   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << "vjPfDrawManager::addDisplay: Got Stereo FB config\n" << vjDEBUG_FLUSH;
   for(i=0;i<stereo_fb_config.size();i++)
      vjDEBUG_CONT(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << "  " << stereo_fb_config[i] << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << "\nPfDrawManager::addDisplay: Got Mono FB config\n" << vjDEBUG_FLUSH;
   for(i=0;i<mono_fb_config.size();i++)
      vjDEBUG_CONT(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << "  " << mono_fb_config[i] << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << std::endl << vjDEBUG_FLUSH;

   //  For the display
   //     -Create a pWin for it
   //     - For each viewport
   //        - Create viewport
   //        - Create channels for the viewports
   vjDEBUG_BEGIN(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager: ---- Opening new Display --------" << std::endl << vjDEBUG_FLUSH;

   pfDisplay pf_disp;            // The pfDisplay to use
   pf_disp.disp = disp;

   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "\tDisplay is:" << (void*)(disp) << std::endl << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "\tPfDrawManager::add Display: Got Display:\n" << (*disp) << vjDEBUG_FLUSH;

   int xo, yo, xs, ys;
   pfPipe* pipe = getPfPipe(disp->getPipe());      // Get the pipe
   vprASSERT(NULL != pipe);                         // Make sure we have a good pipe

   // --- SETUP PWIN --- //
   pf_disp.pWin = allocatePipeWin(disp->getPipe());     //  new pfPipeWindow(pipe);
   vprASSERT(NULL != pf_disp.pWin);

   disp->getOriginAndSize(xo, yo, xs, ys);
   pf_disp.pWin->setOriginSize(xo, yo, xs, ys);

   // Setup window border
   if (disp->shouldDrawBorder())
      pf_disp.pWin->setName(disp->getName().c_str()); // Give the window a name
   else
      pf_disp.pWin->setMode(PFWIN_NOBORDER, 1);          // Get rid of that border

   // Setup Frame Buffer config
   if (disp->inStereo())                            // If we need stereo
   {
      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager::addDisplay: Configuring stereo window attribs.\n" << vjDEBUG_FLUSH;
      pf_disp.pWin->setFBConfigAttrs(&(stereo_fb_config[0]));     // Configure framebuffer for stereo
   }
   else
   {
      vjDEBUG(vjDBG_DRAW_MGR,vjDBG_CONFIG_LVL) << "vjPfDrawManager::addDisplay: Configuring mono window attribs.\n" << vjDEBUG_FLUSH;
      pf_disp.pWin->setFBConfigAttrs(&(mono_fb_config[0]));       // Configure a "norm" window
   }

   // -- Set config info -- //
   pf_disp.pWin->setConfigFunc(PFconfigPWin); // Set config function
   pf_disp.pWin->config();                      // Next pfFrame, config Func will be called

   // --- SETUP VIEWPORTS --- //
   // for(each viewport)
   //   - Allocate channels
   //   - Set draw functions
   //   - Setup master chans
   Viewport* viewport = NULL;
   unsigned num_vps = disp->getNumViewports();

   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_CONFIG_LVL) << "   Num viewports: " << num_vps << std::endl << vjDEBUG_FLUSH;
   for(unsigned vp_num=0; vp_num < num_vps; vp_num++)
   {
      viewport = disp->getViewport(vp_num);

      if(viewport->isActive())      // is viewport active
      {
         Viewport::View view = viewport->getView();
         pfViewport pf_viewport;                         // The viewport to build up
         pf_viewport.viewport = viewport;
         float vp_ox, vp_oy, vp_sx, vp_sy;

         // Get channel info
         // Primary channel - (Left in stereo)
         viewport->getOriginAndSize(vp_ox, vp_oy, vp_sx, vp_sy);
         pf_viewport.chans[pfViewport::PRIMARY] = new pfChannel(pipe);
         pf_viewport.chans[pfViewport::PRIMARY]->setViewport(vp_ox, vp_ox+vp_sx, vp_oy, vp_oy+vp_sy);
         pf_disp.pWin->addChan(pf_viewport.chans[pfViewport::PRIMARY]);

         // Secondary channel - (Right in stereo)
         if(disp->inStereo())
         {
            pf_viewport.chans[pfViewport::SECONDARY] = new pfChannel(pipe);
            pf_viewport.chans[pfViewport::SECONDARY]->setViewport(vp_ox, vp_ox+vp_sx, vp_oy, vp_oy+vp_sy);
            pf_disp.pWin->addChan(pf_viewport.chans[pfViewport::SECONDARY]);
         }

         // Set draw function
         if(disp->inStereo() && (!viewport->isSimulator()))
         {
            pf_viewport.chans[pfViewport::PRIMARY]->setTravFunc(PFTRAV_DRAW, PfDrawFuncStereoLeft);
            pf_viewport.chans[pfViewport::SECONDARY]->setTravFunc(PFTRAV_DRAW, PfDrawFuncStereoRight);
         }
         else if(viewport->isSimulator())
         {
            pf_viewport.chans[pfViewport::PRIMARY]->setTravFunc(PFTRAV_DRAW, PfDrawFuncSimulator);
         }
         else
         {
            pf_viewport.chans[pfViewport::PRIMARY]->setTravFunc(PFTRAV_DRAW, PfDrawFuncMonoBackbuffer);
         }

         // if surface ==> Setup surface channels
         if (viewport->isSurface())
         {
            vprASSERT(pf_viewport.chans[pfViewport::PRIMARY] != NULL);

            // Primary
            if(NULL == mSurfMasterChan)      // If NULL, then add us as the new one
            {
               mSurfMasterChan = pf_viewport.chans[pfViewport::PRIMARY];
               mSurfMasterChan->setScene(mRoot);
               initChanGroupAttribs(mSurfMasterChan);
               mSurfChannels.push_back(mSurfMasterChan);
            }
            else
            {
               mSurfChannels.push_back(pf_viewport.chans[pfViewport::PRIMARY]);
               mSurfMasterChan->attach(pf_viewport.chans[pfViewport::PRIMARY]);
            }

            // Secondary
            if(NULL != pf_viewport.chans[pfViewport::SECONDARY])
            {
               mSurfChannels.push_back(pf_viewport.chans[pfViewport::SECONDARY]);
               mSurfMasterChan->attach(pf_viewport.chans[pfViewport::SECONDARY]);
            }

         }
         // if sim ==> setup sim channels
         else if(viewport->isSimulator())
         {
            vprASSERT(pf_viewport.chans[pfViewport::PRIMARY] != NULL);

            // Primary
            if(NULL == mSimMasterChan)        // If NULL, then make us the master
            {
               mSimMasterChan = pf_viewport.chans[pfViewport::PRIMARY];
               mSimMasterChan->setScene(mRootWithSim);       // Set the shared "normal" scene
               initChanGroupAttribs(mSimMasterChan);       // Setup the channel group attribs
               mSimChannels.push_back(mSimMasterChan);
            }
            else
            {
               mSimChannels.push_back(pf_viewport.chans[pfViewport::PRIMARY]);
               mSimMasterChan->attach(pf_viewport.chans[pfViewport::PRIMARY]);
            }

            // Secondary
            if(NULL != pf_viewport.chans[pfViewport::SECONDARY])
            {
               mSimChannels.push_back(pf_viewport.chans[pfViewport::SECONDARY]);
               mSimMasterChan->attach(pf_viewport.chans[pfViewport::SECONDARY]);
            }
         }

         // Add viewport to the display list
         pf_disp.viewports.push_back(pf_viewport);

      }  // is viewport active
   }     // for each viewport

   // -- Add new pfDisp to disp Vector -- //
   mDisplays.push_back(pf_disp);


   vjDEBUG_END(vjDBG_DRAW_MGR,vjDBG_STATE_LVL) << "---- Display Open (done) --------" << std::endl << vjDEBUG_FLUSH;

   // Dump the state
   vjDEBUG(vjDBG_DRAW_MGR, 1) << "Reconfiged the pfDrawManager.\n" << vjDEBUG_FLUSH;
   //vjDEBUG(vjDBG_DRAW_MGR, 1) << (*this) << vjDEBUG_FLUSH;
   debugDump(1);
}


//: Callback when display is removed to display manager
//! PRE: disp must be a valid display that we have
//! POST: window for disp is removed from the draw manager and child pipes
void PfDrawManager::removeDisplay(Display* disp)
{
   // Find the pfDisplay
   std::vector<pfDisplay>::iterator disp_i = std::find_if(mDisplays.begin(), mDisplays.end(),
                         std::compose1( std::bind2nd( std::equal_to<Display*>(),disp),
                                        pfDisplay_disp()) );


   if(mDisplays.end() == disp_i)
   {
      vjDEBUG(vjDBG_DRAW_MGR, vjDBG_CRITICAL_LVL) << "ERROR: PfDrawManager::removeDisplay: Tried to remove a non-existant display\n" << vjDEBUG_FLUSH;
      return;
   }


   // Release the pfDisplay
   releaseDisplay(*disp_i);

   // Remove display from the list
   mDisplays.erase(disp_i);
}

void PfDrawManager::releaseDisplay(pfDisplay& disp)
{
   // Release all viewports
   for(std::vector<pfViewport>::iterator i=disp.viewports.begin(); i != disp.viewports.end(); i++)
   {
      releaseViewport(disp, *i);
   }

   // Release the pipe window
   releasePipeWin(disp.pWin, disp.disp->getPipe());
}


void PfDrawManager::releaseViewport(pfDisplay& disp, pfViewport& vp)
{
   std::vector<pfChannel*>::iterator chan_i;

   // Release the channel stuff
   for(int i=0;i<2;i++)
   {
      pfChannel* chan = vp.chans[i];

      // if we have a channel to delete
      if(chan != NULL)
      {
         if(vp.viewport->isSurface())         // SURFACE display
         {
            // Remove the channel from the list of channels
            chan_i = std::find(mSurfChannels.begin(), mSurfChannels.end(), chan);
            if(chan_i == mSurfChannels.end())
            {
               vprASSERT(false && "Trying to remove a non-existant channel");
               vjDEBUG(0,0) << "Trying to remove a non-existant pfChannel\n" << vjDEBUG_FLUSH;
            }

            // Check if we were the master
            if(chan == mSurfMasterChan)
            {
               mSurfMasterChan = NULL;                      // Get new master channel
               if(mSurfChannels.size() > 0)
                  mSurfMasterChan = mSurfChannels[0];
            }

            if(mSurfMasterChan != NULL)                  // Dettach from the channel
               chan->detach(mSurfMasterChan);
         }
         else if(vp.viewport->isSimulator())    // SIMULATOR display
         {
            // Remove the channel from the list of channels
            chan_i = std::find(mSimChannels.begin(), mSimChannels.end(), chan);
            if(chan_i == mSimChannels.end())
            {
               vprASSERT(false && "Trying to remove a non-existant channel");
               vjDEBUG(0,0) << "Trying to remove a non-existant pfChannel";
            }

            // Check if we were the master
            if(chan == mSimMasterChan)
            {
               mSimMasterChan = NULL;                      // Get new master channel
               if(mSimChannels.size() > 0)
                  mSimMasterChan = mSimChannels[0];
            }

            if(mSimMasterChan != NULL)                  // Dettach from the channel
               chan->detach(mSimMasterChan);
         }

         chan->setScene(NULL);
         disp.pWin->removeChan(chan);     // Remove channel from pwin
         pfDelete( chan);                     // Delete the channel
      }
   }
}



// Get a pipe window to use
// This either allocates a new pipe window or grabs an unused one that was previously released
pfPipeWindow* PfDrawManager::allocatePipeWin(unsigned pipeNum)
{
   pfPipeWindow* ret_val;

   if(mPipeWindows[pipeNum].size() > 0)   // Is one available
   {
      ret_val = mPipeWindows[pipeNum].back();
      mPipeWindows[pipeNum].pop_back();
   }
   else
   {
      ret_val = new pfPipeWindow(getPfPipe(pipeNum));         // Allocate a new one
   }

   vprASSERT((ret_val != NULL) && "We have a null pfPipeWindow*");

   return ret_val;
}

// Just store the old pipe window in the list
void PfDrawManager::releasePipeWin(pfPipeWindow* pipeWin, unsigned pipeNum)
{
   mPipeWindows[pipeNum].push_back(pipeWin);
}



// Initialize the parameters of the master channel
// Sets the attribs to share
void PfDrawManager::initChanGroupAttribs(pfChannel* masterChan)
{
   //masterChan->setNearFar(0.05, 10000.0f);      // XXX: Look here near far information

   vprASSERT(masterChan != NULL);

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

   //masterChan->setTravFunc(PFTRAV_APP, PfAppFunc);
   //masterChan->setShare(PFCHAN_NEARFAR | PFCHAN_EARTHSKY |
   //                     PFCHAN_STRESS | PFCHAN_SWAPBUFFERS |
   //                     PFCHAN_APPFUNC | PFCHAN_CULLFUNC );
}


//: Return the needed mono frame buffer config
std::vector<int> PfDrawManager::getMonoFBConfig()
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
std::vector<int> PfDrawManager::getStereoFBConfig()
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

void PfDrawManager::initLoaders()
{
   if(!mHeadModel.empty())
      pfdInitConverter(mHeadModel.c_str());
   if(!mWandModel.empty())
      pfdInitConverter(mWandModel.c_str());
}


// Initializes the application's scene <br>
// Set's the sceneRoot
void PfDrawManager::initPerformerGraph()
{
   mRoot = new pfScene;
   mSceneGroup = new pfGroup;                // (Placeholder until app loads theirs)
   mRoot->addChild(mSceneGroup);        // Create the base scene without sim
   mRootWithSim->addChild(mSceneGroup);      // Create base scene with sim
}

// Initialize the application graph
// - init scene
// - Remove old app scene child
// - replace with new app scene child
void PfDrawManager::initAppGraph()
{
   app->initScene();
   if(mSceneRoot != NULL)
      mSceneGroup->removeChild(mSceneRoot);

   mSceneRoot = app->getScene();
   mSceneGroup->addChild(mSceneRoot);
}


void PfDrawManager::initSimulatorGraph()
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

void PfDrawManager::updateSimulator(SimViewport* sim_vp)
{
   Matrix vj_head_mat = sim_vp->getHeadPos();          // Get Juggler matrices
   Matrix vj_wand_mat = sim_vp->getWandPos();
   pfMatrix head_mat = GetPfMatrix(vj_head_mat);    // Convert to Performer
   pfMatrix wand_mat = GetPfMatrix(vj_wand_mat);
   mHeadDCS->setMat(head_mat);                        // Set the DCS nodes
   mWandDCS->setMat(wand_mat);
}

void PfDrawManager::closeAPI()
{
   pfExit();
}


void PfDrawManager::updateProjections()
{
   vjDEBUG(vjDBG_DRAW_MGR,vjDBG_VERB_LVL) << "vjPfDrawManager::updateProjections: Entering." << std::endl << vjDEBUG_FLUSH;

   // --- Update the channel projections --- //
   //for(each pfDisp)
   //   for(each viewport)
   //       update Performer specific stuff.
   for (unsigned disp_id=0;disp_id<mDisplays.size();disp_id++)
   {
      pfDisplay* cur_disp = &(mDisplays[disp_id]);

      vprASSERT(cur_disp->disp != NULL);
      for(unsigned vp=0;vp<cur_disp->viewports.size();vp++)
      {
         pfViewport* pf_vp = &(cur_disp->viewports[vp]);
         vprASSERT(pf_vp != NULL);
         vprASSERT(pf_vp->viewport != NULL);

         SurfaceViewport* surf_vp(NULL);
         SimViewport* sim_vp(NULL);
         Viewport::View view;

         // FIND Type of viewport and do stuff
         switch(pf_vp->viewport->getType())
         {
            // Surface viewport
         case Viewport::SURFACE:
            surf_vp = dynamic_cast<SurfaceViewport*>(pf_vp->viewport);
            vprASSERT(surf_vp != NULL && "Could not cast supposedly surface display to SurfaceDisplay.");
            view = surf_vp->getView();

            if(Viewport::LEFT_EYE == view)
            {
               updatePfProjection(pf_vp->chans[pfViewport::PRIMARY], surf_vp->getLeftProj(),false);
            }
            else if(Viewport::RIGHT_EYE == view)
            {
               updatePfProjection(pf_vp->chans[pfViewport::PRIMARY], surf_vp->getRightProj(),false);
            }
            else if(Viewport::STEREO == view)
            {
               updatePfProjection(pf_vp->chans[pfViewport::PRIMARY], surf_vp->getLeftProj(),false);
               updatePfProjection(pf_vp->chans[pfViewport::SECONDARY], surf_vp->getRightProj(),false);
            }
            else
            {
               vprASSERT(false && "vjPfDrawManager::updateProjections(): We don't have a valid display type, don't know what to do");
            }
            break;

            // Sim viewport
         case Viewport::SIM:
               sim_vp = dynamic_cast<SimViewport*>(pf_vp->viewport);
               vprASSERT(sim_vp != NULL && "Could not cast supposedly simulator display to SimDisplay.");

               updateSimulator(sim_vp);
               updatePfProjection(pf_vp->chans[pfViewport::PRIMARY], sim_vp->getCameraProj(), true);
               break;
         default:
            vprASSERT(false && "Viewport type not found");
            break;
         }

      }
   }
}

//! POST: chan has it's view matrix set to the Performer
//+        equivalent of proj's projection data.
void PfDrawManager::updatePfProjection(pfChannel* chan, Projection* proj, bool simulator)
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
      chan->setNearFar(proj->mFrustum[Frustum::VJ_NEAR], proj->mFrustum[Frustum::VJ_FAR]);
      chan->makePersp(proj->mFrustum[Frustum::VJ_LEFT], proj->mFrustum[Frustum::VJ_RIGHT],
                      proj->mFrustum[Frustum::VJ_BOTTOM], proj->mFrustum[Frustum::VJ_TOP]);
   } else {
      CameraProjection* cam_proj = dynamic_cast<CameraProjection*>(proj);
      vprASSERT(cam_proj != NULL && "Trying to use non-camera projection for simulator");
      chan->setAutoAspect(PFFRUST_CALC_HORIZ);
      chan->setNearFar(proj->mFrustum[Frustum::VJ_NEAR], proj->mFrustum[Frustum::VJ_FAR]);
      chan->setFOV(0.0f, cam_proj->mVertFOV);
   }

   vjDEBUG(vjDBG_DRAW_MGR,7) << "Frustum: l:" << proj->mFrustum[Frustum::VJ_LEFT]
              << "   r: " << proj->mFrustum[Frustum::VJ_RIGHT]
              << "   b: " << proj->mFrustum[Frustum::VJ_BOTTOM]
              << "   t: " << proj->mFrustum[Frustum::VJ_TOP] << std::endl << vjDEBUG_FLUSH;

   vjDEBUG_END(vjDBG_DRAW_MGR,6) << "vjPfDrawManager::updatePfProjection: Exiting.\n" << vjDEBUG_FLUSH;
}


//: Helper function that finds the pfDisp given a channel
// This function just loops through all the entries in the disps variable,
// looking for one that contains the channel.  When it is found, it is
// returned.
// NOTE: The "cool" STL functor search didn't work for some reason
/*
PfDrawManager::pfDisp* PfDrawManager::getPfDisp(pfChannel* chan)
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
*/

void PfDrawManager::debugDump(int debugLevel)
{
   vjDEBUG_BEGIN(vjDBG_DRAW_MGR,debugLevel) << "-- DEBUG DUMP --------- " << clrOutNORM(clrCYAN,"vjPfDrawManager:") << (void*)this << " ------------" << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)       << "app:" << (void*)app << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)       << "scene:" << (void*)mRoot << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)       << "sim scene:" << (void*)mRootWithSim << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)       << "Disps:" << mDisplays.size() << std::endl << vjDEBUG_FLUSH;
   for (std::vector<pfDisplay>::iterator i = mDisplays.begin(); i != mDisplays.end(); i++)
   {
      debugDumpPfDisp(&(*i),debugLevel);
   }

   vjDEBUG_END(vjDBG_DRAW_MGR,debugLevel) << "-------- Dump end ----\n" << vjDEBUG_FLUSH;
}


void PfDrawManager::debugDumpPfDisp(pfDisplay* pf_disp, int debugLevel)
{
   vjDEBUG_BEGIN(vjDBG_DRAW_MGR,debugLevel) << "Display:" << (void*)(pf_disp->disp) << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)  << "pWin:" << (void*)(pf_disp->pWin) << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)  << "vis id:" << std::hex << pf_disp->pWin->getFBConfigId() << std::dec << std::endl << vjDEBUG_FLUSH;

   for(int vp=0;vp<pf_disp->viewports.size();vp++)
   {
      vprASSERT((pf_disp->viewports[vp].viewport != NULL) && "NULL viewport in pf_disp. Check if it was ever set.");

      vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel) << "Viewport: " << vp << vjDEBUG_FLUSH;
      vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel) << "      vp: " << *(pf_disp->viewports[vp].viewport) << vjDEBUG_FLUSH;

      for(int ch=0;ch<2;ch++)
      {
         pfChannel* cur_chan = pf_disp->viewports[vp].chans[ch];
         unsigned chan_mask(0);
         if(cur_chan != NULL)
            chan_mask = cur_chan->getShare();
         vjDEBUG_NEXT(vjDBG_DRAW_MGR,debugLevel)
              << "chan:" << ch << " -- " << (void*)cur_chan
              << "  shared: FOV:" << (chan_mask & PFCHAN_FOV)
              << " Scene:" << (chan_mask & PFCHAN_SCENE)
              << " AppFunc:" << (chan_mask & PFCHAN_APPFUNC)
              << " SwapBuff:" << (chan_mask & PFCHAN_SWAPBUFFERS)
              << " SwapBuff-HW:" << (chan_mask & PFCHAN_SWAPBUFFERS_HW)
              << std::endl << vjDEBUG_FLUSH;
      }
   }

   vjDEBUG_CONT_END(vjDBG_DRAW_MGR,debugLevel) << vjDEBUG_FLUSH;
}



// Config function called in draw proc after window is set up
void PFconfigPWin(pfPipeWindow* pWin)
{
   PfDrawManager* dm = PfDrawManager::instance();

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
void PfAppFunc(pfChannel *chan, void* chandata)
{
   vjDEBUG(vjDBG_DRAW_MGR, 0) << "--- PfAppFunc: Enter ---.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR, 0) << "chan: " << chan << std::endl << vjDEBUG_FLUSH;

   pfChannel* master_chan = PfDrawManager::instance()->mMasterChan;
   if(master_chan == chan)
   {
      vjDEBUG(vjDBG_DRAW_MGR,0) << "I am the master of the house:\n" << vjDEBUG_FLUSH;
      if(PfDrawManager::instance()->mPfAppCalled == false)      // Haven't called it yet
      {
         vjDEBUG(vjDBG_DRAW_MGR,0) << "pfApp has not been called yet.  Let me do it...\n" << vjDEBUG_FLUSH;
         PfDrawManager::instance()->mPfAppCalled = true;
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
void PfDrawFunc(pfChannel *chan, void* chandata,bool left_eye, bool right_eye, bool stereo, bool simulator)
{
   vjDEBUG_BEGIN(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "--- PfDrawFunc: Enter ---.\n" << vjDEBUG_FLUSH;
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
   PfDrawManager::instance()->app->drawChan(chan, chandata);     // Draw the channel
                                          // Note: This function calls pfDraw and clears

   // How should we draw the simulator
   /*
   if(cur_pf_disp->disp->isSimulator())
   {

   }
   */

   vjDEBUG_END(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "--- PfDrawFunc: Exit ---.\n" << vjDEBUG_FLUSH;
}

/*********************************************************************
*  Draw traversal functions.
*
*  These functions are just place holders for the drawing function
*  They call the drawing function with specific parameters
***********************************************************************/
void PfDrawFuncStereoLeft(pfChannel *chan, void* chandata)
{
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "--- PfDrawFuncStereoLeft: Enter ---.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "chan: " << chan << std::endl << vjDEBUG_FLUSH;
   PfDrawFunc(chan,chandata,true,false,true,false);
}

void PfDrawFuncStereoRight(pfChannel *chan, void* chandata)
{
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "--- PfDrawFuncStereoRight: Enter ---.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "chan: " << chan << std::endl << vjDEBUG_FLUSH;
   PfDrawFunc(chan,chandata,false,true,true,false);
}

void PfDrawFuncMonoBackbuffer(pfChannel *chan, void* chandata)
{
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "--- PfDrawFuncMonoBackbuffer: Enter ---.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "chan: " << chan << std::endl << vjDEBUG_FLUSH;
   PfDrawFunc(chan,chandata,false,false,false,false);
}

void PfDrawFuncSimulator(pfChannel* chan, void* chandata)
{
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "--- PfDrawFuncSimulator: Enter ---.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_DRAW_MGR, vjDBG_VERB_LVL) << "chan: " << chan << std::endl << vjDEBUG_FLUSH;
   PfDrawFunc(chan,chandata,false,false,false,true);
}

};
