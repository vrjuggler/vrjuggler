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

#include <vpr/Util/ReturnStatus.h>


namespace vpr
{

/** \class MutexPosix MutexPosix.h vpr/Sync/Mutex.h
 *
 * Mutex wrapper for POSIX-compliant systems using pthreads mutex variables
 * for the implementation.  This is typedef'd to vpr::Mutex.
 */
class VPR_CLASS_API MutexPosix
{
public:
   /**
    * Constructor for vpr::MutexPosix class.
    *
    * @post The mutex variable is initialized for use.  It must be
    *       initialized before any other member functions can do anything
    *       with it.
    */
   MutexPosix();

   /**
    * Destructor for vpr::MutexPosix class.
    *
    * @pre The mutex variable should be unlocked before being destroyed,
    *      but if it is not, this routine will unlock it and then destroy
    *      it.
    * @post The mutex variable is destroyed and unlocked if necessary.
    */
   ~MutexPosix()
   {
      // Destroy the mutex.
      if ( pthread_mutex_destroy(&mMutex) == -1 )
      {
         pthread_mutex_unlock(&mMutex);
         pthread_mutex_destroy(&mMutex);
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
    */
   vpr::ReturnStatus acquire()
   {
      int retval = pthread_mutex_lock(&mMutex);

      // Locking succeeded.
      if ( retval == 0 )
      {
         return vpr::ReturnStatus();
      }
#ifdef _DEBUG
      // This thread tried to lock the mutex twice and a deadlock condition
      // was reported.
      else if ( retval == EDEADLK )
      {
         perror("Tried to lock mutex twice");
         assert(false && "Mutex deadlock detected");
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
    * Acquires a read lock on this mutex.
    *
    * @post A lock on the mutex variable is acquired by the caller.  If a
    *       lock has already been acquired by another process/thread, the
    *       caller blocks until the mutex has been freed.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock is acquired.
    * @return vpr::ReturnStatus::Fail is returned if an error occurred.
    *
    * @note No special read lock has been defined for now.
    */
   vpr::ReturnStatus acquireRead()
   {
      return this->acquire();
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
    *
    * @note No special write lock has been defined for now.
    */
   vpr::ReturnStatus acquireWrite()
   {
      return this->acquire();
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
    */
   vpr::ReturnStatus tryAcquire()
   {
      if ( pthread_mutex_trylock(&mMutex) == 0 )
      {
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
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
      return this->tryAcquire();
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
      return this->tryAcquire();
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
      int retval = pthread_mutex_unlock(&mMutex);

      if (0 == retval )
      {
         return vpr::ReturnStatus();
      }
      else if(EPERM == retval)
      {
         perror("Tried to release a mutex we do not own");
         assert(false && "Mutex release by non-owning thread.");
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
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
   int test() const;

   // Allow the vpr::CondPosix class to access the private and protected
   // members of this class.  Specifically, direct access is needed to the
   // mutex variable.
   friend class CondVarPosix;

protected:
   pthread_mutex_t mMutex;    /**< Mutex variable for the class. */

   // = Prevent assignment and initialization.
   void operator= (const MutexPosix &) {;}
   MutexPosix (const MutexPosix &) {;}
};

} // End of vpr namespace


#endif  /* ifdef _VPR_MUTEX_POSIX_H_ */
