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
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#ifndef _VPR_MUTEX_WIN32_H_
#define _VPR_MUTEX_WIN32_H_

#include <vpr/vprConfig.h>

#include <assert.h>
#include <boost/noncopyable.hpp>

#include <vpr/Sync/LockException.h>


namespace vpr
{

/** \class MutexPosix MutexPosix.h vpr/Sync/Mutex.h
 *
 * Mutex wrapper for Win32 systems. Used for critical section protection. This
 * is typedef'd to vpr::Mutex.
 *
 * @date November 7, 1997
 *
 * @note This class was originally part of VR Juggler 1.0 and was brought
 *       back into VPR in version 1.1.36.
 */
class VPR_CLASS_API MutexWin32 : boost::noncopyable
{
public:
   /**
    * Constructor for vpr::MutexWin32 class.
    *
    * @post The mutex variable is initialized and ready for use. The mutex
    *       operations will not fail.
    *
    * @throw vpr::ResourceException is thrown if the mutex cannot be
    *        allocated.
    */
   MutexWin32();

   /**
    * Destructor for vpr::MutexWin32 class.
    *
    * @pre No thread should be in a lock-specific function.
    * @post The mutex variable is destroyed.
    */
   ~MutexWin32()
   {
      // ---- Delete the mutex --- //
      const int result = CloseHandle(mMutex);
      assert(result);
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
      const DWORD dw = WaitForSingleObject(mMutex, INFINITE);
      if ( dw == WAIT_OBJECT_0 )
      {
         mLocked = true;
      }
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
      const DWORD dw = WaitForSingleObject(mMutex, 0);
      if ( dw == WAIT_OBJECT_0 )
      {
         mLocked = true;
         return true;
      }

      return false;
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
    * @throw vpr::LockException Thrown if the current thread was not the
    *                           one that locked this mutex.
    */
   void release()
   {
      const BOOL result = ReleaseMutex(mMutex);

      if (! result)
      {
         throw vpr::LockException(
            "Tried to release a mutex that this thread does not own",
            VPR_LOCATION
         );
      }

      mLocked = false;       // If failure, I will keep it locked
      assert(result);
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

private:
   HANDLE mMutex;

   /**
    * Says whether this mutex is locked.
    *
    * We need this variable because win32 allows multiple acquisitions by the
    * same thread. In order for us to determine if the thread is currently
    * locked we need to test this variable. Notice that the only place this
    * variable is touched is in acquire and release where we implicitly have
    * protection.
    */
   bool mLocked;

   friend class CondVarWin32;
};

} // End of vpr namespace


#endif /* _VPR_MUTEX_WIN32_H_ */
