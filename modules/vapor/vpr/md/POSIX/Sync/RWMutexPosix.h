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

#include <vpr/Util/ReturnStatus.h>


namespace vpr
{

/** \class RWMutexPosix RWMutexPosix.h vpr/Sync/RWMutex.h
 *
 * RWMutex wrapper for POSIX-compliant systems using pthreads rw mutex variables
 * for the implementation.  This is typedef'd to vpr::RWMutex.
 */
class RWMutexPosix
{
public:
   /**
    * Constructor for vpr::RWMutexPosix class.
    *
    * @post The mutex variable is initialized for use.  It must be
    *       initialized before any other member functions can do anything
    *       with it.
    */
   RWMutexPosix() : mLocked(0)
   {
      // Initialize the mutex.
      pthread_rwlock_init(&mRWMutex, NULL);
   }

   /**
    * Destructor for vpr::RWMutexPosix class.
    *
    * @pre The mutex variable should be unlocked before being destroyed,
    *      but if it is not, this routine will unlock it and then destroy
    *      it.
    * @post The mutex variable is destroyed and unlocked if necessary.
    */
   ~RWMutexPosix()
   {
      // Destroy the mutex.
      if ( pthread_rwlock_destroy(&mRWMutex) == -1 )
      {
         pthread_rwlock_unlock(&mRWMutex);
         pthread_rwlock_destroy(&mRWMutex);
      }
   }

   /**
    * Locks this mutex.
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another process/thread, the
    *       caller blocks until the mutex has been freed.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock is acquired.
    * @return vpr::ReturnStatus::Fail is returned if an error occurred.
    *
    * @note Acquires the write lock.
    */
   vpr::ReturnStatus acquire()
   { 
      return acquireWrite();
   }

   /**
    * Acquires a read lock on this mutex.
    *
    * @pre None.
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another process/thread, the
    *       caller blocks until the mutex has been freed.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock is acquired.
    * @return vpr::ReturnStatus::Fail is returned if an error occurred.
    */
   vpr::ReturnStatus acquireRead()
   {
      int retval = pthread_rwlock_rdlock(&mRWMutex);

      // Locking succeeded.
      if ( retval == 0 )
      {
         mLocked = 1;
         return vpr::ReturnStatus();
      }
#ifdef _DEBUG
      // This thread tried to lock the mutex twice and a deadlock condition
      // was reported.
      else if ( retval == EDEADLK )
      {
         perror("Tried to lock mutex twice (RWMutexPosix.h:146)");
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
#endif
      // Some other error occurred.
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   /**
    * Acquires a write lock on this mutex.
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another process/thread, the
    *       caller blocks until the mutex has been freed.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock is acquired.
    * @return vpr::ReturnStatus::Fail is returned if an error occurred.
    */
   vpr::ReturnStatus acquireWrite()
   {
      int retval = pthread_rwlock_wrlock(&mRWMutex);

      // Locking succeeded.
      if ( retval == 0 )
      {
         mLocked = 1;
         return vpr::ReturnStatus();
      }
#ifdef _DEBUG
      // This thread tried to lock the mutex twice and a deadlock condition
      // was reported.
      else if ( retval == EDEADLK )
      {
         perror("Tried to lock mutex twice (RWMutexPosix.h:184)");
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
#endif
      // Some other error occurred.
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   /**
    * Tries to acquire a lock on this mutex variable (does not block).
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another process/thread, the
    *       caller returns does not wait for it to be unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock is acquired.
    * @return vpr::ReturnStatus::Fail is returned if the mutex is already
    *         locked.
    *
    * @note Tries to acquire the write mutex.
    */
   vpr::ReturnStatus tryAcquire()
   {
      return tryAcquireWrite();
   }

   /**
    * Tries to acquire a read lock on this mutex (does not block).
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another process/thread, the
    *       caller returns does not wait for it to be unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read lock is
    *         acquired.
    * @return vpr::ReturnStatus::Fail is returned if the mutex is already
    *         locked.
    */
   vpr::ReturnStatus tryAcquireRead()
   {
      if ( pthread_rwlock_tryrdlock(&mRWMutex) == 0 )
      {
         mLocked = 1;
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }

   }

   /**
    * Tries to acquire a write lock on this mutex (does not block).
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another process/thread, the
    *       caller returns does not wait for it to be unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write lock is
    *         acquired.
    * @return vpr::ReturnStatus::Fail is returned if the mutex is already
    *         locked.
    */
   vpr::ReturnStatus tryAcquireWrite()
   {
      if ( pthread_rwlock_trywrlock(&mRWMutex) == 0 )
      {
         mLocked = 1;
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }

   }

   /**
    * Releases this mutex.
    *
    * @pre The mutex variable must be locked.
    * @post The mutex variable is unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock is released
    *         successfully.
    * @return vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus release()
   {
      mLocked = 0;
      if ( pthread_rwlock_unlock(&mRWMutex) == 0 )
      {
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   /**
    * Tests the current lock status.
    *
    * @post The state of the mutex variable is returned.
    *
    * @return 0 is returned if this mutex is not currently locked.
    * @return 1 is returned if this mutex is locked.
    */
   int test()
   {
      return mLocked;
   }

protected:
   pthread_rwlock_t  mRWMutex;    /**< RWMutex variable for the class. */
   int               mLocked;     /**< If true, then we are locked */

   // = Prevent assignment and initialization.
   void operator= (const RWMutexPosix &) {;}
   RWMutexPosix (const RWMutexPosix &) {;}
};

} // End of vpr namespace


#endif  /* ifdef _VPR_RWMUTEX_POSIX_H_ */
