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

#ifndef _VPR_RWGuard_h_
#define _VPR_RWGuard_h_

#include <vpr/vprConfig.h>
#include <vpr/Sync/RWMutex.h>


namespace vpr
{

/** \class ReadGuard RWGuard.h vpr/Sync/RWGuard.h
 *
 * Read Guard wrapper.
 */
class ReadGuard
{
public:
   /**
    * Acquires the read lock implicitly. If \p block is true, then use a
    * blocking read lock acquisition operation. Otherwise, use a non-blocking
    * read lock acquisition call.
    *
    * @post \c mLockStatus reflects whether the given read lock was acquired.
    *
    * @param lock  The read-write mutex to associate with this guard.
    * @param block A flag indicating whether a blocking read lock acquisition
    *              operation should be used to acquire the lock. This
    *              parameter is optional and defaults to true if it is not
    *              specified.
    */
   ReadGuard(RWMutex& lock, bool block = true)
      : mTheLock(&lock)
   {
      mLockStatus = block ? acquire() : tryAcquire();
   }

   /** Releases the lock. */
   ~ReadGuard()
   {
      if ( mLockStatus )
      {
         mTheLock->release();
      }
   }

   /**
    * Indicates whether this read guard is currently locked.
    *
    * @return \c true is returned if this guard is locked; \c false is
    *         returned otherwise.
    */
   bool locked() const
   {
      return mLockStatus;
   }

   /** Acquires a read lock. */
   bool acquire()
   {
      mTheLock->acquireRead();
      mLockStatus = true;
      return mLockStatus;
   }

   /** Tries to acquire a read lock. */
   bool tryAcquire()
   {
      mLockStatus = mTheLock->tryAcquireRead();
      return mLockStatus;
   }

   /** Explicity releases the read lock. */
   void release()
   {
      mLockStatus = false;
      mTheLock->release();
   }

private:
   RWMutex* mTheLock;    /**< The lock that we are using */
   bool     mLockStatus; /**< Are we locked or not */
};

/** \class WriteGuard RWGuard.h vpr/Sync/RWGuard.h
 *
 * Write Guard wrapper.
 */
class WriteGuard
{
public:
   /**
    * Acquires the write lock implicitly. If \p block is true, then use a
    * blocking write lock acquisition operation. Otherwise, use a non-blocking
    * write lock acquisition call.
    *
    * @post \c mLockStatus reflects whether the given write lock was acquired.
    *
    * @param lock  The read-write mutex to associate with this guard.
    * @param block A flag indicating whether a blocking write lock acquisition
    *              operation should be used to acquire the lock. This
    *              parameter is optional and defaults to true if it is not
    *              specified.
    */
   WriteGuard(RWMutex& lock, bool block = true)
      : mTheLock(&lock)
   {
      mLockStatus = block ? acquire() : tryAcquire();
   }

   /** Releases the write lock. */
   ~WriteGuard()
   {
      if ( mLockStatus )
      {
         mTheLock->release();
      }
   }

   /**
    * Indicates whether this read guard is currently locked.
    *
    * @return \c true is returned if this guard is locked; \c false is
    *         returned otherwise.
    */
   bool locked() const
   {
      return mLockStatus;
   }

   /** Acquires the write lock. */
   bool acquire()
   {
      mTheLock->acquireWrite();
      mLockStatus = true;
      return mLockStatus;
   }

   /** Tries to acquire write lock. */
   bool tryAcquire()
   {
      mLockStatus = mTheLock->tryAcquireWrite();
      return mLockStatus;
   }

   /** Explicity releases the write lock. */
   void release()
   {
      mLockStatus = false;
      mTheLock->release();
   }

private:
   RWMutex* mTheLock;    /**< The lock that we are using */
   bool     mLockStatus; /**< Are we locked or not */
};

} // End of vpr namespace


#endif
