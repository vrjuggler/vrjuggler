/*
 * ---------------------------------------------------------------------------
 * vjThreadPosix.C
 * $Revision$
 * $Date$
 * ---------------------------------------------------------------------------
 */

#include <config.h>
#include <iostream.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <Threads/vjThread.h>
#include <Threads/vjThreadPosix.h>


typedef struct sched_param	sched_param_t;


// ---------------------------------------------------------------------------
//: Create a new thread that will execute functorPtr.
//
//! PRE: None.
//! POST: A thread (with any specified attributes) is created that begins
//+      executing func().  Depending on the scheduler, it may begin
//+      execution immediately, or it may block for a short time before
//+      beginning execution.
// ---------------------------------------------------------------------------
int
vjThreadPosix::spawn (vjBaseThreadFunctor* functorPtr, long flags,
                       u_int priority, void* stack_addr, size_t stack_size)
{
    int ret_val;
    pthread_attr_t thread_attrs;

    // Initialize thread_attrs and set the priority of the thread if it is
    // supported.  HP-UX requires a slightly different syntax than other
    // operating systems.
#ifdef _PTHREADS_DRAFT_4
    pthread_attr_create(&thread_attrs);

#   ifdef _POSIX_THREAD_REALTIME_SCHEDULING
        if ( priority > 0 ) {
            pthread_attr_setprio(&thread_attrs, priority);
        }
#   endif	/* _POSIX_THREAD_REALTIME_SCHEDULING */

#else
    sched_param_t prio_param;

    pthread_attr_init(&thread_attrs);

    // If this pthreads implementation supports it and an alternate address
    // is specified, set the stack address for the thread.
#   ifdef _POSIX_THREAD_ATTR_STACKADDR
        if ( stack_addr != NULL && stack_addr > 0 ) {
            pthread_attr_setstackaddr(&thread_attrs, stack_addr);
        }
#   endif	/* _POSIX_THREAD_ATTR_STACKADDR */

#   ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
        pthread_attr_setscope(&thread_attrs, PTHREAD_SCOPE_SYSTEM);

        if ( priority > 0 ) {
            prio_param.sched_priority = priority;
            pthread_attr_setschedparam(&thread_attrs, &prio_param);
        }
#   endif	/* _POSIX_THREAD_PRIORITY_SCHEDULING */

#endif	/* _PTHREADS_DRAFT_4 */

    // Set the stack size if a value greater than 0 is specified and this
    // pthreads implementation supports it.  Ensure that
    // _POSIX_THREAD_ATTR_STACKSIZE is defined before trying to test its
    // value.
#ifdef _POSIX_THREAD_ATTR_STACKSIZE
    if ( stack_size > 0 ) {
        // ** STACK SIZE CHECK NEEDED **

        pthread_attr_setstacksize(&thread_attrs, stack_size);
    }
#endif

    // Finally create the thread.
#ifdef _PTHREADS_DRAFT_4
    ret_val = pthread_create(&threadID, thread_attrs,
                             (pthread_startroutine_t) ThreadFunctorFunction,
                             (pthread_addr_t) functorPtr);
#else
    ret_val = pthread_create(&threadID, &thread_attrs, &ThreadFunctorFunction,
                             (void *) functorPtr);
#endif

    // Inform the caller if the thread was not created successfully.
    if ( ret_val != 0 ) {
        perror("vjThreadPosix::create() - Cannot create thread");
    }

    return ret_val;
}





// ---------------------------------------------------------------------------
// Get this thread's priority.
//
// PRE: None.
// POST: The priority of this thread is returned in the integer pointer
//       variable.
// ---------------------------------------------------------------------------
int
vjThreadPosix::getPrio (int* prio) {
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
    int policy, ret_val;
    sched_param_t fifo_sched_param;

    ret_val = pthread_getschedparam(threadID, &policy, &fifo_sched_param);
    *prio = fifo_sched_param.sched_priority;

    return ret_val;
#else
    cerr << "vjThreadPosix::getprio(): Not supported\n";

    return -1;
#endif
}

// ---------------------------------------------------------------------------
// Set this thread's priority.
//
// PRE: None.
// POST: This thread has its priority set to the specified value.
// ---------------------------------------------------------------------------
int
vjThreadPosix::setPrio (int prio) {
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
    sched_param_t fifo_sched_param;

    fifo_sched_param.sched_priority = prio;

    return pthread_setschedparam(threadID, SCHED_FIFO, &fifo_sched_param);
#else
    cerr << "vjThreadPosix::setprio(): Not supported\n";

    return -1;
#endif
}
