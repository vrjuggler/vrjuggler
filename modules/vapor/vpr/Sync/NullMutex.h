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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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


namespace vpr {

/**
 * Null mutex wrapper.  Used to pass a do nothing mutex as a template type.
 *
 * @author Allen Bierbaum
 * @date 1-21-97
 */
class VPR_CLASS_API NullMutex
{
public:
    NullMutex () {}
    ~NullMutex() {}


    /**
     * Locks the mutex.
     *
     * @return 1 is returned if the mutex is acquired.<br>
     *         -1 is returned if an error occurs.
     */
    int acquire() const
    {
        return 1;
    }

    /// Acquires a read mutex.
    int acquireRead() const
    {
        return this->acquire();	    // No special "read" semaphore -- For now
    }

    /// Acquires a write mutex.
    int acquireWrite() const
    {
        return this->acquire();	    // No special "write" semaphore -- For now
    }

    /**
     * Try to acquire the lock.  Returns immediately even if we don't acquire
     * the lock.
     *
     * @return 1 is returned if the mutex is acquired.<br>
     *         0 is returned if the mutex is not acquired.
     */
    int tryAcquire () const
    {
        return 1;	    // Try 100 spins.
    }

    /// Tries to acquire a read mutex.
    int tryacquire_read () const
    {
        return this->tryAcquire();
    }

    /// Tries to acquire a write mutex.
    int tryacquire_write () const
    {
        return this->tryAcquire();
    }

    /**
     * Release the mutex.
     *
     * @return 0 is returned on success; -1 is returned otherwise.
     */
    int release() const
    {
        return 0;
    }

    /**
     * Tests the current lock status.
     *
     * @return 0 is returned if the mutex is not locked.<br>
     *         1 is returend if the mutex is locked.
     */
    int test()
    {
        return 0;     // Just return 0 since it is a null lock
    }

    /// Dumps the mutex debug stuff and current state.
    void dump (FILE* dest = stderr, const char* message = "\n------ Mutex Dump -----\n") const
    {
       std::cerr << message << "NULL Mutex\n";
    }

protected:
    // = Prevent assignment and initialization.
    void operator= (const NullMutex &) {}
    NullMutex (const NullMutex &) {}
};

}; // End of vpr namespace


#endif
