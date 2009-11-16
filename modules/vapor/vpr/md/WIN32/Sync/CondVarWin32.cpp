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

// This must be defined to 0x0400 or higher in order to have the function
// SignalObjectAndWait() declared.
#define _WIN32_WINNT 0x0400

#include <vpr/vprConfig.h>

#include <sstream>
#include <limits>
#include <assert.h>

#include <vpr/Sync/LockException.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Error.h>
#include <vpr/Util/ResourceException.h>
#include <vpr/Util/IllegalArgumentException.h>
#include <vpr/md/WIN32/Sync/CondVarWin32.h>


namespace vpr
{

CondVarWin32::CondVarWin32(MutexWin32* mutex)
   : mWaitersCount(0)
   , mSema(NULL)
   , mWaitersDone(NULL)
   , mWasBroadcast(false)
   , mCondMutex(NULL)
{
   // If the caller did not specify a mutex variable to use with the
   // condition variable, use mDefaultMutex.
   if ( NULL == mutex )
   {
      mutex = &mDefaultMutex;
   }

   // Create an unnamed semaphore with no security initially set to 0 with a
   // maximum count of 0x7fffffff.
   mSema = CreateSemaphore(NULL, 0, 0x7fffffff, NULL);

   if ( NULL == mSema )
   {
      const std::string err_msg(vpr::Error::getCurrentErrorMsg());

      std::ostringstream msg_stream;
      msg_stream << "Condition variable allocation failed: " << err_msg;
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }

   InitializeCriticalSection(&mWaitersCountLock);

   // Create an unnamed event with no security that resets automatically and
   // is initially non-signaled.
   mWaitersDone = CreateEvent(NULL, FALSE, FALSE, NULL);

   if ( NULL == mWaitersDone )
   {
      const std::string err_msg(vpr::Error::getCurrentErrorMsg());

      int result = CloseHandle(mSema);
      assert(result);

      std::ostringstream msg_stream;
      msg_stream << "Condition variable allocation failed: " << err_msg;
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }

   mCondMutex = mutex;
}

CondVarWin32::~CondVarWin32()
{
   int result(0);
   result = CloseHandle(mSema);
   assert(result);
   result = CloseHandle(mWaitersDone);
   assert(result);

   mSema        = NULL;
   mWaitersDone = NULL;
}

// From the article:
//
//    "This implementation [...] ensures that the external mutex is held until
//    all threads waiting on [this condition variable] have a chance to wait
//    again on the external mutex before returning to their callers. This
//    solution relies on the fact that Windows NT mutex requests are queued in
//    FIFO order, rather than in, e.g., priority order. Because the external
//    mutex queue is serviced in FIFO order, all waiting threads will acquire
//    the external mutex before any of them can reacquire it a second time.
//    This property is essential to ensure fairness."
bool CondVarWin32::wait(const vpr::Interval& timeToWait)
{
   // If not locked ...
   if ( ! mCondMutex->test() )
   {
      std::ostringstream msg_stream;
      msg_stream << "Condition variable mutex must be locked before calling "
                 << "wait()";
      throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
   }

   bool status(true);

   EnterCriticalSection(&mWaitersCountLock);
   ++mWaitersCount;
   LeaveCriticalSection(&mWaitersCountLock);

   // Wait indefinitely on the condition variable by default.
   DWORD timeout(INFINITE);

   // If the caller specified timeout value other than NoTimeout, then this
   // call will not block longer than that timeout.
   if ( vpr::Interval::NoTimeout != timeToWait )
   {
      timeout = timeToWait.msec();
   }

   int result(0);

   // SignalObjectAndWait unlocks the given mutex and waits on the semaphore
   // until signal() or broadcast() is called by another thread. This direct
   // manipulation of mCondMutex->mLocked is safe because this thread holds
   // the mutex when we make the change.
   mCondMutex->mLocked = false;
   result = SignalObjectAndWait(mCondMutex->mMutex, mSema, timeout, FALSE);

   // If we did not wait infinitely on SignalObjectAndWait(), then we need to
   // determine if the call timed out or if we got the lock on mSema.
   if ( INFINITE != timeout )
   {
      assert(result != WAIT_FAILED && result != WAIT_ABANDONED);
      status = (result == WAIT_OBJECT_0);
   }

   // At this point, if status is true, then we had better have the lock on
   // mSema.
   if ( status )
   {
      assert(result == WAIT_OBJECT_0);

      EnterCriticalSection(&mWaitersCountLock);

      // We are not waiting now.
      --mWaitersCount;

      // Check to see if w3e are the last waiter after broadcast().
      const bool last_waiter = mWasBroadcast && mWaitersCount == 0;

      LeaveCriticalSection(&mWaitersCountLock);

      // If we are the last waiter thread during this broadcast, then let all
      // the other threads proceed.
      if ( last_waiter )
      {
         // This atomically signals the mWaitersDone event and waits until it
         // can acquire the given mutex. This is required to ensure fairness.
         SignalObjectAndWait(mWaitersDone, mCondMutex->mMutex, INFINITE,
                             FALSE);
         mCondMutex->mLocked = true;
      }
      else
      {
         // Always regain the external mutex sing that is the guarantee that
         // we give to our callers.
         mCondMutex->acquire();
      }
   }
   else
   {
      // Always regain the external mutex sing that is the guarantee that we
      // give to our callers.
      mCondMutex->acquire();
   }

   assert(mCondMutex->test() && "External mutex is supposed to be locked");

   return status;
}

void CondVarWin32::signal()
{
   if ( ! mCondMutex->test() )
   {
      throw vpr::LockException("Condition variable mutex must be locked",
                               VPR_LOCATION);
   }

   EnterCriticalSection(&mWaitersCountLock);
   const bool have_waiters = mWaitersCount > 0;
   LeaveCriticalSection(&mWaitersCountLock);

   if ( have_waiters )
   {
      ReleaseSemaphore(mSema, 1, 0);
   }
}

// From the article:
//
//    "[This] function is more complex and requires two steps:
//    
//       1. It wakes up all the threads waiting on [mSema], which can be done
//          atomically by passing [mWaitersCount] to ReleaseSemaphore().
//       2. It then blocks on the auto-reset [mWaitersDone] event until the
//          last thread in the group of waiting threads exits the [wait()]
//          critical section."
void CondVarWin32::broadcast()
{
   // ASSERT: We have been locked
   if ( ! mCondMutex->test() )
   {
      throw vpr::LockException("Condition variable mutex must be locked",
                               VPR_LOCATION);
   }

   // Protect access to mWaitersCount and mWasBroadcast.
   EnterCriticalSection(&mWaitersCountLock);
   bool have_waiters(false);

   if ( mWaitersCount > 0 )
   {
      // We are broadcasting even if there was just one waiter.
      mWasBroadcast = true;
      have_waiters = true;
   }

   if ( have_waiters )
   {
      // Wake up all the waiters atomically.
      ReleaseSemaphore(mSema, mWaitersCount, 0);
      LeaveCriticalSection(&mWaitersCountLock);

      // Wait for all the awakened threads to acquire the semaphore.
      WaitForSingleObject(mWaitersDone, INFINITE);

      // This is safe because no other waiter threads can wake up to access
      // it.
      mWasBroadcast = false;
   }
   else
   {
      LeaveCriticalSection(&mWaitersCountLock);
   }
}

} // End of vpr namespace
