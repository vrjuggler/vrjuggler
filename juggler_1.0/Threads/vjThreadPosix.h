/*
 * --------------------------------------------------------------------------
 * vjThreadPosix.h
 * $Revision$
 * $Date$
 * --------------------------------------------------------------------------
 * NOTES:
 *    - This file (vjThreadPosix.h) MUST be included by vjThread.h, not the
 *      other way around.
 *    - The following libraries must be linked in at compile time:
 *         HP-UX 10.20 --> -lcma
 *         IRIX 6.x    --> -lpthread
 * --------------------------------------------------------------------------
 */

#ifndef _THREAD_POSIX_H_
#define _THREAD_POSIX_H_


#include <config.h>
#include <iostream.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>

#include <Threads/vjThreadFunctor.h>


//: Threads implementation using POSIX threads (both Draft 4 and the "final"
//+ draft of the standard are supported).

class vjThreadPosix {
public:
    // -----------------------------------------------------------------------
    //: Constructor.
    // -----------------------------------------------------------------------
    vjThreadPosix (void) {
        new_thread();
    }

    // -----------------------------------------------------------------------
    //: Constructor with arguments.  This will actually start a new thread
    //+ that will execute the specified function.
    // -----------------------------------------------------------------------
    vjThreadPosix (THREAD_FUNC func, void* arg = 0, long flags = 0,
                 u_int priority = 0, void* stack_addr = NULL,
                 size_t stack_size = 0)
    {
        vjThreadNonMemberFunctor* NonMemFunctor = new vjThreadNonMemberFunctor(func, arg);

        new_thread();
        create(NonMemFunctor, flags, priority, stack_addr, stack_size);
    }

    // -----------------------------------------------------------------------
    //: Constructor with arguments (functor version).  This will start a new
    //+ thread that will execute the specified function.
    // -----------------------------------------------------------------------
    vjThreadPosix ( vjBaseThreadFunctor* functorPtr, long flags = 0,
                 u_int priority = 0, void* stack_addr = NULL,
                 size_t stack_size = 0)
    {
        new_thread();
        create(functorPtr, flags, priority, stack_addr, stack_size);
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
    //! RETURNS:  0 - Successful thread creation
    //! RETURNS: -1 - Error
    //
    //! NOTE: The pthreads implementation on HP-UX 10.20 does not allow the
    //+       stack address to be changed.
    // -----------------------------------------------------------------------
    int create ( vjBaseThreadFunctor* functorPtr, long flags = 0,
                u_int priority = 0, void* stack_addr = NULL,
                size_t stack_size = 0);

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
    int create_n (vjThreadPosix thread_ids[], int n, THREAD_FUNC func, void* arg,
                  long flags = 0, u_int priority = 0, void* stack_addr[] = 0,
                  size_t stack_size[] = 0);

    // -----------------------------------------------------------------------
    //: Make the calling thread wait for the termination of this thread.
    //
    //! PRE: None.
    //! POST: The caller blocks until this thread finishes its execution
    //+       (i.e., calls the exit() method).  This routine may return
    //+       immediately if this thread has already exited.
    //
    //! ARGS: status - Status value of the terminating thread when that
    //+                thread calls the exit routine (optional).
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline int
    join (void** status = 0) {
        return pthread_join(threadID, status);
    }

    // -----------------------------------------------------------------------
    //: Wait for one or more threads to exit.
    //
    //! PRE: None.
    //! POST: The caller blocks until the specified threads finish their
    //+       execution (i.e., call the exit() method).  This routine may
    //+       return immediately if the specified threads have already
    //+       exited.
    //
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
    int join (vjThreadPosix thread_array[], int n, void** ret_val = 0);

    // -----------------------------------------------------------------------
    //: Resume the execution of a thread that was previously suspended using
    //+ suspend().
    //
    //! PRE: This thread was previously suspended using the suspend() member
    //+      function.
    //! POST: This thread is sent the SIGCONT signal and is allowed to begin
    //+       executing again.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is not currently supported on HP-UX 10.20.
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
    //
    //! NOTE: This is not currently supported on HP-UX 10.20.
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
    //
    //! NOTE: This is only supported on systems that support thread priority
    //+       scheduling in their pthreads implementation.
    // -----------------------------------------------------------------------
    int getprio (int* prio);

    // -----------------------------------------------------------------------
    //: Set this thread's priority.
    //
    //! PRE: None.
    //! POST: This thread has its priority set to the specified value.
    //
    //! ARGS: prio - The new priority for this thread.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    //
    //! NOTE: This is only supported on systems that support thread priority
    //+       scheduling in their pthreads implementation.
    // -----------------------------------------------------------------------
    int setprio (int prio);

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
    //: Store the address of the vjThreadPosix data structure in the given
    //+ pointer.
    //
    //! PRE: None.
    //! POST: The address of the data structure for the calling thread is
    //+       stored in the specified pointer variable.
    //
    //! ARGS: thread_id - A pointer to a vjThreadPosix data structure variable.
    // -----------------------------------------------------------------------
    inline void
    self (vjThreadPosix* thread_id) {
        thread_id = this;
    }

    // -----------------------------------------------------------------------
    //: Get the address of the vjThreadPosix structure.
    //
    //! PRE: None.
    //! POST: The address of a pointer to a thread data structure that
    //+       contains the data structure for the calling thread.
    //
    //! RETURNS: pointer - A pointer to this vjThreadPosix object's address.
    // -----------------------------------------------------------------------
    inline vjThreadPosix*
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
        return PID;
    }

    // -----------------------------------------------------------------------
    //: Get the "thread ID" of this vjThreadPosix object.  This is a unique
    //+ integer value assigned when the thread was created.
    //
    //! PRE: None.
    //! POST: None.
    // -----------------------------------------------------------------------
    inline int
    gettid (void) {
        return thread_val;
    }

    // -----------------------------------------------------------------------
    //: Cause this thread to exit.
    //
    //! PRE: None.
    //! POST: The calling thread terminates.
    //
    //! ARGS: status - Address value copied and returned to a thread that is
    //+                waiting on this thread using join() (optional).
    // -----------------------------------------------------------------------
    inline void
    exit (int status = 0) {
        pthread_exit((void *) status);
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
    //
    //! NOTE: This is not currently supported on HP-UX 10.20.
    // -----------------------------------------------------------------------
    inline int
    kill (int signum) {
#ifdef _PTHREADS_DRAFT_4
        cerr << "vjThreadPosix::kill(): Signals cannot be sent to threads "
             << "with this POSIX threads implementation.\n";

        return -1;
#else
        return pthread_kill(threadID, signum);
#endif
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
    //+       This kill() and cancel() do exactly the same thing.
    // -----------------------------------------------------------------------
    inline void
    kill (void) {
        pthread_cancel(threadID);
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
        return pthread_cancel(threadID);
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
        pthread_testcancel();
    }

    // -----------------------------------------------------------------------
    //: Disable thread cancellation.
    //
    //! PRE: None.
    //! POST: On IRIX, the old cancellation state is returned in the integer
    //+       pointer variable if specified and non-NULL.
    //
    //! ARGS: old_state - Storage for returning the previous state
    //+                   (optional).  This is only applicable on IRIX.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    static inline int
    disablecancel (cancel_state_t* old_state = NULL) {
#ifdef _PTHREADS_DRAFT_4
        return pthread_setcancel(CANCEL_OFF);
#else
        return pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, old_state);
#endif
    }

    // -----------------------------------------------------------------------
    //: Enable thread cancellation.
    //
    //! PRE: None.
    //! POST: On IRIX, the old cancellation state is returned in the integer
    //+       pointer variable if specified and non-NULL.
    //
    //! ARGS: old_state - Storage for returning the previous state
    //+                   (optional).  This is only applicable on IRIX.
    //! ARGS: flag - Not used in this implementation.
    //
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    static inline int
    enablecancel (cancel_state_t* old_state = NULL, int flag = 0) {
#ifdef _PTHREADS_DRAFT_4
        return pthread_setcancel(CANCEL_ON);
#else
        return pthread_setcanceltype(PTHREAD_CANCEL_ENABLE, old_state);
#endif
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
    //
    //! NOTE: This is not currently supported on HP-UX 10.20.
    // -----------------------------------------------------------------------
    static inline int
    setcancelstate (cancel_state_t& new_state,
                    cancel_state_t* old_state = NULL)
    {
#ifdef _PTHREADS_DRAFT_4
        cerr << "vjThreadPosix::setcancelstate() not supported in this POSIX "
             << "threads implementation.\n";

        return -1;
#else
        return pthread_setcancelstate(new_state, old_state);
#endif
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
    //
    //! NOTE: This is not currently supported on HP-UX 10.20.
    // -----------------------------------------------------------------------
    inline int
    sigsetmask (int operation, const sigset_t* set, sigset_t* old_set = NULL) {
#ifdef _PTHREADS_DRAFT_4
        cerr << "vjThreadPosix::sigsetmask(): Signal masks cannot be used with "
             << "this POSIX threads implementation.\n";

        return -1;
#else
        return pthread_sigmask(operation, set, old_set);
#endif
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
    //: Overload the << operator to allow printing of the process and thread
    //+ ID neatly.
    // -----------------------------------------------------------------------
    ostream&
    operator<< (ostream& outfile) {
        outfile << "[PID " << this->getpid() << ":" << this->gettid() << "]";

        return outfile;
    }


// Private member functions.
private:
    // -----------------------------------------------------------------------
    //: Store the current value of the thread counter variable and the PID of
    //+ this process and increment the thread counter.
    //
    //! PRE: None.
    //! POST: The PID of this process and the "thread ID" of this thread are
    //+       recorded and the thread counter is incremented by one.
    // -----------------------------------------------------------------------
    inline void
    new_thread (void) {
        thread_val = thread_count++;
        PID = ::getpid();
    }

// Private member variables.
private:
    pthread_t	threadID;	//: pthread_t data structure for this thread
    pid_t	PID;		//: Process ID
    uint32_t	thread_val;	//: The unique identifier for this thread

    static int	thread_count;	//: Total number of threads created
};

// ---------------------------------------------------------------------------
//: Overload the << operator to allow printing of the process and thread ID
//+ neatly.
// ---------------------------------------------------------------------------
ostream& operator<< (ostream& outfile, vjThreadPosix& thread);

#endif	/* _THREAD_POSIX_H_ */
