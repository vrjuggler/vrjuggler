/*
 * ---------------------------------------------------------------------------
 * vjThreadPosix.cpp
 * $Revision$
 * $Date$
 * ---------------------------------------------------------------------------
 */

#include <vjConfig.h>
#include <iostream.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include <Threads/vjThread.h>


thread_id_t vjThreadPosix::thread_count = 1;
hash_map<caddr_t, thread_id_t, hash<caddr_t>, eq_thread> vjThreadPosix::mPthreadHash;
vjThreadTable<thread_id_t> vjThreadPosix::mThreadTable;

typedef struct sched_param	sched_param_t;


// ---------------------------------------------------------------------------
//: Spawning constructor
//
//  This will actually start a new thread that will execute the specified
//+ function.
// ---------------------------------------------------------------------------
vjThreadPosix::vjThreadPosix (THREAD_FUNC func, void* arg, long flags,
                              u_int priority, void* stack_addr,
                              size_t stack_size)
{
    vjThreadManager* vj_tm_inst;

    vj_tm_inst = vjThreadManager::instance();

    vj_tm_inst->lock();
    {
        int ret_val;
        vjThreadNonMemberFunctor* NonMemFunctor;

        NonMemFunctor = new vjThreadNonMemberFunctor(func, arg);

        ret_val = spawn(NonMemFunctor, flags, priority, stack_addr,
                        stack_size);
        checkRegister(ret_val);
    }
    vj_tm_inst->unlock();
}

// ---------------------------------------------------------------------------
// Spawning constructor with arguments (functor version).
//
// This will start a new thread that will execute the specified function.
// ---------------------------------------------------------------------------
vjThreadPosix::vjThreadPosix (vjBaseThreadFunctor* functorPtr, long flags,
                              u_int priority, void* stack_addr,
                              size_t stack_size)
{
    vjThreadManager* vj_tm_inst;

    vj_tm_inst = vjThreadManager::instance();

    vj_tm_inst->lock();
    {
        int ret_val;

        ret_val = spawn(functorPtr, flags, priority, stack_addr, stack_size);
        checkRegister(ret_val);
    }
    vj_tm_inst->unlock();
}

// ---------------------------------------------------------------------------
// Destructor.
//
// PRE: None.
// POST: This thread is removed from the thread table and from the local
//       thread hash.
// ---------------------------------------------------------------------------
vjThreadPosix::~vjThreadPosix (void) {
    vjPthreadObj me;
    int status;

    mThreadTable.removeThread(gettid());
    me.obj = pthread_self();
    mPthreadHash.erase((caddr_t) &me);

    status = 0;
    pthread_exit((void*) &status);
}

// ---------------------------------------------------------------------------
// Create a new thread that will execute functorPtr.
//
// PRE: None.
// POST: A thread (with any specified attributes) is created that begins
//       executing func().  Depending on the scheduler, it may begin
//       execution immediately, or it may block for a short time before
//       beginning execution.
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

    // If thread priority scheduling is available, set the thread's priority
    // if it is set to be higher than 0.
#   ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
        pthread_attr_setscope(&thread_attrs, VJ_THREAD_SCOPE);

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
    ret_val = pthread_create(&(mThread.obj), thread_attrs,
                             (pthread_startroutine_t) ThreadFunctorFunction,
                             (pthread_addr_t) functorPtr);
#else
    ret_val = pthread_create(&(mThread.obj), &thread_attrs,
                             ThreadFunctorFunction, (void *) functorPtr);
#endif

    // Inform the caller if the thread was not created successfully.
    if ( ret_val != 0 ) {
        perror("vjThreadPosix::spawn() - Cannot create thread");
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

    ret_val = pthread_getschedparam(mThread.obj, &policy, &fifo_sched_param);
    *prio = fifo_sched_param.sched_priority;

    return ret_val;
#else
    cerr << "vjThreadPosix::getPrio(): Not supported\n";

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

    return pthread_setschedparam(mThread.obj, SCHED_FIFO, &fifo_sched_param);
#else
    cerr << "vjThreadPosix::setPrio(): Not supported\n";

    return -1;
#endif
}

// ===========================================================================
// Private methods follow.
// ===========================================================================

// ---------------------------------------------------------------------------
//: Check the status of the thread creation in order to determine if this
//+ thread should be registered in the thread table or not.
//
//! PRE: An attempt must have been made to create a thread using spawn().
//! POST: If status is 0, the thread gets registered in the thread table and
//+       in the local thread hash.  The count of created threads is
//+       incremented as well.
//
//! ARGS: status - The integer status returned by spawn().
// ---------------------------------------------------------------------------
void
vjThreadPosix::checkRegister (int status) {
    if ( status == 0 ) {
        mThread.id = thread_count;
        mPthreadHash[(caddr_t) &mThread] = mThread.id;
        registerThread(true);
        mThreadTable.addThread(this, mThread.id);
        thread_count++;
    } else {
        registerThread(false);	// Failed to create
    }
}

// ---------------------------------------------------------------------------
//: Get this thread's ID (a non-decreasing, positive number strictly greater
//+ than 0).
//
//! PRE: None.
//! POST: If this thread is found in the local thread hash, its unique ID is
//        returned to the caller.
//
//! RETURNS: 0 - This thread was not found in the thread hash.
//! RETURNS: Nonzero - The ID of this tread.
// ---------------------------------------------------------------------------
thread_id_t
vjThreadPosix::gettid (void) {
    vjPthreadObj me;
    hash_map<caddr_t, thread_id_t, hash<caddr_t>, eq_thread>::iterator i;

    // Find me in the local thread hash.
    me.obj = pthread_self();
    i = mPthreadHash.find((caddr_t) &me);

    if ( i == mPthreadHash.end() ) {
        return 0;		// There is no thread with ID 0
    } else {
        return (*i).second;
    }
}
