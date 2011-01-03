/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_THREAD_MANAGER_H_
#define _VPR_THREAD_MANAGER_H_

#include <vpr/vprConfig.h>

//#include <vpr/Thread/Thread.h>
#include <vpr/Thread/TSTable.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>

#include <vpr/Util/Singleton.h>


namespace vpr
{

/** \class ThreadManager ThreadManager.h vpr/Thread/ThreadManager.h
 *
 * Holds list of all threads in system.
 *
 * @deprecated This class may go away in the future.
 */
class VPR_CLASS_API ThreadManager
{
public:
   /**
    * Called when a thread has been added to the system.
    *
    * @pre Manager must be locked.
    */
   void addThread(Thread* thread);

   /**
    * Called when a thread has been removed from the system.
    *
    * @pre Manager must be locked.
    */
   void removeThread(Thread* thread);

   /**
    * Locks the manager so that we have complete control to do stuff.
    * The manager should be locked whenever there is about to be a change in
    * number of threads in the system.
    */
   void lock()
   {
      mThreadVectorMutex.acquire();
   }

   /** Unlocks the manager to allow people to use it again. */
   void unlock()
   {
      mThreadVectorMutex.release();
   }

   /**
    * Gets list of all threads in system.
    * @note WARNING: This method is for advanced usage only.  Use at your own
    *       risk!
    */
   std::vector<Thread*> getThreads()
   {
      return mThreads;
   }

   std::vector<Thread*>::size_type getNumThreads()
   {
      return mThreads.size();
   }

   Thread* getThread(const std::vector<Thread*>::size_type i)
   {
      return mThreads[i];
   }

   /** Dumps the state of the manager to debug. */
   void debugDump();

private:
   Mutex                mThreadVectorMutex;  /**< Mutex to protect the threads vector */
   std::vector<Thread*> mThreads;            /**< List of all threads in system */

   // ----------------------- //
   // --- SINGLETON STUFF --- //
   // ----------------------- //
protected:
   /** Constructor.  Hidden, so no instantiation is allowed. */
   ThreadManager()
   {;}

   ~ThreadManager()
   {;}

   // These two have to be here because Visual C++ will try to make them
   // exported public symbols.  This causes problems because copying vpr::Mutex
   // objects is not allowed.
   ThreadManager(const ThreadManager&) {;}
   void operator=(const ThreadManager&) {;}

   vprSingletonHeader(ThreadManager);
};

} // End of vpr namespace


#endif
