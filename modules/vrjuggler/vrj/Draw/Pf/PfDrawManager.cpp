/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <vrj/vrjConfig.h>

#include <cluster/ClusterManager.h>

#ifdef HAVE_EXT_FUNCTIONAL
#include <ext/functional>
#else
#include <functional>
#endif

#include <Performer/pf.h>
#include <Performer/pfdu.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pf/pfScene.h>
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pfdu.h>
#include <Performer/pf/pfTraverser.h>

#include <vpr/Util/FileUtils.h>

#include <vrj/Display/DisplayManager.h>
#include <vrj/Draw/Pf/PfDrawManager.h>
#include <vrj/Draw/Pf/PfApp.h>
#include <vrj/Util/Debug.h>

#include <vrj/Display/Display.h>
#include <vrj/Display/Projection.h>
#include <vrj/Display/CameraProjection.h>

#include <vrj/Display/SimViewport.h>
#include <vrj/Display/SurfaceViewport.h>

#include <jccl/Config/ConfigChunk.h>

#include <gmtl/Output.h>

namespace vrj
{

// Draw Callbacks
void PfDrawFuncStereoLeft(pfChannel *chan, void* chandata);
void PfDrawFuncStereoRight(pfChannel *chan, void* chandata);
void PfDrawFuncMonoBackbuffer(pfChannel *chan, void* chandata);
//void PfDrawFuncSimulator(pfChannel* chan, void* chandata);
//void PfAppFunc(pfChannel *chan, void* chandata);


void PfPipeSwapFunc(pfPipe *p, pfPipeWindow *pw);

//vjPfDrawManager* PfDrawManager::_instance = NULL;
vprSingletonImp(PfDrawManager);


/**
 * Can the handler handle the given chunk?
 * @return true if we can handle it; false if we can't.
 */
bool PfDrawManager::configCanHandle(jccl::ConfigChunkPtr chunk)
{
   std::string chunk_type = chunk->getDescToken();
   return ( chunk_type == std::string("apiPerformer"));
}

/**
 * Adds the chunk to the configuration.
 * @pre configCanHandle(chunk) == true
 * @return success
 */
bool PfDrawManager::configAdd(jccl::ConfigChunkPtr chunk)
{
   vprASSERT(configCanHandle(chunk));

   std::string chunk_type = chunk->getDescToken();

   if(chunk_type == std::string("apiPerformer"))
   {
      return configPerformerAPI(chunk);
   }

   return false;
}


// Configure the Performer display settings that are needed
// - Number of pipes to allow
// - The xpipe strings to use
bool PfDrawManager::configDisplaySystem(jccl::ConfigChunkPtr chunk)
{
   vprASSERT(chunk.get() != NULL);
   vprASSERT(chunk->getDescToken() == std::string("displaySystem"));

   // ---- SETUP PipeStr's ---- //
   vprDEBUG_BEGIN(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
      << "------------- PfDrawManager::config ----------------" << std::endl
      << vprDEBUG_FLUSH;
   mNumPipes = chunk->getProperty<unsigned int>("numpipes");

   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "NumPipes: " << mNumPipes
                                            << std::endl << vprDEBUG_FLUSH;
   for (unsigned int i=0;i<mNumPipes;i++)
   {
      std::string cur_disp_name = "-1";

      mPipeStrs.push_back(chunk->getProperty<std::string>("xpipes", i));

      if(mPipeStrs[i] == cur_disp_name)    // Use display env
      {
         const char env_var[] = "DISPLAY";
         char* display_env = getenv(env_var);
         if ( display_env != NULL )
         {
            mPipeStrs[i] = display_env;
         }
      }
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "Pipe:" << i << ": "
                                               << mPipeStrs[i] << std::endl
                                               << vprDEBUG_FLUSH;
   }
   return true;
}

/** Configures the Performer API stuff. */
bool PfDrawManager::configPerformerAPI(jccl::ConfigChunkPtr chunk)
{
   vprASSERT(chunk->getDescToken() == std::string("apiPerformer"));

   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "vjPfDrawManager::configPerformerAPI:"
                                            << " Configuring Performer\n" << vprDEBUG_FLUSH;

   // --- Get simulator model info --- //
   std::string head_file = chunk->getProperty<std::string>("simHeadModel");
   std::string wand_file = chunk->getProperty<std::string>("simWandModel");
   if(head_file.empty())
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "WARNING: PfDrawManager::config: simHeadModel not set."
         << std::endl << vprDEBUG_FLUSH;
   }
   if(wand_file.empty())
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL)
         << "WARNING: PfDrawManager::config: simWandModel not set."
         << std::endl << vprDEBUG_FLUSH;
   }

   mHeadModel = vpr::replaceEnvVars(head_file);
   mWandModel = vpr::replaceEnvVars(wand_file);

   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
      << "Head Model: " << mHeadModel.c_str() << std::endl
      << "Wand Model: " << mWandModel.c_str() << std::endl << vprDEBUG_FLUSH;
   return true;
}


void PfDrawManager::sync()
{
   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL) << "vjPfDrawManager::sync\n" << vprDEBUG_FLUSH;
   pfSync();
}

/**
 * @post Calls pfFrame()
 */
void PfDrawManager::draw()
{
   vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL) << "vjPfDrawManager::calling appChanFuncs\n" << vprDEBUG_FLUSH;
   callAppChanFuncs();

   updatePfProjections();

   vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL) << "vjPfDrawManager::draw\n" << vprDEBUG_FLUSH;
   //vprDEBUG(vprDBG_ALL,0) << "vjPfDrawManager::draw\n" << vprDEBUG_FLUSH;

   pfFrame();
}

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
               mApp->appChanFunc(mDisplays[dispIndex].viewports[vp].chans[ch]);
         }
      }
   }
}


/**
 * Sets the app the draw whould interact with.
 * @pre None.
 * @post dynamic_cast<> of the app to a Pf app.
 */
void PfDrawManager::setApp(App* _app)
{
   //vprASSERT(app != NULL);
   mApp = dynamic_cast<PfApp*>(_app);
   if(mPfHasForked)
      initAppGraph();         // If pf is already started, then intialize the app scene graph

}

/**
 * @post Calls pfInit() and sets up the system.
 * - Configures multiprocessing mode
 * - Configures number of pipes
 * - Forks off the processes
 */
void PfDrawManager::initAPI()
{
   pfInit();

   // XXX: This call should really be triggered by a change in draw manager or something
   vprASSERT(mDisplayManager != NULL);
   configDisplaySystem(mDisplayManager->getDisplaySystemChunk());    // Configure all the display system stuff

   mApp->preForkInit();

   vprDEBUG_BEGIN(vrjDBG_DRAW_MGR,vprDBG_STATE_LVL) << "vjPfDrawManager::initAPI: Entering." << std::endl << vprDEBUG_FLUSH;

   // Set params for Multi-pipe and Multiprocess
   pfMultipipe(mNumPipes);

   // XXX: One and only one of the next two lines should be uncommented
   pfMultiprocess(PFMP_APP_CULL_DRAW);    // XXX: Uncomment this line for normal operation
   //pfMultiprocess(PFMP_APPCULLDRAW);    // XXX: Uncomment this line to get synchronization on the cluster

   initLoaders();          // Must call before pfConfig

   // --- FORKS HERE --- //
   pfConfig();
   mPfHasForked = true;

   // Initialize the pipes that the system may need
   // If we don't do this, then pf automatically give each pipe a big black channel
   initPipes();

   initSimulatorGraph();        // Create the simulator scene graph nodes
   initPerformerGraph();        // Create the other scene graph nodes
   if(mApp != NULL)
      initAppGraph();           // App was already set, but pf was not loaded.  So load graph now


   vprASSERT(mRoot != NULL && "We have a NULL root in PfDrawManager");
   vprASSERT(mRootWithSim != NULL && "We have a NULL sim root scene in PfDrawManager");
   vprASSERT(mSceneRoot != NULL && "We have a NULL root scene in PfDrawManager");

   //pfFrame();

   // Dump the state
//   debugDump(vprDBG_CONFIG_LVL);

   vprDEBUG_END(vrjDBG_DRAW_MGR,vprDBG_STATE_LVL) << "vjPfDrawManager::initAPI: Exiting." << std::endl << vprDEBUG_FLUSH;
}

/**
 * Gets a Performer pipe.
 *
 * @pre pipe_num < mNumPipes
 *       Fork must have happened.
 */
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
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "vjPfDrawManager::initPipes: Opening Pipe." << std::endl << vprDEBUG_FLUSH;
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "\tpipe:" << pipe_num << ": " << mPipeStrs[pipe_num] << std::endl << vprDEBUG_FLUSH;

      mPipes[pipe_num] = pfGetPipe(pipe_num);
      mPipes[pipe_num]->setWSConnectionName(mPipeStrs[pipe_num].c_str());
      mPipes[pipe_num]->setScreen(pipe_num);

      pfPipeWindow* pw = allocatePipeWin(pipe_num);   // new pfPipeWindow(mPipes[pipe_num]);
      pw->setOriginSize(0,0,1,1);

      // XXX: Set the swap func
      mPipes[pipe_num]->setSwapFunc( PfPipeSwapFunc );      // Set to the given swap func
   }
}


/**
 * Callback when display is added to display manager.
 *
 * @pre Must be in kernel controlling thread.
 *      Must have already initialized Performer.
 */
void PfDrawManager::addDisplay(Display* disp)
{
   vprASSERT(disp != NULL);    // Can't add a null display
   vprASSERT((true == mPfHasForked) && "Trying to add display when performer has not been initialized");

   // Get frame buffer config
   std::vector<int> stereo_fb_config = getStereoFBConfig(disp);
   std::vector<int> mono_fb_config = getMonoFBConfig(disp);

   // ouput debug info about the frame buffer config recieved
   unsigned int i;
   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL) << "vjPfDrawManager::addDisplay: Got Stereo FB config\n" << vprDEBUG_FLUSH;
   for(i=0;i<stereo_fb_config.size();i++)
      vprDEBUG_CONT(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL) << "  " << stereo_fb_config[i] << vprDEBUG_FLUSH;
   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL) << "\nPfDrawManager::addDisplay: Got Mono FB config\n" << vprDEBUG_FLUSH;
   for(i=0;i<mono_fb_config.size();i++)
      vprDEBUG_CONT(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL) << "  " << mono_fb_config[i] << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_CONT(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL) << std::endl << vprDEBUG_FLUSH;

   //  For the display
   //     -Create a pWin for it
   //     - For each viewport
   //        - Create viewport
   //        - Create channels for the viewports
   vprDEBUG_OutputGuard(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL,
                              std::string("vjPfDrawManager: ---- Opening new Display --------\n"),
                              std::string("vjPfDrawManager: ---- Display Open (done) --------\n"));



   pfDisplay pf_disp;            // The pfDisplay to use
   pf_disp.disp = disp;

   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "\tDisplay is:" << (void*)(disp) << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "\tPfDrawManager::add Display: Got Display:\n" << (*disp) << vprDEBUG_FLUSH;

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
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "vjPfDrawManager::addDisplay: Configuring stereo window attribs.\n" << vprDEBUG_FLUSH;
      pf_disp.pWin->setFBConfigAttrs(&(stereo_fb_config[0]));     // Configure framebuffer for stereo
   }
   else
   {
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "vjPfDrawManager::addDisplay: Configuring mono window attribs.\n" << vprDEBUG_FLUSH;
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

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL) << "   Num viewports: " << num_vps << std::endl << vprDEBUG_FLUSH;
   for(unsigned vp_num=0; vp_num < num_vps; vp_num++)
   {
      viewport = disp->getViewport(vp_num);

      if(viewport->isActive())      // is viewport active
      {
         //Viewport::View view = viewport->getView();
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
         if(disp->inStereo())
         {
            pf_viewport.chans[pfViewport::PRIMARY]->setTravFunc(PFTRAV_DRAW, PfDrawFuncStereoLeft);
            pf_viewport.chans[pfViewport::SECONDARY]->setTravFunc(PFTRAV_DRAW, PfDrawFuncStereoRight);
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

   // Dump the state
   vprDEBUG(vrjDBG_DRAW_MGR, 1) << "Reconfiged the pfDrawManager.\n" << vprDEBUG_FLUSH;
   //vprDEBUG(vrjDBG_DRAW_MGR, 1) << (*this) << vprDEBUG_FLUSH;
   debugDump(1);
}


/**
 * Callback when display is removed to display manager.
 *
 * @pre disp must be a valid display that we have.
 * @post Window for disp is removed from the draw manager and child pipes.
 */
void PfDrawManager::removeDisplay(Display* disp)
{
   // Find the pfDisplay
#ifdef VPR_OS_Win32
   // Visual C++ does not have std::compose1(), so we have to do this the
   // tedious, non-template-coolness way.
   std::vector<pfDisplay>::iterator disp_i;
   for ( disp_i = mDisplays.begin(); disp_i != mDisplays.end(); ++disp_i )
   {
      if ( disp == (*disp_i).disp )
      {
         break;
      }
   }
#else
   std::vector<pfDisplay>::iterator disp_i = std::find_if(mDisplays.begin(), mDisplays.end(),
                         std::compose1( std::bind2nd( std::equal_to<Display*>(),disp),
                                        pfDisplay_disp()) );
#endif


   if(mDisplays.end() == disp_i)
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL) << "ERROR: PfDrawManager::removeDisplay: Tried to remove a non-existant display\n" << vprDEBUG_FLUSH;
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
               vprDEBUG(vrjDBG_DRAW_MGR, 0) << "Trying to remove a non-existant pfChannel\n" << vprDEBUG_FLUSH;
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
               vprDEBUG(vrjDBG_DRAW_MGR, 0) << "Trying to remove a non-existant pfChannel";
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

   // Grow the pipe window vector to the size we need for pipeNum.
   if ( mPipeWindows.size() <= pipeNum )
   {
      mPipeWindows.resize(pipeNum + 1);
   }

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


/** Returns the needed mono frame buffer config. */
std::vector<int> PfDrawManager::getMonoFBConfig(vrj::Display* disp)
{
   std::vector<int> mono_fb;
   mono_fb.push_back(PFFB_DOUBLEBUFFER);
   mono_fb.push_back(PFFB_RGBA);
   configFrameBuffer(disp, mono_fb);

   // Add application requests
   std::vector<int> app_fb = mApp->getFrameBufferAttrs();
   mono_fb.insert(mono_fb.end(), app_fb.begin(), app_fb.end());

#ifdef VPR_OS_Win32
   mono_fb.push_back(0);
#else
   mono_fb.push_back(None);
#endif

   return mono_fb;
}

/** Returns the needed stereo frame buffer config. */
std::vector<int> PfDrawManager::getStereoFBConfig(vrj::Display* disp)
{
   std::vector<int> stereo_fb;
   stereo_fb.push_back(PFFB_DOUBLEBUFFER);
   stereo_fb.push_back(PFFB_RGBA);
   stereo_fb.push_back(PFFB_STEREO);
   configFrameBuffer(disp, stereo_fb);

   // Add application requests
   std::vector<int> app_fb = mApp->getFrameBufferAttrs();
   stereo_fb.insert(stereo_fb.end(), app_fb.begin(), app_fb.end());

#ifdef VPR_OS_Win32
   stereo_fb.push_back(0);
#else
   stereo_fb.push_back(None);
#endif

   return stereo_fb;
}

// XXX: It might be better if this returned a vector in the same way as
// getMonoFBConfig() and getStereoFBConfig().  That vector could then be
// merged with the stereo and mono vectors.  I didn't do this because I
// did not want to mess with the order of the options.
void PfDrawManager::configFrameBuffer(vrj::Display* disp,
                                      std::vector<int>& attrs)
{
   int red_size(8), green_size(8), blue_size(8), alpha_size(8), db_size(16);

   jccl::ConfigChunkPtr fb_chunk = disp->getGlFrameBufferConfig();

   if ( fb_chunk.get() != NULL )
   {
      red_size   = fb_chunk->getProperty<int>("redSize");
      green_size = fb_chunk->getProperty<int>("greenSize");
      blue_size  = fb_chunk->getProperty<int>("blueSize");
      alpha_size = fb_chunk->getProperty<int>("alphaSize");
      db_size    = fb_chunk->getProperty<int>("depthBufferSize");

      if ( red_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Red channel size was negative, set to: " << red_size
            << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         red_size = 1;
      }

      if ( green_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Green channel size was negative, set to: "
            << green_size << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         green_size = 1;
      }

      if ( blue_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Blue channel size was negative, set to: " << blue_size
            << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         blue_size = 1;
      }

      if ( alpha_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Alpha channel size was negative, set to: "
            << alpha_size << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         alpha_size = 1;
      }

      if ( db_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "WARNING: Depth buffer size was negative, set to: " << db_size
            << ".  Setting to 1.\n" << vprDEBUG_FLUSH;
         db_size = 1;
      }
   }

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << "Frame buffer visual settings for " << disp->getName()
      << ": R:" << red_size << " G:" << green_size << " B:" << blue_size
      << " A:" << alpha_size << " DB:" << db_size << std::endl
      << vprDEBUG_FLUSH;

   attrs.push_back(PFFB_RED_SIZE);   attrs.push_back(red_size);
   attrs.push_back(PFFB_GREEN_SIZE); attrs.push_back(green_size);
   attrs.push_back(PFFB_BLUE_SIZE);  attrs.push_back(blue_size);
   attrs.push_back(PFFB_ALPHA_SIZE); attrs.push_back(alpha_size);
   attrs.push_back(PFFB_DEPTH_SIZE); attrs.push_back(db_size);
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
   mApp->initScene();
   if(mSceneRoot != NULL)
      mSceneGroup->removeChild(mSceneRoot);

   mSceneRoot = mApp->getScene();
   mSceneGroup->addChild(mSceneRoot);
}


void PfDrawManager::initSimulatorGraph()
{
   pfNode* head_node(NULL);
   pfNode* wand_node(NULL);

   if(!mHeadModel.empty())
   {
      head_node = pfdLoadFile(mHeadModel.c_str());     // Load head model
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "vjPfDrawManager: Loaded head model: " << mHeadModel.c_str() << std::endl << vprDEBUG_FLUSH;
   } else {
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "vjPfDrawManager: No wand head specified.\n" << vprDEBUG_FLUSH;
   }

   if(!mWandModel.empty())
   {
      wand_node = pfdLoadFile(mWandModel.c_str());     // Load wand model
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "vjPfDrawManager: Loaded wand model: " << mWandModel.c_str() << std::endl << vprDEBUG_FLUSH;
   } else {
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "vjPfDrawManager: No wand model specified.\n" << vprDEBUG_FLUSH;
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
   gmtl::Matrix44f vj_head_mat = sim_vp->getHeadPos();          // Get Juggler matrices
   gmtl::Matrix44f vj_wand_mat = sim_vp->getWandPos();
   pfMatrix head_mat = GetPfMatrix(vj_head_mat);    // Convert to Performer
   pfMatrix wand_mat = GetPfMatrix(vj_wand_mat);
   mHeadDCS->setMat(head_mat);                        // Set the DCS nodes
   mWandDCS->setMat(wand_mat);
}

void PfDrawManager::closeAPI()
{
   pfExit();
}


void PfDrawManager::updatePfProjections()
{
   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL) << "vjPfDrawManager::updatePfProjections: Entering." << std::endl << vprDEBUG_FLUSH;

   // Update display projections
   float scale_factor = mApp->getDrawScaleFactor();
   mDisplayManager->updateProjections(scale_factor);

   // --- Update the channel projections --- //
   //for(each pfDisp)
   //   for(each viewport)
   //       update Performer specific stuff.
   for (unsigned disp_id=0;disp_id<mDisplays.size();disp_id++)    // each display
   {
      pfDisplay* cur_disp = &(mDisplays[disp_id]);

      vprASSERT(cur_disp->disp != NULL);
      for(unsigned vp=0;vp<cur_disp->viewports.size();vp++)       // each viewport
      {
         pfViewport* pf_vp = &(cur_disp->viewports[vp]);
         vprASSERT(pf_vp != NULL);
         vprASSERT(pf_vp->viewport != NULL);

         /*
         SurfaceViewport* surf_vp(NULL);
         */

         Viewport* cur_vp(pf_vp->viewport);
         Viewport::View view;

         // --- All viewports --- //
         //surf_vp = dynamic_cast<SurfaceViewport*>(pf_vp->viewport);
         //vprASSERT(surf_vp != NULL && "Could not cast supposedly surface display to SurfaceDisplay.");
         view = cur_vp->getView();

         if(Viewport::LEFT_EYE == view)
         {
            updatePfProjection(pf_vp->chans[pfViewport::PRIMARY], cur_vp->getLeftProj());
         }
         else if(Viewport::RIGHT_EYE == view)
         {
            updatePfProjection(pf_vp->chans[pfViewport::PRIMARY], cur_vp->getRightProj());
         }
         else if(Viewport::STEREO == view)
         {
            updatePfProjection(pf_vp->chans[pfViewport::PRIMARY], cur_vp->getLeftProj());
            updatePfProjection(pf_vp->chans[pfViewport::SECONDARY], cur_vp->getRightProj());
         }
         else
         {
            vprASSERT(false && "vjPfDrawManager::updateProjections(): We don't have a valid display type, don't know what to do");
         }

        // Sim viewport
        if(cur_vp->isSimulator())
        {
            SimViewport* sim_vp = dynamic_cast<SimViewport*>(pf_vp->viewport);
            vprASSERT(sim_vp != NULL && "Could not cast supposedly simulator display to SimDisplay.");

            updateSimulator(sim_vp);
        //    updatePfProjection(pf_vp->chans[pfViewport::PRIMARY], sim_vp->getCameraProj(), true);
         }

      }
   }
}

/**
 * @post chan has its view matrix set to the Performer.
 *       Equivalent of proj's projection data.
 */
void PfDrawManager::updatePfProjection(pfChannel* chan, Projection* proj)  //, bool simulator)
{

   vprDEBUG_BEGIN(vrjDBG_DRAW_MGR,vprDBG_HVERB_LVL) << "vjPfDrawManager::updatePfProjection: Entering. viewMat:\n"
                    << proj->getViewMatrix() << std::endl << vprDEBUG_FLUSH;


   pfMatrix pfViewMat;
   pfViewMat.set(const_cast<float*>(proj->getViewMatrix().mData));  // Hmm...

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

   /*
   if(!simulator)
   {
   */
      vrj::Frustum frust(proj->getFrustum());
      chan->setAutoAspect(PFFRUST_CALC_NONE);         // No auto aspect
      chan->setNearFar(frust[Frustum::VJ_NEAR], frust[Frustum::VJ_FAR]);
      chan->makePersp(frust[Frustum::VJ_LEFT], frust[Frustum::VJ_RIGHT],
                      frust[Frustum::VJ_BOTTOM], frust[Frustum::VJ_TOP]);
   /*
   }
   else
   {
      CameraProjection* cam_proj = dynamic_cast<CameraProjection*>(proj);
      vprASSERT(cam_proj != NULL && "Trying to use non-camera projection for simulator");
      chan->setAutoAspect(PFFRUST_CALC_HORIZ);
      chan->setNearFar(proj->getFrustum()[Frustum::VJ_NEAR],
                       proj->getFrustum()[Frustum::VJ_FAR]);
      chan->setFOV(0.0f, cam_proj->mVertFOV);
   }
   */

   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_HEX_LVL)
      << "Frustum: l:" << proj->getFrustum()[Frustum::VJ_LEFT]
      << "   r: " << proj->getFrustum()[Frustum::VJ_RIGHT]
      << "   b: " << proj->getFrustum()[Frustum::VJ_BOTTOM]
      << "   t: " << proj->getFrustum()[Frustum::VJ_TOP] << std::endl
      << vprDEBUG_FLUSH;

   vprDEBUG_END(vrjDBG_DRAW_MGR,vprDBG_HVERB_LVL)
      << "vjPfDrawManager::updatePfProjection: Exiting.\n" << vprDEBUG_FLUSH;
}


/**
 * Helper function that finds the pfDisp given a channel.
 * This function just loops through all the entries in the disps variable,
 * looking for one that contains the channel.  When it is found, it is
 * returned.
 *
 * @note The "cool" STL functor search didn't work for some reason.
 */
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
   vprDEBUG_BEGIN(vrjDBG_DRAW_MGR,debugLevel) << "-- DEBUG DUMP --------- " << clrOutNORM(clrCYAN,"vjPfDrawManager:") << (void*)this << " ------------" << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel)       << "app:" << (void*)mApp << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel)       << "scene:" << (void*)mRoot << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel)       << "sim scene:" << (void*)mRootWithSim << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel)       << "Disps:" << mDisplays.size() << std::endl << vprDEBUG_FLUSH;
   for (std::vector<pfDisplay>::iterator i = mDisplays.begin(); i != mDisplays.end(); i++)
   {
      debugDumpPfDisp(&(*i),debugLevel);
   }

   vprDEBUG_END(vrjDBG_DRAW_MGR,debugLevel) << "-------- Dump end ----\n" << vprDEBUG_FLUSH;
}


void PfDrawManager::debugDumpPfDisp(pfDisplay* pf_disp, int debugLevel)
{
   vprDEBUG_BEGIN(vrjDBG_DRAW_MGR,debugLevel) << "Display:" << (void*)(pf_disp->disp) << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel)  << "pWin:" << (void*)(pf_disp->pWin) << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel)  << "vis id:" << std::hex << pf_disp->pWin->getFBConfigId() << std::dec << std::endl << vprDEBUG_FLUSH;

   for(unsigned vp=0; vp<pf_disp->viewports.size(); ++vp)
   {
      vprASSERT((pf_disp->viewports[vp].viewport != NULL) && "NULL viewport in pf_disp. Check if it was ever set.");

      vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel) << "Viewport: " << vp << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel) << "      vp: " << *(pf_disp->viewports[vp].viewport) << vprDEBUG_FLUSH;

      for(int ch=0;ch<2;ch++)
      {
         pfChannel* cur_chan = pf_disp->viewports[vp].chans[ch];
         unsigned chan_mask(0);
         if(cur_chan != NULL)
            chan_mask = cur_chan->getShare();
         vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel)
              << "chan:" << ch << " -- " << (void*)cur_chan
              << "  shared: FOV:" << (chan_mask & PFCHAN_FOV)
              << " Scene:" << (chan_mask & PFCHAN_SCENE)
              << " AppFunc:" << (chan_mask & PFCHAN_APPFUNC)
              << " SwapBuff:" << (chan_mask & PFCHAN_SWAPBUFFERS)
              << " SwapBuff-HW:" << (chan_mask & PFCHAN_SWAPBUFFERS_HW)
              << std::endl << vprDEBUG_FLUSH;
      }
   }

   vprDEBUG_CONT_END(vrjDBG_DRAW_MGR,debugLevel) << vprDEBUG_FLUSH;
}



// Config function called in draw proc after window is set up
void PFconfigPWin(pfPipeWindow* pWin)
{
   PfDrawManager* dm = PfDrawManager::instance();

   // Init the vj monitor modes
   pWin->open();
   pfInitGfx();

   // Call user config function
   dm->mApp->configPWin(pWin);

   // Ouput the visual id
   int fb_id = pWin->getFBConfigId();
   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
      << "vjPFConfigPWin: framebuffer id: x" << std::hex << fb_id << std::dec
      << std::endl << vprDEBUG_FLUSH;
}


/*
void PfAppFunc(pfChannel *chan, void* chandata)
{
   vprDEBUG(vrjDBG_DRAW_MGR, 0) << "--- PfAppFunc: Enter ---.\n" << vprDEBUG_FLUSH;
   vprDEBUG(vrjDBG_DRAW_MGR, 0) << "chan: " << chan << std::endl << vprDEBUG_FLUSH;

   pfChannel* master_chan = PfDrawManager::instance()->mMasterChan;
   if(master_chan == chan)
   {
      vprDEBUG(vrjDBG_DRAW_MGR,0) << "I am the master of the house:\n" << vprDEBUG_FLUSH;
      if(PfDrawManager::instance()->mPfAppCalled == false)      // Haven't called it yet
      {
         vprDEBUG(vrjDBG_DRAW_MGR,0) << "pfApp has not been called yet.  Let me do it...\n" << vprDEBUG_FLUSH;
         PfDrawManager::instance()->mPfAppCalled = true;
         pfApp();
      }
   }
   else
   {
      vprDEBUG(vrjDBG_DRAW_MGR,0) << "I am not master. I can't do anything.\n" << vprDEBUG_FLUSH;
   }
}
*/


// --- Traversal functions --- //
// This function is the performer draw callback function
// - Sets up the correct OGL drawing buffer
// - Calls the app draw chan function
//template <bool left_eye, bool right_eye, bool stereo, bool simulator>
void PfDrawFunc(pfChannel *chan, void* chandata,bool left_eye, bool right_eye, bool stereo)  // , bool simulator)
{
   vprDEBUG_OutputGuard(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL,
                        std::string("--- PfDrawFunc: Enter ---.\n"),
                        std::string("--- PfDrawFunc: Exit ---.\n"));

   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL) << "chan: " << chan << std::endl << vprDEBUG_FLUSH;

      // Select correct buffer to draw to
      // If we are in stereo and not a simulator
   if(stereo)
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL) << "vjPfDrawFunc: Drawing stereo win\n" << vprDEBUG_FLUSH;
      if(left_eye)
      {
         glDrawBuffer(GL_BACK_LEFT);
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL) << "vjPfDrawFunc: Set to BACK_LEFT\n" << vprDEBUG_FLUSH;
      }
      else
      {
         glDrawBuffer(GL_BACK_RIGHT);
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL) << "vjPfDrawFunc: Set to BACK_RIGHT\n" << vprDEBUG_FLUSH;
      }
   }
   else                                // No Stereo or have sim, so just go to back buffer
   {
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL) << "vjPfDrawFunc: Drawing mono window\n" << vprDEBUG_FLUSH;
      glDrawBuffer(GL_BACK);
   }

   // -- Configure buffers for correct eye/stereo etc -- //
   PfDrawManager::instance()->mApp->drawChan(chan, chandata);     // Draw the channel
                                          // Note: This function calls pfDraw and clears

   // How should we draw the simulator
   /*
   if(cur_pf_disp->disp->isSimulator())
   {

   }
   */
}

/*********************************************************************
*  Draw traversal functions.
*
*  These functions are just place holders for the drawing function
*  They call the drawing function with specific parameters
***********************************************************************/
void PfDrawFuncStereoLeft(pfChannel *chan, void* chandata)
{
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL) << "--- PfDrawFuncStereoLeft: Enter ---.\n" << vprDEBUG_FLUSH;
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL) << "chan: " << chan << std::endl << vprDEBUG_FLUSH;
   PfDrawFunc(chan,chandata,true,false,true);
}

void PfDrawFuncStereoRight(pfChannel *chan, void* chandata)
{
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL) << "--- PfDrawFuncStereoRight: Enter ---.\n" << vprDEBUG_FLUSH;
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL) << "chan: " << chan << std::endl << vprDEBUG_FLUSH;
   PfDrawFunc(chan,chandata,false,true,true);
}

void PfDrawFuncMonoBackbuffer(pfChannel *chan, void* chandata)
{
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL) << "--- PfDrawFuncMonoBackbuffer: Enter ---.\n" << vprDEBUG_FLUSH;
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL) << "chan: " << chan << std::endl << vprDEBUG_FLUSH;
   PfDrawFunc(chan,chandata,false,false,false);
}

/*******************************
* Callback for swapping buffers
********************************/
void PfPipeSwapFunc(pfPipe *p, pfPipeWindow *pw)
{
    PfDrawManager* pf_draw_mgr = PfDrawManager::instance();     // get the draw manager

    // If "first pw", then sync
    // What is the "first" pw and how do I know?

    // For each pfDisplay
    //    if(pw == display->pipeWindow)
    //

    // Swap the buffer
    vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL) << "--- PfPipeSwapFunc: pipe:" << pf_draw_mgr << " -- pw:" << pw << "\n" << vprDEBUG_FLUSH;

    // Barrier for Cluster
    //vprDEBUG(vprDBG_ALL,1) <<  "BARRIER: Going to sleep for: " << num << std::endl << vprDEBUG_FLUSH;
   cluster::ClusterManager::instance()->createBarrier();
    //vprDEBUG(vprDBG_ALL,1) <<  "BARRIER: IS DONE" << std::endl << vprDEBUG_FLUSH;

    pw->swapBuffers();
}

}
