/*
 * --------------------------------------------------------------------------
 * vjThreadKeyPosix.h
 * $Revision$
 * $Date$
 * --------------------------------------------------------------------------
 * NOTES:
 *    - This file (vjThreadKeyPosix.h) MUST be included by vjThread.h, not the
 *      other way around.
 * --------------------------------------------------------------------------
 */

#ifndef _THREAD_KEY_POSIX_H_
#define _THREAD_KEY_POSIX_H_

#include <vjConfig.h>
#include <pthread.h>
#include <sys/types.h>

#include <Threads/vjThreadFunctor.h>


// Key destructor function type.
#ifdef _PTHREADS_DRAFT_4
    typedef pthread_destructor_t	vjKeyDestructor;
#else
    typedef THREAD_FUNC			vjKeyDestructor;
#endif


//:

class vjThreadKeyPosix {
public:
    // -----------------------------------------------------------------------
    //: Constructor.
    // -----------------------------------------------------------------------
    vjThreadKeyPosix (THREAD_FUNC destructor, void* arg = 0) {
        keycreate(destructor, arg);
    }

    // -----------------------------------------------------------------------
    //: Constructor.
    // -----------------------------------------------------------------------
    vjThreadKeyPosix ( vjBaseThreadFunctor* destructor) {
        keycreate(destructor);
    }

    // -----------------------------------------------------------------------
    //: Destructor.
    // -----------------------------------------------------------------------
    vjThreadKeyPosix (void) {
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
    //+       it requires arguments.  Otherwise, use the two-argument version
    //+       of keycreate().
    // -----------------------------------------------------------------------
    int
    keycreate (THREAD_FUNC destructor, void* arg = 0) {
        vjThreadNonMemberFunctor *NonMemFunctor = new vjThreadNonMemberFunctor(destructor, arg);

        return keycreate(NonMemFunctor);
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
#ifdef _PTHREADS_DRAFT_4
        return pthread_keycreate(&keyID, (vjKeyDestructor) destructor);
#else
        return pthread_key_create(&keyID, (vjKeyDestructor) destructor);
#endif
    }

    // -----------------------------------------------------------------------
    //: Free up this key so that other threads may reuse it.
    //
    //! PRE: This key must have been properly created using the keycreate()
    //+      member function.
    //! POST: This key is destroyed using the destructor function previously
    //+       associated with it, and its resources are freed.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is not currently supported on HP-UX 10.20.
    // -----------------------------------------------------------------------
    int
    keyfree (void) {
#ifdef _PTHREADS_DRAFT_4
        cerr << "keyfree() not supported with this POSIX threads "
             << "implementation\n";

        return -1;
#else
        return pthread_key_delete(keyID);
#endif
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
        return pthread_setspecific(keyID, value);
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
#ifdef _PTHREADS_DRAFT_4
        return pthread_getspecific(keyID, valuep);
#else
        *valuep = pthread_getspecific(keyID);

        return 0;
#endif
    }

private:
    pthread_key_t keyID;		//: Thread key ID
};

#endif	/* _THREAD_KEY_POSIX_H_ */
