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


#ifndef _VPR_THREAD_NSPR_H_
#define _VPR_THREAD_NSPR_H_
//#pragma once

#include <vpr/vprConfig.h>

#ifdef HAVE_UNISTD_H
#   include <unistd.h>
#endif

#ifdef VPR_OS_Win32
#   include <process.h>
#endif

#include <prthread.h>
#include <prtypes.h>
#include <prinrval.h>

#include <vpr/Thread/BaseThread.h>
#include <vpr/Thread/ThreadManager.h>


namespace vpr {

//: Threads implementation using the NSPR API.
//!PUBLIC_API:
class VPR_CLASS_API ThreadNSPR : public BaseThread
{
public:
    // -----------------------------------------------------------------------
    //: Spawning constructor.
    //
    // This will actually start a new thread that will execute the specified
    // function.
    // -----------------------------------------------------------------------
    ThreadNSPR(thread_func_t func, void* arg = 0,
               VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
               VPRThreadScope scope = VPR_GLOBAL_THREAD,
               VPRThreadState state = VPR_JOINABLE_THREAD,
               PRUint32 stack_size = 0);

    // -----------------------------------------------------------------------
    //: Spawning constructor with arguments (functor version).
    //
    // This will start a new thread that will execute the specified function.
    // -----------------------------------------------------------------------
    ThreadNSPR(BaseThreadFunctor* functor_ptr,
               VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
               VPRThreadScope scope = VPR_GLOBAL_THREAD,
               VPRThreadState state = VPR_JOINABLE_THREAD,
               PRUint32 stack_size = 0);

    // -----------------------------------------------------------------------
    //: Destructor.
    //
    //! PRE: None.
    //! POST: This thread is removed from the thread table and from the local
    //+       thread hash.
    // -----------------------------------------------------------------------
    virtual ~ThreadNSPR(void);

    // -----------------------------------------------------------------------
    //: Create a new thread that will execute functorPtr.
    //
    //! PRE: None.
    //! POST: A thread (with any specified attributes) is created that begins
    //+       executing func().  Depending on the scheduler, it may being
    //+       execution immediately, or it may block for a short time before
    //+       beginning execution.
    //
    //! ARGS: functor_ptr - Function to be executed by the thread.
    //! ARGS: priority - Priority of created thread (optional).
    //! ARGS: stack_size - Size for thread's stack (optional).
    //
    //! RETURNS: 0 - Succeedful thread creation
    //! RETURNS: Nonzero - Error
    // -----------------------------------------------------------------------
    int spawn(BaseThreadFunctor* functor_ptr,
              VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
              VPRThreadScope scope = VPR_GLOBAL_THREAD,
              VPRThreadState state = VPR_JOINABLE_THREAD,
              size_t stack_size = 0);

    // -----------------------------------------------------------------------
    //: Make the calling thread wait for the termination of this thread.
    //
    //! PRE: None.
    //! POST: The caller blocks until this thread finishes its execution
    //+       (i.e., finishes its root function).  This routine may return
    //+       immediately if this thread has already exited.
    //
    //! RETURNS:  0 - Succeedful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline virtual int
    join (void** status = NULL) {
        return PR_JoinThread(mThread);
    }

    // -----------------------------------------------------------------------
    //: Resume the execution of a thread that was previously suspended using
    //+ suspend().
    //
    //! PRE: This thread was previously suspended using the suspend() member
    //+      function.
    //! POST: This thread is sent the SIGCONT signal and is allowed to begin
    //+       executing again.
    //
    //! RETURNS:  0 - Succeedful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline virtual int
    resume (void) {
//        return kill(SIGCONT);
        return -1;
    }

    // -----------------------------------------------------------------------
    //: Suspend the execution of this thread.
    //
    //! PRE: None.
    //! POST: This thread is sent the SIGSTOP signal and is thus suspended
    //+       from execution until the member function resume() is called.
    //
    //! RETURNS:  0 - Succeedful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline virtual int
    suspend (void) {
//        return kill(SIGSTOP);
        return -1;
    }

    // -----------------------------------------------------------------------
    //: Get this thread's priority.
    //
    //! PRE: This is a valid thread.
    //! POST: The priority of this thread is returned in the integer pointer
    //+       variable.
    //
    //! ARGS: prio - Pointer to an int variable that will have the thread's
    //+              priority stored in it.
    //
    //! RETURNS:  0 - Succeedful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline virtual int
    getPrio (VPRThreadPriority* prio) {
        *prio = nsprThreadPriorityToVPR(PR_GetThreadPriority(mThread));

        return 0;
    }

    // -----------------------------------------------------------------------
    //: Set this thread's priority.
    //
    //! PRE: None.
    //! POST: This thread has its priority set to the specified value.
    //
    //! ARGS: prio - The new priority for this thread.
    //
    //! RETURNS:  0 - Succeedful completion
    //! RETURNS: -1 - Invalid priority given
    //
    //! NOTE: The priority must correspond to a value in the PRThreadPriority
    //+       enumerated type.
    // -----------------------------------------------------------------------
    virtual int setPrio(VPRThreadPriority prio);

    // -----------------------------------------------------------------------
    //: Send the specified signal to this thread (not necessarily SIGKILL).
    //
    //! PRE: None.
    //! POST: This thread receives the specified signal.
    //
    //! ARGS: signum - The signal to send to the specified thread.
    //
    //! RETURNS:  0 - Succeedful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    inline virtual int
    kill (int signum) {
        return -1;
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
    inline virtual void
    kill (void) {
    }

    // -----------------------------------------------------------------------
    //: Get a ptr to the thread we are in.
    //
    //! RETURNS: NULL - Thread is not in global table
    //! RETURNS: NonNull - Ptr to the thread that we are running within
    // -----------------------------------------------------------------------
    inline static BaseThread*
    self (void) {
        return mThreadTable.getThread(gettid());
    }

    // -----------------------------------------------------------------------
    //: Yield execution of the calling thread to allow a different blocked
    //+ thread to execute.
    //
    //! PRE: None.
    //! POST: The caller yields its execution control to another thread or
    //+       process.
    // -----------------------------------------------------------------------
    inline static void
    yield (void) {
        PR_Sleep(PR_INTERVAL_NO_WAIT);
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static int
    usleep (PRUint32 micro) {
        return PR_Sleep(PR_MicrosecondsToInterval(micro));
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static int
    msleep (PRUint32 milli) {
        return PR_Sleep(PR_MillisecondsToInterval(milli));
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static int
    sleep (PRUint32 seconds) {
        return PR_Sleep(PR_SecondsToInterval(seconds));
    }

    // -----------------------------------------------------------------------
    //: Provide a way of printing the process ID neatly.
    // -----------------------------------------------------------------------
    inline virtual std::ostream&
    outStream (std::ostream& out) {
        out.setf(std::ios::right);
        out << std::setw(7) << std::setfill('0') << getpid() << "/";
        out.unsetf(std::ios::right);
        BaseThread::outStream(out);
        out << std::setfill(' ');
        return out;
    }

// All private member variables and functions.
private:
    PRThread*	mThread;	//: PRThread data structure for this thread

    // -----------------------------------------------------------------------
    //: Check the status of the thread creation in order to determine if this
    //+ thread should be registered in the thread table or not.
    //
    //! PRE: An attempt must have been made to create a thread using spawn().
    //! POST: If status is 0, the thread gets registered in the thread table
    //+       and in the local thread hash.  The count of created threads is
    //+       incremented as well.
    //
    //! ARGS: status - The integer status returned by spawn().
    // -----------------------------------------------------------------------
    void checkRegister(const int status);

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    PRThreadPriority vprThreadPriorityToNSPR(const VPRThreadPriority priority);

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    PRThreadScope vprThreadScopeToNSPR(const VPRThreadScope scope);

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    PRThreadState vprThreadStateToNSPR(const VPRThreadState state);

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    VPRThreadPriority nsprThreadPriorityToVPR(const PRThreadPriority priority);

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    VPRThreadScope nsprThreadScopeToVPR(const PRThreadScope scope);

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    VPRThreadState nsprThreadStateToVPR(const PRThreadState state);

    // -----------------------------------------------------------------------
    //: Get this thread's ID (i.e., its hash index for the thread table).  It
    //+ will always be a valid pointer.
    //
    //! PRE: None.
    //! POST: The hash index ID for this thread is returned to the caller.
    //
    //! RETURNS: Nonzero - The index of this tread.
    // -----------------------------------------------------------------------
    inline static PRThread*
    gettid (void) {
        return PR_GetCurrentThread();
    }

    static PRUint32			mTicksPerSec;
    static ThreadTable<PRThread*>	mThreadTable;
};

}; // End of vpr namespace


#endif  /* _VPR_THREAD_NSPR_H_ */
