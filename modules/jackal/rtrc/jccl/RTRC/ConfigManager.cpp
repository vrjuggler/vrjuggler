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

#include <jccl/jcclConfig.h>

#include <vpr/IO/Socket/InetAddr.h>

#include <jccl/RTRC/ConfigManager.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ChunkFactory.h>
#include <jccl/RTRC/DependencyManager.h>
#include <jccl/RTRC/ConfigChunkHandler.h>
/*
#include <jccl/Net/JackalServer.h>
#include <jccl/Net/Connect.h>
*/
#include <jccl/Util/Debug.h>



namespace jccl
{

vprSingletonImp(ConfigManager);


ConfigManager::ConfigManager()
{
   mPendingCheckCount = 0;
   mLastPendingSize = 0;
//   mConfigCommunicator = new XMLConfigCommunicator(this);
}

ConfigManager::~ConfigManager()
{
   ;
}

//-------------------- Pending List Stuff -------------------------------

// Add the given chunk db to the pending list as adds
void ConfigManager::addPendingAdds(ConfigChunkDB* db)
{
   vprASSERT(0 == mPendingLock.test());     // ASSERT: Make sure we don't already have it
   lockPending();

   PendingChunk pending;
   pending.mType = PendingChunk::ADD;

   // Begin Machine Specific Code
   if(mCachedLocalHostName.empty())
   {
      // Get Local Host Name
      vpr::InetAddr local_addr;
      vpr::InetAddr::getLocalHost(local_addr);
      std::string host_name = local_addr.getHostname();
      if ( host_name.find('.') != std::string::npos )
      {
         host_name = host_name.substr(0, host_name.find('.'));
      }
      if ( host_name.find(':') != std::string::npos )
      {
         host_name = host_name.substr(0, host_name.find('.'));
      }
      mCachedLocalHostName = host_name;
   }
   // End Machine Specific Code

   for ( std::vector<ConfigChunkPtr>::iterator i = db->vec().begin();
         i != db->vec().end();
         ++i )
   {
      pending.mChunk = (*i);
      mPendingConfig.push_back(pending);

      // Begin Machine Specific Code
      if ( (*i)->getDescToken() == std::string("MachineSpecific") )
      {
         if ( (*i)->getProperty<std::string>("host_name") == mCachedLocalHostName ||
              (*i)->getProperty<std::string>("host_name") == "localhost")
         {
            // NOTE: Add all machine dependent ConfigChunkPtr's here
            vprASSERT((*i)->getNum("display_system") == 1 && "A Cluster System Chunk must have exactly 1 display_system chunk");

            std::vector<jccl::ConfigChunkPtr> machine_specific_chunks = (*i)->getEmbeddedChunks();

            for (std::vector<jccl::ConfigChunkPtr>::iterator i= machine_specific_chunks.begin();
                 i != machine_specific_chunks.end();
                 i++)
            {
               PendingChunk pending;
               pending.mType = PendingChunk::ADD;
               pending.mChunk = (*i);
               mPendingConfig.push_back(pending);
               vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << clrSetBOLD(clrCYAN)
                  << "[ConfigManager] Adding Machine specific ConfigChunk: "
                  << (*i)->getName() << clrRESET << std::endl << vprDEBUG_FLUSH;
            }
         }
      } // End Machine Specific Code
   }

   unlockPending();

   refreshPendingList();
}

void ConfigManager::addPendingRemoves(ConfigChunkDB* db)
{
   vprASSERT(0 == mPendingLock.test());     // ASSERT: Make sure we don't already have it
   lockPending();

   PendingChunk pending;
   pending.mType = PendingChunk::REMOVE;

   for ( std::vector<ConfigChunkPtr>::iterator i = db->vec().begin();
         i != db->vec().end();
         ++i )
   {
      pending.mChunk = (*i);
      mPendingConfig.push_back(pending);
   }

   unlockPending();

   refreshPendingList();
}

void ConfigManager::removePending(std::list<PendingChunk>::iterator item)
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
   
//: Do we need to check the pending list
//! CONCURRENCY: concurrent
// The routine counts the number of pending chunks
// each time it is called.
// if it goes pending_repeat_limit calls without
// changing size, then it returns false until mLastPendingSize changes
bool ConfigManager::pendingNeedsChecked()
{
   int cur_pending_size = 0;
   bool ret_val = false;

   mPendingCountMutex.acquire();

   cur_pending_size = mPendingConfig.size();
   if ( cur_pending_size != mLastPendingSize )
   {
      ret_val = true;                           // Flag it for a check
      mPendingCheckCount = 0;                     // Reset the counter
      mLastPendingSize = cur_pending_size;      // Keep track of size
   }
   else if ( mPendingCheckCount < pending_repeat_limit )
   {
      // allowed in at least once [1...pending_repeat_limit]
      mPendingCheckCount++;   // Increment it

      if ( mPendingCheckCount < pending_repeat_limit )
      {
         ret_val = true;        // Repeats still allowed
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "ConfigManager::pendingNeedsChecked: Pending list is now\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrGREEN,"STALE: ")
            << cur_pending_size << " items still in pending\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "NOTE: These items have been specified in configuration,\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "      but have not been loaded.\n" << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "      This may be a problem in the configuration OR\n"
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "      it may be waiting for more configuration information.\n"
            << vprDEBUG_FLUSH;
         //vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << vprDEBUG_FLUSH;

         lockPending();
         debugDumpPending(vprDBG_CRITICAL_LVL); // Output the stale pending list
         unlockPending();

         ret_val = false;
      }
   }
   else
   {
      ret_val = false;
   }

   mPendingCountMutex.release();

   return ret_val;
}

bool ConfigManager::isPendingStale()
{
   mPendingCountMutex.acquire();
   bool return_value = (mPendingCheckCount >= pending_repeat_limit);
   mPendingCountMutex.release();
   return return_value;
}

void ConfigManager::addPending(PendingChunk& pendingChunk)
{
   vprASSERT(0 == mPendingLock.test());
   lockPending();
   mPendingConfig.push_back(pendingChunk);
   unlockPending();

   refreshPendingList();
}

// Look for items in the active list that don't have their dependencies filled anymore
//
//! POST: Any chunks in active with dependencies not filled are added to the
//+       the pending list. (A remove and an add are added to the pending)
//! RETURNS: The number of lost dependencies found
int ConfigManager::scanForLostDependencies()
{
   vprASSERT(0 == mActiveLock.test());
   // We can't hold the lock upon entry

   vprDEBUG_BEGIN(vprDBG_ALL,vprDBG_CONFIG_LVL)
      << "ConfigManager::scanForLostDependencies: Entered: \n"
      << vprDEBUG_FLUSH;

   DependencyManager* dep_mgr = DependencyManager::instance();
   std::vector<ConfigChunkPtr> chunks;
   int num_lost_deps(0);

   // NOTE: can't hold this lock because the isSatisfied routines make
   // use of the activeLock as well
   // NOTE: Make the copy of the chunks so that we can iterate without
   // fear of active changing
   mActiveLock.acquire();
   chunks = mActiveConfig.vec();   // Get a copy of the chunks
   mActiveLock.release();

   // Now test them
   for ( unsigned int i=0;i<chunks.size();i++ )
   {
      if ( !dep_mgr->isSatisfied(chunks[i]) )     // We are not satisfied
      {
         vprDEBUG_NEXT(vprDBG_ALL,1)
            << chunks[i]->getProperty<std::string>("name")
            << " type: " << chunks[i]->getDescToken()
            << " has lost dependencies.\n" << vprDEBUG_FLUSH;

         num_lost_deps++;              // Keep a count of the number lost deps found

         // Add the pending removal
         PendingChunk pending;
         pending.mType = PendingChunk::REMOVE;
         pending.mChunk = chunks[i];
         addPending(pending);

         // Add the pending re-addition
//         ConfigChunkPtr copy_of_chunk;          // Need a copy so that the remove can delete the chunk
//         copy_of_chunk = new ConfigChunk(*chunks[i]);
         pending.mType = PendingChunk::ADD;
         pending.mChunk = chunks[i];//copy_of_chunk;
         addPending(pending);                   // Add the add item
      }
   }

   vprDEBUG_END(vprDBG_ALL,1)
      << "ConfigManager::scanForLostDependencies: Exiting: \n"
      << vprDEBUG_FLUSH;

   return num_lost_deps;
}

void ConfigManager::debugDumpPending(int debug_level)
{
   vprASSERT(1 == mPendingLock.test());
   vprDEBUG(vprDBG_ALL,debug_level)
      << clrSetNORM(clrGREEN)
      << "---- Pending list: " << mPendingConfig.size() << " items ----\n"
      << clrRESET << vprDEBUG_FLUSH;
   std::list<ConfigManager::PendingChunk>::iterator current, end;
   current = getPendingBegin();
   end = getPendingEnd();

   while ( current != end )
   {
      ConfigChunkPtr cur_chunk = (*current).mChunk;

      if ( (*current).mType == PendingChunk::ADD )
      {
         vprDEBUG_NEXT(vprDBG_ALL,debug_level) << "   ADD -->" << vprDEBUG_FLUSH;
      }
      else if ( (*current).mType == PendingChunk::REMOVE )
      {
         vprDEBUG_NEXT(vprDBG_ALL,debug_level) << "REMOVE -->" << vprDEBUG_FLUSH;
      }

      vprDEBUG_CONT(vprDBG_ALL,debug_level)
         << cur_chunk->getName() << " type: " << cur_chunk->getDescToken()
         << std::endl << vprDEBUG_FLUSH;
      current++;
   }
   vprDEBUG_NEXT(vprDBG_ALL,debug_level)
      << "----------------------------------\n" << vprDEBUG_FLUSH;
}


//------------------ Active List Stuff -------------------------------

//: Is the chunk in the active configuration??
//! CONCURRENCY: concurrent
//! NOTE: This locks the active list to do processing
bool ConfigManager::isChunkInActiveList(std::string chunk_name)
{
   vpr::Guard<vpr::Mutex> guard(mActiveLock);     // Lock the current list

   std::vector<ConfigChunkPtr>::iterator i;
   for ( i = mActiveConfig.vec().begin(); i != mActiveConfig.vec().end(); ++i )
   {
      if ( std::string((*i)->getName()) == chunk_name )
      {
         return true;
      }
   }

   return false;     // Not found, so return false
}

//: Is there a chunk of this type in the active configuration?
//! CONCURRENCY: concurrent
//! NOTE: This locks the active list to do processing
bool ConfigManager::isChunkTypeInActiveList(std::string chunk_type)
{
   vpr::Guard<vpr::Mutex> guard(mActiveLock);     // Lock the current list

   // std::cout << "isChunkTypeInActiveList ActiveConfig.getChunks().size == " << mActiveConfig.getChunks().size() << std::endl;

   std::vector<ConfigChunkPtr>::iterator i;
   for ( i = mActiveConfig.vec().begin(); i != mActiveConfig.vec().end(); ++i )
   {
      // std::cout << "trying to match " << std::string((*i)->getType()) << " with " << chunk_type << std::endl;
      if ( std::string((*i)->getDescToken()) == chunk_type )
      {
         // std::cout << "match!" << std::endl;
         return true;
      }
   }
   // std::cout << "no match!" << std::endl;
   return false;     // Not found, so return false
}

//: Is there a chunk of this type in the pending list??
//! CONCURRENCY: concurrent
//! NOTE: This locks the pending list to do processing
bool ConfigManager::isChunkTypeInPendingList(std::string chunk_type)
{
   vpr::Guard<vpr::Mutex> guard(mActiveLock);     // Lock the current list

   // std::cout << "isChunkTypeInPendingList(): mPendingConfig.size == " << mPendingConfig.size() << std::endl;

   std::list<PendingChunk>::iterator i;
   for ( i = mPendingConfig.begin(); i != mPendingConfig.end(); ++i )
   {
      // std::cout << "trying to match " << std::string((*i).mChunk->getType()) << " with " << chunk_type << std::endl;
      if ( std::string((*i).mChunk->getDescToken()) == chunk_type )
      {
         // std::cout << "match!" << std::endl;
         return true;
      }
   }
   // std::cout << "no match!" << std::endl;
   return false;     // Not found, so return false
}

//: Add an item to the active configuration
//! NOTE: This DOES NOT process the chunk
//+     it just places it into the active configuration list
//! PRE: Current list must NOT be locked
void ConfigManager::addActive(ConfigChunkPtr chunk)
{
   vprASSERT(0 == mActiveLock.test());
   lockActive();
   mActiveConfig.vec().push_back(chunk);
   unlockActive();
}

//: Erase an item from the list
//! PRE: Active list must be locked && item must be in list
//! POST: list = old(list).erase(item) && item is invalid
void ConfigManager::removeActive(const std::string& chunk_name)
{
   vprASSERT(0 == mActiveLock.test());
   lockActive();
   mActiveConfig.remove(chunk_name);
   unlockActive();
}

//------------------ DynamicReconfig Stuff ------------------------------

void ConfigManager::addConfigChunkHandler(ConfigChunkHandler* h)
{
   mChunkHandlers.push_back(h);
}

void ConfigManager::removeConfigChunkHandler(ConfigChunkHandler* h)
{
   std::vector<ConfigChunkHandler*>::iterator it;
   for ( it = mChunkHandlers.begin(); it != mChunkHandlers.end(); it++ )
   {
      if ( *it == h )
      {
         mChunkHandlers.erase(it);
         break;
      }
   }
}

int ConfigManager::attemptReconfiguration()
{
   int chunks_processed(0);     // Needs to return this value
   if ( pendingNeedsChecked() )
   {
      vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_STATE_LVL,
      std::string("ConfigManager::attemptReconfiguration: Examining pending list.\n"),
      std::string("ConfigManager::attemptReconfiguration: Done Examining pending list.\n"));
      
      lockPending();

      for (unsigned int i = 0 ; i < mChunkHandlers.size() ; i++)
      {
         mChunkHandlers[i]->configProcessPending();
      }

      unlockPending();
   }

/*
   if ( chunks_processed > 0 )
   {
      mConfigCommunicator->configChanged();
   }
*/

   return chunks_processed;
}

enum PendItemResult
{
   SUCCESS, FAILED, NEED_DEPS
};

/*
//------------------ JackalControl Stuff --------------------------------

void ConfigManager::addConnect(Connect *c)
{
   c->addCommunicator(mConfigCommunicator);
   //addActive(new ConfigChunk(*(c->getConfiguration())));
   addActive(c->getConfiguration());
}

void ConfigManager::removeConnect(Connect* c)
{
   removeActive(c->getConfiguration()->getName());
}
*/

} // namespace jccl
