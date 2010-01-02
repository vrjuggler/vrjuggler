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

#ifndef _VPR_GUARD_H_
#define _VPR_GUARD_H_


namespace vpr
{

/** \class Guard Guard.h vpr/Sync/Guard.h
 *
 * Scoped wrapper for a lock.
 *
 * @date January 31, 1997
 */
template<class LOCK_TYPE>
class Guard
{
public:
   /**
    * Acquires the lock implicitly. If \p block is true, then use a blocking
    * mutex acquisition operation. Otherwise, use a non-blocking acquisition
    * call.
    *
    * @post \c mLockStatus reflects whether the given lock was acquired.
    *
    * @param lock  The mutex to associate with this guard.
    * @param block A flag indicating whether a blocking acquisition operation
    *              should be used to acquire the lock. This parameter is
    *              optional and defaults to true if it is not specified.
    */
   Guard(LOCK_TYPE& lock, const bool block = true)
      : mTheLock(&lock)
   {
      mLockStatus = block ? acquire() : tryAcquire();
   }

   /** Releases the lock. */
   ~Guard()
   {
      if ( mLockStatus )
      {
         mTheLock->release();
      }
   }

   /**
    * Indicates whether this guard is currently locked.
    *
    * @return \c true is returned if this guard is locked; \c false is
    *         returned otherwise.
    */
   bool locked() const
   {
      return mLockStatus;
   }

   /** Acquires the lock. */
   bool acquire()
   {
      mTheLock->acquire();
      mLockStatus = true;
      return mLockStatus;
   }

   /** Tries to acquire lock. */
   bool tryAcquire()
   {
      mLockStatus = mTheLock->tryAcquire();
      return mLockStatus;
   }

   /** Explicity releases the lock. */
   void release()
   {
      mLockStatus = false;
      mTheLock->release();
   }

private:
   LOCK_TYPE* mTheLock;    /**< The lock that we are using */
   bool       mLockStatus; /**< Are we locked or not */
};

} // End of vpr namespace


#endif
