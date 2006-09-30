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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

/**
 * \file
 *
 * @note This file must be included by vpr/Sync/Cond.h, not the other way
 *       around.
 */

#ifndef _VPR_COND_VAR_POSIX_H_
#define _VPR_COND_VAR_POSIX_H_

#include <vpr/vprConfig.h>

#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <boost/noncopyable.hpp>

#include <vpr/Sync/LockException.h>
#include <vpr/md/POSIX/Sync/MutexPosix.h>
#include <vpr/Util/Interval.h>


namespace vpr
{

/** \class CondVarPosix CondVarPosix.h vpr/Sync/CondVar.h
 *
 * Condition variable wrapper for POSIX-compliant systems using pthreads
 * condition variables for the implementation.  This is typedef'd to
 * vpr::CondVar.
 */
class CondVarPosix : boost::noncopyable
{
public:
   /**
    * Constructor for vpr::CondVarPosix class.
    *
    * @post The condition variable is intialized, and the mutex variable
    *       associated with it is defined. The condition variable operations
    *       will not fail.
    *
    * @param mutex Pointer to a vpr::MutexPosix variable that is used in
    *              association with this condition variable in this class
    *              (optional).
    *
    * @throw vpr::ResourceException is thrown if the condition variable cannot
    *        be allocated.
    */
   CondVarPosix(MutexPosix* mutex = NULL);

   /**
    * Destructor for vpr::CondVarPosix class.
    *
    * @pre This condition variable is not locked by another thread.
    * @post The condition variable is destroyed.
    */
   ~CondVarPosix()
   {
      const int result = pthread_cond_destroy(&mCondVar);
      assert(result == 0);
   }

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
    * @throw vpr::IllegalArgumentException is thrown if \p timeToWait is not
    *        vpr::Interval::NoTimeout and an invalid value is passed to
    *        pthread_cond_timedwait().
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
   void signal()
   {
      if ( ! mCondMutex->test() )
      {
         throw vpr::LockException("Condition variable mutex must be locked",
                                  VPR_LOCATION);
      }

      const int result = pthread_cond_signal(&mCondVar);
      assert(result == 0);
   }

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
   void broadcast()
   {
      // ASSERT:  We have been locked
      if ( ! mCondMutex->test() )
      {
         throw vpr::LockException("Condition variable mutex must be locked",
                                  VPR_LOCATION);
      }

      const int result = pthread_cond_broadcast(&mCondVar);
      assert(result == 0);
   }

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
    * @see vpr::MutexPosix::release()
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
    * @param mutex Pointer to a vpr::MutexPosix variable that is used in
    *              association with the condition variable in this class.
    *
    * @note NEVER call except to initialize explicitly.
    */
   void setMutex(vpr::MutexPosix* mutex)
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
      std::cerr << "------------- vpr::CondVarPosix::Dump ---------\n"
                << "Not Implemented yet.\n";
   }

private:
   pthread_cond_t mCondVar;      /**< Condition variable */
   MutexPosix*    mCondMutex;    /**< Mutex for the condition variable */
   MutexPosix     mDefaultMutex; /**< A default mutex variable */
};

} // End of vpr namespace


#endif  /* _VPR_COND_POSIX_H_ */
