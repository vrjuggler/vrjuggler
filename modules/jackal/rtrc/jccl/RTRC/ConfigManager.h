/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _JCCL_CONFIG_MANGER_H_
#define _JCCL_CONFIG_MANGER_H_

#include <jccl/jcclConfig.h>
#include <vector>
#include <list>
#include <boost/signal.hpp>

#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Singleton.h>
#include <vpr/DynLoad/Library.h>

#include <jccl/Config/Configuration.h>
#include <jccl/Config/ConfigDefinitionRepository.h>
#include <jccl/Config/ConfigElement.h>

#include <jccl/Util/Debug.h>


namespace jccl
{

class ConfigElementHandler;
class RemoteReconfig;

/** \class ConfigManager ConfigManager.h jccl/RTRC/ConfigManager.h
 *
 * Dynamic reconfiguration management for JCCL.  The Config Manager provides
 * a complete solution for configuring an application via JCCL's ConfigElement
 * type.  The Config Manager can configure based on static configuration
 * files  or dynamically via a network interface.
 *
 * The Config Manager can be used in a number of ways; it provides a complete
 * default solution to configuration, but also exposes enough of its inner
 * workings to allow applications to implement their own dynamic configuration
 * algorithms.
 *
 * The simplest way to use the Config Manager is to create one or more types
 * that implement the jccl::ConfigElementHandler interface and to register
 * instances of these using the method
 * jccl::ConfigManager::addConfigElementHandler().
 *
 * Requests to add config elements can be added via the network or by the
 * addPending*() methods of this class.  These requests are added to the
 * Config Manager's "pending" list.  The Config Manager also maintains an
 * "active" list, containing all the config elements that have been
 * successfully configured.
 *
 * Once config element handlers have been registered with the Config Manager,
 * the application should periodically call the method
 * jccl::ConfigManager::attemptReconfiguration().  This will try to match items
 * in the pending list with instances of jccl::ConfigElementHandler that know
 * how to handle them.
 *
 * For more advanced uses, jccl::ConfigManager provides accessor functions that
 * allow direct manipulation of the pending and active lists.  This allows an
 * application to decide on its own when and how to process requests in the
 * pending list.  However, the attemptReconfiguration() interface should be
 * sufficient for almost all uses.
 *
 * @date January 13, 2000
 */
class JCCL_CLASS_API ConfigManager
{
public:
   struct PendingElement
   {
      PendingElement() : mType(ADD)
      {;}

      enum Type
      {
         ADD    = 0,  /**< Pending element is to be added to the active configuration. */
         REMOVE = 1   /**< Pending element is to be removed from the active configuration. */
      };

      unsigned mType;           /**< What type of element is it (ADD or REMOVE) */
      ConfigElementPtr mElement;
   };


public: // -- Query functions --- //

   //@{
   /** @name Query functions. */

   /**
    * Attempts to find a config element matching the given name in the active
    * list.  If such an element is found, it is returned to the caller.  If
    * not, an empty jccl::ConfigElementPtr is returned.
    *
    * @param elementName The name of the element to find.
    */
   jccl::ConfigElementPtr getElementFromActive(const std::string& elementName);

   /**
    * Attempts to find a config element matching the given name in the pending
    * list.  If such an element is found, it is returned to the caller.  If
    * not, an empty jccl::ConfigElementPtr is returned.
    *
    * @param elementName The name of the element to find.
    */
   jccl::ConfigElementPtr getElementFromPending(const std::string& elementName);

   /**
    * Attempts to find a config element matching the given name, first in the
    * active list.  If the element is not found in the active list, the pending
    * list is searched.  If an element with a matching name is found in either
    * list, it is returned to the caller.  If not, an empty
    * jccl::ConfigElementPtr is returned.
    *
    * @param elementName The name of the element to find.
    */
   jccl::ConfigElementPtr getElementNamed(const std::string& elementName);

   /** Checks if the named conifg element is in the active configuration.
    *  This locks the active list to do processing.
    *
    *  @note This should not be used often. Use this at your own risk.
    */
   bool isElementInActiveList(const std::string& elementName);

   /** Is the element of this type in the active configuration?
    *  This locks the active list to do processing.
    *
    *  @note This should not be used often. Use this at your own risk.
    */
   bool isElementTypeInActiveList(const std::string& elementType);

   /** Is there a element of this type in the pending list?
    *  This locks the pending list to do processing.
    *
    *  @note This should not be used often. Use this at your own risk.
    */
   bool isElementTypeInPendingList(const std::string& elementType);

   /**
    * Searches the active list for an element matching the given type.  If
    * none is found, the pending list is searched.
    *
    * @param elementType The type of config element for which a search of the
    *                    pending and active lists will be performed.
    *
    * @return true is returned if an element of the given type is found in
    *         either the active or the pending list (in that order).  false is
    *         returned if no such element is found in either list.
    */
   bool hasElementType(const std::string& elementType);
   //@}

public:   // ----- PENDING LIST ----- //

   //@{
   /** @name Pending List accessors and manipulators. */

   /** Marks pending list as "not stale".
    *  To minimize time spent on configuration calls, the Configuration
    *  Manager will mark the pending list as "stale" if it does not
    *  change for several consecutive calls of attemptReconfiguration.
    *  Call this method when something happens that might allow
    *  items on a stale pending list to be processed.
    *
    *  For example, in VR Juggler, applications (which are
    *  ConfigElementHandlers) can be explicitly changed via a vrj::Kernel
    *  method.  When this happens, the VR Juggler kernel calls
    *  refreshPendingList because the new application object may be able
    *  to process config elements that the old one could not.
    *
    *  Generally, if an object is added to the system via
    *  ConfigElementHandler's configAdd method, it is not necessary to
    *  call this function explicitly; the ConfigManager will notice that
    *  the pending and active lists have changed and will consider the
    *  pending list to be fresh.
    *
    *  @see shouldCheckPending
    */
   void refreshPendingList();

   /** Erases an item from the pending list.
    *  The pending list must be locked && item must be in list.
    *  Item is invalid after this operation.
    */
   void removePending(std::list<PendingElement>::iterator item);

   /** Checks to see if the Pending List is stale, meaning that the
    *  ConfigManager is not actively trying to configure anything
    *  right now.
    */
   bool isPendingStale();

   /** Locks the pending list.
    *  This function blocks until it can lock the list of pending
    *  configuration changes.
    *  The caller of this method must call unlockPending() when it
    *  is finished viewing/modifying the pending list.
    */
   void lockPending()
   {
      mPendingLock.acquire();
   }

   /** Unlocks the pending list.
    *  Unlocks the mutex held on the pending list.
    *  The caller of this method must first have called lockPending().
    */
   void unlockPending()
   {
      mPendingLock.release();
   }

   /** Get an iterator to the beginning of the pending list.
    *  The caller of this method must have locked the pending list.
    */
   std::list<PendingElement>::iterator getPendingBegin()
   {
      vprASSERT(1 == mPendingLock.test());
      return mPendingConfig.begin();
   }

   /** Get an iterator to the end of the pending list.
    *  The caller of this method must have locked the pending list.
    */
   std::list<PendingElement>::iterator getPendingEnd()
   {
      vprASSERT(1 == mPendingLock.test());
      return mPendingConfig.end();
   }

   /** Get the size of the pending list.
    *  CONCURRENCY: concurrent
    */
   std::list<PendingElement>::size_type getNumPending()
   {
      return mPendingConfig.size();
   }
   //@}

private:
   /** Add an entry to the pending list.
    *  The pending list must not be locked.
    *  A copy of the pendingElement is placed on the pending list.
    *
    * @pre mPendingLock mutex must be locked.
    */
   void addPending(PendingElement& pendingElement);

   /** Print a copy of the pending list to vprDEBUG.
    *  The caller of this method must have locked the pending list.
    */
   void debugDumpPending(int debug_level);

   /** Checks if we need to check the pending list.
    *  Checks if the pending list is "fresh" or if it should be marked
    *  as "stale".  If the pending list has been checked several times
    *  without changing at all, we can assume that the elements inside of
    *  it cannot be processed by the application.
    *  This is a utility function for attemptReconfiguration.
    *  CONCURRENCY: concurrent.
    *
    *  @see attemptReconfiguration
    */
   bool shouldCheckPending();

   /**
    * Merges the contents of the incoming config element list to the
    * pending list.
    *
    * @pre The pending list is not locked.
    * @post Everything in the incoming list is moved to the pending list.
    *       The incoming list is empty.
    */
   void mergeIncomingToPending();

public:   // ----- ACTIVE LIST ----- //
   //@{
   /** @name Active List accessors and manipulators. */

   /** Checks if active list is empty.
    *  CONCURRENCY: concurrent
    */
   bool isActiveEmpty()
   {
      return mActiveConfig.vec().empty();
   }

   /** Locks the active list.
    *  This function blocks until it can lock the list of active
    *  config elements.
    *  The caller of this method must call unlockActive() when it
    *  is finished viewing/modifying the active list.
    */
   void lockActive()
   {
      mActiveLock.acquire();
   }

   /** Unlocks the active list.
    *  The method releases the lock on the active configuration list.
    *  The caller of this method must have previously locked the active
    *  list with lockActive().
    */
   void unlockActive()
   {
      mActiveLock.release();
   }

   /** Get an iterator to the beginning of the active list.
    *  The caller of this method must have locked the active list.
    */
   std::vector<jccl::ConfigElementPtr>::iterator getActiveBegin()
   {
      vprASSERT(1 == mActiveLock.test());
      return mActiveConfig.vec().begin();
   }

   /** Get an iterator to the end of the active list.
    *  The caller of this method must have locked the active list.
    */
   std::vector<jccl::ConfigElementPtr>::iterator getActiveEnd()
   {
      vprASSERT(1 == mActiveLock.test());
      return mActiveConfig.vec().end();
   }

   /** Removes the named ConfigElement from the active list.
    *  The caller of this method must have locked the active list.
    *  If no element with a matching name is found, this method has
    *  no effect.
    */
   void removeActive(const std::string& elementName);

   /** Adds a ConfigElement to the active list.
    *  This method locks the active list; therefore, the caller
    *  MUST NOT have locked the list before calling it.
    *  This does not process the ConfigElement in anyway; it simply
    *  appends it to the active list.
    *  If a element with the same name is already in the active list,
    *  the old element is replaced by the new one.
    */
   void addActive(ConfigElementPtr element);

   /** Get a pointer to the active list (as a jccl::Configuration).
    *  The caller of this method must have locked the active list.
    *  The pointer returned is only valid until the list is unlocked.
    *  CONCURRENCY: sequential
    */
   Configuration* getActiveConfig()
   {
      vprASSERT(1 == mActiveLock.test());
      return &mActiveConfig;
   }
   //@}

public:   // ----- INCOMING LIST ----- //
   //@{
   /** @name Incoming List accessors and manipulators. */

   /**
    * Adds all the config elements in the given Configuration object to the
    * incoming list.  No modifications are made to the pending or active
    * lists, so this method is safe for a ConfigElementHandler object to call
    * when it needs to add config elements from its configAdd method.
    *
    * @post All the config elements in cfg are appended to mIncomingConfig.
    */
   void addConfigurationAdditions(jccl::Configuration* cfg);

   void addConfigurationRemovals(jccl::Configuration* cfg);

   void addConfigElement(jccl::ConfigElementPtr elt, PendingElement::Type t);

   std::list<PendingElement>::size_type getNumIncoming();
   //@}

public:
   /** @name Configuration Change Signals */
   //@{
   /**
    * Type definition for configuration signal that is emitted when
    * configuration changes are made. Slots take a pointer to a Configuration
    * and the change type.
    *
    * @since 1.3.0
    */
   typedef boost::signal<void(Configuration*, PendingElement::Type)> config_signal_t;

   /**
    * Connects the given slot to the signal that is emitted when a
    * configuration addition or removal happens.
    *
    * @post The given slot is connected to \c mConfigurationSignal.
    *
    * @param slot The slot object to be connected to the configuration change
    *             signal.
    *
    * @return The connection object that holds the association between the
    *         configuration signal and the given slot.
    *
    * @since 1.3.0
    */
   boost::signals::connection
      addConfigurationCallback(config_signal_t::slot_function_type slot);

   //@}

public:
   /** Scan the active list for items that don't have their dependencies
    *  filled.
    *  Any elements in the active list with dependencies not filled are
    *  added to the pending list with a pair of entries - a remove and
    *  an equivalent add.  This way, the object will be removed from the
    *  system on the next check of the pending list, and will be re-added
    *  if its dependencies are ever subsequently met.
    *
    *  @return The number of lost dependencies found.
    */
   int scanForLostDependencies();

   //@{
   /** @name Default dynamic reconfiguration handling code. */

   void addConfigElementHandler(ConfigElementHandler* h);
   void removeConfigElementHandler(ConfigElementHandler* h);

   /**
    * Loops over the list of registered config element handlers and invokes
    * their configProcessPending() method.
    *
    * @return The total number of config elements processed by all the
    *         handlers.  If this number is zero, then either no element
    *         handlers are registered, there are no pending config elements,
    *         or none of the pending config elements could be handled by the
    *         registered handlers.
    *
    * @see jccl::ConfigElementHandler
    */
   int attemptReconfiguration();

   //int attemptHandlerReconfiguration(ConfigElementHandler* h);
   //@}

   /**
    * Shutdown the Configuration system. Removes all new, pending, and active
    * configuration elements.
    *
    * @param attemptRemoval If true, add all active ConfigElements as removals
    *                       and attempt to reconfig.
    *
    * @since 1.1.11
    */
   void shutdown(bool attemptRemoval = false);

   void setRemoteReconfigPlugin(jccl::RemoteReconfig* plugin);

private:
   Configuration             mActiveConfig;  /**< Current configuration.     */
   std::list<PendingElement> mPendingConfig; /**< Pending config changes.    */
   vpr::Mutex                mPendingLock;   /**< Lock on pending list.      */
   vpr::Mutex                mActiveLock;    /**< Lock on active config list.*/

   std::list<PendingElement> mIncomingConfig;
   vpr::Mutex                mIncomingLock;

   /** List of objects that know how to handle configuration changes. */
   std::vector<ConfigElementHandler*> mElementHandlers;

   //@{
   /**
    * @name Variables used to implement some logic that "stales" the pending
    *       list.
    *
    * @see shouldCheckPending
    */

   vpr::Mutex mPendingCountMutex; /**< Lock to protect mPendingCheckCount. */

   /** Number of times pending list has been checked since it last changed. */
   int mPendingCheckCount;

   /** Size of pending list when last checked (used to check for changes). */
   std::list<PendingElement>::size_type mLastPendingSize;
   //@}

   vpr::LibraryPtr mRemoteRtrcPlugin;

   /** Network communications object for reconfiguration control. */
   RemoteReconfig* mReconfigIf;

   /** Configuration signal invoked when configuration changes occur. */
   config_signal_t mConfigurationSignal;

protected:
   ConfigManager();
   virtual ~ConfigManager();

   /** Enables the remote runtime reconfiguration interface object. */
   void loadRemoteReconfig();

   // needed for windows:
   ConfigManager(const ConfigManager&) {;}
   void operator= (const ConfigManager&) {;}

   vprSingletonHeader(ConfigManager);

}; // class ConfigManager

} // namespace jccl


#endif
