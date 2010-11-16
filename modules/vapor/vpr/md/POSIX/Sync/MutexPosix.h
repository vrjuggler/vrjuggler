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
 * @note This file must be included by vpr/Sync/Mutex.h, not the other way
 *       around.
 */

#ifndef _VPR_MUTEX_POSIX_H_
#define _VPR_MUTEX_POSIX_H_

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>
#include <boost/noncopyable.hpp>
#include <boost/concept_check.hpp>

#include <vpr/Sync/LockException.h>
#include <vpr/Sync/DeadlockException.h>


namespace vpr
{

/** \class MutexPosix MutexPosix.h vpr/Sync/Mutex.h
 *
 * Mutex wrapper for POSIX-compliant systems using pthreads mutex variables
 * for the implementation.  This is typedef'd to vpr::Mutex.
 */
class MutexPosix : boost::noncopyable
{
public:
   /**
    * Constructor for vpr::MutexPosix class.
    *
    * @post The mutex variable is initialized and ready for use. The mutex
    *       operations will not fail.
    *
    * @throw vpr::ResourceException is thrown if the mutex cannot be
    *        allocated.
    */
   MutexPosix();

   /**
    * Destructor for vpr::MutexPosix class.
    *
    * @pre No thread should be in a lock-specific function.
    * @post The mutex variable is destroyed.
    */
   ~MutexPosix()
   {
      const int result = pthread_mutex_destroy(&mMutex);
      assert(result == 0);
      boost::ignore_unused_variable_warning(result);
   }

   /**
    * Locks this mutex.
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another thread, the caller
    *       blocks until the mutex has been freed.
    *
    * @throw vpr::DeaadockException is thrown if the current thread has
    *        already locked this mutex.
    */
   void acquire()
   {
      const int result = pthread_mutex_lock(&mMutex);

      // This thread tried to lock the mutex twice, and a deadlock condition
      // was reported.
      if ( EDEADLK == result )
      {
         throw vpr::DeadlockException(
            "Tried to lock mutex twice in the same thread", VPR_LOCATION
         );
      }
      else if ( EINVAL == result )
      {
         throw vpr::LockException(
            "Tried to lock a mutex that does not refer to a initialized mutex object.",
            VPR_LOCATION
         );
      }
      else if ( EAGAIN == result )
      {
         throw vpr::LockException(
            "Tried to lock a mutex whose maximum number of recursive locks has been exceeded.",
            VPR_LOCATION
         );
      }

      assert(result == 0);
      boost::ignore_unused_variable_warning(result);
   }

   /**
    * Acquires a read lock on this mutex.
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another thread, the caller
    *       blocks until the mutex has been freed.
    *
    * @note No special read lock has been defined for now.
    */
   void acquireRead()
   {
      this->acquire();
   }

   /**
    * Acquires a write lock on this mutex.
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another thread, the caller
    *       blocks until the mutex has been freed.
    *
    * @note No special write lock has been defined for now.
    */
   void acquireWrite()
   {
      this->acquire();
   }

   /**
    * Tries to acquire a lock on this mutex variable (does not block).
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another thread, the caller
    *       returns does not wait for it to be unlocked.
    *
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the mutex is already locked.
    */
   bool tryAcquire()
   {
      return pthread_mutex_trylock(&mMutex) == 0;
   }

   /**
    * Tries to acquire a read lock on this mutex (does not block).
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another thread, the caller
    *       returns does not wait for it to be unlocked.
    *
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the mutex is already locked.
    */
   bool tryAcquireRead()
   {
      return this->tryAcquire();
   }

   /**
    * Tries to acquire a write lock on this mutex (does not block).
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another thread, the caller
    *       returns does not wait for it to be unlocked.
    *
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the mutex is already locked.
    */
   bool tryAcquireWrite()
   {
      return this->tryAcquire();
   }

   /**
    * Releases this mutex.
    *
    * @pre The mutex variable must be locked.
    * @post The mutex variable is unlocked.
    *
    * @throw vpr::LockException is thrown if the current thread was not the
    *        one that locked this mutex.
    */
   void release()
   {
      const int result = pthread_mutex_unlock(&mMutex);

      if ( EPERM == result )
      {
         throw vpr::LockException(
            "Tried to release a mutex that this thread does not own",
            VPR_LOCATION
         );
      }

      assert(result == 0);
      boost::ignore_unused_variable_warning(result);
   }

   /**
    * Tests the current lock status.
    *
    * @post The state of the mutex variable is returned.
    *
    * @return \c false is returned if this mutex is not currently locked.
    *         \c true is returned if it is.
    */
   bool test() const;

   // Allow the vpr::CondPosix class to access the private and protected
   // members of this class.  Specifically, direct access is needed to the
   // mutex variable.
   friend class CondVarPosix;

protected:
   pthread_mutex_t mMutex;    /**< Mutex variable for the class. */
};

} // End of vpr namespace


#endif  /* ifdef _VPR_MUTEX_POSIX_H_ */
