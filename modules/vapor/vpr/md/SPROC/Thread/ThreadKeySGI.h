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

/*
 * --------------------------------------------------------------------------
 * NOTES:
 *    - This file (vprThreadKeySGI.h) MUST be included by vprThread.h, not
 *      the other way around.
 * --------------------------------------------------------------------------
 */

#ifndef _VPR_THREAD_KEY_SGI_H_
#define _VPR_THREAD_KEY_SGI_H_
//#pragma once

#include <vpr/vprConfig.h>
#include <sys/types.h>

#include <vpr/Thread/ThreadFunctor.h>

namespace vpr {

// Key destructor function type.
typedef thread_func_t	KeyDestructor;

class ThreadKeySGI {
public:
    // -----------------------------------------------------------------------
    //: Constructor.
    // -----------------------------------------------------------------------
    ThreadKeySGI (thread_func_t destructor, void* arg = 0) {
        keycreate(destructor, arg);
    }

    // -----------------------------------------------------------------------
    //: Constructor.
    // -----------------------------------------------------------------------
    ThreadKeySGI (BaseThreadFunctor* destructor) {
        keycreate(destructor);
    }

    // -----------------------------------------------------------------------
    //: Destructor.
    // -----------------------------------------------------------------------
    ~ThreadKeySGI (void) {
        keyfree();
    }

    // -----------------------------------------------------------------------
    //: Allocate a <keyp> that is used to identify data that is specific to
    //+ each thread in the process, is global to all threads in the process
    //+ and is destroyed using the spcefied destructor function that takes a
    //+ single argument.
    //
    //! PRE: None.
    //! POST: A key is created and is associated with the specified
    //+       destructor function and argument.
    //
    //! ARGS: dest_func - The destructor function for the key.  This uses
    //+                   the functor data structure.
    //! ARGS: arg - Argument to be passed to destructor (optional).
    //
    //! RETURNS:  0 - Succeedful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: Use this routine to construct the key destructor function if
    //+       it requires arguments.  Otherwise, use the two-argument
    //+       version of keycreate().
    // -----------------------------------------------------------------------
    int
    keycreate (thread_func_t destructor, void* arg = 0) {
        std::cerr << "vpr::ThreadKeySGI::keycreate() not implemented yet!\n";

        return -1;
    }

    // -----------------------------------------------------------------------
    //: Allocates a <keyp> that is used to identify data that is specific to
    //+ each thread in the process, is global to all threads in the process
    //+ and is destroyed by the specified destructor function.
    //
    //! PRE: None.
    //! POST: A key is created and is associated with the specified
    //+       destructor function.
    //
    //! ARGS: desctructor - Procedure to be called to destroy a data value
    //+                     associated with the key when the thread
    //+                     terminates.
    //
    //! RETURNS:  0 - Succeedful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    int
    keycreate (BaseThreadFunctor* destructor) {
        std::cerr << "vpr::ThreadKeySGI::keycreate() not implemented yet!\n";

        return -1;
    }

    // -----------------------------------------------------------------------
    //: Free up the key so that other threads may reuse it.
    //
    //! PRE: The specified key must have been properly created using the
    //+      keycreate() member function.
    //! POST: The specified key is destroyed using the destructor function
    //+       previously associated with it, and its resources are freed.
    //
    //! RETURNS:  0 - Succeedful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is not currently supported on HP-UX 10.20.
    // -----------------------------------------------------------------------
    int
    keyfree (void) {
        std::cerr << "vpr::ThreadKeySGI::keyfree() not implemented yet!\n";

        return -1;
    }

    // -----------------------------------------------------------------------
    //: Bind value to the thread-specific data key, <key>, for the calling
    //+ thread.
    //
    //! PRE: The specified key must have been properly created using the
    //+      keycreate() member function.
    //! POST: The specified value is associated with the key for the calling
    //+       thread.
    //
    //! ARGS: value - Address containing data to be associated with the
    //+               specified key for the current thread.
    //
    //! RETURNS:  0 - Succeedful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    int
    setspecific (void* value) {
        std::cerr << "vpr::ThreadKeySGI::setspecific() not implemented yet!\n";

        return -1;
    }

    // -----------------------------------------------------------------------
    //: Stores the current value bound to <key> for the calling thread into
    //+ the location pointed to by <valuep>.
    //
    //! PRE: The specified key must have been properly created using the
    //+      keycreate() member function.
    //! POST: The value associated with the key is obtained and stored in the
    //+       pointer valuep so that the caller may work with it.
    //
    //! ARGS: valuep - Address of the current data value associated with the
    //+                key.
    //
    //! RETURNS:  0 - Succeedful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    int
    getspecific (void** valuep) {
        std::cerr << "vpr::ThreadKeySGI::getspecific() not implemented yet!\n";

        return -1;
    }

private:
    int keyID;		//: Thread key ID
};

}; // End of vpr namespace


#endif	/* _VPR_THREAD_KEY_SGI_H_ */
