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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
#  include <unistd.h>
#endif

#ifdef VPR_OS_Win32
#  include <process.h>
#endif

#include <prthread.h>
#include <prtypes.h>

#include <vpr/Thread/BaseThread.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/Thread/ThreadManager.h>

#include <vpr/md/NSPR/Sync/CondVarNSPR.h>


namespace vpr
{

/**
 * Threads implementation using the NSPR API.
 */
class VPR_CLASS_API ThreadNSPR : public BaseThread
{
public:
   /** Non-spawning constructor.  This will not start a thread. */
   ThreadNSPR(VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
              VPRThreadScope scope = VPR_GLOBAL_THREAD,
              VPRThreadState state = VPR_JOINABLE_THREAD,
              PRUint32 stack_size = 0);

   /**
    * Spawning constructor.  This will start a new thread that will execute
    * the specified function.
    */
   ThreadNSPR(thread_func_t func, void* arg = NULL,
              VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
              VPRThreadScope scope = VPR_GLOBAL_THREAD,
              VPRThreadState state = VPR_JOINABLE_THREAD,
              PRUint32 stack_size = 0);

   /**
    * Spawning constructor (functor version).  This will start a new thread
    * that will execute the specified functor.
    */
   ThreadNSPR(BaseThreadFunctor* functor_ptr,
              VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
              VPRThreadScope scope = VPR_GLOBAL_THREAD,
              VPRThreadState state = VPR_JOINABLE_THREAD,
              PRUint32 stack_size = 0);

   /**
    * Destructor.
    *
    * @pre None.
    * @post This thread is removed from the thread table and from the local
    *        thread hash.
    */
   virtual ~ThreadNSPR();

   /**
    * Sets the functor that this thread will execute.
    *
    * @pre The thread is not already running.  The functor is valid.
    */
   virtual void setFunctor(BaseThreadFunctor* functorPtr);

   /**
    * Creates a new thread that will execute this thread's functor.
    *
    * @pre The functor to execute has been set.  The thread is not already
    *      running.
    * @post A thread (with any specified attributes) is created that begins
    *       executing our functor.  Depending on the scheduler, it may begin
    *       execution immediately, or it may block for a short time before
    *       beginning execution.
    *
    * @return A vpr::ReturnStatus obj is returned to indicate the result of
    *         the thread creation.
    */
   virtual vpr::ReturnStatus start();

   /**
    * Makes the calling thread wait for the termination of this thread.
    *
    * @pre None.
    * @post The caller blocks until this thread finishes its execution
    *        (i.e., finishes its root function).  This routine may return
    *        immediately if this thread has already exited.
    *
    * @return  0 - Succeedful completion
    * @return -1 - Error
    */
   virtual int join (void** status = NULL)
   {
      return PR_JoinThread(mThread);
   }

   /**
    * Resumes the execution of a thread that was previously suspended using
    * suspend().
    *
    * @pre This thread was previously suspended using the suspend() member
    *       function.
    * @post This thread is sent the SIGCONT signal and is allowed to begin
    *        executing again.
    *
    * @return  0 - Succeedful completion
    * @return -1 - Error
    */
   virtual int resume (void)
   {
//        return kill(SIGCONT);
      return -1;
   }

   /**
    * Suspends the execution of this thread.
    *
    * @pre None.
    * @post This thread is sent the SIGSTOP signal and is thus suspended
    *        from execution until the member function resume() is called.
    *
    * @return  0 - Succeedful completion
    * @return -1 - Error
    */
   virtual int suspend (void)
   {
//        return kill(SIGSTOP);
      return -1;
   }

   /**
    * Gets this thread's priority.
    *
    * @pre This is a valid thread.
    * @post The priority of this thread is returned in the integer pointer
    *        variable.
    *
    * @param prio Pointer to an int variable that will have the thread's
    *             priority stored in it.
    *
    * @return  0 - Succeedful completion
    * @return -1 - Error
    */
   virtual int getPrio (VPRThreadPriority* prio)
   {
      *prio = nsprThreadPriorityToVPR(PR_GetThreadPriority(mThread));

      return 0;
   }

   /**
    * Sets this thread's priority.
    *
    * @pre None.
    * @post This thread has its priority set to the specified value.
    *
    * @param prio  The new priority for this thread.
    *
    * @return  0 - Succeedful completion
    * @return -1 - Invalid priority given
    *
    * @note The priority must correspond to a value in the PRThreadPriority
    *        enumerated type.
    */
   virtual int setPrio(VPRThreadPriority prio);

   /**
    * Sends the specified signal to this thread (not necessarily SIGKILL).
    *
    * @pre None.
    * @post This thread receives the specified signal.
    *
    * @param signum  The signal to send to the specified thread.
    *
    * @return  0 - Succeedful completion
    * @return -1 - Error
    */
   virtual int kill (int signum)
   {
      return -1;
   }

   /**
    * Kills (cancels) this thread.
    *
    * @pre None.
    * @post This thread is cancelled.  Depending on the cancellation
    *        attributes of the specified thread, it may terminate
    *        immediately, it may wait until a pre-defined cancel point to
    *        stop or it may ignore the cancel altogether.  Thus, immediate
    *        cancellation is not guaranteed.
    *
    * @note For the sake of clarity, it is probably better to use the
    *        cancel() routine instead of kill() because a two-argument
    *        version of kill() is also used for sending signals to threads.
    *        This kill() and cancel() do exactly the same thing.
    */
   virtual void kill (void)
   {
   }

   /**
    * Gets a pointer to the thread we are in.
    *
    * @return NULL - Thread is not in global table
    * @return NonNull - Ptr to the thread that we are running within
    */
   static BaseThread* self (void)
   {
      return mThreadTable.getThread(gettid());
   }

   /**
    * Yields execution of the calling thread to allow a different blocked
    * thread to execute.
    *
    * @pre None.
    * @post The caller yields its execution control to another thread or
    *        process.
    */
   static void yield (void)
   {
      PR_Sleep(PR_INTERVAL_NO_WAIT);
   }

   /**
    * Provides a way of printing the process ID neatly.
    */
   virtual std::ostream& outStream (std::ostream& out)
   {
      out.setf(std::ios::right);
      out << std::setw(7) << std::setfill('0') << getpid() << "/";
      out.unsetf(std::ios::right);
      BaseThread::outStream(out);
      out << std::setfill(' ');
      return out;
   }

// All private member variables and functions.
private:
   /**
    * Called by the spawn routine to start the user thread function
    *
    * @pre Called ONLY by a new thread
    * @post The new thread will have started the user thread function
    */
   void startThread(void* null_param);

   PRThread*          mThread;    /**<  PRThread data structure for this thread */
   BaseThreadFunctor* mUserThreadFunctor;     /**< The functor to call when
                                                   the thread starts */
   VPRThreadPriority  mPriority;
   VPRThreadScope     mScope;
   VPRThreadState     mState;
   PRUint32           mStackSize;

   bool              mThreadStartCompleted;  /**< Flag for signaling when thread start is completed */
   vpr::CondVarNSPR  mThreadStartCondVar;    /**< CondVar for thread starting */

   /**
    * Checks the status of the thread creation in order to determine if this
    * thread should be registered in the thread table or not.
    *
    * @pre An attempt must have been made to create a thread using spawn().
    * @post If status is 0, the thread gets registered in the thread table
    *       and in the local thread hash.  The count of created threads is
    *       incremented as well.
    *
    * @param status  The integer status returned by spawn().
    */
   //void checkRegister(const int status);

   PRThreadPriority vprThreadPriorityToNSPR(const VPRThreadPriority priority);

   PRThreadScope vprThreadScopeToNSPR(const VPRThreadScope scope);

   PRThreadState vprThreadStateToNSPR(const VPRThreadState state);

   VPRThreadPriority nsprThreadPriorityToVPR(const PRThreadPriority priority);

   VPRThreadScope nsprThreadScopeToVPR(const PRThreadScope scope);

   VPRThreadState nsprThreadStateToVPR(const PRThreadState state);

   /**
    * Gets this thread's ID (i.e., its hash index for the thread table).  It
    * will always be a valid pointer.
    *
    * @pre None.
    * @post The hash index ID for this thread is returned to the caller.
    *
    * @return Nonzero - The index of this tread.
    */
   static PRThread* gettid (void)
   {
      return PR_GetCurrentThread();
   }

   static PRUint32               mTicksPerSec;
   static ThreadTable<PRThread*> mThreadTable;
};

} // End of vpr namespace


#endif  /* _VPR_THREAD_NSPR_H_ */
