/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

#ifndef _VPR_THREAD_MANAGER_H_
#define _VPR_THREAD_MANAGER_H_
//#pragma once

#include <vpr/vprConfig.h>

#include <vpr/Thread/BaseThread.h>
#include <vpr/Thread/TSTable.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>

#include <vpr/Util/Debug.h>
#include <vpr/Util/Singleton.h>


namespace vpr {

/**
 * Manager that maintains data about all threads.
 *
 * This class holds data on: <br>
 * - List of all threads in system <br>
 * - Thread Specific data items
 *
 * @author Allen Bierbaum
 */
class VPR_CLASS_API ThreadManager
{
public:
   /**
    * Called when a thread has been added to the system.
    *
    * @pre Manager must be locked.
    */
   void addThread(BaseThread* thread);

   /**
    * Called when a thread has been removed from the system.
    *
    * @pre Manager must be locked.
    */
   void removeThread(BaseThread* thread);

   /**
    * Locks the manager so that we have complete control to do stuff.
    * The manager should be locked whenever there is about to be a change in
    * number of threads in the system.
    */
   void lock()
   {
      mThreadVectorMutex.acquire();
   }

   /// Unlocks the manager to allow people to use it again.
   void unlock()
   {
      mThreadVectorMutex.release();
   }

   /// Dumps the state of the manager to debug.
   void debugDump();

private:
   Mutex                    mThreadVectorMutex;  //! Mutex to protect the threads vector
   std::vector<BaseThread*> mThreads;            //! List of all threads in system

public:
   /**
    * Generates a unique key for Thread Specific data.
    * This value will be used locally by each thread in the system.
    */
   long generateNewTSKey()
   {
      Guard<Mutex> guard(mTSKeyMutex);
      return mNextTSObjectKey++;
   }

private:
   Mutex     mTSKeyMutex;       //! Mutex to protect allocate of TS keys
   long      mNextTSObjectKey;  //! The next available object key

   // ----------------------- //
   // --- SINGLETON STUFF --- //
   // ----------------------- //
protected:
   /// Constructor.  Hidden, so no instantiation is allowed.
   ThreadManager() : mNextTSObjectKey(0)
   {;}

   // These two have to be here because Visual C++ will try to make them
   // exported public symbols.  This causes problems because copying vpr::Mutex
   // objects is not allowed.
   ThreadManager(const ThreadManager& t) {;}
   void operator=(const ThreadManager& t) {;}

vprSingletonHeader(ThreadManager);
};

}; // End of vpr namespace


#endif
