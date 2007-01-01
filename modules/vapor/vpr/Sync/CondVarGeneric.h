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
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _VPR_COND_VAR_GENERIC_H_
#define _VPR_COND_VAR_GENERIC_H_

#include <vpr/vprConfig.h>

#include <iostream>
#include <iomanip>
#include <boost/noncopyable.hpp>

#ifdef VPR_OS_Windows
#  include <process.h>
#endif

#include <sys/types.h>

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif

#include <vpr/Sync/Semaphore.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Interval.h>


namespace vpr
{

/** \class CondVarGeneric CondVarGeneric.h vpr/Sync/CondVar.h
 *
 * Condition Variable wrapper for the any system.  This is typedef'd to
 * vpr::CondVar on platforms or with threading subsystems that do not have
 * native support for condition variables.
 *
 * Condition variables allow a locked test waiting for specific conditions to
 * be satisfied.  For example waiting for a flag to be set or for a counter to
 * fall below a certain value.
 *
 * This is typedef'd to vpr::CondVar.
 *
 * @date January 29, 1997
 */
class VPR_CLASS_API CondVarGeneric : boost::noncopyable
{
public:
   /**
    * Constructor.
    *
    * @param mutex A pointer to a user specified mutex.  If this is NULL, we
    *              use the internal default mutex.
    */
   CondVarGeneric(Mutex* mutex = NULL)
      : mWaiters(0)
      , mCondMutex(NULL)
   {
      if ( mutex == NULL )
      {
         mutex = &mDefaultMutex;
      }

      mCondMutex = mutex;

      std::cerr << "------------------------------------\n"
                << "  vpr::CondVarGeneric: DOES NOT WORK\n"
                << "------------------------------------\n";
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
    * Signals a condition change.
    * This call tells all waiters that the condition has changed.
    * They can then check to see if it now sarisfies the condition.
    *
    * @pre The condition variable must be locked.
    *
    * @throw vpr::LockException is thrown if the mutex for this condition
    *        variable was not locked prior to invoking this method.
    */
   void signal()
   {
      std::cerr << std::setw(5) << getpid() << "  Signal" << std::endl;
      // ASSERT:  We have been locked
      if ( ! mCondMutex->test() )    // Not locked
      {
         throw vpr::LockException("Condition variable mutex must be locked",
                                  VPR_LOCATION);
      }

      if ( mWaiters > 0 )
      {
         mSema.release();
      }
   }

   /**
    * Signals all waiting threads.
    * This releases all waiting threads.
    *
    * @pre The condition variable must be locked.
    *
    * @throw vpr::LockException is thrown if the mutex for this condition
    *        variable was not locked prior to invoking this method.
    */
   void broadcast()
   {
      // ASSERT:  We have been locked
      if ( ! mCondMutex->test() )    // Not locked
      {
         throw vpr::LockException("Condition variable mutex must be locked",
                                  VPR_LOCATION);
      }

      for ( int i = mWaiters; i > 0; --i )
      {
         mSema.release();
      }
   }

   /**
    * Acquires a lock on the mutex variable associated with this condition
    * variable.
    *
    * @post A lock is acquired on the mutex variable associated with the
    *       condition variable. If a lock is acquired, the caller controls
    *       the mutex variable. If it was previously locked, the caller
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
    */
   void release()
   {
      mCondMutex->release();
   }

   /**
    * Explicitly sets the mutex to use.
    *
    * @note NEVER call except to initialize explicitly.
    */
   void setMutex(Mutex* mutex)
   {
      mutex->release();       // NOT exactly correct, but just make sure not to leave it locked
      mCondMutex = mutex;
   }

   /**
    * Tests the mutex associated with this condition variable to see if it
    * is held.
    *
    * @return \c false is returned if this mutex is not currently locked.
    *         \c true is returned if it is.
    */
   bool test()
   {
      return mCondMutex->test();
   }

   /**
    * Prints out information about the condition variable to stderr.
    *
    * @post All important data and debugging information related to the
    *       condition variable and its mutex are dumped to stdout.
    */
   void dump() const;

private:
   // --- These make up the "condition variable" ---- ///
   Semaphore mSema;     /**< Condition variable. */
   long mWaiters;       /**< The number of processes waiting */

   Mutex* mCondMutex;   /**< Mutex for the condition variable - User specified */
   Mutex mDefaultMutex; /**< Mutex to use if user does not specify one */
};

} // End of vpr namespace


#endif
