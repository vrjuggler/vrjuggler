#include <vjConfig.h>
#include <Kernel/vjConfigManager.h>
#include <Config/vjConfigChunk.h>
#include <Config/vjChunkFactory.h>
#include <Kernel/vjDependencyManager.h>
#include <Kernel/vjDebug.h>
#include <stdlib.h>

vjConfigManager* vjConfigManager::_instance = NULL;

// Add the given chunk db to the pending list as adds
void vjConfigManager::addChunkDB(vjConfigChunkDB* db)
{
   vjASSERT(0 == mPendingLock.test());     // ASSERT: Make sure we don't already have it
   lockPending();
   {
      vjPendingChunk pending;
      pending.mType = vjPendingChunk::ADD;
   
      for(std::vector<vjConfigChunk*>::iterator i=db->begin();i!=db->end();i++)
      {
         pending.mChunk = (*i);
         mPendingConfig.push_back(pending);
      }
   }
   unlockPending();

   // Reset pending count
   mPendingCountMutex.acquire();
   mPendingCheckCount = 0;
   mPendingCountMutex.release();

}

void vjConfigManager::removeChunkDB(vjConfigChunkDB* db)
{
   vjASSERT(0 == mPendingLock.test());     // ASSERT: Make sure we don't already have it
   lockPending();
   {
      vjPendingChunk pending;
      pending.mType = vjPendingChunk::REMOVE;
   
      for(std::vector<vjConfigChunk*>::iterator i=db->begin();i!=db->end();i++)
      {
         pending.mChunk = (*i);
         mPendingConfig.push_back(pending);
      }
   }
   unlockPending();

   // Reset pending count
   mPendingCountMutex.acquire();
   mPendingCheckCount = 0;
   mPendingCountMutex.release();

}

// Look for items in the active list that don't have their dependencies filled anymore
//
//! POST: Any chunks in active with dependencies not filled are added to the 
//+       the pending list. (A remove and an add are added to the pending)
void vjConfigManager::scanForLostDependencies()
{
   vjASSERT(0 == mActiveLock.test());

   vjDependencyManager* dep_mgr = vjDependencyManager::instance();

   std::vector<vjConfigChunk*> chunks;

   mActiveLock.acquire();
      chunks = mActiveConfig.getChunks();   // Get a copy of the chunks
   mActiveLock.release();

   // Now test them
   for(unsigned int i=0;i<chunks.size();i++)
   {
      if(!dep_mgr->depSatisfied(chunks[i]))      // We are not satisfied
      {
      }
   }
   
}


