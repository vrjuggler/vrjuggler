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

/**
 * \file
 *
 * @note This file MUST be included by Thread.h, not the other way around.
 */

#ifndef _VPR_THREAD_POSIX_H_
#define _VPR_THREAD_POSIX_H_

#include <vpr/vprConfig.h>

#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <sched.h>

// To get the POSIX key stuff for storing self.
#include <vpr/md/POSIX/Thread/ThreadKeyPosix.h>
#include <vpr/Thread/UncaughtThreadException.h>
#include <vpr/md/POSIX/Sync/CondVarPosix.h>

namespace vpr
{

typedef vpr::Uint32 thread_id_t;

/** \class ThreadPosix ThreadPosix.h vpr/Thread/Thread.h
 *
 * Threads implementation using POSIX threads (both Draft 4 and the "final"
 * draft of the standard are supported).
 *
 * This works by recieving a function in the constructor that is the function
 * to call when the new thread is created.  The function is stored internally
 * to the class, then the class is "boot-strapped" by spawning a call to the
 * startThread() function with in turn will call the previously set thread
 * function.
 *
 * This is typedef'd to vpr::Thread.
 */
class VPR_CLASS_API ThreadPosix : public BaseThread
{
public:  // ---- Thread CREATION and SPAWNING -----
   /** Non-spawning constructor.  This will not start a thread. */
   ThreadPosix(VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
               VPRThreadScope scope = VPR_GLOBAL_THREAD,
               VPRThreadState state = VPR_JOINABLE_THREAD,
               size_t stack_size = 0);

   /**
    * Spawning constructor with argument.  This will start a new thread that
    * will execute the specified function.
    */
   ThreadPosix(const vpr::thread_func_t& func,
               VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
               VPRThreadScope scope = VPR_GLOBAL_THREAD,
               VPRThreadState state = VPR_JOINABLE_THREAD,
               size_t stack_size = 0);

   /**
    * Destructor.
    *
    * @post This thread is removed from the thread table and from the local
    *       thread hash.
    */
   virtual ~ThreadPosix();

   /**
    * Sets the functor that this thread will execute.
    *
    * @pre The thread is not already running.  The functor is valid.
    */
   virtual void setFunctor(const vpr::thread_func_t& functor);

   /**
    * Starts this thread's execution.
    *
    * @pre The functor to execute has been set.  The thread is not already
    *      running.
    * @post A thread (with any specified attributes) is created that begins
    *       executing our functor.  Depending on the scheduler, it may begin
    *       execution immediately, or it may block for a short time before
    *       beginning execution.
    */
   virtual vpr::ReturnStatus start();

protected:
   /**
    * Creates a new thread that will execute functorPtr.
    *
    * @post A thread (with any specified attributes) is created that begins
    *       executing func().  Depending on the scheduler, it may begin
    *       execution immediately, or it may block for a short time before
    *       beginning execution.
    *
    * @return A vpr::ReturnStatus obj is returned to indicate the result of
    *         the thread creation.
    */
   vpr::ReturnStatus spawn();

   /**
    * Called by the spawn routine to start the user thread function.
    *
    * @pre Called ONLY by a new thread.
    * @post The new thread will have started the user thread function.
    *       Any necessary thread registration is performed.  The user thread
    *       functor is called.
    */
   void startThread();

private:
   vpr::thread_func_t mUserThreadFunctor;     /**< The functor to call when
                                                   the thread starts */

public:  // ----- Various other thread functions ------

   /**
    * Makes the calling thread wait for the termination of this thread.
    *
    * @post The caller blocks until this thread finishes its execution
    *       (i.e., calls the exit() method).  This routine may return
    *       immediately if this thread has already exited.
    *
    * @param status Current state of the terminating thread when that
    *               thread calls the exit routine (optional).
    *
    * @return 0 is returned if this thread is "joined" successfully.<br>
    *         -1 is returned on an error condition.
    */
   virtual int join(void** status = 0) throw (UncaughtThreadException)
   {
      int return_status = pthread_join(mThread, status);

      if (mCaughtException)
      {
         throw mException;
      }

      return return_status;
   }

   /**
    * Resumes the execution of a thread that was previously suspended using
    * suspend().
    *
    * @pre This thread was previously suspended using the suspend() member
    *      function.
    * @post This thread is sent the SIGCONT signal and is allowed to begin
    *       executing again.
    *
    * @return 0 is returned if this thread resumes execuation successfully.
    * @return -1 is returned otherwise.
    *
    * @note This is not currently supported on HP-UX 10.20.
    */
   virtual int resume()
   {
      return kill(SIGCONT);
   }

   /**
    * Suspends the execution of this thread.
    *
    * @post This thread is sent the SIGSTOP signal and is thus suspended
    *       from execution until the member function resume() is called.
    *
    * @return 0 is returned if this thread is suspended successfully.
    * @return -1 is returned otherwise.
    *
    * @note This is not currently supported on HP-UX 10.20.
    */
   virtual int suspend (void)
   {
      return kill(SIGSTOP);
   }

   /**
    * Gets this thread's priority.
    *
    * @post The priority of this thread is returned in the integer pointer
    *       variable.
    *
    * @param prio Pointer to an int variable that will have the thread's
    *             priority stored in it.
    *
    * @return 0 is returned if the priority was retrieved successfully.
    * @return -1 is returned if the priority could not be read.
    *
    * @note This is only supported on systems that support thread priority
    *       scheduling in their pthreads implementation.
    */
   virtual int getPrio(VPRThreadPriority* prio);

   /**
    * Sets this thread's priority.
    *
    * @post This thread has its priority set to the specified value.
    *
    * @param prio The new priority for this thread.
    *
    * @return 0 is returned if this thread's priority was set successfully.
    * @return -1 is returned otherwise.
    *
    * @note This is only supported on systems that support thread priority
    *       scheduling in their pthreads implementation.
    */
   virtual int setPrio(VPRThreadPriority prio);

   /**
    * Sets the CPU affinity for this thread (the CPU on which this thread
    * will exclusively run).
    *
    * @pre The thread must have been set to be a system-scope thread.
    * @post The CPU affinity is set or an error status is returned.
    *
    * @param cpu The CPU on which this thread will run exclusively.
    *
    * @return 0 is returned if the affinity is set successfully.
    * @return -1 is returned otherwise.
    *
    * @note This is currently only available on IRIX 6.5 and is non-portable.
    */
   virtual int setRunOn(int cpu);

   /**
    * Gets the CPU affinity for this thread (the CPU on which this thread
    * exclusively runs).
    *
    * @pre The thread must have been set to be a system-scope thread, and
    *      a previous affinity must have been set using setRunOn().
    * @post The CPU affinity for this thread is stored in the cur_cpu
    *       pointer.
    *
    * @param cur_cpu The CPU affinity for this thread (set by a previous
    *                call to setRunOn().
    *
    * @return 0 is returned if the affinity is retrieved successfully.
    * @return -1 is returned otherwise.
    *
    * @note This is currently only available on IRIX 6.5 and is non-portable.
    */
   virtual int getRunOn(int* cur_cpu);

   /**
    * Yields execution of the calling thread to allow a different blocked
    * thread to execute.
    *
    * @post The caller yields its execution control to another thread or
    *       process.
    */
   static void yield()
   {
      sched_yield();
   }

   /**
    * Sends the specified signal to this thread (not necessarily SIGKILL).
    *
    * @post This thread receives the specified signal.
    *
    * @param signum The signal to send to the specified thread.
    *
    * @return 0 is returned if the signal was sent successfully.
    * @return -1 is returned otherwise.
    *
    * @note This is not currently supported with Pthreads Draft 4.
    */
   virtual int kill(int signum);

   /**
    * Kills (cancels) this thread.
    *
    * @post This thread is cancelled.  Depending on the cancellation
    *       attributes of the specified thread, it may terminate
    *       immediately, it may wait until a pre-defined cancel point to
    *       stop or it may ignore the cancel altogether.  Thus, immediate
    *       cancellation is not guaranteed.
    *
    * @note For the sake of clarity, it is probably better to use the
    *       cancel() routine instead of kill() because a two-argument
    *       version of kill() is also used for sending signals to threads.
    *       This kill() and cancel() do exactly the same thing.
    */
   virtual void kill()
   {
      pthread_cancel(mThread);
   }

   /**
    * Get a pointer to the thread we are in.
    *
    * @return NULL is returned if this thread is not in the global table.
    * @return A non-NULL pointer is returned that points to the thread in
    *         which we are currently running.
    */
   static Thread* self();

   /** Provides a way of printing the process ID neatly. */
   std::ostream& outStream(std::ostream& out);

// All private member variables and functions.
private:
   bool              mRunning;
   pthread_t         mThread;        /**< pthread_t data structure for this thread */
   VPRThreadPriority mPriority;
   VPRThreadScope    mScope;         /**< Scope (process or system) of this thread */
   VPRThreadState    mState;
   size_t            mStackSize;
   vpr::UncaughtThreadException mException;
   bool                         mCaughtException;

   /** Flag for signaling when thread start is completed. */
   bool mThreadStartCompleted;

   /** CondVar for thread starting. */
   vpr::CondVarPosix mThreadStartCondVar;

   /**
    * Functor used for calling back vpr::ThreadPosix::startThread().  This
    * technique helps to ensure proper thread registration.
    */
   vpr::thread_func_t mStartFunctor;

   /** Converts a VPR thread priority to its Pthread equivalent. */
   int vprThreadPriorityToPOSIX(const VPRThreadPriority priority);

   /** Converts a VPR thread scope to its Pthread equivalent. */
   int vprThreadScopeToPOSIX(const VPRThreadScope scope);

   /** Converts a VPR thread state value to its Pthread equivalent. */
   int vprThreadStateToPOSIX(const VPRThreadState state);

   /** Converts a Pthread thread priority to its VPR equivalent. */
   VPRThreadPriority posixThreadPriorityToVPR(const int priority);

   /** Converts a Pthread thread scope to its VPR equivalent. */
   VPRThreadScope posixThreadScopeToVPR(const int scope);

   /** Converts a Pthread thread state value to its VPR equivalent. */
   VPRThreadState posixThreadStateToVPR(const int state);

// ===========================================================================
// Static stuff follows.
// ===========================================================================

   struct staticWrapper
   {
      staticWrapper()
         : mStaticsInitialized(1221)
         , mThreadIdKey(NULL)
      {;}

      unsigned       mStaticsInitialized;    // Just a debug helper to help find when called before initialized
      ThreadKeyPosix mThreadIdKey;           // Key for the id of the local thread
   };

   static ThreadKeyPosix& threadIdKey()
   {
      return statics.mThreadIdKey;
   }

   static staticWrapper statics;
};

} // End of vpr namespace


#endif   /* _VPR_THREAD_POSIX_H_ */
