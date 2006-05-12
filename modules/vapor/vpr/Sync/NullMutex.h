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

#ifndef _vprNullMutex_h_
#define _vprNullMutex_h_

#include <vpr/vprConfig.h>
#include <stdio.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr
{

/** \class NullMutex NullMutex.h vpr/Sync/NullMutex.h
 *
 * Null mutex wrapper.  Used to pass a do nothing mutex as a template type.
 *
 * @date January 21, 1997
 */
class VPR_CLASS_API NullMutex
{
public:
   NullMutex()
   {;}

   ~NullMutex()
   {;}

   /**
    * Locks the mutex.
    *
    * @return 1 is returned if the mutex is acquired.
    * @return -1 is returned if an error occurs.
    */
   vpr::ReturnStatus acquire() const
   {
      return vpr::ReturnStatus();
   }

   /** Acquires a read mutex. */
   vpr::ReturnStatus acquireRead() const
   {
      return this->acquire();     // No special "read" semaphore -- For now
   }

   /** Acquires a write mutex. */
   vpr::ReturnStatus acquireWrite() const
   {
      return this->acquire();     // No special "write" semaphore -- For now
   }

   /**
    * Tries to acquire the lock.  Returns immediately even if we don't acquire
    * the lock.
    *
    * @return 1 is returned if the mutex is acquired.
    * @return 0 is returned if the mutex is not acquired.
    */
   vpr::ReturnStatus tryAcquire() const
   {
      return vpr::ReturnStatus();
   }

   /** Tries to acquire a read mutex. */
   vpr::ReturnStatus tryacquire_read() const
   {
      return this->tryAcquire();
   }

   /** Tries to acquire a write mutex. */
   vpr::ReturnStatus tryacquire_write() const
   {
      return this->tryAcquire();
   }

   /**
    * Releases the mutex.
    *
    * @return 0 is returned on success; -1 is returned otherwise.
    */
   vpr::ReturnStatus release() const
   {
      return vpr::ReturnStatus();
   }

   /**
    * Tests the current lock status.
    *
    * @return 0 is returned if the mutex is not locked.
    * @return 1 is returend if the mutex is locked.
    */
   int test()
   {
      return 0;     // Just return 0 since it is a null lock
   }

   /** Dumps the mutex debug stuff and current state. */
   void dump(FILE* dest = stderr,
             const char* message = "\n------ Mutex Dump -----\n") const
   {
      fprintf(dest, "%sNULL Mutex", message);
   }

protected:
   // = Prevent assignment and initialization.
   void operator= (const NullMutex &) {}
   NullMutex (const NullMutex &) {}
};

} // End of vpr namespace


#endif
