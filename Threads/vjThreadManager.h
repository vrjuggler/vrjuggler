/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#ifndef _VJ_THREAD_MANAGER_H_
#define _VJ_THREAD_MANAGER_H_
//#pragma once

#include <vjConfig.h>
#include <Threads/vjThread.h>
#include <Threads/vjTSTable.h>
#include <Sync/vjMutex.h>
#include <Sync/vjGuard.h>

#include <Kernel/vjDebug.h>

//----------------------------------------------------
//: Manager that maintains data about all threads.
//
// This class holds data on: <br>
// - List of all threads in system <br>
// - Thread Specific data items
//
//-----------------------------------------------------
//!PUBLIC_API:
class vjThreadManager
{
public:
   //-----------------------------------------------------
   //: Called when a thread has been added to the system.
   //
   //! PRE: Manager must be locked.
   //-----------------------------------------------------
   void addThread(vjBaseThread* thread)
   {
      vjASSERT((mThreadVectorMutex.test()==1) && (mTSMutex.test()==1)); // Assert manager locked
      vjASSERT(thread->getTID() >= 0);

      // Insert thread into local table
      while ((int)mThreads.size() <= thread->getTID())
         mThreads.push_back(NULL);
      mThreads[thread->getTID()] = thread;

      tsThreadAdded(thread);     // Tell TS routines that thread added
      debugDump();               // Dump current state
   }

   //-----------------------------------------------------
   //: Called when a thread has been removed from the
   //+ system.
   //
   //! PRE: Manager must be locked.
   //-----------------------------------------------------
   void removeThread(vjBaseThread* thread)
   {
      vjASSERT((mThreadVectorMutex.test()==1) && (mTSMutex.test()==1)); // Assert manager locked
      vjASSERT((thread->getTID() >= 0) && (thread->getTID() < (int)mThreads.size()));
      mThreads[(unsigned int)thread->getTID()] = NULL;
      tsThreadRemoved(thread);   // Tell TS routines that thread removed
   }

   //-----------------------------------------------------
   //: Lock the manager so that we have complete control
   //+ to do stuff.
   // The manager should be locked whenever there is
   // about to be a change in number of threads in the
   // system.
   //-----------------------------------------------------
   void lock()
   {
      mThreadVectorMutex.acquire();
      mTSMutex.acquire();
   }

   //-----------------------------------------------------
   //: Unlock the manager to allow people to use it
   //+ again.
   //-----------------------------------------------------
   void unlock()
   {
      mTSMutex.release();
      mThreadVectorMutex.release();
   }

   //-----------------------------------------------------
   //: Dump the state of the manager to debug.
   //-----------------------------------------------------
   void debugDump();

private:
   vjMutex                 mThreadVectorMutex;     //: Mutex to protect the threads vector
   std::vector<vjBaseThread*> mThreads;            //: List of all threads in system

      /******** TS DATA **********/
public:
   //-----------------------------------------------------
   //: Add an object to all the tables.
   //
   //! POST: The new object has been assigned to all
   //+       tables in system.
   //-----------------------------------------------------
   long addTSObject(vjTSBaseObject* object);

   //-----------------------------------------------------
   //: Remove an object from all tables.
   //
   //! POST: The object of key is removed from all tables
   //+       in system.
   //-----------------------------------------------------
   void removeTSObject(long key);

   //-----------------------------------------------------
   //: Return the table for the current thread.
   //-----------------------------------------------------
   vjTSTable* getCurrentTSTable();

private:
   //-----------------------------------------------------
   //: Called when a thread has been added to the system.
   //-----------------------------------------------------
   void tsThreadAdded(vjBaseThread* thread);

   //-----------------------------------------------------
   //: Called when a thread has been removed from the
   //+ system.
   //-----------------------------------------------------
   void tsThreadRemoved(vjBaseThread* thread);

   //-----------------------------------------------------
   //: Returns a newly generated TS key to use.
   //-----------------------------------------------------
   long generateNewTSKey()
   { return mNextTSObjectKey++; }

private:
   vjMutex             mTSMutex;          //: Mutex to protect access to all TS stuff
   std::vector<vjTSTable*> mTSTables;     //: The table of TSTables
                                          // Not that the thread each have a cached copy of this as well
   vjTSTable           mBaseTSTable;      //: This is the "good" copy of the table to use as source for copies
   long                mNextTSObjectKey;  //: The next available object key


   // ----------------------- //
   // --- SINGLETON STUFF --- //
   // ----------------------- //
protected:
   //-----------------------------------------------------
   //: Constructor.  Hidden, so no instantiation is
   //+ allowed.
   //-----------------------------------------------------
   vjThreadManager() : mNextTSObjectKey(0)
   {;}

public:
   //-----------------------------------------------------
   //: Get instance of singleton object.
   //-----------------------------------------------------
   static vjThreadManager* instance()
   {
      if (_instance == NULL)
         _instance = new vjThreadManager;
      return _instance;
   }

private:
   static vjThreadManager* _instance;   //: The instance
};

#endif
