
#ifndef _VJ_THREAD_H_
#define _VJ_THREAD_H_

#include <config.h>
#include <unistd.h>


// Typedefs to help with cross-platform abilities
typedef void (*THREAD_FUNC)(void *);

#ifdef VJ_SGI_IPC    // ---- SGI IPC Barrier ------ //
    typedef struct cancel_state	cancel_state_t;

#   include <Threads/vjThreadSGI.h>
#   include <Threads/vjThreadKeySGI.h>

    typedef class vjThreadSGI	vjThreadId;
    typedef class vjThreadKeySGI	vjKeyId;

    // This is used for making calls to static class member functions.
    typedef class vjThreadSGI	ThreadClass;
#else
#ifdef WIN32
#   include <windows.h>
#   include <process.h>
#   include <Threads/vjThreadWin32.h>
#   include <Threads/vjThreadKeyWin32.h>
    typedef vjThreadWin32 vjThreadId;
	typedef vjThreadKeyWin32 vjKeyId;
	typedef vjThreadWin32 ThreadClass;
#else
#ifdef VJ_USE_PTHREADS
    typedef int		cancel_state_t;

#   define _POSIX_C_SOURCE 2
#   include <Threads/vjThreadPosix.h>
#   include <Threads/vjThreadKeyPosix.h>

    typedef class vjThreadPosix		vjThreadId;
    typedef class vjThreadKeyPosix	vjKeyId;

    // This is used for making calls to static class member functions.
    typedef class vjThreadPosix		ThreadClass;
#else
#   include <Threads/vjThreadFunctor.h>
#endif	/* VJ_USE_PTHREADS */
#endif	/* WIN32 */
#endif	/* VJ_SGI_IPC */


#define THREAD_BOUND               0x00000001
#define THREAD_NEW_LWP             0x00000002
#define THREAD_DETACHED            0x00000040
#define THREAD_SUSPENDED           0x00000080
#define THREAD_DAEMON              0x00000100
#define THREAD_JOINABLE            0x00010000
#define THREAD_SCHED_FIFO          0x00020000
#define THREAD_SCHED_RR            0x00040000
#define THREAD_SCHED_DEFAULT       0x00080000
#if 1
#   define THREAD_SCOPE_SYSTEM        0x00100000
#else
#   define THR_SCOPE_SYSTEM        THR_BOUND
#endif /* ACE_HAS_IRIX62_THREADS */
#define THREAD_SCOPE_PROCESS       0x00200000
#define THREAD_INHERIT_SCHED       0x00400000
#define THREAD_EXPLICIT_SCHED      0x00800000
#define THREAD_USE_AFX             0x01000000


//// These are the various states a thread managed by the
//// <Thread_Manager> can be in.
//enum ThreadState
//{
//  THREAD_IDLE,
//  // Uninitialized.
//
//  THREAD_SPAWNED,
//  // Created but not yet running.
//
//  THREAD_RUNNING,
//  // Thread is active (naturally, we don't know if it's actually
//  // *running* because we aren't the scheduler...).
//
//  THREAD_SUSPENDED,
//  // Thread is suspended.
//
//  THREAD_CANCELLED,
//  // Thread has been cancelled (which is an indiction that it needs to
//  // terminate...).
//
//  THREAD_TERMINATED
//  // Thread has shutdown, but the slot in the thread manager hasn't
//  // been reclaimed yet.
//};


//: Thread "handle" manager class.  All thread handles must pass through here
//+ in order for operations to be performed on them.

class vjThread {
public:
    // ------------------------------------------------------------------------
    //: Spawn a thread that executes func() with argument arg.
    //
    //! PRE: None.
    //! POST: A thread (with any specified attributes) is created that begins
    //+       executing func().  Depending on the scheduler, it may begin
    //+       execution immediately, or it may block for a short time before
    //+       starting.
    //
    //! ARGS: func - Function the thread will execute.
    //! ARGS: arg - Argument passed to func (optional).
    //! ARGS: flags - Flags for the thread--not currently used in this
    //+               implementation (optional).
    //! ARGS: priority - Priority of created thread (optional).
    //! ARGS: stack_addr - Alternate address for the thread's stack
    //+                    (optional).
    //! ARGS: stack_size - Sizes for the thread's stack (optional).
    //
    //! RETURNS: thread - A pointer to a newly created thread "handle" that
    //+                   can be maniuplated using the other routines in this
    //+                   class.
    //! RETURNS: NULL - Error
    // ------------------------------------------------------------------------
    static inline vjThreadId*
    spawn (THREAD_FUNC func, void* arg = 0, long flags = 0, u_int priority = 0,
           void* stack_addr = NULL, size_t stack_size = 0)
    {
        vjThreadNonMemberFunctor* NonMemFunctor = new vjThreadNonMemberFunctor(func, arg);
        vjThreadId* thread = new vjThreadId(NonMemFunctor, flags, priority,
                                            stack_addr, stack_size);

        return thread;
    }

    // -----------------------------------------------------------------------
    //: Spawn a new thread that will execute functorPtr.
    //
    //! PRE: None.
    //! POST: A thread (with any specified attributes) is created that begins
    //+       executing func().  Depending on the scheduler, it may being
    //+       execution immediately, or it may block for a short time before
    //+       beginning execution.
    //
    //! ARGS: functorPtr - Function to be executed by the thread.
    //! ARGS: flags - Flags for the thread--not currently used in this
    //+               implementation (optional).
    //! ARGS: priority - Priority of created thread (optional).
    //! ARGS: stack_addr - Alternate address for thread's stack (optional).
    //! ARGS: stack_size - Size for thread's stack (optional).
    //
    //! RETURNS: thread - A pointer to a newly created thread "handle" that
    //+                   can be maniuplated using the other routines in this
    //+                   class.
    //! RETURNS: NULL - Error
    // ------------------------------------------------------------------------
    static inline vjThreadId*
    spawn ( vjBaseThreadFunctor* functorPtr, long flags = 0, u_int priority = 0,
           void* stack_addr = NULL, size_t stack_size = 0)
    {
        vjThreadId* thread = new vjThreadId(functorPtr, flags, priority,
                                            stack_addr, stack_size);

        return thread;
    }

    // -----------------------------------------------------------------------
    //: Make the calling thread wait for the termination of the specified
    //+ thread.
    //
    //! PRE: None.
    //! POST: The caller blocks until the specified thread angle finishes its
    //+       execution (i.e., calls the exit() method).  This routine may
    //+       return immediately if the thread has already exited.
    //
    //! ARGS: thread - The thread handle on which the caller wants to wait.
    //! ARGS: status - Status value of the terminating thread when that
    //+                thread calls the exit routine (optional).
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    static inline int
    join (vjThreadId* thread, void** status = 0) {
        return thread->join(status);
    }

    // -----------------------------------------------------------------------
    //: Wait for one or more threads to exit.
    //
    //! PRE: None.
    //! POST: The caller blocks until the specified threads finish their
    //+       execution (i.e., call the exit() method).  This routine may
    //+       return immediately if these threads have already exited.
    //
    //! ARGS: thread - The first thread handle on which the caller wants to
    //+                wait.
    //! ARGS: thread_array - Array of one or more thread IDs to be joined.
    //! ARGS: n - The number of threads to join.
    //! ARGS: ret_val - Storage for return value of an exiting thread
    //+                 (optional).
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: The behavior of this routine is not yet well-defined, and it
    //+       may change.
    // -----------------------------------------------------------------------
    static inline int
    join (vjThreadId* thread, vjThreadId thread_array[], int n,
          void** status = 0)
    {
        return thread->join(thread_array, n, status);
    }

    // -----------------------------------------------------------------------
    //: Resume the execution of a thread that was previously suspended using
    //+ suspend().
    //
    //! PRE: The specified thread was previously suspended using the
    //+      suspend() member function.
    //! POST: The specified thread is sent the SIGCONT signal and is allowed
    //+       to begin executing again.
    //
    //! ARGS: thread - The thread handle to be resumed.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is not supported by all threading implementations.
    // -----------------------------------------------------------------------
    static inline int
    resume (vjThreadId* thread) {
        return thread->resume();
    }

    // -----------------------------------------------------------------------
    //: Suspend the execution of the specified thread.
    //
    //! PRE: None.
    //! POST: The thread is sent the SIGSTOP signal and is thus suspended
    //+       from execution until the member function resume() is called.
    //
    //! ARGS: thread - The thread handle to be suspended.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is not supported by all threading implementations.
    // -----------------------------------------------------------------------
    static inline int
    suspend (vjThreadId* thread) {
        return thread->suspend();
    }

    // -----------------------------------------------------------------------
    //: Get the given thread's priority.
    //
    //! PRE: None.
    //! POST: The priority of the specified thread is returned in the integer
    //+       pointer variable.
    //
    //! ARGS: thread - The thread handle whose priority is to be returned.
    //! ARGS: prio - Pointer to an int variable that will have the thread's
    //+              priority stored in it.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is only supported on systems that allow thread priority
    //+       scheduling.
    // -----------------------------------------------------------------------
    static inline int
    getprio (vjThreadId* thread, int* prio) {
        return thread->getprio(prio);
    }

    // -----------------------------------------------------------------------
    //: Set the given thread's priority.
    //
    //! PRE: None.
    //! POST: The thread has its priority set to the specified value.
    //
    //! ARGS: thread - The thread handle whose priority is to be set.
    //! ARGS: prio - The new priority for the specified thread.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is only supported on systems that support allow priority
    //+       scheduling.
    // -----------------------------------------------------------------------
    static inline int
    setprio (vjThreadId* thread, int prio) {
        return thread->setprio(prio);
    }

    // -----------------------------------------------------------------------
    //: Yield execution of the calling thread to allow a different blocked
    //+ thread to execute.
    //
    //! PRE: None.
    //! POST: The caller yields its execution control to another thread or
    //+       process.
    //
    //! ARGS: thread -
    // -----------------------------------------------------------------------
    static inline void
    yield (vjThreadId* thread) {
        thread->yield();
    }

    // -----------------------------------------------------------------------
    //: Get the ID of the thread handle that is currently running.
    //
    //! PRE: None.
    //! POST: None.
    // -----------------------------------------------------------------------
    static inline int
    self (void) {
        return ::getpid();
    }

    // -----------------------------------------------------------------------
    //: Get the ID of the process in which the specified thread is running.
    //
    //! PRE: None.
    //! POST: None.
    //
    //! ARGS: thread - The thread handle whose PID is to be returned.
    // -----------------------------------------------------------------------
    static inline pid_t
    getpid (vjThreadId* thread) {
        return thread->getpid();
    }

    // -----------------------------------------------------------------------
    //: Cause the specified thread to exit.
    //
    //! PRE: None.
    //! POST: The given thread terminates.
    //
    //! ARGS: thread - The thread handle that will end its execution.
    //! ARGS: status - Address value copied and returned to a thread that is
    //+                waiting on this thread using join() (optional).
    // -----------------------------------------------------------------------
    static inline void
    exit (vjThreadId* thread, int status = 0) {
        thread->exit(status);
    }

    // -----------------------------------------------------------------------
    //: Send the specified signal to the given thread (not necessarily
    //+ SIGKILL).
    //
    //! PRE: None.
    //! POST: This thread receives the specified signal.
    //
    //! ARGS: threads - The thread handle that will receive the given signal.
    //! ARGS: signum - The signal to send to the specified thread.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is not supported by all threading implementations.
    // -----------------------------------------------------------------------
    static inline int
    kill (vjThreadId* thread, int signum) {
        return thread->kill(signum);
    }

    // -----------------------------------------------------------------------
    //: Kill (cancel) the specified thread.
    //
    //! PRE: None.
    //! POST: The given thread is cancelled.  Depending on the cancellation
    //+       attributes of the specified thread, it may terminate
    //+       immediately, it may wait until a pre-defined cancel point to
    //+       stop or it may ignore the cancel altogether.  Thus, immediate
    //+       cancellation is not guaranteed.
    //
    //! ARGS: thread - The thread to be killed.
    //
    //! NOTE: For the sake of clarity, it is probably better to use the
    //+       cancel() routine instead of kill() because a two-argument
    //+       version of kill() is also used for sending signals to threads.
    //+       This kill() and cancel() do exactly the same thing.
    // -----------------------------------------------------------------------
    static inline void
    kill (vjThreadId* thread) {
        thread->kill();
    }

    // -----------------------------------------------------------------------
    //: Cancel the specified thread.
    //
    //! PRE: None.
    //! POST: The specified thread is cancelled.  Depending on the
    //+       cancellation attributes of the thread, it may terminate
    //+       immediately, it may wait until a pre-defined cancel point to
    //+       stop or it may ignore the cancel altogether.  Thus, immediate
    //+       cancellation is not guaranteed.
    //
    //! ARGS: thread - The thread handle to have its execution cancelled.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    static inline int
    cancel (vjThreadId* thread) {
        return thread->cancel();
    }

    // -----------------------------------------------------------------------
    //: Request delivery of a pending cancel to the given thread.
    //
    //! PRE: A cancel message may have been sent to the specified thread.
    //! POST: If a pending cancel message is found, the specified thread is
    //+       cancelled.  Otherwise, it continues execution.
    //
    //! ARGS: thread - The thread handle requesting the pending cancellation
    //+                message delivery.
    // -----------------------------------------------------------------------
    static inline void
    testcancel (vjThreadId* thread) {
        thread->testcancel();
    }

    // -----------------------------------------------------------------------
    //: Disable thread cancellation for all threads.
    //
    //! PRE: None.
    //! POST: With Draft 10 of the pthreads standard, the old cancellation
    //+       state is returned in the integer pointer variable if specified
    //+       and non-NULL.
    //
    //! ARGS: old_state - Storage for returning the previous state
    //+                   (optional).
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    static inline int
    disablecancel (cancel_state_t* old_state = NULL) {
        return ThreadClass::disablecancel(old_state);
    }

    // -----------------------------------------------------------------------
    //: Enable thread cancellation for all threads.
    //
    //! PRE: None.
    //! POST: The old cancellation state may be returned in the integer
    //+       pointer variable if it is specified, non-NULL and supported
    //+       by the threading implementation.
    //
    //! ARGS: old_state - Storage for returning the previous state
    //+                   (optional).  This is only applicable if supported
    //+                   by the threading implementation.
    //! ARGS: flag - Not used in this implementation.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    static inline int
    enablecancel (cancel_state_t* old_state = NULL, int flag = 0) {
        return ThreadClass::enablecancel(old_state, flag);
    }

    // -----------------------------------------------------------------------
    //: Set the cancellation state for all threads.
    //
    //! PRE: None.
    //! POST: If the integer pointer old_state is specified and is non-NULL,
    //+       the previous cancellation state will be stored in it.
    //
    //! ARGS: new_state - A reference variable to the new cancelability state
    //+                   for the current thread.
    //! ARGS: old_state - Storage for returning the previous state
    //+                   (optional).
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is not supported by all threading implementations.
    // -----------------------------------------------------------------------
    static inline int
    setcancelstate (cancel_state_t& new_state,
                    cancel_state_t* old_state = NULL)
    {
        return ThreadClass::setcancelstate(new_state, old_state);
    }

    // -----------------------------------------------------------------------
    //: Change and/or examine the given thread's signal mask.
    //
    //! PRE: None.
    //! POST: The signal mask of the calling thread is set to the new value.
    //+       Its current signal mask (before being modified) is returned in
    //+       the old_set pointer if it is specified and it is not NULL.
    //
    //! ARGS: thread - The thread handle that will have its signal masked
    //+                changed and/or examined.
    //! ARGS: operation - The operation (one of SIG_BLOCK, SIG_UNBLOCK,
    //+                   SIG_SETMASK) to be performed on the current signal
    //+                   mask.
    //! ARGS: set - The new set of signals to be masked.
    //! ARGS: old_set - Storage for the current set of blocked signals
    //+                 (optional). This is used for examining the current
    //+                 signal mask.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is not supported by all threading implementations.
    // -----------------------------------------------------------------------
    static inline int
    sigsetmask (vjThreadId* thread, int operation, const sigset_t* set,
                sigset_t* old_set = 0)
    {
        return thread->sigsetmask(operation, set, old_set);
    }

    // -----------------------------------------------------------------------
    //: Allocate a thread key handle that is used to identify data that is
    //+ specific to each thread handle in the process, is global to all
    //+ threads in the process and is destroyed using the spcefied destructor
    //+ function that takes a single argument.
    //
    //! PRE: None.
    //! POST: A key "handle" is created and is associated with the specified
    //+       destructor function and argument.
    //
    //! ARGS: dest_func - The destructor function for the key.  This uses
    //+                   the functor data structure.
    //! ARGS: arg - Argument to be passed to THREAD_FUNC (optional).
    //
    //! RETURNS: key - A thread key ID handle that will be passed to other
    //+                thread key routines.
    //! RETURNS: NULL - Error
    //
    //! NOTE: Use this routine to construct the key destructor function if
    //+       it requires arguments.  Otherwise, use the two-argument version
    //+       of keycreate().
    // -----------------------------------------------------------------------
    static inline vjKeyId*
    keycreate (THREAD_FUNC destructor, void* arg = 0) {
        vjKeyId* key = new vjKeyId(destructor, arg);

        return key;
    }

    // -----------------------------------------------------------------------
    //: Allocates a thead key handle that is used to identify data that is
    //+ specific to each thread handle in the process, is global to all
    //+ threads in the process and is destroyed by the specified destructor
    //+ function.
    //
    //! PRE: None.
    //! POST: A thread key handle is created and is associated with the
    //+       specified destructor function.
    //
    //! ARGS: desctructor - Procedure to be called to destroy a data value
    //+                     associated with the key handle when the thread
    //+                     handle terminates.
    //
    //! RETURNS: key - A thread key ID handle that will be passed to other
    //+                thread key routines.
    //! RETURNS: NULL - Error
    // -----------------------------------------------------------------------
    static inline vjKeyId*
    keycreate ( vjBaseThreadFunctor* destructor) {
        vjKeyId* key = new vjKeyId(destructor);

        return key;
    }

    // -----------------------------------------------------------------------
    //: Free up the given key handle so that other threads may reuse it.
    //
    //! PRE: The specified key must have been properly created using the
    //+      keycreate() member function.
    //! POST: The specified key is destroyed using the destructor function
    //+       previously associated with it, and its resources are freed.
    //
    //! ARGS: key - The thread key handle to be destroyed.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is not supported by all threading implementations.
    // -----------------------------------------------------------------------
    static inline int
    keyfree (vjKeyId* key) {
        return key->keyfree();
    }

    // -----------------------------------------------------------------------
    //: Bind value to the given thread-specific key handle for the calling
    //+ thread.
    //
    //! PRE: The specified key handle must have been properly created using
    //+      the keycreate() member function.
    //! POST: The given value is associated with the key handle for the
    //+       calling thread.
    //
    //! ARGS: key - The thread key handle to be modified.
    //! ARGS: value - Address containing data to be associated with the
    //+               specified key for the current thread.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    static inline int
    setspecific (vjKeyId* key, void* value) {
        return key->setspecific(value);
    }

    // -----------------------------------------------------------------------
    //: Stores the current value bound to the given key handle for the
    //+ calling thread into the location pointed to by valuep.
    //
    //! PRE: The specified key must have been properly created using the
    //+      keycreate() member function.
    //! POST: The value associated with the key is obtained and stored in the
    //+       pointer valuep so that the caller may work with it.
    //
    //! ARGS: key - The thread key handle which will have its value returned.
    //! ARGS: valuep - Address of the current data value associated with the
    //+                key.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    static inline int
    getspecific (vjKeyId* key, void** valuep) {
        return key->getspecific(valuep);
    }

//  static inline int getconcurrency (vjThreadId* thread);
//  static inline int setconcurrency (vjThreadId* thread, int value);

private:
    vjThread (void) {
        // Do nothing to ensure that this class is not instantiated.
        ;
    }
};

#endif	/* _VJ_THREAD_H_ */
