/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


/*
 * --------------------------------------------------------------------------
 * NOTES:
 *    - This file (vjThreadKeyWin32.h) MUST be included by vjThread.h, not
 *      the other way around.
 * --------------------------------------------------------------------------
 */

#ifndef _VJ_THREAD_KEY_WIN_32_H_
#define _VJ_THREAD_KEY_WIN_32_H_

#include <iostream.h>
#include <sys/types.h>

#include <Threads/vjThreadFunctor.h>

// Key destructor function type.
typedef THREAD_FUNC	vjKeyDestructor;

class vjThreadKeyWin32 {
public:
    // -----------------------------------------------------------------------
    //: Constructor.
    // -----------------------------------------------------------------------
    vjThreadKeyWin32 (THREAD_FUNC destructor, void* arg = 0) {
        keycreate(destructor, arg);
    }

    // -----------------------------------------------------------------------
    //: Constructor.
    // -----------------------------------------------------------------------
    vjThreadKeyWin32 ( vjBaseThreadFunctor* destructor) {
        keycreate(destructor);
    }

    // -----------------------------------------------------------------------
    //: Destructor.
    // -----------------------------------------------------------------------
    ~vjThreadKeyWin32 (void) {
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
    //! ARGS: arg - Argument to be passed to THREAD_FUNC (optional).
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: Use this routine to construct the key destructor function if
    //+       it requires arguments.  Otherwise, use the two-argument
    //+       version of keycreate().
    // -----------------------------------------------------------------------
    int
    keycreate (THREAD_FUNC destructor, void* arg = 0) {
        cerr << "vjThreadKeyWin32::keycreate() not implemented yet!\n";

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
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    int
    keycreate ( vjBaseThreadFunctor* destructor) {
        cerr << "vjThreadKeyWin32::keycreate() not implemented yet!\n";

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
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is not currently supported on HP-UX 10.20.
    // -----------------------------------------------------------------------
    int
    keyfree (void) {
        cerr << "vjThreadKeyWin32::keyfree() not implemented yet!\n";

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
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    int
    setspecific (void* value) {
        cerr << "vjThreadKeyWin32::setspecific() not implemented yet!\n";

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
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    int
    getspecific (void** valuep) {
        cerr << "vjThreadKeyWin32::getspecific() not implemented yet!\n";

        return -1;
    }

private:
    int keyID;		//: Thread key ID
};

#endif	/* _VJ_THREAD_KEY_WIN_32_H_ */
