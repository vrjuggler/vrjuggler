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


#ifndef _THREAD_WIN32_H
#define _THREAD_WIN32_H

// NOTE: This file(vjThreadWin32.h) MUST be included by vjThread.h.
// Not the other way around

#include <vjConfig.h>

#include <process.h>

#include <VPR/Threads/vjThreadFunctor.h>
#include <VPR/Threads/vjBaseThread.h>

//typedef int cancel_state_t;
//typedef int pid_t;
//typedef int sigset_t;
//-----------------------------------------------
//: Wrapper for Win32 thread handling functions.
//-----------------------------------------------
//!PUBLIC_API:
class VJ_CLASS_API vjThreadWin32 : public vjBaseThread
{
public:
    // -----------------------------------------------------------------------
    //: Constructor with arguments.  This will start a new thread that will
    //+ execute the specified function with the given argument.
    // -----------------------------------------------------------------------
    vjThreadWin32(vj_thread_func_t func, void* arg = 0,
                  VJThreadPriority priority = VJ_PRIORITY_NORMAL,
                  VJThreadScope scope = VJ_LOCAL_THREAD,
                  VJThreadState state = VJ_JOINABLE_THREAD,
                  size_t stack_size = 0);

    // -----------------------------------------------------------------------
    //: Constructor with arguments (functor version).  This will start a new
    //+ thread that will execute the specified function.
    // -----------------------------------------------------------------------
    vjThreadWin32(vjBaseThreadFunctor* functorPtr,
                  VJThreadPriority priority = VJ_PRIORITY_NORMAL,
                  VJThreadScope scope = VJ_LOCAL_THREAD,
                  VJThreadState state = VJ_JOINABLE_THREAD,
                  size_t stack_size = 0);

    // -----------------------------------------------------------------------
    //: Destructor.
    // -----------------------------------------------------------------------
    virtual ~vjThreadWin32 (void) {
        ;
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
    //! RETURNS:  non-zero - Successful thread creation
    //! RETURNS:        -1 - Error
    // -----------------------------------------------------------------------
    virtual int spawn(vjBaseThreadFunctor* functorPtr,
                      VJThreadPriority priority = VJ_PRIORITY_NORMAL,
                      VJThreadScope scope = VJ_LOCAL_THREAD,
                      VJThreadState state = VJ_JOINABLE_THREAD,
                      size_t stack_size = 0);

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
        WaitForSingleObject(mThreadHandle,INFINITE);
        return 0;
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
    //! RETURNS:  0 - Successful completion
    //! RETURNS: -1 - Error
    // -----------------------------------------------------------------------
    virtual int
    resume (void) {
        if( 0xFFFFFFFF == ResumeThread(mThreadHandle))
           return -1;
        else
           return 0;
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
    virtual int
    suspend (void) {
        if( 0xFFFFFFFF == SuspendThread(mThreadHandle))
           return -1;
        else
           return 0;
    }

    // -----------------------------------------------------------------------
    //: Get this thread's priority
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
        std::cerr << "vjThreadWin32::getprio() not implemented yet!\n";

        return -1;
    }

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
    // -----------------------------------------------------------------------
    inline int
    setprio (int prio) {
        std::cerr << "vjThreadWin32::setprio() not implemented yet!\n";

        return -1;
    }

    // -----------------------------------------------------------------------
    //: Yield execution of the calling thread to allow a different blocked
    //+ thread to execute.
    //
    //! PRE: None.
    //! POST: The caller yields its execution control to another thread or
    //+       process.
    // -----------------------------------------------------------------------
    static void
    vjThreadWin32::yield (void) {
       Sleep(0);     // Sleep for 0 ms, this gives up our time-slice
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static int
    usleep (const unsigned int micro) {
        Sleep(micro / 1000);
        return 0;
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static int
    msleep (const unsigned int milli) {
        Sleep(milli);
        return 0;
    }

    // -----------------------------------------------------------------------
    // -----------------------------------------------------------------------
    inline static int
    sleep (const unsigned int seconds) {
        Sleep(seconds * 1000);
        return 0;
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
    //: Make the calling thread wait for the termination of this thread.
    //! NOTE: For the sake of clarity, it is probably better to use the
    //+       cancel() routine instead of kill() because a two-argument
    //+       version of kill() is also used for sending signals to threads.
    //+       This kill() and cancel() do exactly the same thing.
    // -----------------------------------------------------------------------
    virtual int
    kill (int signum) {
        std::cerr << "vjThreadWin32::kill() not implemented yet!\n";

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
    // -----------------------------------------------------------------------
    virtual void
    kill (void) {
        CloseHandle(mThreadHandle);
    }

   // -----------------------------------------------------------------------
   //: Output the state of the object.
   // -----------------------------------------------------------------------
   virtual std::ostream& outStream(std::ostream& out)
   {
      out.setf(std::ios::right);
      out << std::setw(7) << std::setfill('0') << mThreadTID << "/";
      out.unsetf(std::ios::right);
      vjBaseThread::outStream(out);
      out << std::setfill(' ');
      return out;
   }


// Private member variables.
private:
    HANDLE     mThreadHandle;    //: HANDLE* data structure for this thread
    DWORD      mThreadTID;       //: The win32 thread ID


public:
    // -----------------------------------------------------------------------
    //: Get the address of thread we are in
    //! PRE: None.
    //! POST: The address of a pointer to a thread data structure that
    //+       contains the data structure for the calling thread.
    //
    //! RETURNS: pointer - A pointer to this vjThreadWin32 object's address.
    // -----------------------------------------------------------------------
    static vjBaseThread* self(void);

private:
    static vjThreadTable<DWORD> mThreadTable;
};


#endif	/* _THREAD_WIN32_H */
