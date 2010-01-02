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

#include <cstdlib>
#include <prlock.h>
#include <boost/noncopyable.hpp>

#include <vpr/Sync/LockException.h>


namespace vpr
{

/** \class MutexNSPR MutexNSPR.h vpr/Sync/Mutex.h
 *
 * Mutex wrapper for NSPR locks.  This is typedef'd to vpr::Mutex.
 */
class VPR_CLASS_API MutexNSPR : boost::noncopyable
{
public:
   /**
    * Constructor.
    *
    * @post This mutex is initialized for use. The mutex operations will not
    *       fail.
    *
    * @throw vpr::ResourceException is thrown if the mutex cannot be
    *        allocated.
    */
   MutexNSPR();

   /**
    * Destructor.
    *
    * @pre No thread should be in a lock-specific function.
    * @post The mutex variable is destroyed.
    */
   ~MutexNSPR()
   {
      if ( NULL != mMutex )
      {
         PR_DestroyLock(mMutex);
      }
   }

   /**
    * Locks this mutex.
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another thread, the caller blocks until
    *       the mutex has been freed.
    */
   void acquire()
   {
      PR_Lock(mMutex);
      mLocked = true;
   }

   /**
    * Acquires a read lock.
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another thread, the caller blocks until
    *       the mutex has been freed.
    *
    * @note No special read mutex has been defined for now.
    */
   void acquireRead()
   {
      this->acquire();
   }

   /**
    * Acquires a write lock.
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another thread, the caller blocks until
    *       the mutex has been freed.
    *
    * @note No special write mutex has been defined for now.
    */
   void acquireWrite()
   {
      this->acquire();
   }

   /**
    * Tries to acquire a lock on this mutex (does not block).
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another thread, the caller returns does
    *       not wait for it to be unlocked.
    *
    * @return \c true is returned if the lock is acquired by this thread, and \c false is
    *         returned if the mutex is already locked.
    */
   bool tryAcquire()
   {
      // XXX: Possible race condition exists in this function implementation
      if ( mLocked == false )
      {
         this->acquire();
         return true;
      }
      return false;
   }

   /**
    * Tries to acquire a read lock (does not block).
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another thread, the caller returns does
    *       not wait for it to be unlocked.
    *
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the mutex is already locked.
    */
   bool tryAcquireRead()
   {
      return this->tryAcquire();
   }

   /**
    * Tries to acquire a write lock (does not block).
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another thread, the caller returns does
    *       not wait for it to be unlocked.
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
    * @pre This mutex must be locked.
    * @post This mutex is unlocked.
    *
    * @throw vpr::LockException is thrown if the unlock operation fails.
    */
   void release()
   {
      mLocked = false;
      const PRStatus result = PR_Unlock(mMutex);

      if ( PR_FAILURE == result )
      {
         throw vpr::LockException("Mutex unlock operation failed",
                                  VPR_LOCATION);
      }
   }

   /**
    * Tests the current lock status.
    *
    * @post The state of this mutex is returned.
    *
    * @return \c false is returned if this mutex is not locked. \c true is
    *         returned if it is locked.
    */
   bool test() const
   {
      return mLocked;
   }

   // Allow the vpr::CondNSPR class to access the private and protected
   // members of this class.  Specifically, direct access is needed to the
   // mutex variable.
   friend class CondVarNSPR;

protected:
   PRLock* mMutex;     /**< Mutex variable for the class */
   bool    mLocked;    /**< Hack used to implement mutex testing */
};

} // End of vpr namespace


#endif   /* ifdef _VPR_MUTEX_NSPR_H_ */
