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

#ifndef _VRJ_KERNEL_H_
#define _VRJ_KERNEL_H_
//#pragma once

#include <vrj/vrjConfig.h>
#include <vector>
#include <boost/function.hpp>
#include <boost/program_options.hpp>

#include <vpr/vpr.h>
#include <vpr/Util/Singleton.h>
#include <vpr/Sync/CondVar.h>
#include <jccl/RTRC/ConfigElementHandler.h>
#include <gadget/Type/DigitalInterface.h>

#include <vrj/Kernel/UserPtr.h>


namespace gadget
{
   class InputManager;
}

namespace cluster
{
   class ClusterManager;
}

namespace vpr
{
   class BaseThread;
}

namespace vrj
{

class DisplayManager;
class PerformanceMediator;
class DrawManager;
class SoundManager;
class App;
class CocoaWrapper;

/** \class Kernel Kernel.h vrj/Kernel/Kernel.h
 *
 * Main control class for all VR Juggler applications.
 *
 * The kernel takes care of all initialization and object creation.  This
 * class is the only class that MUST be instantiated for all application
 * objects.
 */
class VJ_CLASS_API Kernel : public jccl::ConfigElementHandler
{
public:
   /**
    * Determines whether the kernel singleton will use the vrj::CocoaWrapper
    * internally. The idea is to allow a user to provide an alternative to
    * vrj::CocoaWrapper and its NSApplication delegate for better and more
    * flexible integration with other Cocoa application environments. If this
    * is called with a value of \c false, then the internal instance of
    * vrj::CocoaWrapper will not be created, and the kernel will not attempt
    * to communicate with the Cocoa system at all. The default behavior (i.e.,
    * what is done if this function is not used at all) is to create the
    * vrj::CocoaWrapper class and to have the kernel interacting with Cocoa
    * (to the limited extent that is necessary) through it.
    *
    * @pre The vrj::Kernel singleton instance has not yet been constructed.
    * @post \c sUseCocoaWrapper has the value of \p useWrapper.
    *
    * @param useWrapper A boolean flag that indicates whether 
    *
    * @note This is, of course, for Mac OS X only.
    *
    * @since 2.1.21
    */
   static void setUseCocoaWrapper(const bool useWrapper)
   {
      sUseCocoaWrapper = useWrapper;
   }

   /**
    * Get program options description for VR Juggler.
    *
    * @since 2.3
    */
   boost::program_options::options_description& getGeneralOptions();
   boost::program_options::options_description& getClusterOptions();
   boost::program_options::options_description& getConfigOptions();

   /**
    * Parse command line arguments.
    *
    * @since 2.3
    */
   bool init(int& argc, char* argv[]);

   /**
    * Initialize kernel with given variables.
    *
    * @since 2.3
    */
   bool init(const boost::program_options::variables_map& vm);

   /** @name Kernel Control Loop Functions */
   //@{
   /**
    * Starts the VR Juggler microkernel running by spawning the control loop
    * thread. The methods of the user-supplied application object are invoked
    * from the spawned thread.
    *
    * @post A signal handler is registered with the operating system for
    *       \c SIGINT, \c SIGTERM, and (on Windows only) \c SIGBREAK. Upon
    *       successful completion, a new thread is running the kernel control
    *       loop.
    *
    * @return 0 is returned if the kernel control loop thread is started
    *         successfully. 1 is returned if the thread fails to spawn.
    *
    * @see setApplication()
    * @see waitForKernelStop()
    * @see stop()
    * @see addHandlerPreCallback()
    * @see addHandlerPostCallback()
    * @see handleSignal()
    * @see controlLoop()
    */
   int start();

   /**
    * Stops the kernel control loop.
    * If there is an application set, then it will be closed first.
    *
    * @post The kernel exits, and the VR Juggler system shuts down.
    */
   void stop();

   /** Returns the status of kernel. */
   bool isRunning() const
   {
      return mIsRunning;
   }

   /**
    * Blocks until the kernel exits.
    *
    * @see stop()
    */
   void waitForKernelStop();

   /**
    * This method is for internal use only. Do not call it directly. Instead,
    * call waitForKernelStop().
    *
    * @see waitForKernelStop()
    *
    * @since 2.1.21
    */
   void doWaitForKernelStop();
   //@}

   /**
    * Type definition fo the pre- and post-stop callbacks invoked by the
    * kernel signal handler. These callbacks take a signal integer argument
    * (the signal number) and return nothing.
    *
    * @see addHandlerPreCallback
    * @see addHandlerPostCallback
    *
    * @since 2.1.14
    */
   typedef boost::function<void(const int)> signal_callback_t;

   /** @name Signal Handler Callbacks */
   //@{
   /**
    * Adds a "pre-stop" callback to be invoked if a handled signal is received
    * by this kernel instance. The job of the kernel signal handler is to try
    * to shut down the kernel cleanly upon receiving one of the handled
    * signals. Prior to stopping the kernel, all registered pre-stop callbacks
    * are invoked regardless of whether the control loop for this kernel is
    * currently running.
    *
    * @post The given callback is appended to \c mPreStopCallbacks.
    *
    * @param callback The callback to be invoked by this kernel's signal
    *                 handler prior to stopping the kernel control loop.
    *
    * @note To avoid a race condition wherein the signal handler gets
    *       registered with the operating system and a signal is delivered
    *       prior to this method being invoked, it is recommended that
    *       pre-stop callbacks be registered prior to invoking start().
    *
    * @see start()
    * @see handleSignal()
    *
    * @since 2.1.14
    */
   void addHandlerPreCallback(signal_callback_t callback);

   /**
    * Adds a "post-stop" callback to be invoked if a handled signal is
    * received by this kernel instance. The job of the kernel signal handler
    * is to try to shut down the kernel cleanly upon receiving one of the
    * handled signals. After stopping the kernel, all registered post-stop
    * callbacks are invoked regardless of whether the control loop for this
    * kernel is currently running.
    *
    * @post The given callback is appended to \c mPostStopCallbacks.
    *
    * @param callback The callback to be invoked by this kernel's signal
    *                 handler after stopping the kernel control loop.
    *
    * @note To avoid a race condition wherein the signal handler gets
    *       registered with the operating system and a signal is delivered
    *       prior to this method being invoked, it is recommended that
    *       post-stop callbacks be registered prior to invoking start().
    *
    * @see start()
    * @see handleSignal()
    *
    * @since 2.1.14
    */
   void addHandlerPostCallback(signal_callback_t callback);
   //@}

   /**
    * Loads initial configuration data for the managers.
    * @post Input Manager, Display Manager, and kernel configuration files
    *       are loaded and handed to configAdd().
    */
   void initConfig();

   /** The Kernel loop. */
   void controlLoop();

   /**
    * Sets the application object for the Kernel to deal with.
    * If there is another application active, the kernel has to stop that
    * application first and then restart all the graphics API-specific
    * Managers.
    *
    * @param newApp If NULL, stops current application.
    */
   void setApplication(vrj::App* newApp);

   /**
    * Loads configuration data for the kernel.
    *
    * @post Config data has been read into initial the buffer.
    *
    * @param filename The name of the configuration file to load.
    */
   void loadConfigFile(const char* filename)
   {
      std::string filename_str = std::string(filename);
      loadConfigFile(filename_str);
   }

   /**
    * Loads configuration data for the kernel.
    *
    * @post Config data has been read into initial the buffer.
    *
    * @param filename The name of the configuration file to load.
    */
   void loadConfigFile(std::string filename);

   /**
    * Scans the given directory (or directories) for .jdef files and loads all
    * discovered files into the JCCL Element Factory.
    *
    * @post The config element factory can now manage elements with the
    *       discovered types.
    *
    * @param path One or more directories, delineated by a platform-specific
    *             path separator, that will be searched for .jdef files.
    */
   void scanForConfigDefinitions(const std::string& path);

protected:  // -- CONFIG ELEMENT HANDLER
   /** @name Config element handler interface implementation */
   //@{
   /**
    * Can the handler handle the given element?
    * @return true if we can handle the element; false otherwise.
    */
   virtual bool configCanHandle(jccl::ConfigElementPtr element);

   /**
    * Adds the element to the configuration.
    *
    * @pre configCanHandle(element) == true.
    *
    * @return Success.
    */
   virtual bool configAdd(jccl::ConfigElementPtr element);

   /**
    * Removes the element from the current configuration.
    * @pre    configCanHandle(element) == true.
    * @return success.
    */
   virtual bool configRemove(jccl::ConfigElementPtr element);
   //@}

protected:
   /** @name Local config functions */
   //@{
   /** Adds a User to the system. */
   bool addUser(jccl::ConfigElementPtr element);

   /**
    * Removes a User from the system.
    */
   bool removeUser(jccl::ConfigElementPtr element);
   //@}

protected:
   /**
    * Updates any data that needs updated once a frame (Trackers, etc.).
    * @post All tracker data is ready for next frame.
    */
   void updateFrameData();

   /**
    * Checks to see if there is reconfiguration to be done.
    * @post Any reconfiguration needed has been completed.
    * @note Can only be called by the kernel thread.
    */
   void checkForReconfig();

   /**
    * Changes the application in use.
    * If there is another application object active, it has to stop that
    * application first then restart all API-specific Managers.
    *
    * @param newApp If NULL, stops current application.
    *
    * @note This can only be called from the kernel thread.
    */
   void changeApplication(vrj::App* newApp);

   /** Initializes the signal buttons for the kernel. */
   void initSignalButtons();

   /** Checks the signal buttons to see if anything has been triggered. */
   void checkSignalButtons();

protected:
   /** @name Draw Manager routines */
   //@{
   /**
    * Starts the Draw Manager running.
    * Calls the app callbacks for the Draw Manager.
    *
    * @pre The application object, current Draw Manager, and current Display
    *      Manager have all been set.
    * @post All processes and data should have been created by Draw Manager.
    *
    * @param newMgr Is this a new manager or the same one.
    */
   void startDrawManager(bool newMgr);

   /**
    * Stops the Draw Manager, closes its resources, and deletes it.
    * @post The Draw Manager's resources are closed, the Draw Manager is
    *       deleted, and the Display Manger set to have a NULL Draw Manager
    *       instance.
    */
   void stopDrawManager();
   //@}

   /** @name Sound Manager Routines */
   //@{
   /**
    * Stops the Sound Manager and closes its resources.
    *
    * @post \c mSoundManager is NULL.
    *
    * @since 2.3.15
    */
   void stopSoundManager();
   //@}

public:
   /** @name Global "get" interface */
   //@{
   /** Gets the Input Manager. */
   gadget::InputManager* getInputManager();

   /**
    * Gets the user associated with the given name.
    *
    * @param userName The name of the user to return.
    *
    * @return NULL if not found.
    */
   vrj::UserPtr getUser(const std::string& userName);

   /** Returns a list of the users. */
   const std::vector<vrj::UserPtr>& getUsers() const
   {
      return mUsers;
   }

   const vpr::Thread* getThread()
   {
      return mControlThread;
   }
   //@}

protected:
   /**
    * Handles a signal delivered by the operating system. The signals to be
    * handled were registered in start(). The job of this method is to stop
    * the kernel if it is still running. User-provided callbacks (if any) are
    * invoked before and after stopping the kernel regardless of whether this
    * kernel is currently running.
    *
    * @post The action for \p signum is restored to use the default behavior.
    *       Thus, if the same signal is delivered again, this method will not
    *       be invoked.
    *
    * @param signum The signal ID.
    *
    * @see start()
    * @see addHandlerPreCallback()
    * @see addHandlerPostCallback()
    *
    * @since 2.1.14
    */
   void handleSignal(const int signum);


   boost::program_options::options_description mGeneralOptionDesc; /**< Command line options description. */
   boost::program_options::options_description mClusterOptionDesc; /**< Command line options description. */
   boost::program_options::options_description mConfigOptionDesc;  /**< Command line options description. */

   vrj::App*      mApp;                  /**< The current active app object */
   vrj::App*      mNewApp;               /**< New application to set */
   bool           mNewAppSet;            /**< Flag to notify that a new application should be set */

   bool               mIsRunning;        /**< Flag for wether the kernel is currently running */
   bool               mExitFlag;         /**< Set true when the kernel should exit */
   vpr::Thread*       mControlThread;    /**< The thread in control of me */
   vpr::CondVar       mExitWaitCondVar;  /**< Cond var for waiting for exit */

   /** @name Factories and Managers */
   //@{
   gadget::InputManager*      mInputManager;          /**< The input manager for the system  */
   DrawManager*               mDrawManager;           /**< The Draw Manager we are currently using */
   SoundManager*              mSoundManager;          /**< The Audio Manager we are currently using  */
   DisplayManager*            mDisplayManager;        /**< The Display Manager we are currently using */
   cluster::ClusterManager*   mClusterManager;        /**< The Cluster Manager for the system*/
   PerformanceMediator*       mPerformanceMediator;
   //@}

   /** @name Multi-user information */
   //@{
   std::vector<vrj::UserPtr> mUsers;    /**< A list of user objects in system */
   //@}

   /** Control "signals" from input interfaces. */
   gadget::DigitalInterface   mStopKernelSignalButton;

   /** @name Signal Handler Callbacks */
   //@{
   /** Callbacks invoked by handleSignal() before stopping the kernel. */
   std::vector<signal_callback_t> mPreStopCallbacks;

   /** Callbacks invoked by handleSignal() after stopping the kernel. */
   std::vector<signal_callback_t> mPostStopCallbacks;
   //@}

   /** @name Cluster configuration. */
   //@{
   bool mClusterMode;
   bool mClusterMaster;
   bool mClusterSlave;
   //@}

   static bool sUseCocoaWrapper;
#if defined(VPR_OS_Darwin) && defined(VRJ_USE_COCOA)
   CocoaWrapper* mCocoaWrapper;
#endif

   // ----------------------- //
   // --- SINGLETON STUFF --- //
   // ----------------------- //
protected:
   /** Constructor: Hidden, so no direct instantiation is allowed. */
   Kernel();

   Kernel(const vrj::Kernel& k) : jccl::ConfigElementHandler(k) {;}
   void operator=(const vrj::Kernel&) {;}

   virtual ~Kernel();

   vprSingletonHeader( Kernel );
};

} // end namespace

#endif
