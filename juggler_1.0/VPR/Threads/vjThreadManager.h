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


#ifndef _VJ_THREAD_MANAGER_H_
#define _VJ_THREAD_MANAGER_H_
//#pragma once

#include <vjConfig.h>

#include <VPR/Threads/vjBaseThread.h>
#include <VPR/Threads/vjTSTable.h>
#include <VPR/Sync/vjMutex.h>
#include <VPR/Sync/vjGuard.h>

#include <Utils/vjDebug.h>
#include <Utils/vjSingleton.h>

//----------------------------------------------------
//: Manager that maintains data about all threads.
//
// This class holds data on: <br>
// - List of all threads in system <br>
// - Thread Specific data items
//
//-----------------------------------------------------
//!PUBLIC_API:
class VJ_CLASS_API vjThreadManager
{
public:
   //-----------------------------------------------------
   //: Called when a thread has been added to the system.
   //
   //! PRE: Manager must be locked.
   //-----------------------------------------------------
   void addThread(vjBaseThread* thread);

   //-----------------------------------------------------
   //: Called when a thread has been removed from the
   //+ system.
   //
   //! PRE: Manager must be locked.
   //-----------------------------------------------------
   void removeThread(vjBaseThread* thread);

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
   }

   //-----------------------------------------------------
   //: Unlock the manager to allow people to use it
   //+ again.
   //-----------------------------------------------------
   void unlock()
   {
      mThreadVectorMutex.release();
   }

   //-----------------------------------------------------
   //: Dump the state of the manager to debug.
   //-----------------------------------------------------
   void debugDump();

private:
   vjMutex                    mThreadVectorMutex;     //: Mutex to protect the threads vector
   std::vector<vjBaseThread*> mThreads;            //: List of all threads in system

public:
   //: Generate a unique key for Thread Specific data
   // This value will be used locally by each thread in the system
   long generateNewTSKey()
   {
      vjGuard<vjMutex> guard(mTSKeyMutex);
      return mNextTSObjectKey++;
   }

private:
   vjMutex     mTSKeyMutex;          //: Mutex to protect allocate of TS keys
   long        mNextTSObjectKey;  //: The next available object key


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

   vjThreadManager (const vjThreadManager& o) {;}
   void operator= (const vjThreadManager& o) {;}

vjSingletonHeader(vjThreadManager);
/*
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
   */
};

#endif
