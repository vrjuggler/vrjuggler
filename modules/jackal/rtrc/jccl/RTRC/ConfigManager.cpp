/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <jccl/jcclConfig.h>

#include <iomanip>
#include <boost/filesystem/path.hpp>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/DynLoad/LibraryLoader.h>
#include <vpr/IO/Socket/InetAddr.h>

#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ElementFactory.h>
#include <jccl/RTRC/DependencyManager.h>
#include <jccl/RTRC/ConfigElementHandler.h>
#include <jccl/Util/Debug.h>
#include <jccl/RTRC/RemoteReconfig.h>
#include <jccl/RTRC/ConfigManager.h>


namespace fs = boost::filesystem;

namespace jccl
{

vprSingletonImpLifetime(ConfigManager, 10);


ConfigManager::ConfigManager()
   : mReconfigIf(NULL)
{
   mPendingCheckCount = 0;
   mLastPendingSize = 0;

   loadRemoteReconfig();
}

void ConfigManager::loadRemoteReconfig()
{
   vprASSERT(NULL == mReconfigIf &&
             "RTRC interface object already instantiated.");

   const std::string jccl_base_dir("JCCL_BASE_DIR");
   const std::string vj_base_dir("VJ_BASE_DIR");
   std::string base_dir;

   if ( ! vpr::System::getenv(jccl_base_dir, base_dir) )
   {
      if ( vpr::System::getenv(vj_base_dir, base_dir) )
      {
         vprDEBUG(jcclDBG_RECONFIG, vprDBG_WARNING_LVL)
            << "JCCL_BASE_DIR environment variable not set.\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(jcclDBG_RECONFIG, vprDBG_WARNING_LVL)
            << "Using VJ_BASE_DIR instead.\n" << vprDEBUG_FLUSH;
      }
      else
      {
         base_dir = JCCL_ROOT_DIR;

         vprDEBUG(jcclDBG_RECONFIG, vprDBG_WARNING_LVL)
            << "Neither JCCL_BASE_DIR nor VJ_BASE_DIR is set."
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(jcclDBG_RECONFIG, vprDBG_WARNING_LVL)
            << "Using default " << base_dir << " instead."
            << std::endl << vprDEBUG_FLUSH;
      }
   }

   const std::string no_rtrc_plugin("NO_RTRC_PLUGIN");
   std::string junk;

   // If the user has the environment variable NO_RTRC_PLUGIN set (to any
   // value), do not attempt to load the plug-in.
   if ( vpr::System::getenv(no_rtrc_plugin, junk) )
   {
      vprDEBUG(jcclDBG_RECONFIG, vprDBG_STATE_LVL)
         << "Remote reconfig plug-in loading disabled via NO_RTRC_PLUGIN."
         << std::endl << vprDEBUG_FLUSH;
      return;
   }

#if defined(VPR_OS_IRIX) && defined(_ABIN32)
   const std::string bit_suffix("32");
#elif defined(VPR_OS_IRIX) && defined(_ABI64) || \
      defined(VPR_OS_Linux) && defined(__x86_64__)
   const std::string bit_suffix("64");
#else
   const std::string bit_suffix("");
#endif
   
   const std::string jccl_subdir_base("jccl");
   // If versioning is enabled, then the name of the directory containing the
   // Sonix plug-ins must contain version information.
#if defined(JCCL_USE_VERSIONING)
   std::string jccl_ver_str;
   const std::string jccl_version("JCCL_VERSION");

   // If $JCCL_VERSION is set, use the value of that environment variable
   // as the version component of the plug-in subdirectory name. Otherwise,
   // use the compile-time value provided by JCCL_VERSION_DOT.
   if ( ! vpr::System::getenv(jccl_version, jccl_ver_str) )
   {
      jccl_ver_str = JCCL_VERSION_DOT;
   }

   std::string jccl_subdir(jccl_subdir_base + std::string("-") +
                             jccl_ver_str );

   // If versioning is not enabled, then the directory containing the
   // Jackal plug-ins will not incorporate version information.
#else
   std::string jccl_subdir(jccl_subdir_base);
#endif


   std::vector<fs::path> search_path(1);
   search_path[0] = fs::path(base_dir, fs::native) /
                       (std::string("lib") + bit_suffix) /
                       std::string(jccl_subdir) / std::string("plugins");

   // In the long run, we may not want to hard-code the base name of the
   // plug-in we load.  If we ever reach a point where we have multiple ways
   // of implementing remote run-time reconfiguration, we could have options
   // for which plug-in to load.
   const std::string reconfig_dso("corba_rtrc");
   mRemoteRtrcPlugin = vpr::LibraryLoader::findDSO(reconfig_dso, search_path);

   bool load_done(true);

   if ( mRemoteRtrcPlugin.get() != NULL )
   {
      vprASSERT(! mRemoteRtrcPlugin->isLoaded() &&
                "Plug-in should not already be loaded");
      vprDEBUG(jcclDBG_RECONFIG, vprDBG_CONFIG_STATUS_LVL)
         << "Loading library: " << std::setiosflags(std::ios::right)
         << std::setfill(' ') << std::setw(50) << mRemoteRtrcPlugin->getName()
         << std::resetiosflags(std::ios::right) << "     " << vprDEBUG_FLUSH;

      try
      {
         mRemoteRtrcPlugin->load();

         const std::string init_func_name("initPlugin");
         void* entry_point = mRemoteRtrcPlugin->findSymbol(init_func_name);

         if ( NULL != entry_point )
         {
            vprDEBUG_CONT(jcclDBG_RECONFIG, vprDBG_CONFIG_STATUS_LVL)
               << "[ " << clrSetNORM(clrGREEN) << "OK" << clrRESET << " ]\n"
               << vprDEBUG_FLUSH;

            jccl::RemoteReconfig* (*init_func)(jccl::ConfigManager*);

            // Cast the entry point function to the correct signature so that we
            // can call it.  All dynamically plug-ins must have an entry point
            // function that takes no argument and returns a pointer to an
            // implementation of the jccl::RemoteReconfig interface.
            init_func =
               (jccl::RemoteReconfig* (*)(jccl::ConfigManager*)) entry_point;

            // Call the entry point function.
            jccl::RemoteReconfig* plugin = (*init_func)(this);

            if ( NULL != plugin )
            {
               setRemoteReconfigPlugin(plugin);
            }
            else
            {
               load_done = false;
               vprDEBUG_CONT(jcclDBG_RECONFIG, vprDBG_CONFIG_STATUS_LVL)
                  << "[ " << clrSetNORM(clrRED) << "entry point call FAILED"
                  << clrRESET << " ]\n" << vprDEBUG_FLUSH;
            }
         }
         else
         {
            load_done = false;
            vprDEBUG_CONT(jcclDBG_RECONFIG, vprDBG_CONFIG_STATUS_LVL)
               << "[ " << clrSetNORM(clrRED) << "FAILED lookup" << clrRESET
               << " ]\n" << vprDEBUG_FLUSH;
         }
      }
      catch (vpr::Exception& ex)
      {
         load_done = false;
         vprDEBUG_CONT(jcclDBG_RECONFIG, vprDBG_CONFIG_STATUS_LVL)
            << "[ " << clrSetNORM(clrRED) << "FAILED" << clrRESET << " ]\n"
            << ex.what() << std::endl << vprDEBUG_FLUSH;
      }
   }
   else
   {
      load_done = false;
   }

   if ( ! load_done )
   {
      vprDEBUG(jcclDBG_RECONFIG, vprDBG_WARNING_LVL)
         << "Failed to load the remote run-time reconfiguration"
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(jcclDBG_RECONFIG, vprDBG_WARNING_LVL)
         << "plug-in. Remote run-time reconfiguration is disabled."
         << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(jcclDBG_RECONFIG, vprDBG_WARNING_LVL)
         << "(This is not a fatal error.)" << std::endl << vprDEBUG_FLUSH;

      // The plug-in is not usable, so we can unload it.
      if ( mRemoteRtrcPlugin.get() != NULL )
      {
         if ( mRemoteRtrcPlugin->isLoaded() )
         {
            mRemoteRtrcPlugin->unload();
         }

         mRemoteRtrcPlugin.reset();
      }
   }
}

void ConfigManager::setRemoteReconfigPlugin(jccl::RemoteReconfig* plugin)
{
   // If we already have a remote reconfig plug-in, discard it first.
   if ( NULL != mReconfigIf )
   {
      vprDEBUG(jcclDBG_RECONFIG, vprDBG_STATE_LVL)
         << "[ConfigManager::setRemoteReconfigPlugin()] "
         << "Removing old remote reconfig plug-in\n" << vprDEBUG_FLUSH;

      if ( mReconfigIf->isEnabled() )
      {
         mReconfigIf->disable();
      }

      mReconfigIf = NULL;
   }

   vprDEBUG(jcclDBG_RECONFIG, vprDBG_VERB_LVL)
      << "[ConfigManager::setRemoteReconfigPlugin()] "
      << "Enabling new remote reconfig plug-in\n" << vprDEBUG_FLUSH;
   mReconfigIf = plugin;

   if ( NULL != mReconfigIf )
   {
      // Attempt to initialize the remote run-time reconfiguration component.
      if ( mReconfigIf->init() )
      {
         // Now, attempt to enable remote run-time reconfiguration.
         if ( ! mReconfigIf->enable() )
         {
            vprDEBUG(jcclDBG_RECONFIG, vprDBG_WARNING_LVL)
               << clrOutBOLD(clrYELLOW, "WARNING:")
               << " Failed to enable remote run-time reconfiguration.\n"
               << vprDEBUG_FLUSH;
            mReconfigIf = NULL;
         }
      }
      // Initialization failed.
      else
      {
         vprDEBUG(jcclDBG_RECONFIG, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Failed to initialize remote run-time reconfiguration.\n"
            << vprDEBUG_FLUSH;
         mReconfigIf = NULL;
      }
   }
}

ConfigManager::~ConfigManager()
{
   // We do not deal with shutting down or unloading the remote run-time
   // reconfiguration plug-in directly here. If the plug-in was loaded, then
   // the reference counted memory in mRemoteRtrcPlugin will handle cleaning
   // that up automatically.
}

//-------------------- Pending List Stuff -------------------------------

void ConfigManager::removePending(std::list<PendingElement>::iterator item)
{
   vprASSERT(1 == mPendingLock.test());
   mPendingConfig.erase(item);
}

void ConfigManager::refreshPendingList()
{
   vprASSERT(0 == mPendingCountMutex.test());
   mPendingCountMutex.acquire();
   mPendingCheckCount = 0;
   mPendingCountMutex.release();
}

static const int pending_repeat_limit = 3;    // Must be one or greater.  1 means only allow one time of no changes

/**
 * This routine counts the number of pending elements each time it is called.
 * if it goes pending_repeat_limit calls without changing size, then it returns
 * false until mLastPendingSize changes.
 *
 * CONCURRENCY: concurrent
 */
bool ConfigManager::shouldCheckPending()
{
   // - Lock PendingCountMutex
   // - If the size of the pending list has changed
   //   - Reset the pending check count to allow reconfig
   //   - Record the new size to check against next time
   //   - Set return value to true
   // - Else if the pending check count is less the limit
   //   - Increment the pendling check count (decrease the
   //     number of remaining iterations)
   //   - Set the return value to true
   // - Else if the pending check count equals the limit
   //   - Increment the pendling check count (needed to 
   //     push count over limit)
   //   - Print the list of stale configuration elements
   //   - Set the return value to false
   // - Else
   //   - Set the return value to false
   // - Unlock PendingCountMutex
   
   std::list<PendingElement>::size_type cur_pending_size(0);
   bool ret_val = false;

   mPendingCountMutex.acquire();

   cur_pending_size = mPendingConfig.size();
   if ( cur_pending_size != mLastPendingSize )
   {
      ret_val = true;                           // Flag it for a check
      mPendingCheckCount = 0;                   // Reset the counter
      mLastPendingSize = cur_pending_size;      // Keep track of size
   }
   else if ( mPendingCheckCount < pending_repeat_limit )
   {
      mPendingCheckCount++;
      ret_val = true;
   }
   else if( mPendingCheckCount == pending_repeat_limit )
   {
      mPendingCheckCount++;
      
      vprDEBUG(jcclDBG_RECONFIG, vprDBG_CRITICAL_LVL)
         << "Pending list is now "
         << clrOutNORM(clrGREEN, "STALE") << ":\n" << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(jcclDBG_RECONFIG, vprDBG_CRITICAL_LVL)
         << cur_pending_size << " items are still in the pending list\n"
         << vprDEBUG_FLUSH;

      if ( cur_pending_size > 0 )
      {
         vprDEBUG_NEXT(jcclDBG_RECONFIG, vprDBG_CRITICAL_LVL)
            << "NOTE: These items have been specified in the\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(jcclDBG_RECONFIG, vprDBG_CRITICAL_LVL)
            << "      configuration but have not been loaded.\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(jcclDBG_RECONFIG, vprDBG_CRITICAL_LVL)
            << "      This may be a problem in the configuration OR\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(jcclDBG_RECONFIG, vprDBG_CRITICAL_LVL)
            << "      it may be waiting for more configuration\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(jcclDBG_RECONFIG, vprDBG_CRITICAL_LVL)
            << "      information.\n" << vprDEBUG_FLUSH;
      }

      lockPending();
      debugDumpPending(vprDBG_CRITICAL_LVL); // Output the stale pending list
      unlockPending();

      ret_val = false;
   }
   else
   {
      ret_val = false;
   }

   mPendingCountMutex.release();

   return ret_val;
}

void ConfigManager::mergeIncomingToPending()
{
   lockPending();
   {
      vpr::Guard<vpr::Mutex> g(mIncomingLock);

      for ( std::list<PendingElement>::iterator i = mIncomingConfig.begin();
            i != mIncomingConfig.end();
            ++i )
      {
         mPendingConfig.push_back(*i);
      }

      mIncomingConfig.clear();
   }
   unlockPending();
}

bool ConfigManager::isPendingStale()
{
   mPendingCountMutex.acquire();
   bool return_value = (mPendingCheckCount >= pending_repeat_limit);
   mPendingCountMutex.release();
   return return_value;
}

void ConfigManager::addPending(PendingElement& pendingElement)
{
   // Assert that the pending list is locked prior to calling this method.
   vprASSERT(1 == mPendingLock.test());

   mPendingConfig.push_back(pendingElement);

   refreshPendingList();
}

// Look for items in the active list that don't have their dependencies filled
// anymore.
//
// POST: Any elements in active with dependencies not filled are added to the
//       the pending list. (A remove and an add are added to the pending).
// RETURNS: The number of lost dependencies found.
int ConfigManager::scanForLostDependencies()
{
   vprASSERT(0 == mActiveLock.test());
   // We can't hold the lock upon entry

   vpr::DebugOutputGuard og(vprDBG_ALL, vprDBG_CONFIG_LVL,
                            "ConfigManager::scanForLostDependencies()\n",
                            "ConfigManager::scanForLostDependencies() done.\n");

   DependencyManager* dep_mgr = DependencyManager::instance();
   std::vector<ConfigElementPtr> elements;
   int num_lost_deps(0);

   // NOTE: can't hold this lock because the isSatisfied routines make
   // use of the activeLock as well
   // NOTE: Make the copy of the elements so that we can iterate without
   // fear of active changing
   mActiveLock.acquire();
   elements = mActiveConfig.vec();   // Get a copy of the elements
   mActiveLock.release();

   // Now test them
   for ( unsigned int i=0;i<elements.size();i++ )
   {
      if ( !dep_mgr->isSatisfied(elements[i]) )     // We are not satisfied
      {
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_WARNING_LVL)
            << elements[i]->getName()
            << " type: " << elements[i]->getID()
            << " has lost dependencies.\n" << vprDEBUG_FLUSH;

         num_lost_deps++;              // Keep a count of the number lost deps found

         // Add the pending removal
         PendingElement pending;
         pending.mType = PendingElement::REMOVE;
         pending.mElement = elements[i];
         
         vprASSERT(1 == mPendingLock.test());
         addPending(pending);

         // Add the pending re-addition
//         ConfigElementPtr copy_of_element;          // Need a copy so that the remove can delete the element
//         copy_of_element = new ConfigElement(*elements[i]);
         pending.mType = PendingElement::ADD;
         pending.mElement = elements[i];//copy_of_element;
         
         vprASSERT(1 == mPendingLock.test());
         addPending(pending);                   // Add the add item
      }
   }

   return num_lost_deps;
}

void ConfigManager::debugDumpPending(int debug_level)
{
   vprASSERT(1 == mPendingLock.test());
   vprDEBUG(vprDBG_ALL,debug_level)
      << clrSetNORM(clrGREEN)
      << "---- Pending list: " << mPendingConfig.size() << " items ----\n"
      << clrRESET << vprDEBUG_FLUSH;
   std::list<ConfigManager::PendingElement>::iterator current, end;
   current = getPendingBegin();
   end = getPendingEnd();

   while ( current != end )
   {
      ConfigElementPtr cur_element = (*current).mElement;

      if ( (*current).mType == PendingElement::ADD )
      {
         vprDEBUG_NEXT(vprDBG_ALL,debug_level) << "   ADD -->" << vprDEBUG_FLUSH;
      }
      else if ( (*current).mType == PendingElement::REMOVE )
      {
         vprDEBUG_NEXT(vprDBG_ALL,debug_level) << "REMOVE -->" << vprDEBUG_FLUSH;
      }

      vprDEBUG_CONT(vprDBG_ALL,debug_level)
         << cur_element->getName() << " type: " << cur_element->getID()
         << std::endl << vprDEBUG_FLUSH;
      current++;
   }
   vprDEBUG_NEXT(vprDBG_ALL,debug_level)
      << "----------------------------------\n" << vprDEBUG_FLUSH;
}


//------------------ Active List Stuff -------------------------------

jccl::ConfigElementPtr ConfigManager::getElementFromActive(const std::string& elementName)
{
   jccl::ConfigElementPtr elt;

   vpr::Guard<vpr::Mutex> guard(mActiveLock);     // Lock the list

   std::vector<ConfigElementPtr>::iterator i;
   for ( i = mActiveConfig.vec().begin(); i != mActiveConfig.vec().end(); ++i )
   {
      if ( (*i)->getName() == elementName )
      {
         elt = *i;
         break;
      }
   }

   return elt;
}

jccl::ConfigElementPtr ConfigManager::getElementFromPending(const std::string& elementName)
{
   jccl::ConfigElementPtr elt;

   vpr::Guard<vpr::Mutex> guard(mPendingLock);     // Lock the list

   std::list<PendingElement>::iterator i;
   for ( i = mPendingConfig.begin(); i != mPendingConfig.end(); ++i )
   {
      if ( (*i).mElement->getName() == elementName )
      {
         elt = (*i).mElement;
      }
   }

   return elt;
}

jccl::ConfigElementPtr ConfigManager::getElementNamed(const std::string& elementName)
{
   jccl::ConfigElementPtr elt = getElementFromActive(elementName);

   // If the element is not in the active list, check the pending list.
   if ( elt.get() == NULL )
   {
      elt = getElementFromPending(elementName);
   }

   return elt;
}

// Is the element in the active configuration??
// CONCURRENCY: concurrent
// NOTE: This locks the active list to do processing.
bool ConfigManager::isElementInActiveList(const std::string& element_name)
{
   vpr::Guard<vpr::Mutex> guard(mActiveLock);     // Lock the current list

   std::vector<ConfigElementPtr>::iterator i;
   for ( i = mActiveConfig.vec().begin(); i != mActiveConfig.vec().end(); ++i )
   {
      if ( std::string((*i)->getName()) == element_name )
      {
         return true;
      }
   }

   return false;     // Not found, so return false
}

// Is there an element of this type in the active configuration?
// CONCURRENCY: concurrent
// NOTE: This locks the active list to do processing.
bool ConfigManager::isElementTypeInActiveList(const std::string& elementType)
{
   vpr::Guard<vpr::Mutex> guard(mActiveLock);     // Lock the current list

   std::vector<ConfigElementPtr>::iterator i;
   for ( i = mActiveConfig.vec().begin(); i != mActiveConfig.vec().end(); ++i )
   {
      if ( std::string((*i)->getID()) == elementType )
      {
         return true;
      }
   }
   return false;     // Not found, so return false
}

// Is there an element of this type in the pending list?
// CONCURRENCY: concurrent
// NOTE: This locks the pending list to do processing.
bool ConfigManager::isElementTypeInPendingList(const std::string& elementType)
{
   vpr::Guard<vpr::Mutex> guard(mPendingLock);     // Lock the current list

   std::list<PendingElement>::iterator i;
   for ( i = mPendingConfig.begin(); i != mPendingConfig.end(); ++i )
   {
      if ( std::string((*i).mElement->getID()) == elementType )
      {
         return true;
      }
   }
   return false;     // Not found, so return false
}

bool ConfigManager::hasElementType(const std::string& elementType)
{
   return isElementTypeInActiveList(elementType) ||
          isElementTypeInPendingList(elementType);
}

// Add an item to the active configuration
// NOTE: This DOES NOT process the element it just places it into the active
//       configuration list.
// PRE: Current list must NOT be locked.
void ConfigManager::addActive(ConfigElementPtr element)
{
   vprASSERT(0 == mActiveLock.test());
   lockActive();
   mActiveConfig.vec().push_back(element);
   unlockActive();
}

// Erase an item from the list.
// PRE: Active list must be locked && item must be in list.
// POST: list = old(list).erase(item) && item is invalid.
void ConfigManager::removeActive(const std::string& elementName)
{
   vprASSERT(0 == mActiveLock.test());
   lockActive();
   mActiveConfig.remove(elementName);
   unlockActive();
}

void ConfigManager::addConfigurationAdditions(jccl::Configuration* cfg)
{
   vpr::Guard<vpr::Mutex> g(mIncomingLock);

   PendingElement pending;
   pending.mType = PendingElement::ADD;

   for ( std::vector<ConfigElementPtr>::iterator i = cfg->vec().begin();
         i != cfg->vec().end();
         ++i )
   {
      pending.mElement = (*i);
      mIncomingConfig.push_back(pending);
   }
}

void ConfigManager::addConfigurationRemovals(jccl::Configuration* cfg)
{
   vpr::Guard<vpr::Mutex> g(mIncomingLock);

   PendingElement pending;
   pending.mType = PendingElement::REMOVE;

   for ( std::vector<ConfigElementPtr>::iterator i = cfg->vec().begin();
         i != cfg->vec().end();
         ++i )
   {
      pending.mElement = (*i);
      mIncomingConfig.push_back(pending);
   }
}

void ConfigManager::addConfigElement(jccl::ConfigElementPtr elt,
                                     ConfigManager::PendingElement::Type t)
{
   PendingElement pending;
   pending.mType = t;
   pending.mElement = elt;
   mIncomingConfig.push_back(pending);
}

std::list<ConfigManager::PendingElement>::size_type ConfigManager::getNumIncoming()
{
   vpr::Guard<vpr::Mutex> g(mIncomingLock);
   return mIncomingConfig.size();
}

//------------------ DynamicReconfig Stuff ------------------------------

void ConfigManager::addConfigElementHandler(ConfigElementHandler* h)
{
   mElementHandlers.push_back(h);
}

void ConfigManager::removeConfigElementHandler(ConfigElementHandler* h)
{
   std::vector<ConfigElementHandler*>::iterator it;
   for ( it = mElementHandlers.begin(); it != mElementHandlers.end(); it++ )
   {
      if ( *it == h )
      {
         mElementHandlers.erase(it);
         break;
      }
   }
}

void ConfigManager::shutdown(bool attemptRemoval)
{
   // Attempt to remove configutation elements
   // the "correct" way.
   if (attemptRemoval)
   {
      mActiveLock.acquire();
      addConfigurationRemovals(&mActiveConfig);
      mActiveLock.release();

      unsigned num_processed(0);

      do
      {
         num_processed = attemptReconfiguration();
      }
      while (num_processed > 0);
   }

   // Clear all pending configuration elements.
   mPendingLock.acquire();
   mPendingConfig.clear();
   mPendingLock.release();

   // Clear all active configuration elements.
   mActiveLock.acquire();
   mActiveConfig.vec().clear();
   mActiveConfig = Configuration();
   mActiveLock.release();

   // Clear all incoming configuration elements.
   mIncomingLock.acquire();
   mIncomingConfig.clear();
   mIncomingLock.release();

   // Clear all element handlers.
   mElementHandlers.clear();

   // Reset all count values.
   mPendingCountMutex.acquire();
   mPendingCheckCount = 0;
   mLastPendingSize = 0;
   mPendingCountMutex.release();
}

int ConfigManager::attemptReconfiguration()
{
   int elements_processed(0);     // Needs to return this value

   // Copy the contents of the incoming config element list to the pending
   // list.  Config element handlers may have added new elements to the
   // incoming list.
   mergeIncomingToPending();

   if ( shouldCheckPending() )
   {
      vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_STATE_LVL,
      std::string("ConfigManager::attemptReconfiguration: Examining pending list.\n"),
      std::string("ConfigManager::attemptReconfiguration: Done Examining pending list.\n"));

      lockPending();

      for (unsigned int i = 0 ; i < mElementHandlers.size() ; i++)
      {
         elements_processed += mElementHandlers[i]->configProcessPending();
      }

      unlockPending();
   }

/*
   if ( elements_processed > 0 )
   {
      mConfigCommunicator->configChanged();
   }
*/

   return elements_processed;
}

} // namespace jccl
