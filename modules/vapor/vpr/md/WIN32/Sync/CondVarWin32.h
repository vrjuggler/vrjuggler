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
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

/**
 * \file
 *
 * @note This file must be included by vpr/Sync/Cond.h, not the other way
 *       around.
 */

#ifndef _VPR_COND_VAR_WIN32_H_
#define _VPR_COND_VAR_WIN32_H_

#include <vpr/vprConfig.h>

#include <iostream>
#include <boost/noncopyable.hpp>

#include <vpr/md/WIN32/Sync/MutexWin32.h>
#include <vpr/Util/Interval.h>


namespace vpr
{

/** \class CondVarWin32 CondVarWin32.h vpr/Sync/CondVar.h
 *
 * Condition variable wrapper for Win32 systems. This is typedef'd to
 * vpr::CondVar.
 *
 * @since 1.1.36
 *
 * @note This implementation is based on the "SignalObjectAndWait Solution"
 *       from the article "Strategies for Implementing POSIX Condition
 *       Variables on Win32" by Douglas C. Scmidt and Irfan Pyarali. The
 *       article text can be found at
 *       http://www.cs.wustl.edu/~schmidt/win32-cv-1.html
 */
class VPR_CLASS_API CondVarWin32 : boost::noncopyable
{
public:
   /**
    * Constructor for vpr::CondVarWin32 class.
    *
    * @post The condition variable is intialized, and the mutex variable
    *       associated with it is defined. The condition variable operations
    *       will not fail.
    *
    * @param mutex Pointer to a vpr::MutexWin32 variable that is used in
    *              association with this condition variable in this class
    *              (optional).
    *
    * @throw vpr::ResourceException is thrown if the condition variable cannot
    *        be allocated.
    */
   CondVarWin32(MutexWin32* mutex = NULL);

   /**
    * Destructor for vpr::CondVarWin32 class.
    *
    * @pre This condition variable is not locked by another thread.
    * @post The condition variable is destroyed.
    */
   ~CondVarWin32();

   /**
    * Wait for possible condition change. The lock asociated with this
    * condition variable must be held prior to invoking this method. When
    * invoked, the lock on the variable is released, and the calling thread
    * is blocked until another thread informs it that the condition has
    * changed or until the timeout expires.
    *
    * @pre The mutex variable associated with this condition variable must be
    *      locked.
    * @post The condition variable is locked. If it was previously locked,
    *       the caller blocks until signaled. The condition has been modified,
    *       but it may not be satisfied.
    *
    * @param timeToWait The time to wait on the condition. This parameter is
    *                   optional and defaults to vpr::Interval::NoTimeout
    *                   (block indefinitely) if not given.
    *
    * @return \c true is returned if the condition variable is locked.
    *         \c false is returned if the wait operation times out before
    *         acquiring the lock.
    *
    * @throw vpr::Exception is thrown if something goes wrong while trying to
    *        wait on the condition variable.
    *
    * @note This call blocks until a condition change has been signaled.
    */
   bool wait(const vpr::Interval& timeToWait = vpr::Interval::NoTimeout);

   /**
    * Signals a thread waiting on this condition variable.
    *
    * @pre The condition variable must be locked.
    * @post The condition variable is unlocked, and a signal is sent to a
    *       thread waiting on it.
    *
    * @throw vpr::LockException is thrown if the mutex for this condition
    *        variable was not locked prior to invoking this method.
    */
   void signal();

   /**
    * Signals all waiting threads.
    *
    * @pre The mutex variable associated with this condition variable
    *      must be locked.
    * @post The condition variable is unlocked, and all waiting threads
    *       are signaled of this event.
    *
    * @throw vpr::LockException is thrown if the mutex for this condition
    *        variable was not locked prior to invoking this method.
    */
   void broadcast();

   /**
    * Acquires a lock on the mutex variable associated with this condition
    * variable.
    *
    * @post A lock is acquired on the mutex variable associated with the
    *       condition variable.  If a lock is acquired, the caller controls
    *       the mutex variable.  If it was previously locked, the caller
    *       blocks until it is unlocked.
    */
   void acquire()
   {
      mCondMutex->acquire();
   }

   /**
    * Tries to acquire a lock on the mutex variable associated with this
    * condition variable.
    *
    * @post If the mutex variable is not already locked, the caller
    *       obtains a lock on it.  If it is already locked, the routine
    *       returns immediately to the caller.
    *
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the mutex is already locked.
    */
   bool tryAcquire()
   {
      return mCondMutex->tryAcquire();
   }

   /**
    * Releases the lock on the mutex variable associated with this condition
    * variable.
    *
    * @post The lock held by the caller on the mutex variable is released.
    *
    * @see vpr::MutexWin32::release()
    */
   void release()
   {
      mCondMutex->release();
   }

   /**
    * Changes the condition variable mutex to be the specifiec mutex
    * variable.
    *
    * @pre The specified mutex variable must be initialized.
    * @post The condition variable associated with the mutex variable is
    *       reset to the specified variable.
    *
    * @param mutex Pointer to a vpr::MutexWin32 variable that is used in
    *              association with the condition variable in this class.
    *
    * @note NEVER call except to initialize explicitly.
    */
   void setMutex(vpr::MutexWin32* mutex)
   {
      // NOT exactly correct, but just make sure not to leave it locked
      mutex->release();
      mCondMutex = mutex;
   }

   /**
    * Tests the mutex associated with this condition variable to see if it
    * is held.
    *
    * @return \c false is returned if this mutex is not currently locked.
    *         \c true is returned if it is.
    */
   bool test() const
   {
      return mCondMutex->test();
   }

   /**
    * Prints out information about the condition variable to stderr.
    *
    * @post All important data and debugging information related to the
    *       condition variable and its mutex are dumped to stderr.
    */
   void dump() const
   {
      std::cerr << "------------- vpr::CondVarWin32::Dump ---------\n"
                << "mWaitersCount = " << mWaitersCount << std::endl;
   }

private:
   /** @name Condition Variable State */
   //@{
   int mWaitersCount;                   /**< Number of waiting threads. */
   CRITICAL_SECTION mWaitersCountLock;  /**< Lock for \c mWaitersCount. */

   /**
    * Semaphore used to queue up threads waiting for the condition to
    * become signaled.
    */
   HANDLE mSema;

   /**
    * An auto-reset event used by the broadcast/signal thread to wait for
    * all the waiting thread(s) to wake up and be released from the
    * semaphore.
    */
   HANDLE mWaitersDone;

   /**
    * Keeps track of whether we were broadcasting or signaling. This allows
    * us to optimze the code when just signaling.
    */
   bool mWasBroadcast;
   //@}

   MutexWin32* mCondMutex;      /**< Mutex for the condition variable */
   MutexWin32  mDefaultMutex;   /**< A default mutex variable */
};

} // End of vpr namespace


#endif  /* _VPR_COND_VAR_WIN32_H_ */
