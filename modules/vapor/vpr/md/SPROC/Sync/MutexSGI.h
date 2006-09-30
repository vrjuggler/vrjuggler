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

#ifndef _VPR_MutexSGI_h_
#define _VPR_MutexSGI_h_

#include <vpr/vprConfig.h>

#include <sstream>
#include <string.h>
#include <ulocks.h>
#include <errno.h>
#include <assert.h>
#include <boost/noncopyable.hpp>

#include <vpr/Sync/LockException.h>


namespace vpr
{

class MemPoolSGI;

/** \class MutexSGI MutexSGI.h vpr/Sync/Mutex.h
 *
 * Mutex wrapper for the SGI systems.  Used for critical section protection.
 * This is typedef'd to vpr::Mutex.
 *
 * @date January 20, 1997
 */
class MutexSGI : boost::noncopyable
{
public:
   /**
    * Constructor for vpr::MutexSGI class.
    *
    * @post The mutex variable is initialized and ready for use. The mutex
    *       operations will not fail.
    *
    * @throw vpr::ResourceException is thrown if the mutex cannot be
    *        allocated.
    */
   MutexSGI();

   /**
    * Destructor for vpr::MutexPosix class.
    *
    * @pre No thread should be in a lock-specific function.
    * @post The mutex variable is destroyed.
    */
   ~MutexSGI();

   /**
    * Locks this mutex.
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller blocks
    *       until the mutex has been freed.
    */
   void acquire()
   {
      assert(mMutex != NULL && "in vpr::MutexSGI::aquire() mMutex is NULL");
      ussetlock(mMutex);
   }

   /**
    * Acquires a read lock.
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller
    *       blocks until the mutex has been freed.
    *
    * @note No special read mutex has been defined for now.
    */
   void acquireRead()
   {
      this->acquire();      // No special "read" semaphore -- For now
   }

   /**
    * Acquires a write lock.
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller blocks
    *       until the mutex has been freed.
    *
    * @note No special write mutex has been defined for now.
    */
   void acquireWrite()
   {
      this->acquire();      // No special "write" semaphore -- For now
   }

   /**
    * Tries to acquire a lock on this mutex (does not block).
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller
    *       returns does not wait for it to be unlocked.
    *
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the mutex is already locked.
    */
   bool tryAcquire()
   {
      // Try 100 spins.
      return uscsetlock(mMutex, 100) == 1;
   }

   /**
    * Tries to acquire a read lock (does not block).
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller
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
    * Tries to acquire a write lock (does not block).
    *
    * @post A lock on this mutex is acquired by the caller.  If a lock has
    *       already been acquired by another process/thread, the caller returns
    *       does not wait for it to be unlocked.
    *
    * @return \c true is returned if the lock is acquired, and \c false is
    *         returned if the mutex is already locked.
    */
   bool tryAcquireWrite()
   {
      return this->tryAcquire();
   }

   /**
    * Releases the mutex.
    *
    * @pre This mutex must be locked.
    * @post This mutex is unlocked.
    *
    * @throw vpr::LockException is thrown if the unlock operation fails.
    */
   void release()
   {
      const int result = usunsetlock(mMutex);

      if ( result != 0 )
      {
         std::ostringstream msg_stream;
         msg_stream << "Mutex unlock operation failed: " << sterror(errno);
         throw vpr::LockException(msg_stream.str(), VPR_LOCATION);
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
   bool test() const
   {
      return ustestlock(const_cast<ulock_t>(mMutex));
   }

   /**
    * Dumps the mutex debug stuff and current state.
    *
    * @post All important data and debugging information related to the
    *       mutex are dumped to the specified file descriptor (or to stderr
    *       if none is given).
    *
    * @param dest    File descriptor to which the output will be written.
    *                It defaults to stderr if no descriptor is specified.
    * @param message Message printed out before the output is dumped.
    */
   void dump(FILE* dest = stdout,
             const char* message = "\n------ Mutex Status -----\n") const
   {
      usdumplock(mMutex, dest, message);
   }

protected:
   ulock_t mMutex;

   static MemPoolSGI* mMutexPool;
   static int* mAttachedCounter;
};

} // End of vpr namespace


#endif /* _VPR_MutexSGI_h_ */
