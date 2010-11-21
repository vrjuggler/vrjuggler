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

#ifndef _VPR_NULL_MUTEX_H_
#define _VPR_NULL_MUTEX_H_

#include <vpr/vprConfig.h>
#include <stdio.h>
#include <boost/noncopyable.hpp>


namespace vpr
{

/** \class NullMutex NullMutex.h vpr/Sync/NullMutex.h
 *
 * Null mutex wrapper.  Used to pass a do nothing mutex as a template type.
 *
 * @date January 21, 1997
 */
class NullMutex : boost::noncopyable
{
public:
   NullMutex()
   {;}

   ~NullMutex()
   {;}

   /**
    * Locks the mutex.
    */
   void acquire()
   {
      /* Do nothing. */ ;
   }

   /** Acquires a read mutex. */
   void acquireRead()
   {
      this->acquire();     // No special "read" semaphore -- For now
   }

   /** Acquires a write mutex. */
   void acquireWrite()
   {
      this->acquire();     // No special "write" semaphore -- For now
   }

   /**
    * Tries to acquire the lock.  Returns immediately even if we don't acquire
    * the lock.
    *
    * @return \c true is always returned.
    */
   bool tryAcquire()
   {
      return true;
   }

   /** Tries to acquire a read mutex. */
   bool tryAcquireRead()
   {
      return this->tryAcquire();
   }

   /** Tries to acquire a write mutex. */
   bool tryAcquireWrite()
   {
      return this->tryAcquire();
   }

   /**
    * Releases the mutex.
    */
   void release()
   {
      /* Do nothing. */ ;
   }

   /**
    * Tests the current lock status.
    *
    * @return \c false is always returned.
    */
   bool test() const
   {
      return false;     // Just return false since it is a null lock
   }

   /** Dumps the mutex debug stuff and current state. */
   void dump(FILE* dest = stderr,
             const char* message = "\n------ Mutex Dump -----\n") const
   {
      fprintf(dest, "%sNULL Mutex", message);
   }
};

} // End of vpr namespace


#endif
