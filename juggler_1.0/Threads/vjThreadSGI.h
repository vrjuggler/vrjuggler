#ifndef _THREAD_SGI_H_
#define _THREAD_SGI_H_

// NOTE: This file(vjThreadSGI.h) MUST be included by vjThread.h.
// Not the other way around

#include <config.h>
#include <iostream.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <signal.h>
#include <sched.h>

#include <Threads/vjThreadFunctor.h>


//: Threads implementation using multiple processes created with sproc(2).

class vjThreadSGI {
public:
    // -----------------------------------------------------------------------
    //: Constructor.
    // -----------------------------------------------------------------------
    vjThreadSGI (void) {
        // Do nothing.
        ;
    }

    // -----------------------------------------------------------------------
    //: Constructor with arguments.  This will actually start a new thread
    //+ that will execute the specified function.
    // -----------------------------------------------------------------------
    vjThreadSGI (THREAD_FUNC func, void* arg = 0, long flags = 0,
               u_int priority = 0, void* stack_addr = NULL,
               size_t stack_size = 0)
    {
        vjThreadNonMemberFunctor* NonMemFunctor = new vjThreadNonMemberFunctor(func, arg);

        create(NonMemFunctor, flags, priority, stack_addr, stack_size);
    }

    // -----------------------------------------------------------------------
    //: Constructor with arguments (functor version).  This will start a new
    //+ thread that will execute the specified function.
    // -----------------------------------------------------------------------
    vjThreadSGI ( vjBaseThreadFunctor* func, long flags = 0, u_int priority = 0,
               void* stack_addr = NULL, size_t stack_size = 0)
    {
        create(func, flags, priority, stack_addr, stack_size);
    }

    // -----------------------------------------------------------------------
    //: Create a thread that executes func() with argument arg.
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
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    create (THREAD_FUNC func, void* arg = 0, long flags = 0, u_int priority = 0,
            void* stack_addr = NULL, size_t stack_size = 0)
    {
        vjThreadNonMemberFunctor* NonMemFunctor = new vjThreadNonMemberFunctor(func, arg);

        return create(NonMemFunctor, flags, priority, stack_addr, stack_size);
    }

    // -----------------------------------------------------------------------
    //: Create a new thread that will execute functorPtr.
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
    //! RETURNS: non-zero - Successful thread creation
    //! RETURNS:       -1 - Error
    // -----------------------------------------------------------------------
    inline int
    create ( vjBaseThreadFunctor* functorPtr, long flags = 0, u_int priority = 0,
            void* stack_addr = NULL, size_t stack_size = 0)
    {
//        if (stack_size != 0)
//        {
//            size_t size = stackSize;
//            // -- Check size -- //
//        }
//
//        if (stack_addr != NULL)
//        {
//            // Check for validity and set and stuff
//        }

        threadID = sproc(THREAD_FUNC(&ThreadFunctorFunction), PR_SADDR,
                         functorPtr);

        return threadID;
    }

    // -----------------------------------------------------------------------
    //: Create n new threads that execute func() with the argument arg if
    //+ given.
    //
    //! PRE: The thread_ids[] array should not have anything stored in it.
    //+      Its contents are destroyed in the routine.
    //! POST: The thread ID's created in this routine are stored and returned
    //+       in the array thread_ids[] so that the caller may access them.
    //
    //! ARGS: thread_ids - Storage for the created thread objects.
    //! ARGS: n - Number of threads to create.
    //! ARGS: func - Function that the threads will execute.
    //! ARGS: arg - Argument passed to func.
    //! ARGS: flags - Flags for the threads--not currently used in this
    //+               implementation.
    //! ARGS: priority - Priority of created threads (optional).
    //! ARGS: stack_addr - Alternate addresses for each thread's
    //+                    stacks (optional).
    //! ARGS: stack_size - Sizes for each thread's stacks (optional).
    //
    //! RETURNS: n - Number of threads successfully created.
    // -----------------------------------------------------------------------
    int create_n (vjThreadSGI thread_ids[], int n, THREAD_FUNC func, void* arg,
                  long flags, u_int priority = 0, void* stack_addr[] = 0,
                  size_t stack_size[] = 0);

    // -----------------------------------------------------------------------
    //: Make the calling thread wait for the termination of the specified
    //+ thread.
    //
    //! PRE: None.
    //! POST: The caller blocks until the specified thread finishes its
    //+       execution (i.e., calls the exit() method).  This routine may
    //+       return immediately if the specified thread has already exited.
    //
    //! ARGS: status - Status value of the terminating thread when that
    //+                thread calls the exit routine (optional).
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    join (void** = 0) {
        cerr << "vjThreadSGI::join() not implemented yet!\n";

        return -1;
    }


    // -----------------------------------------------------------------------
    //: Wait for one or more threads to exit.
    //
    //! PRE: None.
    //! POST: The caller blocks until the specified threads finish their
    //+       execution (i.e., calls the exit() method).  This routine may
    //+       return immediately if the specified threads have already
    //+       exited.
    //
    //! ARGS: thread_array - Array of one or more thread IDs to be joined.
    //! ARGS: ret_val - Storage for return value of an exiting thread
    //+                 (optional).
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: The behavior of this routine is not yet well-defined, and it
    //+       may change.
    // -----------------------------------------------------------------------
    inline int
    join (vjThreadSGI thread_array[], int n, void** ret_val = 0) {
        cerr << "vjThreadSGI::join() not implemented yet!\n";

        return -1;
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
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    resume (void) {
        return kill(SIGCONT);
    }

    // -----------------------------------------------------------------------
    //: Suspend the execution of this thread.
    //
    //! PRE: None.
    //! POST: This thread is sent the SIGSTOP signal and is thus suspended
    //+       from execution until the member function resume() is called.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    suspend (void) {
        return kill(SIGSTOP);
    }

    // -----------------------------------------------------------------------
    //: Get this thread's priority.
    //
    //! PRE: None.
    //! POST: The priority of this thread is returned in the integer pointer
    //+       variable.
    //
    //! ARGS: prio - Pointer to an int variable that will have the thread's
    //+              priority stored in it.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    getprio (int* prio) {
        *prio = getpriority(PRIO_PROCESS, threadID);

        if ( *prio == -1 ) {
            return -1;
        } else {
            return 0;
        }
    }

    // -----------------------------------------------------------------------
    //: Set this thread's priority.
    //
    //! PRE: None.
    //! POST: This thread has its priority set to the specified value.
    //
    //! ARGS: prio - The new priority of the specified thread.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is only supported on systems that support thread priority
    //+       scheduling in their pthreads implementation.
    // -----------------------------------------------------------------------
    inline int
    setprio (int prio) {
        return setpriority(PRIO_PROCESS, threadID, prio);
    }

    // -----------------------------------------------------------------------
    //: Yield execution of the calling thread to allow a different blocked
    //+ thread to execute.
    //
    //! PRE: None.
    //! POST: The caller yields its execution control to another thread or
    //+       process.
    // -----------------------------------------------------------------------
    inline void
    yield (void) {
        sched_yield();
    }

    // -----------------------------------------------------------------------
    //: Store the address of the vjThreadSGI data structure in the given
    //+ pointer.
    //
    //! PRE: None.
    //! POST: The address of the data structure for the calling thread is
    //+       stored in the specified pointer variable.
    //
    //! ARGS: thread_id - A pointer to a vjThreadSGI data structure variable.
    // -----------------------------------------------------------------------
    inline void
    self (vjThreadSGI* tId) {
        tId = this;
    }

    // -----------------------------------------------------------------------
    //: Get the address of the vjThreadSGI structure.
    //
    //! PRE: None.
    //! POST: The address of a pointer to a thread data structure that
    //+       contains the data structure for the calling thread.
    //
    //! RETURNS: pointer - A pointer to this vjThreadSGI object's address.
    // -----------------------------------------------------------------------
    inline vjThreadSGI*
    self (void) {
        return this;
    }

    // -----------------------------------------------------------------------
    //: Get the ID of the process in which this thread is running.
    //
    //! PRE: None.
    //! POST: None.
    // -----------------------------------------------------------------------
    inline pid_t
    getpid (void) {
        return threadID;
    }

    // -----------------------------------------------------------------------
    //: Get the "thread ID" of this vjThreadSGI object.  In the SGI IPC case,
    //+ "threads" are processes, so the PID is the important value.  Thus,
    //+ this will always return 0.
    //
    //! PRE: None.
    //! POST: None.
    // -----------------------------------------------------------------------
    inline int
    gettid (void) {
        return 0;
    }

    // -----------------------------------------------------------------------
    //: Cause the calling thread to exit.
    //
    //! PRE: None.
    //! POST: The calling thread terminates.
    //
    //! ARGS: status - Address value copied and returned to a thread that is
    //+                waiting on this thread using join() (optional).
    // -----------------------------------------------------------------------
    inline void
    exit (int status = 0) {
        ::exit(status);
    }

    // -----------------------------------------------------------------------
    //: Send the specified signal to this thread (not necessarily SIGKILL).
    //
    //! PRE: None.
    //! POST: This thread receives the specified signal.
    //
    //! ARGS: signum - The signal to send to the specified thread.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    kill (int signum) {
        return ::kill(threadID, signum);
    }

    // -----------------------------------------------------------------------
    //: Kill (cancel) this thread.
    //
    //! PRE: None.
    //! POST: This thread is cancelled.  Depending on the cancellation
    //+       attributes of the specified thread, it may terminate
    //+       immediately, it may wait until a pre-defined cancel point to
    //+       stop or it may ignore the cancel altogether.  Thus, immediate
    //+       cancellation is not guaranteed.
    //
    //! NOTE: For the sake of clarity, it is probably better to use the
    //+       cancel() routine instead of kill() because a two-argument
    //+       version of kill() is also used for sending signals to threads.
    // -----------------------------------------------------------------------
    inline void
    kill (void) {
        kill(SIGKILL);
    }

    // -----------------------------------------------------------------------
    //: Cancel this thread.
    //
    //! PRE: None.
    //! POST: This thread is cancelled.  Depending on the cancellation
    //+       attributes of the thread, it may terminate immediately, it may
    //+       wait until a pre-defined cancel point to stop or it may ignore
    //+       the cancel altogether.  Thus, immediate cancellation is not
    //+       guaranteed.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    cancel (void) {
        return kill(SIGKILL);
    }

    // -----------------------------------------------------------------------
    //: Request delivery of a pending cancel to the current thread.
    //
    //! PRE: A cancel message may have been sent to the current thread.
    //! POST: If a pending cancel message is found, the current thread is
    //+       cancelled.  Otherwise, it continues execution.
    // -----------------------------------------------------------------------
    inline void
    testcancel (void) {
        cerr << "vjThreadSGI::testcancel() not implemented yet!\n";
    }

    // -----------------------------------------------------------------------
    //: Disable thread cancellation.
    //
    //! PRE: None.
    //! POST: The old cancellation state is returned in the integer pointer
    //+       variable if specified and non-NULL.
    //
    //! ARGS: old_state - Storage for returning the previous state
    //+                   (optional).
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    static inline int
    disablecancel (cancel_state_t* old_state = NULL) {
        cerr << "vjThreadSGI::disablecancel not implemented yet!\n";

        return -1;
    }

    // -----------------------------------------------------------------------
    //: Enable thread cancellation.
    //
    //! PRE: None.
    //! POST: The old cancellation state is returned in the integer pointer
    //+       variable if specified and non-NULL.
    //
    //! ARGS: old_state - Storage for returning the previous state
    //+                   (optional).
    //! ARGS: flag - Not used in this implementation.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    static inline int
    enablecancel (cancel_state_t* old_state = NULL, int flag = 0) {
        cerr << "vjThreadSGI::enablecancel not implemented yet!\n";

        return -1;
    }

    // -----------------------------------------------------------------------
    //: Set the cancellation state.
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
    // -----------------------------------------------------------------------
    static inline int
    setcancelstate (cancel_state_t& new_state, cancel_state_t* old_state) {
        cerr << "vjThreadSGI::setcancelstate not implemented yet!\n";

        return -1;
    }

    // -----------------------------------------------------------------------
    //: Change and/or examine calling thread's signal mask.
    //
    //! PRE: None.
    //! POST: The signal mask of the calling thread is set to the new value.
    //+       Its current signal mask (before being modified) is returned in
    //+       the old_set pointer if it is specified and it is not NULL.
    //
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
    // -----------------------------------------------------------------------
    inline int
    sigsetmask (int operation, const sigset_t* set, sigset_t* old_set = 0) {
        cerr << "vjThreadSGI::sigsetmask not implemented yet!\n";

        return -1;
    }

/*
    // -----------------------------------------------------------------------
    //: Get the LWP concurrency level of the process.
    // -----------------------------------------------------------------------
    int
    getconcurrency (void) {
    }
*/

/*
    // -----------------------------------------------------------------------
    //: Set the LWP concurrency level of the process.
    // -----------------------------------------------------------------------
    int
    setconcurrency (int new_level) {
    }
*/

    // -----------------------------------------------------------------------
    //: Overload the << operator to allow printing of the process ID neatly.
    // -----------------------------------------------------------------------
    ostream&
    operator<< (ostream& outfile) {
        outfile << "[PID " << this->getpid() << "]";

        return outfile;
    }

private:
    pid_t threadID;		//: pid_t data structure for this thread
};

// ---------------------------------------------------------------------------
//: Overload the << operator to allow printing of the process ID neatly.
// ---------------------------------------------------------------------------
ostream& operator<< (ostream&, vjThreadSGI&);

#endif	/* _THREAD_SGI_H_ */
