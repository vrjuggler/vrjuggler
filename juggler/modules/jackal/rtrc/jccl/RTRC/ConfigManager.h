/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_CONFIG_MANGER_H_
#define _VJ_CONFIG_MANGER_H_

#include <vjConfig.h>
#include <Kernel/vjDebug.h>
//#include <Config/vjConfigChunk.h>
class vjConfigChunk;

#include <Config/vjConfigChunkDB.h>
#include <Config/vjChunkDescDB.h>
//#include <Config/vjChunkFactory.h>
#include <Sync/vjMutex.h>
#include <Sync/vjGuard.h>
#include <list>

//: Configuration manager class
//
//
//
//! Created: Jan-13-2000
//! Author: Allen Bierbaum
//
class vjConfigManager
{
public:
   struct vjPendingChunk
   {
      enum { ADD=0, REMOVE=1};
      int            mType;           // What type of chunk is it (ADD or REMOVE)
      vjConfigChunk* mChunk;
   };

   
public: // -- Query functions --- //
   //: Is the chunk in the active configuration??
   //! CONCURRENCY: concurrent
   //! NOTE: This locks the active list to do processing
   bool isChunkInActiveList(std::string chunk_name)
   {
   vjGuard<vjMutex> guard(mActiveLock);     // Lock the current list   
      
      std::vector<vjConfigChunk*>::iterator i;
      for(i=mActiveConfig.begin(); i != mActiveConfig.end();i++)
      {
         if(std::string((*i)->getProperty("name")) == chunk_name)
            return true;
      }

      return false;     // Not found, so return false
   }

   // Add the given chunk db to the pending list as adds
   //! PRE: The pending list can NOT be locked
   //! POST: pendinglist = old(pendinglist) += db
   //! NOTE: The entries are copied
   void addChunkDB(vjConfigChunkDB* db);

   //: Add the given chunks to the db as pending removes
   //! PRE: The pending list can NOT be locked
   //! POST: pendinglist = old(pendinglist) += db
   //! NOTE: The entries are copied
   void removeChunkDB(vjConfigChunkDB* db);


public:   // ----- PENDING LIST ----- //
   //: Do we need to check the pending list
   //! CONCURRENCY: concurrent
   // Implements some logic that allows the pending list to become "stale"
   // If the pending list has been check a bunch of times and has had no
   // changes in size, then we start telling people not to check it because
   bool pendingNeedsChecked()
   { 
      const int pending_repeat_limit(4);
      int cur_pending_size(0);
      bool ret_val(false);

      mPendingCountMutex.acquire();
      {
         cur_pending_size = mPendingConfig.size();
         if(cur_pending_size != mLastPendingSize)
         {
            ret_val = true;                           // Flag it for a check
            mPendingCheckCount=0;                     // Reset the counter
            mLastPendingSize = cur_pending_size;      // Keep track of size
         }
         else if(mPendingCheckCount < pending_repeat_limit)
         {
            ret_val = true;         // Less than count, so do a check
            mPendingCheckCount++;   // Increment it
            if(mPendingCheckCount == pending_repeat_limit)
            {
               vjDEBUG_BEGIN(vjDBG_ALL,0) << "vjConfigManager::pendingNeedsChecked: Pending list is now STALE."
                                          << cur_pending_size << " items still in pending\n" << vjDEBUG_FLUSH;
               lockPending();
               debugDumpPending();     // Output the stale pending list
               unlockPending();              
            }
         }
         else
         {
            ret_val = false;
         }         
      }
      mPendingCountMutex.release();

      return ret_val;
   }

   int getNumPending()
   { return mPendingConfig.size(); }

   //: Add a pending entry
   //! PRE: pending must NOT be locked
   //! POST: A copy of the pendingChunk is placed on the pending list
   //! concurrency: gaurded
   void addPending(vjPendingChunk& pendingChunk)
   {
      vjASSERT(0 == mPendingLock.test());
      lockPending();
      mPendingConfig.push_back(pendingChunk);
      unlockPending();
      
      // Reset pending count
      mPendingCountMutex.acquire();
      mPendingCheckCount = 0;
      mPendingCountMutex.release();
   }


   //: Lock the pending list
   // This function blocks until it can get a lock on the pending list
   //! CONCURRENCY: gaurded
   void lockPending()
   { mPendingLock.acquire(); }

   //: Unlock the pending list
   // Unlocks the mutex held on the pending list
   //! CONCURRENCY: gaurded
   void unlockPending()
   { mPendingLock.release(); }

   //: Get the beginning of the pending list
   //! PRE: Pending list must be locked
   std::list<vjPendingChunk>::iterator getPendingBegin()
   {
      vjASSERT(1 == mPendingLock.test());     // ASSERT: We must have the lock
      return mPendingConfig.begin();
   }

   //: Get the end of the pending list
   //! PRE: Pending list must be locked
   std::list<vjPendingChunk>::iterator getPendingEnd()
   {
      vjASSERT(1 == mPendingLock.test());
      return mPendingConfig.end();
   }

   //: Erase an item from the list
   //! PRE: Pending list must be locked && item must be in list
   //! POST: list = old(list).erase(item) && item is invalid
   void removePending(std::list<vjPendingChunk>::iterator item)
   {
      vjASSERT(1 == mPendingLock.test());
      mPendingConfig.erase(item);
   }

   //: Send a copy of the pending list to debug output
   //! PRE: Pending must be locked
   void debugDumpPending()
   {
      vjASSERT(1 == mPendingLock.test());
      vjDEBUG_BEGIN(vjDBG_ALL,0) << "---- Debug Dump of Pending list: " << mPendingConfig.size() << " items in list\n" << vjDEBUG_FLUSH;
      std::list<vjConfigManager::vjPendingChunk>::iterator current, end;
      current = getPendingBegin();
      end = getPendingEnd();

      while(current != end)
      {
         vjConfigChunk* cur_chunk = (*current).mChunk;

         vjDEBUGlg(vjDBG_ALL,0,false,true) << cur_chunk->getProperty("name")
                                           << " type: " << (std::string)cur_chunk->getType() << endl << vjDEBUG_FLUSH;
         current++;
      }
      vjDEBUG_ENDlg(vjDBG_ALL,0,false,true) << "-------------------------------\n" << vjDEBUG_FLUSH      ;
   }

public:   // ----- ACTIVE LIST ----- //
   //: Are there items in current   //! CONCURRENCY: concurrent
   bool isActiveEmpty()
   { return mActiveConfig.isEmpty(); }

   //: Lock the current list
   // This function blocks until it can get a lock on the current list
   //! CONCURRENCY: gaurded
   void lockActive()
   { mActiveLock.acquire(); }

   //: Unlock the current list
   // Unlocks the mutex held on the current list
   //! CONCURRENCY: gaurded
   void unlockActive()
   { mActiveLock.release(); }

   //: Get the beginning of the current list
   //! PRE: Pending list must be locked
   std::vector<vjConfigChunk*>::iterator getActiveBegin()
   {
      vjASSERT(1 == mActiveLock.test());     // ASSERT: We must have the lock
      return mActiveConfig.begin();
   }

   //: Get the end of the pending list
   //! PRE: Active list must be locked
   std::vector<vjConfigChunk*>::iterator getActiveEnd()
   {
      vjASSERT(1 == mActiveLock.test());
      return mActiveConfig.end();
   }

   //: Erase an item from the list
   //! PRE: Active list must be locked && item must be in list
   //! POST: list = old(list).erase(item) && item is invalid
   void removeActive(std::string chunk_name)
   {
      vjASSERT(1 == mActiveLock.test());
      lockActive();
      mActiveConfig.removeNamed(chunk_name);
      unlockActive();
   }

   //: Add an item to the active configuration
   //! NOTE: This DOES NOT process the chunk
   //+     it just places it into the active configuration list
   //! PRE: Current list must NOT be locked
   void addActive(vjConfigChunk* chunk)
   {
      vjASSERT(0 == mActiveLock.test());
      lockActive();
      mActiveConfig.addChunk(chunk);
      unlockActive();
   }

   //: Return ptr to the active config dhunk db
   //! PRE: active must be locked
   //! NOTE: The pointer is only valid until active is unlocked
   //! CONCURRENCY: sequential
   vjConfigChunkDB* getActiveConfig()
   {
      vjASSERT(1 == mActiveLock.test());
      return &mActiveConfig;
   }   

public:
   //: Scan the active list for items that don't have their dependencies filled
   //! POST: Any chunks in active with dependencies not filled are added to the 
   //+       the pending list. (A remove and an add are added to the pending)
   void scanForLostDependencies();


private:
   vjConfigChunkDB            mActiveConfig;   //: List of current configuration
   std::list<vjPendingChunk>  mPendingConfig;   //: List of pending configuration changes
   vjMutex                    mPendingLock;     //: Lock on pending list
   vjMutex                    mActiveLock;     //: Lock for current config list   
   
   // The following variables are used to implment some logic
   // that "stales" the pending list.   (see pendingNeedsChecked)
   vjMutex                    mPendingCountMutex;
   int                        mPendingCheckCount;  //: How many pending checks since last change to pending
   int                        mLastPendingSize;    //: The size of pending at last check
protected:
   vjConfigManager()
   {;}

   
public:
   //: Get instance of singleton object
   static vjConfigManager* instance()
   {
      if (_instance == NULL)
         _instance = new vjConfigManager;
      return _instance;
   }

private:
   static vjConfigManager* _instance;   //: The instance

};


#endif

