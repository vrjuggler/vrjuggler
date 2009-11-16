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
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#ifndef _VPR_RW_MUTEX_NSPR_H_
#define _VPR_RW_MUTEX_NSPR_H_

#include <vpr/vprConfig.h>

#include <prrwlock.h>
#include <boost/noncopyable.hpp>


namespace vpr
{

/** \class RWMutexNSPR RWMutexNSPR.h vpr/Sync/RWMutex.h
 *
 * Read/write mutex implementation using NSPR read/write mutexes (PRRWLock).
 * This is typedef'd to vpr::RWMutex.
 */
class VPR_CLASS_API RWMutexNSPR : boost::noncopyable
{
public:
   /**
    * Constructor for vpr::RWMutexNSPR class.
    *
    * @post The mutex variable is initialized and ready for use. The mutex
    *       operations will not fail.
    *
    * @throw vpr::ResourceException is thrown if the mutex cannot be
    *        allocated.
    */
   RWMutexNSPR();

   /**
    * Destructor for vpr::RWMutexNSPR class.
    *
    * @pre No thread should be in a lock-specific function.
    * @post The mutex variable is destroyed.
    */
   ~RWMutexNSPR()
   {
      if ( NULL != mRwLock )
      {
         PR_DestroyRWLock(mRwLock);
      }
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
    */
   void acquireRead()
   {
      PR_RWLock_Rlock(mRwLock);
   }

   /**
    * Acquires a write lock on this mutex.
    *
    * @post A lock on the mutex variable is acquired by the caller. If a
    *       lock has already been acquired by another thread, the caller
    *       blocks until the mutex has been freed.
    */
   void acquireWrite()
   {
      PR_RWLock_Wlock(mRwLock);
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
    *
    * @note Not implemented!
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
    *
    * @note Not implemented!
    */
   bool tryAcquireWrite();

   /**
    * Releases the mutex.
    *
    * @pre The mutex variable must be locked by the calling thread.
    * @post The mutex variable is unlocked.
    */
   void release()
   {
      mLocked = false;
      PR_RWLock_Unlock(mRwLock);
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
   bool mLocked;

   int numWaitingReaders;    /**< Number of waiting readers. */
   int numWaitingWriters;    /**< Number of waiting writers. */

   /**
    * Value is -1 if writer has the lock, else this keeps track of the
    * number of readers holding the lock.
    */
   int mRefCount;

   PRRWLock* mRwLock;
};

} // End of vpr namespace


#endif  /* _VPR_RW_MUTEX_NSPR_H_ */
