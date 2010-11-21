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

#ifndef _VPR_SEMAPHORE_WIN32_H_
#define _VPR_SEMAPHORE_WIN32_H_

#include <vpr/vprConfig.h>

#include <assert.h>
#include <boost/concept_check.hpp>
#include <boost/noncopyable.hpp>


namespace vpr
{

/** \class SemaphorePosix SemaphorePosix.h vpr/Sync/Semaphore.h
 *
 * Semaphore wrapper for Win32 systems.  This is typedef'd to vpr::Semaphore.
 *
 * @date November 9, 1997
 *
 * @note This class was originally part of VR Juggler 1.0 and was brought
 *       back into VPR in version 1.1.36.
 */
class VPR_CLASS_API SemaphoreWin32 : boost::noncopyable
{
public:
   /**
    * Constructor for vpr::SemaphoreWin32 class.
    *
    * @param initialValue The initial number of resources controlled by
    *                     the semaphore.  If not specified, the default
    *                     value is 1.
    *
    * @throw vpr::ResourceException is thrown if the semaphore cannot be
    *        allocated.
    */
   SemaphoreWin32(const int initialValue = 1);

   /**
    * Destructor for vpr::SemaphorePosix class.
    *
    * @post The resources used by the semaphore variable are freed.
    */
   ~SemaphoreWin32();

   /**
    * Locks this semaphore.
    *
    * @post The calling thread either acquires the semaphore until
    *       release() is called, or the caller is put at the tail of a wait
    *       and is suspended until such time as it can be freed and allowed
    *       to acquire the semaphore itself.
    *
    * @throw vpr::DeadlockException is thrown if the current thread has
    *        already locked this semaphore.
    */
   void acquire()
   {
      const DWORD result = WaitForSingleObject(mSema, INFINITE);
      assert(result == WAIT_OBJECT_0);
      boost::ignore_unused_variable_warning(result);
   }

   /**
    * Acquires a read lock on this semaphore.
    *
    * @post The calling thread either acquires the semaphore until
    *       release() is called, or the caller is put at the tail of a wait
    *       and is suspended until such time as it can be freed and allowed
    *       to acquire the semaphore itself.
    *
    * @note There is no special read lock for now.
    */
   void acquireRead()
   {
      this->acquire();
   }

   /**
    * Acquires a write lock on this semaphore.
    *
    * @post The calling thread either acquires the semaphore until
    *       release() is called, or the caller is put at the tail of a wait
    *       and is suspended until such time as it can be freed and allowed
    *       to acquire the semaphore itself.
    *
    * @note There is no special write lock for now.
    */
   void acquireWrite()
   {
      this->acquire();
   }

   /**
    * Tries to acquire the semaphore immediately (does not block).
    *
    * @post If the semaphore could be acquired by the caller, the caller
    *       gets control of the semaphore.  If the semaphore was already
    *       locked, the routine returns immediately without suspending the
    *       calling thread.
    *
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the semaphore is already locked.
    *
    * @throw vpr::DeadlockException is thrown if the current thread has
    *        already locked this semaphore.
    */
   bool tryAcquire()
   {
      const DWORD dw = WaitForSingleObject(mSema, 0);
      return dw == WAIT_OBJECT_0;
   }

   /**
    * Tries to acquire a read lock on this semaphore (does not block).
    *
    * @post If the semaphore could be acquired by the caller, the caller
    *       gets control of the semaphore.  If the semaphore was already
    *       locked, the routine returns immediately without suspending the
    *       calling thread.
    *
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the semaphore is already locked.
    */
   bool tryAcquireRead()
   {
      return this->tryAcquire();
   }

   /**
    * Tries to acquire a write lock on this semaphore (does not block).
    *
    * @post If the semaphore could be acquired by the caller, the caller
    *       gets control of the semaphore.  If the semaphore was already
    *       locked, the routine returns immediately without suspending the
    *       calling thread.
    *
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the semaphore is already locked.
    */
   bool tryAcquireWrite()
   {
      return this->tryAcquire();
   }

   /**
    * Releases this semaphore.
    *
    * @pre The semaphore should have been locked before being released.
    * @post The semaphore is released and the thread at the haed of the
    *       wait queue is allowed to execute again.
    */
   void release()
   {
      const BOOL result = ReleaseSemaphore(mSema, 1, NULL);
      assert(result);
      boost::ignore_unused_variable_warning(result);
   }

   /**
    * Resets the semaphore using the given value.
    *
    * @post The semaphore's count is set to the specified value.
    *
    * @param val The value to which the semaphore is reset.
    *
    * @throw vpr::ResourceException is thrown if the semaphore cannot be
    *        reset.
    *
    * @note If processes are waiting on the semaphore, the results are
    *       undefined.
    */
   void reset(const int val);

   //---------------------------------------------------------
   //: Dump the semaphore debug stuff and current state.
   //---------------------------------------------------------
   void dump(FILE* dest = stderr,
             const char* message = "\n------ Semaphore Dump -----\n") const
   {
      std::cout << "vpr::SemaphoreWin32::dump()\nNot implemented on Win32"
                << std::endl;
   }

protected:
   HANDLE mSema;
};

} // End of vpr namespace


#endif
