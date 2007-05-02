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

#include <vrj/vrjConfig.h>
#include <string.h>
#include <algorithm>
#include <boost/bind.hpp>

#include <vrj/vrjParam.h>
#include <vrj/Kernel/Kernel.h>
#include <vrj/Util/Debug.h>
#include <vrj/Util/Version.h>
#include <vrj/Draw/DrawManager.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Kernel/App.h>
#include <vrj/Kernel/User.h>
#include <vrj/Kernel/KernelExceptions.h>
#include <vrj/Performance/PerformanceMediator.h>
#include <vrj/Sound/SoundManager.h>

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/Signal.h>
#include <vpr/System.h>
#include <vpr/Util/Version.h>
#include <vpr/Util/FileUtils.h>
#include <vpr/Perf/ProfileManager.h>

#include <gadget/Util/Version.h>
#include <gadget/InputManager.h>

#include <cluster/ClusterException.h>
#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork.h>

#include <jccl/Config/ParseUtil.h>
#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ElementFactory.h>
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/Util/Version.h>

#if defined(VPR_OS_Darwin) && defined(VRJ_USE_COCOA)
#  include <vrj/Kernel/CocoaWrapper.h>
#endif

// This is included here to avoid conflicts with boost/signal.hpp (included as
// a result of including vpr/Thread/Thread.h).
#include <boost/bind/apply.hpp>


namespace po = boost::program_options;

namespace
{

vrj::Kernel::signal_callback_t gRealHandler;

void signalHandler(int signum)
{
   gRealHandler(signum);
}

}

namespace vrj
{

bool Kernel::sUseCocoaWrapper(true);

vprSingletonImp(Kernel);

bool Kernel::init(int& argc, char* argv[])
{
   // Declare the supported options.
   po::options_description desc("VR Juggler Options");
   desc.add_options()
       ("help,h", "Produce help message")
       ("vrjmaster", po::bool_switch(), "This node is the cluster master.")
       ("vrjslave", po::bool_switch(), "This node is a cluster slave.")
   ;

   // Construct a parser and do the actuall parsing.
   po::command_line_parser parser(argc, argv);
   po::parsed_options parsed = parser.options(desc).allow_unregistered().run();

   // Keep track of the options that we actually use.
   // XXX: This is only needed because of a bug in versions of boost.program_options < 1.34
   //      where calling po::store(parsed_options, variable_map) throws an exception when
   //      calling find() on a unrecognized option.
   po::parsed_options used_options(&desc);

   // Keep track of the tokens that we use so that we know to remove them from argv.
   std::vector<std::string> used_tokens;
   for (unsigned int i = 0; i < parsed.options.size(); i++)
   {
      // If the option was not recognized, and is not positional.
      if (!parsed.options[i].unregistered &&
           parsed.options[i].position_key == -1)
      {
         used_options.options.push_back(parsed.options[i]);
         std::copy(parsed.options[i].original_tokens.begin(),
                   parsed.options[i].original_tokens.end(),
                   std::back_inserter(used_tokens));
      }
   }

   // Keep track of the "new" end of the argv array.
   char** new_end = argv+argc;
   for (std::vector<std::string>::const_iterator itr = used_tokens.begin(); itr != used_tokens.end(); itr++)
   {
      // Don't remove help arguments.
      if (0 == strcmp((*itr).c_str(), "-h") ||
          0 == strcmp((*itr).c_str(), "--help"))
      { continue; }

      // Remove the token from argv if we used it.
      boost::function<bool (char *)> remove_equal = boost::bind(std::equal_to<int>(), 0, boost::bind(strcmp, (*itr).c_str(), _1));
      new_end = std::remove_if(argv, argv + argc, remove_equal);
   }

   // Overwrite argc to contain the new size of argv.
   argc = std::distance<char**>(argv, new_end);

   // Finally store our options and use them.
   po::variables_map vm;
   po::store(used_options, vm);

   if(vm.count("help"))
   {
      std::cout << desc << std::endl;
      return false;
   }

   return init(vm);
}

bool Kernel::init(const po::variables_map& vm)
{
   bool cluster_master = vm["vrjmaster"].as<bool>();
   bool cluster_slave = vm["vrjslave"].as<bool>();
   if (cluster_master && cluster_slave)
   {
      throw vpr::Exception("Can't be a cluster master and slave.", VPR_LOCATION);
   }
   std::cout << "Cluster Master [" << (cluster_master ? "True":"False") << "] Slave: [" << (cluster_slave ? "True":"False") << "]" << std::endl;

   mClusterManager = cluster::ClusterManager::instance();
   mClusterManager->init(cluster_master, cluster_slave);
   mClusterManager->connectToConfigManager();

   return true;
}

// Starts the Kernel loop running.
int Kernel::start()
{
   // Set the real signal handler to be invoked by signalHandler() (see
   // above).
   gRealHandler = boost::bind(&Kernel::handleSignal, this, _1);

   vpr::SignalAction sig_action(signalHandler);
   // Catch SIGINT, SIGTERM, and SIGBREAK so that we can shut down the kernel
   // cleanly.
   vpr::SigHandler::registerHandler(SIGINT, sig_action, false);
   vpr::SigHandler::registerHandler(SIGTERM, sig_action, false);
#if defined(VPR_OS_Windows)
   vpr::SigHandler::registerHandler(SIGBREAK, sig_action, false);
#endif

   if(mControlThread != NULL) // Have already started
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " vrj::Kernel::start() called when kernel already running!\n"
         << vprDEBUG_FLUSH;
      vprASSERT(false);
      exit(0);
   }

   mIsRunning = true;
   initSignalButtons();    // Initialize the signal buttons that may be pressed

   if ( NULL == mClusterManager )
   {
      mClusterManager = cluster::ClusterManager::instance();
   }

   // If we are in cluster mode, start the cluster.
   if (cluster::ClusterManager::instance()->isClusterActive())
   {
      cluster::ClusterManager::instance()->start();

      do
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED,"ERROR:")
            << " vrj::Kernel::start() configuring before starting cluster." << std::endl << vprDEBUG_FLUSH;

         checkForReconfig();
      }
      while (!cluster::ClusterManager::instance()->isClusterReady());
   }

   int status(0);

   // Only start thread if stop has not already been called.
   // This could happen for example if a signal(ex. Ctrl-C) is
   // caught before starting.
   // TODO: Figure out a cleaner way to get this behavior.
   if (mExitFlag)
   {
      mIsRunning = false;
      mExitWaitCondVar.acquire();
      {
         mExitWaitCondVar.signal();
      }
      mExitWaitCondVar.release();
   }
   else
   {
      // Create a new thread to handle the control.
      try
      {
         // mControlThread is set in controlLoop().
         new vpr::Thread(boost::bind(&Kernel::controlLoop, this));

         vprDEBUG(vrjDBG_KERNEL, vprDBG_STATE_LVL)
            << "[vrj::Kernel::start()] Just started control loop." << std::endl
            << vprDEBUG_FLUSH;
         status = 1;
      }
      catch (vpr::Exception& ex)
      {
         vprDEBUG(vrjDBG_KERNEL, vprDBG_CRITICAL_LVL)
            << "[vrj::Kernel::start()] Failed to start control loop thread!\n"
            << ex.what() << std::endl << vprDEBUG_FLUSH;
      }
   }

   return status;
}

// Set the stop flag
// NOTE: The kernel should not actually stop until the application has been
// close (ie. mApp is NULL).
void Kernel::stop()
{
   mExitWaitCondVar.acquire();
   {
      mExitFlag = true;
      mExitWaitCondVar.signal();
   }
   mExitWaitCondVar.release();

   setApplication(NULL);    // Set NULL application so that the app gets closed

#if defined(VPR_OS_Darwin) && defined(VRJ_USE_COCOA)
   if ( NULL != mCocoaWrapper )
   {
      mCocoaWrapper->stop();
   }
#endif
}

/**
 * Blocks until the kernel exits.
 *
 * This method waits for the condition !mIsRunning && mExitFlag.
 * We need both of those because even though exit flag may be triggered
 * we can not exit until the kernel control loop stops.  This is signaled by
 * setting mIsRunning to false.
 */
void Kernel::waitForKernelStop()
{
#if defined(VPR_OS_Darwin) && defined(VRJ_USE_COCOA)
   if ( NULL != mCocoaWrapper )
   {
      mCocoaWrapper->run();
   }
#endif

   doWaitForKernelStop();
}

void Kernel::doWaitForKernelStop()
{
   mExitWaitCondVar.acquire();
   {
      while (! (!mIsRunning && mExitFlag) )   // Wait until !mIsRunning && mExitFlag
      {
         mExitWaitCondVar.wait();
      }
   }
   mExitWaitCondVar.release();
}

void Kernel::addHandlerPreCallback(signal_callback_t callback)
{
   mPreStopCallbacks.push_back(callback);
}

void Kernel::addHandlerPostCallback(signal_callback_t callback)
{
   mPostStopCallbacks.push_back(callback);
}

// The Kernel loop.
void Kernel::controlLoop()
{
   vprDEBUG(vrjDBG_KERNEL, vprDBG_CONFIG_LVL)
      << "vrj::Kernel::controlLoop: Started.\n" << vprDEBUG_FLUSH;
   vprASSERT (NULL != vpr::Thread::self());

   mControlThread = vpr::Thread::self();

   // Do any initial configuration
   initConfig();

   vpr::prof::start("Kernel: controlLoop",10);

   // --- MAIN CONTROL LOOP -- //
   while(! (mExitFlag && (mApp == NULL)))     // While not exit flag set and don't have app. (can't exit until app is closed)
   {
      // Might not want the Kernel to know about the ClusterNetwork
      // It is currently being registered as a jccl::ConfigElementHandler in
      // the ClusterManager constructor
         vpr::prof::start("Cluster: updateNewConnections.",10);
       cluster::ClusterManager::instance()->getNetwork()->updateNewConnections();
         vpr::prof::next("Cluster: sendRequests.",10);
       cluster::ClusterManager::instance()->sendRequests();
         vpr::prof::stop();

      bool cluster = !( cluster::ClusterManager::instance()->isClusterActive() &&
                        !cluster::ClusterManager::instance()->isClusterReady());

      // Iff we have an app and a draw manager
      if((mApp != NULL) && (mDrawManager != NULL) && cluster)
      {
            vprDEBUG(vrjDBG_KERNEL, vprDBG_HVERB_LVL)
               << "vrj::Kernel::controlLoop: mApp->preFrame()\n"
               << vprDEBUG_FLUSH;
         vpr::prof::start("App: PreFrame",10);
         mApp->preFrame();         // PREFRAME: Do Any application pre-draw stuff
         vpr::prof::stop();
            vprDEBUG(vrjDBG_KERNEL, vprDBG_HVERB_LVL)
               << "vrj::Kernel::controlLoop: Update ClusterManager preDraw()\n"
               << vprDEBUG_FLUSH;
      }
      else
      {
         // ??? Should we do this, or just grind up the CPU as fast as possible
         vprASSERT(NULL != mControlThread);      // If control thread is not set correctly, it will seg fault here
         vpr::Thread::yield();   // Give up CPU
      }

      vpr::prof::start("Cluster: preDraw",10);
      mClusterManager->preDraw();
      vpr::prof::stop();

      if((mApp != NULL) && (mDrawManager != NULL) && cluster)
      {
            vprDEBUG(vrjDBG_KERNEL, vprDBG_HVERB_LVL)
               << "vrj::Kernel::controlLoop: mApp->latePreFrame()\n"
               << vprDEBUG_FLUSH;
            vpr::prof::start("App: latePreFrame",10);
         mApp->latePreFrame();
            vprDEBUG(vrjDBG_KERNEL, vprDBG_HVERB_LVL)
               << "vrj::Kernel::controlLoop: drawManager->draw()\n"
               << vprDEBUG_FLUSH;
            vpr::prof::next("trigger draw",10);
         mDrawManager->draw();    // DRAW: Trigger the beginning of frame drawing
            vpr::prof::next("sound mgr update",10);
         mSoundManager->update();
            vprDEBUG(vrjDBG_KERNEL, vprDBG_HVERB_LVL)
               << "vrj::Kernel::controlLoop: mApp->intraFrame()\n"
               << vprDEBUG_FLUSH;
            vpr::prof::next("App: intraFrame",10);
         mApp->intraFrame();        // INTRA FRAME: Do computations that can be done while drawing.  This should be for next frame.
            vprDEBUG(vrjDBG_KERNEL, vprDBG_HVERB_LVL)
               << "vrj::Kernel::controlLoop: drawManager->sync()\n"
               << vprDEBUG_FLUSH;
            vpr::prof::next("sound sync",10);
         mSoundManager->sync();
            vpr::prof::next("draw sync",10);
         mDrawManager->sync();    // SYNC: Block until drawing is done
            vprDEBUG(vrjDBG_KERNEL, vprDBG_HVERB_LVL)
               << "vrj::Kernel::controlLoop: mApp->postFrame()\n"
               << vprDEBUG_FLUSH;
            vpr::prof::next("App: postFrame",10);
         mApp->postFrame();        // POST FRAME: Do processing after drawing is complete
            vpr::prof::stop();
      }
      else
      {
         // ??? Should we do this, or just grind up the CPU as fast as possible
         vprASSERT(NULL != mControlThread);      // If control thread is not set correctly, it will seg fault here
         vpr::Thread::yield();   // Give up CPU
      }

      // --- Stop for reconfiguration -- //
         vpr::prof::start("checkForReconfig",10);
      checkForReconfig();        // Check for any reconfiguration that needs done (system or application)
         vpr::prof::next("check kern signals",10);
      checkSignalButtons();      // Check for any pending control requests
         vprDEBUG(vrjDBG_KERNEL, vprDBG_HVERB_LVL)
            << "vrj::Kernel::controlLoop: Update Trackers\n" << vprDEBUG_FLUSH;
         vpr::prof::next("resetAllDevicesAndProxies", 10);
      getInputManager()->resetAllDevicesAndProxies();
         vpr::prof::next("updateAllDevices",10);
      getInputManager()->updateAllDevices();
         vprDEBUG(vrjDBG_KERNEL, vprDBG_HVERB_LVL)
            << "vrj::Kernel::controlLoop: Update ClusterManager\n"
            << vprDEBUG_FLUSH;
         vpr::prof::next("Cluster: postPostFrame.",10);
      mClusterManager->postPostFrame();
           vprDEBUG(vrjDBG_KERNEL, vprDBG_HVERB_LVL)
            << "vrj::Kernel::controlLoop: Update Proxies\n"
            << vprDEBUG_FLUSH;
         vpr::prof::next("updateAllProxies",10);
      getInputManager()->updateAllProxies();
         vprDEBUG(vrjDBG_KERNEL, vprDBG_HVERB_LVL)
            << "vrj::Kernel::controlLoop: Update Projections\n"
            << vprDEBUG_FLUSH;
         vpr::prof::next("Update frame data",10);
      updateFrameData();         // Any frame-based manager data
         vpr::prof::stop();
   }

   // Shut down managers now that the kernel is done.
   getInputManager()->shutdown();

   vpr::prof::stop();

   vprDEBUG(vrjDBG_KERNEL, vprDBG_WARNING_LVL)
      << "vrj::Kernel::controlLoop: Exiting. \n" << vprDEBUG_FLUSH;

   // Set the running status to false
   mExitWaitCondVar.acquire();
   {
      mIsRunning = false;
      mExitWaitCondVar.signal();
   }
   mExitWaitCondVar.release();
}

// Set the application to run
// XXX: Should have protection here
void Kernel::setApplication(App* newApp)
{
   vprDEBUG(vrjDBG_KERNEL,vprDBG_CONFIG_LVL)
      << "[vrj::Kernel::setApplication()] New application set\n"
      << vprDEBUG_FLUSH;
   mNewApp = newApp;
   mNewAppSet = true;
}

// Checks to see if there is reconfiguration to be done.
void Kernel::checkForReconfig()
{
   vprASSERT(vpr::Thread::self() == mControlThread);      // ASSERT: We are being called from kernel thread
   // ---- RECONFIGURATION --- //
   jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();
   unsigned num_processed(0);

   do
   {
      num_processed = cfg_mgr->attemptReconfiguration();
   }
   while (num_processed > 0);
   // ---- APP SWITCH ---- //
   // check for a new applications
   if(mNewAppSet)
   {
      if((mNewApp == NULL) || (mNewApp->depSatisfied()) )   // If app is NULL or dependencies satisfied
      {
         vprDEBUG(vrjDBG_KERNEL,vprDBG_CONFIG_STATUS_LVL)
            << "New application set; dependencies satisfied.\n"
            << vprDEBUG_FLUSH;
         mNewAppSet = false;
         changeApplication(mNewApp);
      }
      else
      {
#if defined(VPR_OS_Darwin) && defined(VRJ_USE_COCOA)
         sched_yield();
#else
         vprDEBUG(vrjDBG_KERNEL,vprDBG_WARNING_LVL)
            << "New application set; dependencies not satisfied yet.\n"
            << vprDEBUG_FLUSH;
#endif
      }
   }
}

// Changes the application in use
// app = NULL ==> stop draw manager and null out app
// app != NULL ==>
//             Get the draw manager needed
//             Start it
//             Give it the application
void Kernel::changeApplication(App* newApp)
{
   vprDEBUG(vrjDBG_KERNEL, vprDBG_CONFIG_LVL)
      << "vrj::Kernel::changeApplication() Changing to: "
      << std::hex << std::showbase << (void*) newApp << std::dec << std::endl
      << vprDEBUG_FLUSH;

   vprASSERT(vpr::Thread::self() == mControlThread);      // ASSERT: We are being called from kernel thread
   jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();

   // Quick out if for some reason the user calls setApplication with
   // the same application that is running.
   if (newApp == mApp)
   {
      return;
   }

   // XXX: TODO: Free resources (ie closeContext())

   // If the new application is NULL OR
   // we have an old draw manager and it is different from the new one, stop it.
   if (NULL == newApp || NULL != mDrawManager && newApp->getDrawManager() != mDrawManager)
   {
      stopDrawManager();
      cfg_mgr->removeConfigElementHandler(mDrawManager);
      cfg_mgr->removeConfigElementHandler(mSoundManager);
   }

   // If we have an old application, exit it.
   if (NULL != mApp)
   {
      cfg_mgr->removeConfigElementHandler(mApp);
      mApp->exit();
   }

   // Start using the new application.
   // NOTE: The kernel no longer has a reference to the old application
   //       so it can be safely deleted.
   mApp = newApp;

   // If we have a new application
   if (NULL != mApp)
   {
      // If the new application uses a different draw manager.
      if(mApp->getDrawManager() != mDrawManager)
      {
         mDrawManager = mApp->getDrawManager();
         vprASSERT(NULL != mDrawManager);

         mDrawManager = mApp->getDrawManager();             // Get the new draw manager
         mSoundManager = mApp->getSoundManager();           // Get the new sound manager
         cfg_mgr->addConfigElementHandler(mDrawManager);    // Tell config manager about them
         cfg_mgr->addConfigElementHandler(mSoundManager);   // Tell config manager about them
         startDrawManager(true);                      // Start the new one
      }
      else
      {
         startDrawManager(false);                     // Start new app
      }
      // Now handle configuration
      cfg_mgr->addConfigElementHandler(mApp);
      cfg_mgr->refreshPendingList();                  // New managers, so we may be able to handle config requests now
   }
}

// Initialize the signal buttons for the kernel.
void Kernel::initSignalButtons()
{
   mStopKernelSignalButton.init("VJSystemStopKernel");
}

// Check the signal buttons to see if anything has been triggered.
void Kernel::checkSignalButtons()
{
   if(mStopKernelSignalButton->getData() == gadget::Digital::TOGGLE_ON)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "Stop kernel signal button pressed: Kernel will exit.\n"
         << vprDEBUG_FLUSH;
      this->stop();  // Signal kernel to stop
   }
}


// Load config
// Setup Input, Display, and kernel
// NOTE: Does initial configuration and then sends config file to configAdd().
void Kernel::initConfig()
{
   vprDEBUG_BEGIN(vrjDBG_KERNEL, vprDBG_CONFIG_LVL)
      << "vrj::Kernel::initConfig(): Setting initial config.\n"
      << vprDEBUG_FLUSH;

   // ---- ALLOCATE MANAGERS --- //
   //initialSetupInputManager();
   mInputManager = gadget::InputManager::instance();

   //initialSetupDisplayManager();
   mDisplayManager = DisplayManager::instance();  // Get display manager
   vprASSERT(mDisplayManager != NULL);            // Did we get an object

   mPerformanceMediator = new PerformanceMediator();

   //??// processPending() // Should I do this here

   // hook dynamically-reconfigurable managers up to config manager
   // XXX: Should replace with dynamic support in future
   jccl::ConfigManager::instance()->addConfigElementHandler(this);
   jccl::ConfigManager::instance()->addConfigElementHandler(mInputManager);
   jccl::ConfigManager::instance()->addConfigElementHandler(mClusterManager);
   jccl::ConfigManager::instance()->addConfigElementHandler(mDisplayManager);
   vprDEBUG_END(vrjDBG_KERNEL, vprDBG_CONFIG_LVL)
      << "vrj::Kernel::initConfig(): Done.\n" << vprDEBUG_FLUSH;
}

void Kernel::updateFrameData()
{
   // When we have a draw manager, tell it to update its projections
   // XXX: Moved to be updated on demand
   // mDisplayManager->updateProjections();
}

bool Kernel::configCanHandle(jccl::ConfigElementPtr element)
{
   return std::string("user") == element->getID();
}

bool Kernel::configAdd(jccl::ConfigElementPtr element)
{
   const std::string element_type(element->getID());

   vprASSERT(configCanHandle(element));

   bool result(false);

   if(std::string("user") == element_type)
   {
      result = addUser(element);
   }

   return result;
}

bool Kernel::configRemove(jccl::ConfigElementPtr element)
{
   const std::string element_type(element->getID());

   vprASSERT(configCanHandle(element));

   bool result(false);

   if(std::string("user") == element_type)
   {
      result = removeUser(element);
   }

   return result;
}

// Adds a new user to the kernel.
bool Kernel::addUser(jccl::ConfigElementPtr element)
{
   vprASSERT(element->getID() == "user");

   UserPtr new_user = User::create();
   bool success = new_user->config(element);

   if(!success)
   {
      vprDEBUG(vrjDBG_KERNEL, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:") << " Failed to add new User: "
         << element->getName() << std::endl << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(vrjDBG_KERNEL, vprDBG_STATE_LVL)
         << "vrj::Kernel: Added new User: " << new_user->getName() << std::endl
         << vprDEBUG_FLUSH;
      mUsers.push_back(new_user);
   }

   return success;
}

bool Kernel::removeUser(jccl::ConfigElementPtr element)
{
   vprASSERT(element->getID() == "user");
   
   std::string user_name = element->getName();
   vrj::UserPtr user = getUser(user_name);

   if (NULL != user)
   {
      vprDEBUG(vrjDBG_KERNEL, vprDBG_STATE_LVL)
         << "vrj::Kernel: Removing User: " << user->getName() << std::endl
         << vprDEBUG_FLUSH;
         
      mUsers.erase(std::find(mUsers.begin(), mUsers.end(), user));
      return true;
   }
   return false;
}

// --- STARTUP ROUTINES --- //
void Kernel::loadConfigFile(std::string filename)
{
   vprDEBUG(vrjDBG_KERNEL,vprDBG_CONFIG_LVL)
      << "Loading config file: " << filename << std::endl << vprDEBUG_FLUSH;

   // We can allocate this on the stack because the config elements get
   // copied into a new PendingConfigElement from the configuration.
   jccl::Configuration cfg;

   // ------- OPEN Program specified Config file ------ //
   if(filename.empty())   // We have a filename
   {
      return;
   }

   bool cfg_load_success = cfg.load(filename);
   if (!cfg_load_success)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:")
         << "vrj::Kernel::loadConfigFile: Failed to load file: "
         << filename << std::endl << vprDEBUG_FLUSH;
      exit(1);
   }

   // Put all the newly loaded config elments into the Config Manager's
   // incoming element list.
   jccl::ConfigManager::instance()->addConfigurationAdditions(&cfg);
}

// Scans the given directory (or directories) for .jdef files and loads all
// discovered files into the JCCL Element Factory.
void Kernel::scanForConfigDefinitions(const std::string& path)
{
   jccl::ElementFactory::instance()->loadDefs(path);
}

// This starts up the Draw Manager.
void Kernel::startDrawManager(bool newMgr)
{
   vprASSERT((mApp != NULL) && (mDrawManager != NULL) && (mDisplayManager != NULL));

   if(newMgr)
   {
      mDrawManager->setDisplayManager(mDisplayManager);
      jccl::ConfigManager::instance()->lockPending();
      {
         mDrawManager->configProcessPending();                 // Handle any pending configuration requests BEFORE we init and start it going
      }
      jccl::ConfigManager::instance()->unlockPending();
   }

   try
   {
      mDrawManager->setApp(mApp);

      mApp->init();                // Init the app

      if(newMgr)
      {
         // Just sets up API type stuff, possibly starts new processes.
         mDrawManager->initAPI();
      }

      mApp->apiInit();             // Have app do any app-init stuff

      if(newMgr)
      {
         // This can trigger the update of windows to the Draw Manager.
         mDisplayManager->setDrawManager(mDrawManager);
      }
   }
   catch(vrj::DrawMgrException& ex)
   {
      vprDEBUG(vrjDBG_KERNEL, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING:")
         << " Draw Manager rejected application object:" << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vrjDBG_KERNEL, vprDBG_WARNING_LVL)
         << ex.what() << std::endl << vprDEBUG_FLUSH;
   }
}

// Stop the draw manager and close its resources, then delete it>
// XXX: @todo Make stop draw manager reconfigure the draw manager to close all
// the windows.
void Kernel::stopDrawManager()
{
   if(mDrawManager != NULL)
   {
      mDrawManager->closeAPI();
      mDrawManager = NULL;
      mDisplayManager->setDrawManager(NULL);
   }
}

// Gets the Input Manager.
gadget::InputManager* Kernel::getInputManager()
{
   return mInputManager;
}

UserPtr Kernel::getUser(const std::string& userName)
{
   for(unsigned int i = 0; i < mUsers.size(); ++i)
   {
      if(userName == mUsers[i]->getName())
      {
         return mUsers[i];
      }
   }

   return UserPtr();
}

void Kernel::handleSignal(const int signum)
{
   std::for_each(mPreStopCallbacks.begin(), mPreStopCallbacks.end(),
                 boost::bind<void>(boost::apply<void>(), _1, signum));

   if ( isRunning() )
   {
      stop();
   }

   std::for_each(mPostStopCallbacks.begin(), mPostStopCallbacks.end(),
                 boost::bind<void>(boost::apply<void>(), _1, signum));

   // Restore the default action for the signal now that we have done the
   // handling that we wanted to do.
   vpr::SignalAction default_action(vpr::SignalAction::DefaultAction);
   vpr::SigHandler::registerHandler(signum, default_action, false);
}

Kernel::Kernel()
   : mApp(NULL)
   , mNewApp(NULL)
   , mNewAppSet(false)
   , mIsRunning(false)
   , mExitFlag(false)
   , mControlThread(NULL)
   , mInputManager(NULL)
   , mDrawManager(NULL)
   , mSoundManager(NULL)
   , mDisplayManager(NULL)
   , mClusterManager(NULL)
   , mPerformanceMediator(NULL)
   , mClusterMode(false)
   , mClusterMaster(false)
   , mClusterSlave(false)
#if defined(VPR_OS_Darwin) && defined(VRJ_USE_COCOA)
   , mCocoaWrapper(NULL)
#endif
{
#if defined(VPR_OS_Darwin) && defined(VRJ_USE_COCOA)
   if ( sUseCocoaWrapper )
   {
      mCocoaWrapper = new CocoaWrapper();
   }
#endif

   // Print out the Juggler version number when the kernel is created.
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << std::string(strlen(VJ_VERSION) + 12, '=')
      << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << clrOutNORM(clrGREEN, "VR Juggler: ")
      << clrOutNORM(clrGREEN, VJ_VERSION) << clrRESET
      << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << clrOutNORM(clrGREEN, "VPR: ")
      << clrOutNORM(clrGREEN, vpr::getVersionString())
      << clrRESET << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << clrOutNORM(clrGREEN, "JCCL: ")
      << clrOutNORM(clrGREEN, jccl::getVersionString())
      << clrRESET << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << clrOutNORM(clrGREEN, "Gadgeteer: ")
      << clrOutNORM(clrGREEN, gadget::getVersionString())
      << clrRESET << std::endl << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << std::string(strlen(VJ_VERSION) + 12, '=')
      << std::endl << vprDEBUG_FLUSH;

   // Load in the configuration definitions
   std::string def_path;
   if ( vpr::System::getenv("VJ_BASE_DIR", def_path) )
   {
      def_path = "${VJ_BASE_DIR}/" VJ_SHARE_DIR "/data/definitions";
   }
   else
   {
      def_path = VJ_ROOT_DIR "/" VJ_SHARE_DIR "/data/definitions";
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "Loading Definitions: VJ_BASE_DIR is not set.\n"
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
         << "Defaulting to " << def_path << std::endl << vprDEBUG_FLUSH;
   }
   jccl::ElementFactory::instance()->loadDefs(def_path);


   // Set up a default configuration path if the user does not already have
   // one defined in the environment variable $JCCL_CFG_PATH.  If that
   // environment variable is not defined, check to see if $VJ_CFG_PATH is
   // defined.  If it is, use it.  Otherwise, use
   // $VJ_BASE_DIR/share/data/configFiles as a last-ditch fallback.
   // XXX: Should $VJ_CFG_PATH supercede $JCCL_CFG_PATH instead of doing it
   // this way?
   std::string cfg_path;
   if ( ! vpr::System::getenv("JCCL_CFG_PATH", cfg_path) )
   {
      if ( vpr::System::getenv("VJ_CFG_PATH", cfg_path) )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "JCCL_CFG_PATH environment variable not set.\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Using VJ_CFG_PATH instead.\n" << vprDEBUG_FLUSH;
      }
      // Neither $JCCL_CFG_PATH nor $VJ_CFG_PATH is set, so use what basically
      // amounts to a hard-coded default.
      else if ( vpr::System::getenv("VJ_BASE_DIR", cfg_path) )
      {
         cfg_path = "${VJ_BASE_DIR}/" VJ_SHARE_DIR "/data/configFiles";
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Neither JCCL_CFG_PATH nor VJ_CFG_PATH is set.\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Defaulting to " << cfg_path << std::endl << vprDEBUG_FLUSH;
         cfg_path = vpr::replaceEnvVars(cfg_path);
      }
      else
      {
         cfg_path = VJ_ROOT_DIR "/" VJ_SHARE_DIR "/data/configFiles";
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "JCCL_CFG_PATH, VJ_CFG_PATH, and VJ_BASE_DIR are not set.\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Defaulting to " << cfg_path << std::endl << vprDEBUG_FLUSH;
         cfg_path = vpr::replaceEnvVars(cfg_path);
      }

      jccl::ParseUtil::setCfgSearchPath(cfg_path);
   }
}

Kernel::~Kernel()
{
#if defined(VPR_OS_Darwin) && defined(VRJ_USE_COCOA)
   if ( NULL != mCocoaWrapper )
   {
//      delete mCocoaWrapper;
      mCocoaWrapper = NULL;
   }
#endif

   if ( NULL != mPerformanceMediator )
   {
      delete mPerformanceMediator;
      mPerformanceMediator = NULL;
   }

   if ( NULL != mControlThread )
   {
      delete mControlThread;
      mControlThread = NULL;
   }

   mUsers.clear();
}

}
