/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/
#define PF_C_API 1

#include <vrj/Draw/Pf/Config.h>

#include <iomanip>

#ifdef HAVE_EXT_FUNCTIONAL
#include <ext/functional>
#else
#include <functional>
#endif

#include <boost/concept_check.hpp>

#include <Performer/pf.h>
#include <Performer/pfdu.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pf/pfScene.h>
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pf/pfTraverser.h>

#include <gmtl/Output.h>

#include <vpr/vpr.h>

#include <jccl/Config/ConfigElement.h>
#include <jccl/RTRC/ConfigManager.h>

#include <cluster/ClusterManager.h>

#include <vrj/Util/Debug.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Draw/Pf/PfApp.h>
#include <vrj/Draw/Pf/PfInputHandler.h>
#include <vrj/Draw/Pf/PfSimInterface.h>
#include <vrj/Draw/Pf/PfSimInterfaceFactory.h>

#include <vrj/Display/Display.h>
#include <vrj/Display/Projection.h>
#include <vrj/Display/CameraProjection.h>
#include <vrj/Display/SimViewport.h>
#include <vrj/Display/SurfaceViewport.h>

#include <vrj/Draw/Pf/PfDrawManager.h>

namespace vrj
{

// Draw Callbacks
void PfDrawFuncStereoLeft(pfChannel *chan, void* chandata);
void PfDrawFuncStereoRight(pfChannel *chan, void* chandata);
void PfDrawFuncMonoBackbuffer(pfChannel *chan, void* chandata);
//void PfDrawFuncSimulator(pfChannel* chan, void* chandata);
//void PfAppFunc(pfChannel *chan, void* chandata);


void PfPipeSwapFunc(pfPipe *p, pfPipeWindow *pw);

vprSingletonImp(PfDrawManager);

PfDrawManager::~PfDrawManager()
{
   //TODO: Add thread safety.
   for ( std::vector<PfInputHandler*>::iterator itr = mPfInputHandlers.begin();
         itr != mPfInputHandlers.end() ; itr++)
   {
      delete *itr;
   }
   mPfInputHandlers.clear();
}

// Configure the Performer display settings that are needed
// - Number of pipes to allow
// - The xpipe strings to use
bool PfDrawManager::configDisplaySystem(jccl::ConfigElementPtr element)
{
   vprASSERT(element.get() != NULL);
   vprASSERT(element->getID() == std::string("display_system"));

   // ---- SETUP PipeStr's ---- //
   vprDEBUG_BEGIN(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
      << "------------- PfDrawManager::config ----------------" << std::endl
      << vprDEBUG_FLUSH;
   mNumPipes = element->getProperty<unsigned int>("number_of_pipes");

   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "NumPipes: " << mNumPipes
                                            << std::endl << vprDEBUG_FLUSH;

   // Make sure that the user has specified a valid number of pipes in the
   // configuration. This becomes an issue normally since the default number of
   // pipes when creating a new display_system element is 0. It needs to be this
   // because we can not fill in the list of pipes with default values.
   if(mNumPipes < 1)
   {
      mNumPipes = 1;
   }

   for (unsigned int i=0;i<mNumPipes;i++)
   {
      std::string cur_disp_name = "-1";

      // NOTE: ConfigElements return the default value for a property if a value is
      //       not present. So if a pipe string is not specified for this pipe then
      //       it gets the default value of "-1".
      mPipeStrs.push_back(element->getProperty<std::string>("x11_pipes", i));

      if(mPipeStrs[i] == cur_disp_name)    // Use display env
      {
         std::string display_env;
         vpr::System::getenv("DISPLAY", display_env);

         if ( ! display_env.empty() )
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

void PfDrawManager::sync()
{
   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL)
      << "vrj::PfDrawManager::sync\n" << vprDEBUG_FLUSH;
   pfSync();
}

/**
 * @post Calls pfFrame()
 */
void PfDrawManager::draw()
{
   vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL)
      << "[vrj::PfDrawManager::draw()] calling appChanFuncs\n"
      << vprDEBUG_FLUSH;
   callAppChanFuncs();

   updatePfProjections();

   vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL)
      << "[vrj::PfDrawManager::draw] calling pfFrame()\n" << vprDEBUG_FLUSH;

   pfFrame();

   for (std::vector<PfInputHandler*>::iterator itr = mPfInputHandlers.begin() ;
        itr != mPfInputHandlers.end() ; itr++)
   {
      (*itr)->checkEvents();
   }
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
   pfuInitUtil();
   
   // XXX: This call should really be triggered by a change in draw manager or something
   vprASSERT(mDisplayManager != NULL);
   configDisplaySystem(mDisplayManager->getDisplaySystemElement());    // Configure all the display system stuff

   mApp->preForkInit();

   vprDEBUG_BEGIN(vrjDBG_DRAW_MGR,vprDBG_STATE_LVL)
      << "[vrj::PfDrawManager::initAPI()] Entering." << std::endl
      << vprDEBUG_FLUSH;

   // Set params for Multi-pipe and Multiprocess
   pfMultipipe(mNumPipes);

#ifdef VPR_OS_Linux
   // Multi-processing with Performer on SMP Linux machines is unstable, and
   // there is not much of a performance gain to splitting App, Cull, and Draw
   // up among multiple processes on a single-processor machine.
   pfMultiprocess(PFMP_APPCULLDRAW);
#else
   // If we are running in a cluster then we must run in single process mode.
   if(jccl::ConfigManager::instance()->isElementTypeInPendingList("cluster_manager") ||
      jccl::ConfigManager::instance()->isElementTypeInActiveList("cluster_manager"))
   {
      vprDEBUG_BEGIN(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL)
         << "PfDrawManager::initAPI() Running Performer in single process to ensure cluster synchronization."
         << std::endl << vprDEBUG_FLUSH;
      // Single process mode.
      pfMultiprocess(PFMP_APPCULLDRAW);
   }
   else
   {
      // Multiple process mode.
      pfMultiprocess(PFMP_APP_CULL_DRAW);
   }
#endif

// We can not init head and wand model loaders since they are loaded in PfBasicSimInterface
//   initLoaders();          // Must call before pfConfig

   // --- FORKS HERE --- //
   pfConfig();
   
   // Initialize shared memory for utility libraries
   pfuInit();

   mPfHasForked = true;

   // Initialize the pipes that the system may need
   // If we don't do this, then pf automatically give each pipe a big black channel
   initPipes();

   initPerformerGraph();        // Create the other scene graph nodes
   if(mApp != NULL)
      initAppGraph();           // App was already set, but pf was not loaded.  So load graph now


   vprASSERT(mRoot != NULL && "We have a NULL root in PfDrawManager");
   vprASSERT(mRootWithSim != NULL && "We have a NULL sim root scene in PfDrawManager");
   vprASSERT(mSceneRoot != NULL && "We have a NULL root scene in PfDrawManager");

   //pfFrame();

   // Dump the state
   // debugDump(vprDBG_CONFIG_LVL);

   vprDEBUG_END(vrjDBG_DRAW_MGR,vprDBG_STATE_LVL)
      << "[vrj::PfDrawManager::initAPI()] Exiting." << std::endl
      << vprDEBUG_FLUSH;
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
   vprASSERT((NULL != mPipes[pipe_num]) && "Have NULL pipe");

   // Print an error message if an invalid pipe was requested.  This will
   // probably only happen when a configuration is broken.
   if ( pipe_num >= mNumPipes || pipe_num >= mPipes.size() ||
        NULL == mPipes[pipe_num] )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:")
         << " Invalid pipe number (" << pipe_num << ") requested.  "
         << "Check display system configuration\n" << vprDEBUG_FLUSH;
      return NULL;
   }
   else
   {
      // Return the actual pipe
      return mPipes[pipe_num];
   }
}

// Initialize all the pipes that the system may need to use
// XXX: If performer were more flexible, then this would actually be
//      done on demand while the application is running :(
void PfDrawManager::initPipes()
{
   mPipes.resize(mNumPipes, NULL);     // Resize the vector

   for(unsigned pipe_num=0; pipe_num<mNumPipes; pipe_num++)
   {
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
         << "[vrj::PfDrawManager::initPipes()] Opening Pipe." << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
         << "\tpipe:" << pipe_num << ": " << mPipeStrs[pipe_num] << std::endl
         << vprDEBUG_FLUSH;

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
void PfDrawManager::addDisplay(DisplayPtr disp)
{
   vprASSERT(disp.ptr() != NULL);    // Can't add a null display
   vprASSERT((true == mPfHasForked) && "Trying to add display when performer has not been initialized");

   //  For the display
   //     -Create a pWin for it
   //     - For each viewport
   //        - Create viewport
   //        - Create channels for the viewports
   vprDEBUG_OutputGuard(vrjDBG_DRAW_MGR, vprDBG_STATE_LVL,
                        std::string("vrj::PfDrawManager: ---- Opening new Display --------\n"),
                        std::string("vrj::PfDrawManager: ---- Display Open (done) --------\n"));



   pfDisplay pf_disp;            // The pfDisplay to use
   pf_disp.disp = disp;

   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "\tDisplay is:" << (void*)(disp) << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL) << "\tPfDrawManager::add Display: Got Display:\n" << (*disp) << vprDEBUG_FLUSH;

   int xo, yo, xs, ys;
   pfPipe* pipe = getPfPipe(disp->getPipe());      // Get the pipe

   // Make sure we have a good pipe.  If not, there isn't anything we can do
   // to recover.
   if ( NULL == pipe )
   {
      vprASSERT(NULL != pipe);
      return;
   }

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

   jccl::ConfigElementPtr fb_elt = disp->getGlFrameBufferConfig();
   const int visual_id = fb_elt->getProperty<int>("visual_id");

   // If the user requested a specific visual ID, use it and ignore the rest
   // of the frame buffer configuration.
   if ( visual_id != -1 )
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
         << "Requesting visual 0x" << std::hex << visual_id << std::dec
         << "." << std::endl << vprDEBUG_FLUSH;
      pf_disp.pWin->setFBConfigId(visual_id);
   }
   else
   {
      // Setup Frame Buffer config
      if (disp->isStereoRequested())                     // If we need stereo
      {
         std::vector<int> fb_config = getStereoFBConfig(disp);

#ifdef VJ_DEBUG
         // ouput debug info about the frame buffer config recieved
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL)
            << "[vrj::PfDrawManager::addDisplay()] Got Stereo FB config\n"
            << vprDEBUG_FLUSH;
         for ( unsigned int i = 0; i < fb_config.size(); ++i )
         {
            vprDEBUG_CONT(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL)
               << "  " << fb_config[i] << vprDEBUG_FLUSH;
         }
#endif

         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
            << "[vrj::PfDrawManager::addDisplay()] "
            << "Configuring stereo window attribs.\n" << vprDEBUG_FLUSH;
         pf_disp.pWin->setFBConfigAttrs(&(fb_config[0]));     // Configure framebuffer for stereo
      }
      else
      {
         // Get frame buffer config
         std::vector<int> fb_config = getMonoFBConfig(disp);

#ifdef VJ_DEBUG
         // ouput debug info about the frame buffer config recieved
         vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL)
            << "[vrj::PfDrawManager::addDisplay()] Got Mono FB config\n"
            << vprDEBUG_FLUSH;
         for ( unsigned int j = 0 ; j < fb_config.size(); ++j )
         {
            vprDEBUG_CONT(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL)
               << "  " << fb_config[j] << std::endl << vprDEBUG_FLUSH;
         }
         vprDEBUG_CONT(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL)
            << std::endl << vprDEBUG_FLUSH;
#endif

         vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
            << "[vrj::PfDrawManager::addDisplay()] "
            << "Configuring mono window attribs.\n" << vprDEBUG_FLUSH;
         pf_disp.pWin->setFBConfigAttrs(&(fb_config[0]));       // Configure a "norm" window
      }
   }

   // -- Set config info -- //
   pf_disp.pWin->setConfigFunc(PFconfigPWin);   // Set config function
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
         if(disp->isStereoRequested())
         {
            pf_viewport.chans[pfViewport::SECONDARY] = new pfChannel(pipe);
            pf_viewport.chans[pfViewport::SECONDARY]->setViewport(vp_ox, vp_ox+vp_sx, vp_oy, vp_oy+vp_sy);
            pf_disp.pWin->addChan(pf_viewport.chans[pfViewport::SECONDARY]);
         }

         // Set draw function
         if(disp->isStereoRequested())
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
            // -- Finish Simulator setup
            jccl::ConfigElementPtr vp_element = viewport->getConfigElement();

            SimViewport* sim_vp(NULL);
            sim_vp = dynamic_cast<SimViewport*>(viewport);
            vprASSERT(NULL != sim_vp);

            sim_vp->setDrawSimInterface(NULL);

            // Create the simulator stuff
            vprASSERT(1 == vp_element->getNum("simulator_plugin") && "You must supply a simulator plugin.");

            jccl::ConfigElementPtr sim_element =
               vp_element->getProperty<jccl::ConfigElementPtr>("simulator_plugin");

            vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
               << "PfDrawManager::addDisplay() creating simulator of type '"
               << sim_element->getID() << "'\n" << vprDEBUG_FLUSH;

            DrawSimInterface* new_sim_i =
               PfSimInterfaceFactory::instance()->createObject(sim_element->getID());

            // XXX: Change this to an error once the new simulator loading code is
            // more robust.  -PH (4/13/2003)
            vprASSERT(NULL != new_sim_i && "Failed to create draw simulator");
            sim_vp->setDrawSimInterface(new_sim_i);
            new_sim_i->initialize(sim_vp);
            new_sim_i->config(sim_element);


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

   // Call pfFrame to cause the pipeWindow configured to be opened and setup.
   pfFrame();
   
   // This is neccessary because it may take a couple
   // frames for the window to fully open. This information was
   // found in "man pfPipeWindow".

   // Performer says that if you trigger a window from the applciation proccess
   // you must wait for the window to open during the draw process.
   while( !pfIsPWinOpen( pf_disp.pWin ) )
   {
      pfFrame();
      vpr::System::usleep( 500 ); 
   }

   PfInputHandler* new_input_handler = new PfInputHandler(pf_disp.pWin, disp->getName());
   
   // Configure the Performer window to accept events.
   jccl::ConfigElementPtr display_elt = disp->getConfigElement();
   new_input_handler->config(display_elt, disp);

   
   mPfInputHandlers.push_back(new_input_handler);

   if(pf_disp.disp->shouldHideMouse())
   {
      pfuLoadPWinCursor(pf_disp.pWin, PFU_CURSOR_OFF);
   }
   
   // Dump the state
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << "Reconfiged the pfDrawManager.\n" << vprDEBUG_FLUSH;
   //vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL) << (*this) << vprDEBUG_FLUSH;
   debugDump(vprDBG_CONFIG_LVL);
}

/*
#ifndef VPR_OS_Windows
void PfDrawManager::createEmptyCursor(::Display* display, ::Window root)
{
   Pixmap cursormask;
   XGCValues xgc;
   GC gc;
   XColor dummycolour;

   cursormask = XCreatePixmap(display, root, 1, 1, 1);
   xgc.function = GXclear;
   gc =  XCreateGC(display, cursormask, GCFunction, &xgc);
   XFillRectangle(display, cursormask, gc, 0, 0, 1, 1);
   dummycolour.pixel = 0;
   dummycolour.red = 0;
   dummycolour.flags = 04;
   mEmptyCursor = XCreatePixmapCursor(display, cursormask, cursormask,
                                      &dummycolour,&dummycolour, 0,0);
   XFreePixmap(display,cursormask);
   XFreeGC(display,gc);
}
#endif
*/




/**
 * Callback when display is removed to display manager.
 *
 * @pre disp must be a valid display that we have.
 * @post Window for disp is removed from the draw manager and child pipes.
 */
void PfDrawManager::removeDisplay(DisplayPtr disp)
{
   // Find the pfDisplay
#ifdef VPR_OS_Windows
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
                         std::compose1( std::bind2nd( std::equal_to<DisplayPtr>(),disp),
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
               vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL) << "Trying to remove a non-existant pfChannel\n" << vprDEBUG_FLUSH;
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
               vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL) << "Trying to remove a non-existant pfChannel";
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
std::vector<int> PfDrawManager::getMonoFBConfig(vrj::DisplayPtr disp)
{
   std::vector<int> mono_fb;
   mono_fb.push_back(PFFB_DOUBLEBUFFER);
   mono_fb.push_back(PFFB_RGBA);
   configFrameBuffer(disp, mono_fb);

   // Add application requests
   std::vector<int> app_fb = mApp->getFrameBufferAttrs();
   mono_fb.insert(mono_fb.end(), app_fb.begin(), app_fb.end());

#ifdef VPR_OS_Windows
   mono_fb.push_back(0);
#else
   mono_fb.push_back(None);
#endif

   return mono_fb;
}

/** Returns the needed stereo frame buffer config. */
std::vector<int> PfDrawManager::getStereoFBConfig(vrj::DisplayPtr disp)
{
   std::vector<int> stereo_fb;
   stereo_fb.push_back(PFFB_DOUBLEBUFFER);
   stereo_fb.push_back(PFFB_RGBA);
   stereo_fb.push_back(PFFB_STEREO);
   configFrameBuffer(disp, stereo_fb);

   // Add application requests
   std::vector<int> app_fb = mApp->getFrameBufferAttrs();
   stereo_fb.insert(stereo_fb.end(), app_fb.begin(), app_fb.end());

#ifdef VPR_OS_Windows
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
void PfDrawManager::configFrameBuffer(vrj::DisplayPtr disp,
                                      std::vector<int>& attrs)
{
   int red_size(8), green_size(8), blue_size(8), alpha_size(8), db_size(16),
       accum_red_size(1), accum_green_size(1), accum_blue_size(1),
       accum_alpha_size(1), stencil_size(1);
   // glXChooseVisual() will return the visual with the smallest number of
   // auxiliary buffers that meets or exceeds the requested count.
   int num_aux_bufs(0);
   bool want_fsaa(false);

   jccl::ConfigElementPtr fb_element = disp->getGlFrameBufferConfig();

   if ( fb_element.get() != NULL )
   {
      if ( fb_element->getVersion() < 2 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:") << " Display window '"
            << disp->getName() << "'" << std::endl;
         vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "has an out of date OpenGL frame buffer configuration."
            << std::endl;
         vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "Expected version 2 but found version "
            << fb_element->getVersion() << ".\n";
         vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "Default values will be used for some frame buffer settings.\n"
            << vprDEBUG_FLUSH;
      }

      red_size         = fb_element->getProperty<int>("red_size");
      green_size       = fb_element->getProperty<int>("green_size");
      blue_size        = fb_element->getProperty<int>("blue_size");
      alpha_size       = fb_element->getProperty<int>("alpha_size");
      num_aux_bufs     = fb_element->getProperty<int>("auxiliary_buffer_count");
      db_size          = fb_element->getProperty<int>("depth_buffer_size");
      stencil_size     = fb_element->getProperty<int>("stencil_buffer_size");
      accum_red_size   = fb_element->getProperty<int>("accum_red_size");
      accum_green_size = fb_element->getProperty<int>("accum_green_size");
      accum_blue_size  = fb_element->getProperty<int>("accum_blue_size");
      accum_alpha_size = fb_element->getProperty<int>("accum_alpha_size");
      want_fsaa        = fb_element->getProperty<bool>("fsaa_enable");

      if ( red_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Color buffer red channel size was negative ("
            << red_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         red_size = 1;
      }

      if ( green_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Color buffer green channel size was negative ("
            << green_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         green_size = 1;
      }

      if ( blue_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Color buffer blue channel size was negative ("
            << blue_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         blue_size = 1;
      }

      if ( alpha_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Color buffer alpha channel size was negative ("
            << alpha_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         alpha_size = 1;
      }

      if ( num_aux_bufs < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Auxiliary buffer count was negative (" << num_aux_bufs
            << ").  Setting to 0.\n" << vprDEBUG_FLUSH;
         num_aux_bufs = 0;
      }

      if ( db_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Depth buffer size was negative (" << db_size
            << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         db_size = 1;
      }

      if ( stencil_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Stencil buffer size was negative (" << stencil_size
            << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         stencil_size = 1;
      }

      if ( accum_red_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Accumulation buffer red channel size was negative ("
            << accum_red_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_red_size = 1;
      }

      if ( accum_green_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Accumulation buffer green channel size was negative ("
            << accum_green_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_green_size = 1;
      }

      if ( accum_blue_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Accumulation buffer blue channel size was negative ("
            << accum_blue_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_blue_size = 1;
      }

      if ( accum_alpha_size < 0 )
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Accumulation buffer alpha channel size was negative ("
            << accum_alpha_size << ").  Setting to 1.\n" << vprDEBUG_FLUSH;
         accum_alpha_size = 1;
      }
   }

   const unsigned int indent_level(2);
   const std::string indent_text(indent_level, ' ');
   const int pad_width_dot(40 - indent_level);
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << "OpenGL visual request settings for " << disp->getName()
      << ":\n";
   vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << std::setiosflags(std::ios::left) << std::setfill('.')
      << indent_text << std::setw(pad_width_dot)
      << "Color buffer red size " << " " << red_size << std::endl;
   vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << std::setiosflags(std::ios::left) << std::setfill('.')
      << indent_text << std::setw(pad_width_dot)
      << "Color buffer green size " << " " << green_size << std::endl;
   vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << std::setiosflags(std::ios::left) << std::setfill('.')
      << indent_text << std::setw(pad_width_dot)
      << "Color buffer blue size " << " " << blue_size << std::endl;
   vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << std::setiosflags(std::ios::left) << std::setfill('.')
      << indent_text << std::setw(pad_width_dot)
      << "Color buffer alpha size " << " " << alpha_size << std::endl;
   vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << std::setiosflags(std::ios::left) << std::setfill('.')
      << indent_text << std::setw(pad_width_dot)
      << "Auxiliary buffer count " << " " << num_aux_bufs << std::endl;
   vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << std::setiosflags(std::ios::left) << std::setfill('.')
      << indent_text << std::setw(pad_width_dot)
      << "Depth buffer size " << " " << db_size << std::endl;
   vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << std::setiosflags(std::ios::left) << std::setfill('.')
      << indent_text << std::setw(pad_width_dot)
      << "Stencil buffer size " << " " << stencil_size << std::endl;
   vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << std::setiosflags(std::ios::left) << std::setfill('.')
      << indent_text << std::setw(pad_width_dot)
      << "Accumulation buffer red size " << " " << accum_red_size
      << std::endl;
   vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << std::setiosflags(std::ios::left) << std::setfill('.')
      << indent_text << std::setw(pad_width_dot)
      << "Accumulation buffer green size " << " " << accum_green_size
      << std::endl;
   vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << std::setiosflags(std::ios::left) << std::setfill('.')
      << indent_text << std::setw(pad_width_dot)
      << "Accumulation buffer blue size " << " " << accum_blue_size
      << std::endl;
   vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << std::setiosflags(std::ios::left) << std::setfill('.')
      << indent_text << std::setw(pad_width_dot)
      << "Accumulation buffer alpha size " << " " << accum_alpha_size
      << std::endl;
   vprDEBUG_NEXTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << std::setiosflags(std::ios::left) << std::setfill('.')
      << indent_text << std::setw(pad_width_dot)
      << "Full-screen anti-aliasing " << " " << std::boolalpha << want_fsaa
      << std::endl;
   vprDEBUG_CONTnl(vrjDBG_DRAW_MGR, vprDBG_CONFIG_LVL)
      << vprDEBUG_FLUSH;

   attrs.push_back(PFFB_RED_SIZE);         attrs.push_back(red_size);
   attrs.push_back(PFFB_GREEN_SIZE);       attrs.push_back(green_size);
   attrs.push_back(PFFB_BLUE_SIZE);        attrs.push_back(blue_size);
   attrs.push_back(PFFB_ALPHA_SIZE);       attrs.push_back(alpha_size);
   attrs.push_back(PFFB_AUX_BUFFERS);      attrs.push_back(num_aux_bufs);
   attrs.push_back(PFFB_DEPTH_SIZE);       attrs.push_back(db_size);
   attrs.push_back(PFFB_STENCIL_SIZE);     attrs.push_back(stencil_size);
   attrs.push_back(PFFB_ACCUM_RED_SIZE);   attrs.push_back(accum_red_size);
   attrs.push_back(PFFB_ACCUM_GREEN_SIZE); attrs.push_back(accum_green_size);
   attrs.push_back(PFFB_ACCUM_BLUE_SIZE);  attrs.push_back(accum_blue_size);
   attrs.push_back(PFFB_ACCUM_ALPHA_SIZE); attrs.push_back(accum_alpha_size);
   if (want_fsaa)
   {
      attrs.push_back(PFFB_SAMPLE_BUFFER); attrs.push_back(1);
      attrs.push_back(PFFB_SAMPLES); attrs.push_back(1);
   }
}

// Initializes the application's scene <br>
// Set's the sceneRoot
void PfDrawManager::initPerformerGraph()
{
   mRoot = new pfScene;
   mRootWithSim = new pfScene;

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

void PfDrawManager::closeAPI()
{
   // NOTE: We do not call pfExit() here because it would cause the whole
   // process to exit.  If users want to call pfExit(), they must do so on
   // their own at whatever time is appropriate for their application.
   pfuExitUtil();
}

void PfDrawManager::updatePfProjections()
{
   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL)
      << "[vrj::PfDrawManager::updatePfProjections()] Entering." << std::endl
      << vprDEBUG_FLUSH;

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
            vprASSERT(false && "[vrj::PfDrawManager::updateProjections()] We don't have a valid display type, don't know what to do");
         }

         // Sim viewport
         if(cur_vp->isSimulator())
         {
            SimViewport*      sim_vp(NULL);
            PfSimInterface*   draw_sim_i(NULL);

            sim_vp = dynamic_cast<SimViewport*>(pf_vp->viewport);
            vprASSERT(sim_vp != NULL && "Could not cast supposed simulator display to SimDisplay.");

            draw_sim_i = dynamic_cast<PfSimInterface*>(sim_vp->getDrawSimInterface());
            vprASSERT(draw_sim_i != NULL && "Could not cast supposed simulator interface to PfSimInterface.");

            draw_sim_i->updateSimulatorSceneGraph();
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

   vprDEBUG_BEGIN(vrjDBG_DRAW_MGR,vprDBG_HVERB_LVL)
      << "[vrj::PfDrawManager::updatePfProjection()] Entering. viewMat:\n"
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
      << "[vrj::PfDrawManager::updatePfProjection()] Exiting.\n"
      << vprDEBUG_FLUSH;
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
   vprDEBUG_BEGIN(vrjDBG_DRAW_MGR,debugLevel)
      << "-- DEBUG DUMP --------- "
      << clrOutNORM(clrCYAN,"vrj::PfDrawManager: 0x")
      << std::hex << (void*)this << " ------------" << std::dec << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel)
      << "App: 0x" << std::hex << (void*)mApp << std::dec << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel)
      << "Scene: 0x" << std::hex << (void*)mRoot << std::dec << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel)
      << "Sim scene: 0x" << std::hex << (void*)mRootWithSim << std::dec
      << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel)
      << "Number of displays: " << mDisplays.size() << std::endl
      << vprDEBUG_FLUSH;

   for (std::vector<pfDisplay>::iterator i = mDisplays.begin(); i != mDisplays.end(); i++)
   {
      debugDumpPfDisp(&(*i),debugLevel);
   }

   vprDEBUG_END(vrjDBG_DRAW_MGR,debugLevel) << "-------- Dump end ----\n" << vprDEBUG_FLUSH;
}


void PfDrawManager::debugDumpPfDisp(pfDisplay* pf_disp, int debugLevel)
{
   vprDEBUG_BEGIN(vrjDBG_DRAW_MGR,debugLevel)
      << "Display: " << (void*)(pf_disp->disp) << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel)
      << "pWin: " << (void*)(pf_disp->pWin) << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vrjDBG_DRAW_MGR,debugLevel)
      << "visual ID: 0x" << std::hex << pf_disp->pWin->getFBConfigId()
      << std::dec << std::endl << vprDEBUG_FLUSH;

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
   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
      << "[vrj::PFconfigPWin()] framebuffer id: 0x"
      << std::hex << pWin->getFBConfigId() << std::dec << std::endl
      << vprDEBUG_FLUSH;
   /*
   int disp_num = int(pWin->getWSWindow());
   vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_CONFIG_LVL)
      << "[vrj::PFConfigPWin()] Window id: 0x" << std::hex << disp_num
      << std::dec << std::endl << vprDEBUG_FLUSH;
   */
}


/*
void PfAppFunc(pfChannel *chan, void* chandata)
{
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL) << "--- PfAppFunc: Enter ---.\n" << vprDEBUG_FLUSH;
   vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL) << "chan: " << chan << std::endl << vprDEBUG_FLUSH;

   pfChannel* master_chan = PfDrawManager::instance()->mMasterChan;
   if(master_chan == chan)
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL) << "I am the master of the house:\n" << vprDEBUG_FLUSH;
      if(PfDrawManager::instance()->mPfAppCalled == false)      // Haven't called it yet
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL) << "pfApp has not been called yet.  Let me do it...\n" << vprDEBUG_FLUSH;
         PfDrawManager::instance()->mPfAppCalled = true;
         pfApp();
      }
   }
   else
   {
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_CRITICAL_LVL) << "I am not master. I can't do anything.\n" << vprDEBUG_FLUSH;
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
      vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL)
         << "vrj::PfDrawFunc()] Drawing stereo win\n" << vprDEBUG_FLUSH;
      if(left_eye)
      {
         glDrawBuffer(GL_BACK_LEFT);
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL)
            << "[vrj::PfDrawFunc()] Set to BACK_LEFT\n" << vprDEBUG_FLUSH;
      }
      else if(right_eye)
      {
         glDrawBuffer(GL_BACK_RIGHT);
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL)
            << "[vrj::PfDrawFunc()] Set to BACK_RIGHT\n" << vprDEBUG_FLUSH;
      }
      else
         vprASSERT(false);
   }
   else                                // No Stereo or have sim, so just go to back buffer
   {
      vprDEBUG(vrjDBG_DRAW_MGR,vprDBG_VERB_LVL)
         << "[vrj::PfDrawFunc()] Drawing mono window\n" << vprDEBUG_FLUSH;
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
   boost::ignore_unused_variable_warning(p);
    PfDrawManager* pf_draw_mgr = PfDrawManager::instance();     // get the draw manager

    // If "first pw", then sync
    // What is the "first" pw and how do I know?

    // For each pfDisplay
    //    if(pw == display->pipeWindow)
    //

    // Swap the buffer
    vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_VERB_LVL) << "--- PfPipeSwapFunc: pipe:" << pf_draw_mgr << " -- pw:" << pw << "\n" << vprDEBUG_FLUSH;

    // Barrier for Cluster
    //vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) <<  "BARRIER: Going to sleep for: " << num << std::endl << vprDEBUG_FLUSH;
   cluster::ClusterManager::instance()->createBarrier();
    //vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) <<  "BARRIER: IS DONE" << std::endl << vprDEBUG_FLUSH;

    pw->swapBuffers();
}

}
