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
#include <Config/vjConfigChunk.h>
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

   vjConfigManager()
   {;}

public: // -- Query functions --- //
   //: Is the chunk in the active configuration??
   //! CONCURRENCY: concurrent
   //! NOTE: This locks the active list to do processing
   bool isChunkInActiveList(std::string chunk_name)
   {
   vjGuard<vjMutex> guard(mActiveLock);     // Lock the current list   
      
      std::list<vjConfigChunk*>::iterator i;
      for(i=mActiveConfig.begin(); i != mActiveConfig.end();i++)
      {
         if(std::string((*i)->getProperty("name")) == chunk_name)
            return true;
      }

      return false;     // Not found, so return false
   }

public:   // ----- PENDING LIST ----- //
   //: Are there items pending
   //! CONCURRENCY: concurrent
   bool isPendingEmpty()
   { return mPendingConfig.empty(); }

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

public:   // ----- ACTIVE LIST ----- //
   //: Are there items in current   //! CONCURRENCY: concurrent
   bool isActiveEmpty()
   { return mActiveConfig.empty(); }

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
   std::list<vjConfigChunk*>::iterator getActiveBegin()
   {
      vjASSERT(1 == mActiveLock.test());     // ASSERT: We must have the lock
      return mActiveConfig.begin();
   }

   //: Get the end of the pending list
   //! PRE: Active list must be locked
   std::list<vjConfigChunk*>::iterator getActiveEnd()
   {
      vjASSERT(1 == mActiveLock.test());
      return mActiveConfig.end();
   }

   //: Erase an item from the list
   //! PRE: Active list must be locked && item must be in list
   //! POST: list = old(list).erase(item) && item is invalid
   void removeActive(std::list<vjConfigChunk*>::iterator item)
   {
      vjASSERT(1 == mActiveLock.test());
      mActiveConfig.erase(item);
   }

   //: Add an item to the active configuration
   //! NOTE: This DOES NOT process the chunk
   //+     it just places it into the active configuration list
   //! PRE: Current list must be locked
   void addActive(vjConfigChunk* chunk)
   {
      vjASSERT(1 == mActiveLock.test());
      mActiveConfig.push_back(chunk);
   }
   

private:
   std::list<vjConfigChunk*>  mActiveConfig;   //: List of current configuration
   std::list<vjPendingChunk>  mPendingConfig;   //: List of pending configuration changes
   vjMutex                    mPendingLock;     //: Lock on pending list
   vjMutex                    mActiveLock;     //: Lock for current config list

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

