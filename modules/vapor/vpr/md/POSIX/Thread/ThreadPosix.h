/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
#include <vector>
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
   /** @name Constructors */
   //@{
   /**
    * Non-spawning constructor.  This will not start a thread.
    *
    * @param priority  The priority for this thread. This parameter is
    *                  optional and defaults to VPR_PRIORITY_NORMAL if not
    *                  specified.
    * @param scope     The scheduling scope of this thread. This parameter is
    *                  optional and defaults to VPR_GLOBAL_THREAD if not
    *                  specified.
    * @param state     The joinable state of this thread. This parameter is
    *                  optional and defaults to VPR_JOINABLE_THREAD if not
    *                  specified.
    * @param stackSize The default stack size for this thread. This parameter
    *                  is optional and defaults to 0 (indicating that the
    *                  default stack size should be used) if not specified.
    *                  Note that this parameter is only honored on platforms
    *                  that implement the stack size attribute for POSIX
    *                  threads.
    *
    * @see start()
    */
   ThreadPosix(VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
               VPRThreadScope scope = VPR_GLOBAL_THREAD,
               VPRThreadState state = VPR_JOINABLE_THREAD,
               size_t stack_size = 0);

   /**
    * Spawning constructor with argument.  This will start a new thread that
    * will execute the specified function.
    *
    * @param func      The functor that will be executed by the spawned thread.
    *                  This can be any callable that returns nothing and takes
    *                  no parameters. The use of boost::bind() is recommended
    *                  to adapt other callables to this signature.
    * @param priority  The priority for this thread. This parameter is
    *                  optional and defaults to VPR_PRIORITY_NORMAL if not
    *                  specified.
    * @param scope     The scheduling scope of this thread. This parameter is
    *                  optional and defaults to VPR_GLOBAL_THREAD if not
    *                  specified.
    * @param state     The joinable state of this thread. This parameter is
    *                  optional and defaults to VPR_JOINABLE_THREAD if not
    *                  specified.
    * @param stackSize The default stack size for this thread. This parameter
    *                  is optional and defaults to 0 (indicating that the
    *                  default stack size should be used) if not specified.
    *                  Note that this parameter is only honored on platforms
    *                  that implement the stack size attribute for POSIX
    *                  threads.
    *
    * @throw vpr::IllegalArgumentException is thrown if an invalid attribute
    *        is given to pthread_create(3) or if no functor has been set for
    *        this thread object.
    * @throw vpr::ResourceException is thrown if a thread could not be
    *        allocated.
    * @throw vpr::Exception is thrown if anything else went wrong during the
    *        creation of the thread.
    *
    * @see start()
    */
   ThreadPosix(const vpr::thread_func_t& func,
               VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
               VPRThreadScope scope = VPR_GLOBAL_THREAD,
               VPRThreadState state = VPR_JOINABLE_THREAD,
               size_t stackSize = 0);
   //@}

   /**
    * Destructor.
    *
    * @post This thread is removed from the thread table and from the local
    *       thread hash.
    */
   ~ThreadPosix();

   /**
    * Sets the functor that this thread will execute.
    *
    * @pre The thread is not already running.  The functor is valid.
    */
   void setFunctor(const vpr::thread_func_t& functor);

   /**
    * Starts this thread's execution.
    *
    * @pre The functor to execute has been set.  The thread is not already
    *      running.
    * @post A thread (with any specified attributes) is created that begins
    *       executing our functor.  Depending on the scheduler, it may begin
    *       execution immediately, or it may block for a short time before
    *       beginning execution.
    *
    * @throw vpr::IllegalArgumentException is thrown if an invalid attribute
    *        is given to pthread_create(3) or if no functor has been set for
    *        this thread object.
    * @throw vpr::ResourceException is thrown if a thread could not be
    *        allocated.
    * @throw vpr::Exception is thrown if anything else went wrong during the
    *        creation of the thread.
    */
   void start();

protected:
   /**
    * Creates a new thread that will execute functorPtr.
    *
    * @post A thread (with any specified attributes) is created that begins
    *       executing func().  Depending on the scheduler, it may begin
    *       execution immediately, or it may block for a short time before
    *       beginning execution.
    *
    * @throw vpr::IllegalArgumentException is thrown if an invalid attribute
    *        is given to pthread_create(3).
    * @throw vpr::ResourceException is thrown if a thread could not be
    *        allocated.
    * @throw vpr::Exception is thrown if anything else went wrong during the
    *        creation of the thread.
    */
   void spawn();

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
    * @post The caller blocks until this thread finishes its execution. This
    *       routine may return immediately if this thread has already exited.
    *
    * @param status Current state of the terminating thread when that
    *               thread calls the exit routine (optional).
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread or not a joinable thread. In either case, this thread
    *        cannot be joined.
    * @throw vpr::DeadlockException is thrown if deadlock is detected when
    *        trying to join this thread.
    * @throw vpr::UncaughtThreadException is thrown if an exception was
    *        thrown by code executing in this thread and was not caught.
    */
   void join(void** status = NULL);

   /**
    * Resumes the execution of a thread that was previously suspended using
    * suspend().
    *
    * @pre This thread was previously suspended using the suspend() member
    *      function.
    * @post This thread is sent the \c SIGCONT signal and is allowed to begin
    *       executing again.
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread and thus cannot receive a signal.
    *
    * @note This is not currently supported on HP-UX 10.20.
    */
   void resume()
   {
      this->kill(SIGCONT);
   }

   /**
    * Suspends the execution of this thread.
    *
    * @pre This is a valid thread.
    * @post This thread is sent the \c SIGSTOP signal and is thus suspended
    *       from execution until the member function resume() is called.
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread and thus cannot receive a signal.
    *
    * @note This is not currently supported on HP-UX 10.20.
    */
   void suspend()
   {
      this->kill(SIGSTOP);
   }

   /**
    * Gets this thread's priority.
    *
    * @pre This is a valid thread.
    *
    * @return The VPRThreadPriority value indicating the priority of this
    *         thread is returned. If the priority cannot be queried, then
    *         \c VPR_PRIORITY_NORMAL is returned.
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread (and thus cannot have its scheduling queried).
    *
    * @note This is only supported on systems that support thread priority
    *       scheduling in their pthreads implementation.
    */
   VPRThreadPriority getPrio() const;

   /**
    * Sets this thread's priority.
    *
    * @post This thread has its priority set to the specified value.
    *
    * @param prio The new priority for this thread.
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread (and thus cannot have its scheduling changed) or if the
    *        given priority is invalid.
    *
    * @note This is only supported on systems that support thread priority
    *       scheduling in their pthreads implementation.
    */
   void setPrio(const VPRThreadPriority prio);

   /**
    * Sets the CPU affinity for this thread (the CPU on which this thread
    * will exclusively run).
    *
    * @pre The thread must have been set to be a system-scope thread. The
    *      thread from which this method was invoked must be the same as the
    *      thread spawned by this object.
    * @post The CPU affinity is set or an exception is thrown.
    *
    * @param cpu The CPU on which this thread will run exclusively. This value
    *            is zero-based and therefore must be greater than 0 (zero) and
    *            less than the number of processors available on the computer.
    *
    * @throw vpr::IllegalArgumentException
    *           Thrown if the thread spawned through the use of this object is
    *           not the thread from which this method was invoked.
    * @throw vpr::IllegalArgumentException
    *           Thrown if \p cpu is less than 0.
    * @throw vpr::IllegalArgumentException
    *           Thrown if this is not a system-scope (i.e., global) thread.
    * @throw vpr::Exception
    *           Thrown if the CPU affinity for the running thread could not
    *           be changed.
    *
    * @note Currently, this is only available on IRIX 6.5 and Linux.
    */
   void setRunOn(const int cpu);

   /**
    * Gets the CPU affinity for this thread (the CPU on which this thread
    * exclusively runs).
    *
    * @pre The thread must have been set to be a system-scope thread, and
    *      a previous affinity must have been set using setRunOn(). The thread
    *      from which this method was invoked must be the same as the thread
    *      spawned by this object.
    * @post The CPU affinity for this thread is returned to the caller.
    *
    * @return The CPU affinity for this thread (posisbly set by a previous
    *         call to setRunOn()).
    *
    * @throw vpr::IllegalArgumentException
    *           Thrown if the thread spawned through the use of this object is
    *           not the thread from which this method was invoked.
    * @throw vpr::IllegalArgumentException
    *           Thrown if this is not a system-scope (i.e., global) thread.
    * @throw vpr::Exception
    *           Thrown if the CPU affinity for the running thread could not
    *           be queried.
    *
    * @note Currently, this is only available on IRIX 6.5 and Linux.
    * @note The return value of this method was changed from int to
    *       std::vector<unsigned int> in VPR 2.1.6.
    */
   std::vector<unsigned int> getRunOn() const;

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
    * Sends the specified signal to this thread (not necessarily \c SIGKILL).
    *
    * @pre This is a valid thread.
    * @post This thread receives the specified signal.
    *
    * @param signum The signal to send to this thread.
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread and thus cannot receive a signal or if the given signal
    *        number is invalid.
    *
    * @note This is not currently supported with Pthreads Draft 4.
    */
   void kill(const int signum);

   /**
    * Kills (cancels) this thread.
    *
    * @post This thread is cancelled.  Depending on the cancellation
    *       attributes of the specified thread, it may terminate
    *       immediately, it may wait until a pre-defined cancel point to
    *       stop or it may ignore the cancel altogether.  Thus, immediate
    *       cancellation is not guaranteed.
    */
   void kill()
   {
      pthread_cancel(mThread);
   }

   /**
    * Get a pointer to the thread we are in.
    *
    * @return NULL is returned if this thread is not in the global table.
    *         A non-NULL pointer is returned that points to the thread in
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

   /** Condition variable for thread starting. */
   vpr::CondVarPosix mThreadStartCondVar;

   /**
    * Functor used for calling back vpr::ThreadPosix::startThread().  This
    * technique helps to ensure proper thread registration.
    */
   vpr::thread_func_t mStartFunctor;

   /** Converts a VPR thread priority to its Pthread equivalent. */
   int vprThreadPriorityToPOSIX(const VPRThreadPriority priority) const;

   /** Converts a VPR thread scope to its Pthread equivalent. */
   int vprThreadScopeToPOSIX(const VPRThreadScope scope) const;

   /** Converts a VPR thread state value to its Pthread equivalent. */
   int vprThreadStateToPOSIX(const VPRThreadState state) const;

   /** Converts a Pthread thread priority to its VPR equivalent. */
   VPRThreadPriority posixThreadPriorityToVPR(const int priority) const;

   /** Converts a Pthread thread scope to its VPR equivalent. */
   VPRThreadScope posixThreadScopeToVPR(const int scope) const;

   /** Converts a Pthread thread state value to its VPR equivalent. */
   VPRThreadState posixThreadStateToVPR(const int state) const;

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
