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
    *                  that support changing the stack size for threads.
    *
    * @see start()
    */
   ThreadNSPR(VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
              VPRThreadScope scope = VPR_GLOBAL_THREAD,
              VPRThreadState state = VPR_JOINABLE_THREAD,
              PRUint32 stackSize = 0);

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
    *                  that support changing the stack size for threads.
    *
    * @throw vpr::ResourceException is thrown if a thread could not be
    *        allocated.
    *
    * @see start()
    */
   ThreadNSPR(const vpr::thread_func_t& func,
              VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
              VPRThreadScope scope = VPR_GLOBAL_THREAD,
              VPRThreadState state = VPR_JOINABLE_THREAD,
              PRUint32 stackSize = 0);
   //@}

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
    * @throw vpr::ResourceException is thrown if a thread could not be
    *        allocated.
    */
   virtual void start();

   /**
    * Makes the calling thread wait for the termination of this thread.
    *
    * @post The caller blocks until this thread finishes its execution. This
    *       routine may return immediately if this thread has already exited.
    *
    * @param status Current state of the terminating thread when that
    *               thread calls the exit routine (optional). This parameter
    *               is not used by this implementation.
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread or not a joinable thread. In either case, this thread
    *        cannot be joined.
    * @throw vpr::UncaughtThreadException is thrown if an exception was
    *        thrown by code executing in this thread and was not caught.
    */
   virtual void join(void** status = NULL);

   /**
    * Resumes the execution of a thread that was previously suspended using
    * suspend().
    *
    * @pre This thread was previously suspended using the suspend() member
    *       function.
    * @post This thread is sent the \c SIGCONT signal and is allowed to begin
    *        executing again.
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread and thus cannot receive a signal.
    *
    * @note This operation is not currently supported with NSPR threads.
    */
   virtual void resume()
   {
//      this->kill(SIGCONT);
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
    * @note This operation is not currently supported with NSPR threads.
    */
   virtual void suspend()
   {
//      this->kill(SIGSTOP);
   }

   /**
    * Gets this thread's priority.
    *
    * @pre This is a valid thread.
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread (and thus cannot have its scheduling queried).
    */
   virtual VPRThreadPriority getPrio();

   /**
    * Sets this thread's priority.
    *
    * @post This thread has its priority set to the specified value.
    *
    * @param prio The new priority for this thread.
    *
    * @note The priority must correspond to a value in the PRThreadPriority
    *        enumerated type.
    */
   virtual void setPrio(const VPRThreadPriority prio);

   /**
    * Sends the specified signal to this thread (not necessarily \c SIGKILL).
    *
    * @post This thread receives the specified signal.
    *
    * @param signum The signal to send to the specified thread.
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread and thus cannot receive a signal or if the given signal
    *        number is invalid.
    *
    * @note This operation is not currently supported by NSPR threads.
    */
   virtual void kill(const int)
   {
      /* Do nothing. */ ;
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
    * @note This operation is not currently supported by NSPR threads.
    */
   virtual void kill()
   {
      /* Do nothing. */ ;
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
