/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#include <vjConfig.h>

#include <Sync/vjMutexPosix.h>


// ---------------------------------------------------------------------------
// Test the current lock status.
//
// PRE: None.
// POST: The state of the mutex variable is returned.
//
// RETURNS: 0 - Not locked
// RETURNS: 1 - Locked
// ---------------------------------------------------------------------------
int
vjMutexPosix::test (void) {
    int ret_val;

    ret_val = pthread_mutex_trylock(&mMutex);

    // If the return value from pthread_mutex_trylock() is 0, then this
    // process now has a lock on mutex.  Therefore, no other process could
    // have held a lock on it, so unlock the mutex and return 0.
    if ( ret_val == 0 ) {
        pthread_mutex_unlock(&mMutex);
        ret_val = 0;
    }
    // The mutex is currently locked by some thread if ret_val is non-zero.
    else {
        ret_val = 1;
    }

    return ret_val;
}
