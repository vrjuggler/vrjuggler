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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifdef VPR_OS_Windows
#  include <process.h>
#endif

#include <prthread.h>
#include <prtypes.h>
#include <boost/concept_check.hpp>

#include <vpr/Thread/BaseThread.h>
#include <vpr/Thread/ThreadManager.h>
#include <vpr/Thread/UncaughtThreadException.h>

#include <vpr/md/NSPR/Thread/ThreadKeyNSPR.h>
#include <vpr/md/NSPR/Sync/CondVarNSPR.h>


namespace vpr
{

/** \class ThreadNSPR ThreadNSPR.h vpr/Thread/Thread.h
 *
 * Threads implementation using the NSPR API.
 *
 * This works by recieving a function in the constructor that is the function
 * to call when the new thread is created.  The function is stored internally
 * to the class, then the class is "boot-strapped" by spawning a call to the
 * startThread() function with in turn will call the previously set thread
 * function.
 *
 * This is typedef'd to vpr::Thread.
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
   ThreadNSPR(const vpr::thread_func_t& func,
              VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
              VPRThreadScope scope = VPR_GLOBAL_THREAD,
              VPRThreadState state = VPR_JOINABLE_THREAD,
              PRUint32 stack_size = 0);

   /**
    * Destructor.
    *
    * @post This thread is removed from the thread table and from the local
    *        thread hash.
    */
   virtual ~ThreadNSPR();

   /**
    * Sets the functor that this thread will execute.
    *
    * @pre The thread is not already running.  The functor is valid.
    */
   virtual void setFunctor(const vpr::thread_func_t& functor);

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
    * @return 0 is returned upon successful completion.  -1 is returned if
    *         an error occurred.
    */
   virtual int join(void** status = NULL)  throw (UncaughtThreadException);

   /**
    * Resumes the execution of a thread that was previously suspended using
    * suspend().
    *
    * @pre This thread was previously suspended using the suspend() member
    *       function.
    * @post This thread is sent the SIGCONT signal and is allowed to begin
    *        executing again.
    *
    * @return 0 is returned upon successful completion.  -1 is returned if
    *         an error occurred.
    */
   virtual int resume()
   {
//      return kill(SIGCONT);
      return -1;
   }

   /**
    * Suspends the execution of this thread.
    *
    * @post This thread is sent the SIGSTOP signal and is thus suspended
    *        from execution until the member function resume() is called.
    *
    * @return 0 is returned upon successful completion.  -1 is returned if
    *         an error occurred.
    */
   virtual int suspend()
   {
//      return kill(SIGSTOP);
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
    * @return 0 is returned upon successful completion.  -1 is returned if
    *         an error occurred.
    */
   virtual int getPrio(VPRThreadPriority* prio)
   {
      *prio = nsprThreadPriorityToVPR(PR_GetThreadPriority(mThread));

      return 0;
   }

   /**
    * Sets this thread's priority.
    *
    * @post This thread has its priority set to the specified value.
    *
    * @param prio  The new priority for this thread.
    *
    * @return 0 is returned upon successful completion.  -1 is returned if
    *         an error occurred.
    *
    * @note The priority must correspond to a value in the PRThreadPriority
    *        enumerated type.
    */
   virtual int setPrio(VPRThreadPriority prio);

   /**
    * Sends the specified signal to this thread (not necessarily SIGKILL).
    *
    * @post This thread receives the specified signal.
    *
    * @param signum  The signal to send to the specified thread.
    *
    * @return 0 is returned upon successful completion.  -1 is returned if
    *         an error occurred.
    */
   virtual int kill(int signum)
   {
      boost::ignore_unused_variable_warning(signum);
      return -1;
   }

   /**
    * Kills (cancels) this thread.
    *
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
   virtual void kill()
   {
   }

   /**
    * Gets a pointer to the thread we are in.
    *
    * @return NULL is returned if this thread is not in global table.
    * @return A non-NULL value is the pointer to the thread that we are
    *         running within.
    */
   static Thread* self();

   /**
    * Yields execution of the calling thread to allow a different blocked
    * thread to execute.
    *
    * @post The caller yields its execution control to another thread or
    *        process.
    */
   static void yield()
   {
      PR_Sleep(PR_INTERVAL_NO_WAIT);
   }

   /**
    * Provides a way of printing the process ID neatly.
    */
   virtual std::ostream& outStream(std::ostream& out);

// All private member variables and functions.
private:
   /**
    * Called by the spawn routine to start the user thread function.
    *
    * @pre Called ONLY by a new thread.
    * @post The new thread will have started the user thread function.
    */
   void startThread();

   PRThread* mThread;    /**<  PRThread data structure for this thread */

   /** The functor to call when the thread starts. */
   vpr::thread_func_t mUserThreadFunctor;

   /** Memory handed off to PR_CreateThread() as the thread function. */
   vpr::thread_func_t mStartFunctor;

   VPRThreadPriority  mPriority;
   VPRThreadScope     mScope;
   VPRThreadState     mState;
   PRUint32           mStackSize;
   vpr::UncaughtThreadException mException;
   bool                         mCaughtException;
   
   bool              mThreadStartCompleted;  /**< Flag for signaling when thread start is completed */
   vpr::CondVarNSPR  mThreadStartCondVar;    /**< CondVar for thread starting */

   PRThreadPriority vprThreadPriorityToNSPR(const VPRThreadPriority priority);

   PRThreadScope vprThreadScopeToNSPR(const VPRThreadScope scope);

   PRThreadState vprThreadStateToNSPR(const VPRThreadState state);

   VPRThreadPriority nsprThreadPriorityToVPR(const PRThreadPriority priority);

   VPRThreadScope nsprThreadScopeToVPR(const PRThreadScope scope);

   VPRThreadState nsprThreadStateToVPR(const PRThreadState state);

   static PRUint32               mTicksPerSec;

   // ---- STATICS --- //
   struct staticWrapper
   {
      staticWrapper()
         : mStaticsInitialized(1221)
         , mThreadIdKey(NULL)
      {;}

      unsigned       mStaticsInitialized;    // Just a debug helper to help find when called before initialized
      ThreadKeyNSPR mThreadIdKey;           // Key for the id of the local thread
   };

   static ThreadKeyNSPR& threadIdKey()
   {
      return statics.mThreadIdKey;
   }

   static staticWrapper statics;
};

} // End of vpr namespace


#endif  /* _VPR_THREAD_NSPR_H_ */
