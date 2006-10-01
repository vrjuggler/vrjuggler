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
 * @note This file must be included by vpr/Sync/RWMutex.h, not the other way
 *       around.
 */

#ifndef _VPR_RWMUTEX_POSIX_H_
#define _VPR_RWMUTEX_POSIX_H_

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>
#include <boost/noncopyable.hpp>

#include <vpr/Sync/LockException.h>
#include <vpr/Sync/DeadlockException.h>


namespace vpr
{

/** \class RWMutexPosix RWMutexPosix.h vpr/Sync/RWMutex.h
 *
 * RWMutex wrapper for POSIX-compliant systems using pthreads rw mutex variables
 * for the implementation.  This is typedef'd to vpr::RWMutex.
 */
class RWMutexPosix : boost::noncopyable
{
public:
   /**
    * Constructor for vpr::RWMutexPosix class.
    *
    * @post The mutex variable is initialized and ready for use. The mutex
    *       operations will not fail.
    *
    * @throw vpr::ResourceException is thrown if the mutex cannot be
    *        allocated.
    */
   RWMutexPosix();

   /**
    * Destructor for vpr::RWMutexPosix class.
    *
    * @pre No thread should be in a lock-specific function.
    * @post The mutex variable is destroyed.
    */
   ~RWMutexPosix()
   {
      // Destroy the mutex.
      const int result = pthread_rwlock_destroy(&mRWMutex);
      assert(result == 0);
   }

   /**
    * Acquires a write lock on this mutex.
    *
    * @post A write lock on the mutex variable is acquired by the caller. If a
    *       lock has already been acquired by another thread, the caller
    *       blocks until the mutex has been freed.
    *
    * @note Acquires the write lock.
    *
    * @see acquireWrite()
    */
   void acquire()
   { 
      acquireWrite();
   }

   /**
    * Acquires a read lock on this mutex.
    *
    * @pre None.
    * @post A read lock on the mutex variable is acquired by the caller. If a
    *       lock has already been acquired by another thread, the caller
    *       blocks until the mutex has been freed.
    *
    * @throw vpr::DeadlockException is thrown if the current thread already
    *        has a write lock on this read-write mutex.
    * @throw vpr::LockException is thrown if the maximum number of allowed
    *        read locks have been acquired on this read-write mutex. This
    *        maximum is implementation-defined (by the POSIX standard).
    */
   void acquireRead()
   {
      const int result = pthread_rwlock_rdlock(&mRWMutex);

      if ( 0 == result )
      {
         mLocked = true;
      }
      // This thread tried to get a read lock on a mutex where it already has
      // a write lock. This would result in deadlock.
      else if ( EDEADLK == result )
      {
         throw vpr::DeadlockException(
            "This thread already has a write lock on the read-write mutex",
            VPR_LOCATION
         );
      }
      // The implementation-defined maximum number of read locks have been
      // acquired.
      else if ( EAGAIN == result )
      {
         throw vpr::LockException(
            "Maximum numbmer of read locks exceeded", VPR_LOCATION
         );
      }

      assert(result == 0);
   }

   /**
    * Acquires a write lock on this mutex.
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another thread, the caller
    *       blocks until the mutex has been freed.
    *
    * @throw vpr::DeadlockException is thrown if the current thread already
    *        has a read lock or a write lock on this read-write mutex.
    */
   void acquireWrite()
   {
      const int result = pthread_rwlock_wrlock(&mRWMutex);

      // Locking succeeded.
      if ( 0 == result )
      {
         mLocked = true;
      }
      // This thread tried to get a write lock on a mutex where it already has
      // either a write or a read lock. This would result in deadlock.
      else if ( EDEADLK == result )
      {
         throw vpr::DeadlockException(
            "This thread already holds a lock on the read-write mutex",
            VPR_LOCATION
         );
      }

      assert(result == 0);
   }

   /**
    * Tries to acquire a write lock on this mutex variable (does not block).
    *
    * @post If successful, a write lock on the mutex variable is acquired by
    *       the caller. If a write lock could not be acquired, then the method
    *       returns immdeiately.
    *
    * @return \c true is returned if the write lock is acquired, and \c false
    *         is returned if the write lock could not be acquired.
    *
    * @note Tries to acquire the write mutex.
    *
    * @see tryAcquireWrite()
    */
   bool tryAcquire()
   {
      return tryAcquireWrite();
   }

   /**
    * Tries to acquire a read lock on this mutex (does not block).
    *
    * @post If successful, a read lock on the mutex variable is acquired by
    *       the caller. If a read lock could not be acquired, then the method
    *       returns immdeiately.
    *
    * @return \c true is returned if the read lock is acquired, and \c false
    *         is returned if the read lock could not be acquired.
    */
   bool tryAcquireRead()
   {
      const int result = pthread_rwlock_tryrdlock(&mRWMutex);

      if ( 0 == result )
      {
         mLocked = true;
      }

      return result != EBUSY;
   }

   /**
    * Tries to acquire a write lock on this mutex (does not block).
    *
    * @post If successful, a write lock on the mutex variable is acquired by
    *       the caller. If a write lock could not be acquired, then the method
    *       returns immdeiately.
    *
    * @return \c true is returned if the write lock is acquired, and \c false
    *         is returned if the write lock could not be acquired.
    */
   bool tryAcquireWrite()
   {
      const int result = pthread_rwlock_trywrlock(&mRWMutex);

      if ( 0 == result )
      {
         mLocked = true;
      }

      return result != EBUSY;
   }

   /**
    * Releases this mutex.
    *
    * @pre The mutex variable must be locked by the calling thread.
    * @post The mutex variable is unlocked.
    *
    * @throw vpr::LockException is thrown if the current thread was not the
    *        one that locked this mutex.
    */
   void release()
   {
      mLocked = false;
      const int result = pthread_rwlock_unlock(&mRWMutex);

      if ( EPERM == result )
      {
         throw vpr::LockException(
            "Tried to release a read-write mutex that this thread does not own",
            VPR_LOCATION
         );
      }

      assert(result == 0);
   }

   /**
    * Tests the current lock status.
    *
    * @post The state of the mutex variable is returned.
    *
    * @return \c false is returned if this mutex is not currently locked.
    *         \c true is returned if it is.
    */
   bool test() const
   {
      return mLocked;
   }

protected:
   pthread_rwlock_t mRWMutex;    /**< RWMutex variable for the class. */
   bool             mLocked;     /**< If true, then we are locked */
};

} // End of vpr namespace


#endif  /* ifdef _VPR_RWMUTEX_POSIX_H_ */
