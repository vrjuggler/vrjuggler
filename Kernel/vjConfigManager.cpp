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
void scanForLostDependencies()
{

}


