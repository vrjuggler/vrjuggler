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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _VPR_MUTEX_NSPR_H_
#define _VPR_MUTEX_NSPR_H_

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <stdlib.h>
#include <prlock.h>

#include <vpr/Util/Assert.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr
{

/** \class MutexNSPR MutexNSPR.h vpr/Sync/Mutex.h
 *
 * Mutex wrapper for NSPR locks.  This is typedef'd to vpr::Mutex.
 */
class VPR_CLASS_API MutexNSPR
{
public:
   /**
    * Constructor.
    *
    * @post This mutex is initialized for use.  It must be initialized before
    *       any other member functions can do anything with it.
    */
   MutexNSPR() : mLocked(0)
   {
      // ----- Allocate the mutex ----- //
      mMutex = PR_NewLock();
      vprASSERT(mMutex != NULL);
   }

   /**
    * Destructor.
    *
    * @pre No thread should be in a lock-specific function.
    * @post The mutex variable is destroyed.
    */
   ~MutexNSPR()
   {
      PR_DestroyLock(mMutex);
   }

   /**
    * Locks this mutex.
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller blocks
    *       until the mutex has been freed.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock is acquired
    *         successfully.
    * @return vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus acquire()
   {
      PR_Lock(mMutex);
      mLocked = 1;

      return vpr::ReturnStatus();
   }

   /**
    * Acquires a read lock.
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller
    *       blocks until the mutex has been freed.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read lock is
    *         acquired successfully.
    * @return vpr::ReturnStatus::Fail is returned otherwise.
    *
    * @note No special read mutex has been defined for now.
    */
   vpr::ReturnStatus acquireRead()
   {
      return this->acquire();
   }

   /**
    * Acquires a write lock.
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller blocks
    *       until the mutex has been freed.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write lock is
    *         acquired successfully.
    * @return vpr::ReturnStatus::Fail is returned otherwise.
    *
    * @note No special write mutex has been defined for now.
    */
   vpr::ReturnStatus acquireWrite()
   {
      return this->acquire();
   }

   /**
    * Tries to acquire a lock on this mutex (does not block).
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller
    *       returns does not wait for it to be unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the lock is acquired.
    * @return vpr::ReturnStatus::Fail is returned if another thread is
    *         holding the lock already.
    */
   vpr::ReturnStatus tryAcquire()
   {
      // XXX: Possible race condition exists in this function implementation
      if ( mLocked == 0 )
      {
         this->acquire();
         return vpr::ReturnStatus();
      }
      else
      {
         return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
      }
   }

   /**
    * Tries to acquire a read lock (does not block).
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller
    *       returns does not wait for it to be unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read lock is
    *         acquired.
    * @return vpr::ReturnStatus::Fail is returned if another thread is holding
    *         the lock already.
    */
   vpr::ReturnStatus tryAcquireRead()
   {
      return this->tryAcquire();
   }

   /**
    * Tries to acquire a write lock (does not block).
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller returns
    *       does not wait for it to be unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write lock is
    *         acquired.
    * @return vpr::ReturnStatus::Fail is returned if another thread is holding
    *         the lock already.
    */
   vpr::ReturnStatus tryAcquireWrite()
   {
      return this->tryAcquire();
   }

   /**
    * Releases the mutex.
    *
    * @pre This mutex must be locked.
    * @post This mutex is unlocked.
    *
    * @return vpr::ReturnStatus::Succeed is returned if this mutex is unlocked
    *         successfully. 
    * @return vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus release()
   {
      mLocked = 0;
      if ( PR_Unlock(mMutex) == PR_SUCCESS )
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
    * @post The state of this mutex is returned.
    *
    * @return 0 is returned if this mutex is not locked.  1 is returned if it
    *         is locked.
    */
   int test() const
   {
      return mLocked;
   }

   // Allow the vpr::CondNSPR class to access the private and protected
   // members of this class.  Specifically, direct access is needed to the
   // mutex variable.
   friend class CondVarNSPR;

protected:
   PRLock* mMutex;     /**< Mutex variable for the class */
   int     mLocked;    /**< Hack used to implement mutex testing */

   // = Prevent assignment and initialization.
   MutexNSPR& operator=(const MutexNSPR& r)
   {
      mMutex = r.mMutex;
      mLocked = r.mLocked;
      return *this;
   }

   MutexNSPR(const MutexNSPR &)
   {
      /* Do nothing. */ ;
   }
};

} // End of vpr namespace


#endif   /* ifdef _VPR_MUTEX_NSPR_H_ */
