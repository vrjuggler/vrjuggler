/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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


#ifndef _JCCL_CONFIG_MANGER_H_
#define _JCCL_CONFIG_MANGER_H_

#include <jccl/jcclConfig.h>
#include <jccl/JackalServer/JackalControl.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ChunkDescDB.h>
#include <jccl/ConfigManager/XMLConfigCommunicator.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Singleton.h>

#include <list>


namespace jccl {

    class ConfigChunkHandler;

//: Configuration manager class
//
//
//
//! Created: Jan-13-2000
//! Author: Allen Bierbaum
//
class ConfigManager: public JackalControl 
{
public:
   struct PendingChunk
   {
      PendingChunk() : mType(0), mChunk(NULL)
      {;}

      enum { ADD=0, REMOVE=1};
      unsigned mType;           // What type of chunk is it (ADD or REMOVE)
      ConfigChunk* mChunk;
   };


public: // -- Query functions --- //

    //: Is the chunk in the active configuration??
    //! CONCURRENCY: concurrent
    //! NOTE: This locks the active list to do processing
    bool isChunkInActiveList(std::string chunk_name);



public:   // ----- PENDING LIST ----- //


   //: Add a pending entry
   //! PRE: pending must NOT be locked
   //! POST: A copy of the pendingChunk is placed on the pending list
   //! concurrency: gaurded
   void addPending(PendingChunk& pendingChunk);


   // Add the given chunk db to the pending list as adds
   //! PRE: The pending list can NOT be locked
   //! POST: pendinglist = old(pendinglist) += db
   //! NOTE: The entries are copied
   void addPending (ConfigChunkDB* db);


   //: Add the given chunks to the db as pending removes
   //! PRE: The pending list can NOT be locked
   //! POST: pendinglist = old(pendinglist) += db
   //! NOTE: The entries are copied
   void removePending (ConfigChunkDB* db);


    //: Erase an item from the list
    //! PRE: Pending list must be locked && item must be in list
    //! POST: list = old(list).erase(item) && item is invalid
    void removePending(std::list<PendingChunk>::iterator item);


   //: Do we need to check the pending list
   //! CONCURRENCY: concurrent
   // Implements some logic that allows the pending list to become "stale"
   // If the pending list has been check a bunch of times and has had no
   // changes in size, then we start telling people not to check it because
   bool pendingNeedsChecked();


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
   std::list<PendingChunk>::iterator getPendingBegin()
   {
      vprASSERT(1 == mPendingLock.test());     // ASSERT: We must have the lock
      return mPendingConfig.begin();
   }


   //: Get the end of the pending list
   //! PRE: Pending list must be locked
   std::list<PendingChunk>::iterator getPendingEnd()
   {
      vprASSERT(1 == mPendingLock.test());
      return mPendingConfig.end();
   }


   //: Send a copy of the pending list to debug output
   //! PRE: Pending must be locked
   void debugDumpPending(int debug_level);


   //: Get the size of the pending list
   //! CONCURRENCY: concurrent
   int getNumPending()
   { return mPendingConfig.size(); }



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
   std::vector<ConfigChunk*>::iterator getActiveBegin()
   {
      vprASSERT(1 == mActiveLock.test());     // ASSERT: We must have the lock
      return mActiveConfig.begin();
   }


   //: Get the end of the pending list
   //! PRE: Active list must be locked
   std::vector<ConfigChunk*>::iterator getActiveEnd()
   {
      vprASSERT(1 == mActiveLock.test());
      return mActiveConfig.end();
   }


    //: Erase an item from the list
    //! PRE: Active list must be locked && item must be in list
    //! POST: list = old(list).erase(item) && item is invalid
    void removeActive(const std::string& chunk_name);


    //: Add an item to the active configuration
    //! NOTE: This DOES NOT process the chunk
    //+     it just places it into the active configuration list
    //! PRE: Current list must NOT be locked
    void addActive (ConfigChunk* chunk);


   //: Return ptr to the active config dhunk db
   //! PRE: active must be locked
   //! NOTE: The pointer is only valid until active is unlocked
   //! CONCURRENCY: sequential
   ConfigChunkDB* getActiveConfig()
   {
      vprASSERT(1 == mActiveLock.test());
      return &mActiveConfig;
   }


public:
   //: Scan the active list for items that don't have their dependencies filled
   //! POST: Any chunks in active with dependencies not filled are added to the
   //+       the pending list. (A remove and an add are added to the pending)
   //+       The remove item configChunk* == active configChunk*
   //+       The add item configChunk* points to a copy of the chunk
   //! NOTE: We add an add after the removal to allow for the object
   //+       to re-enter the system after its dependencies have been satisfied
   //! RETURNS: The number of lost dependencies found
   int scanForLostDependencies();


    //------------------ DynamicReconfig Stuff ------------------------------

    void addConfigChunkHandler (ConfigChunkHandler* h);
    void removeConfigChunkHandler (ConfigChunkHandler* h);
    int attemptReconfiguration ();

    //------------------ JackalControl Stuff --------------------------------

public:

    virtual void setJackalServer (JackalServer* js);
    virtual void addConnect (Connect *c);
    virtual void removeConnect (Connect* c);




private:
    ConfigChunkDB            mActiveConfig;   //: List of current configuration
    std::list<PendingChunk>  mPendingConfig;  //: List of pending configuration
                                              //  changes
    vpr::Mutex               mPendingLock;    //: Lock on pending list
    vpr::Mutex               mActiveLock;     //: Lock for current config list

    std::vector<ConfigChunkHandler*> chunk_handlers;

    // The following variables are used to implement some logic
    // that "stales" the pending list.   (see pendingNeedsChecked)
    vpr::Mutex               mPendingCountMutex;
    int                      mPendingCheckCount;  //: How many pending checks since last change to pending
    int                      mLastPendingSize;    //: The size of pending at last check

    JackalServer*            jackal_server;
    XMLConfigCommunicator*   config_communicator;


protected:

    ConfigManager();
    virtual ~ConfigManager ();

    vprSingletonHeader(ConfigManager);


}; // class ConfigManager


}; // namespace jccl

#endif

