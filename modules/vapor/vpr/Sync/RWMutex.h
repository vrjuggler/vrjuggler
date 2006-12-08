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

#ifndef _VPR_RW_MUTEX_H_
#define _VPR_RW_MUTEX_H_

/**
 * \file
 *
 * Include this file to get the full declaration of the platform-specific
 * read/write mutex class.  It will be typedef'd to vpr::RWMutex.
 */

#include <vpr/vprConfig.h>

#if VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_NSPR
#  include <vpr/md/NSPR/Sync/RWMutexNSPR.h>
#elif VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_POSIX
#  include <vpr/md/POSIX/Sync/RWMutexPosix.h>
#elif VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_IRIX_SPROC || \
      VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_WINTHREADS

#  include <boost/noncopyable.hpp>

#  include <vpr/Sync/Semaphore.h>
#  include <vpr/Sync/CondVar.h>
#  include <vpr/Sync/Mutex.h>

namespace vpr
{

/** \class RWMutex RWMutex.h vpr/Sync/RWMutex.h
 *
 * Read/write mutex implementation.  This is used on platforms or with
 * threading subsystems that do not have native support for read/write mutexes.
 *
 * @date January 31, 1997
 */
class VPR_CLASS_API RWMutex : boost::noncopyable
{
public:
   RWMutex()
      : waitingReaders(&stateLock)
      , numWaitingReaders(0)
      , waitingWriters(&stateLock)
      , numWaitingWriters(0)
      , refCount(0)
   {
      /* Do nothing. */ ;
   }

   ~RWMutex()
   {
      /* Do nothing. */ ;
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
   void acquireRead();

   /**
    * Acquires a write lock on this mutex.
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another process/thread, the
    *       caller blocks until the mutex has been freed.
    *
    * @throw vpr::DeadlockException is thrown if the current thread already
    *        has a read lock or a write lock on this read-write mutex.
    */
   void acquireWrite();

   /**
    * Tries to acquire a write lock on this mutex variable (does not block).
    * Wait until the semaphore value is greater than 0, then decrement by 1
    * and return. This is a "P" operation.
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
   bool tryAcquireRead();

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
   bool tryAcquireWrite();

   /**
    * Releases this mutex.
    *
    * @pre The mutex variable must be locked by the calling thread.
    * @post The mutex variable is unlocked.
    *
    * @throw vpr::LockException is thrown if the current thread was not the
    *        one that locked this mutex.
    */
   void release();

   /**
    * Tests the current lock status.
    *
    * @return \c false is returned if this mutex is not currently locked.
    *         \c true is returned if it is.
    */
   bool test() const
   {
      return stateLock.test();
   }

   /** Dumps the mutex debug stuff and current state. */
   void dump (FILE* dest = stderr,
              const char* message = "\n------ Mutex Dump -----\n") const
   {
      stateLock.dump();
   }

protected:
   Mutex stateLock;        /**< Serialize access to internal state */
   CondVar waitingReaders; /**< Reader threads waiting to acquire the lock */
   int numWaitingReaders;  /**< Number of waiting readers */

   CondVar waitingWriters; /**< Writer threads waiting to acquire the lock */
   int numWaitingWriters;  /**< Number of waiting writers */

   /**
    * Value is -1 if writer has the lock, else this keeps track of the
    * number of readers holding the lock.
    */
   int refCount;
};

} // End of vpr namespace

#endif  /* ! VPR_USE_NSPR */


#endif /* _VPR_RW_MUTEX_H_ */
