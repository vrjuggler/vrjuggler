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

#ifndef _VPR_THREAD_WIN32_H_
#define _VPR_THREAD_WIN32_H_

#include <vpr/vprConfig.h>

#include <process.h>
#include <boost/concept_check.hpp>

// To get the Win32 key stuff for storing self.
#include <vpr/md/WIN32/Thread/ThreadKeyWin32.h>
#include <vpr/md/WIN32/Sync/CondVarWin32.h>
#include <vpr/Thread/UncaughtThreadException.h>


namespace vpr
{

/** \class ThreadWin32 ThreadWin32.h vpr/Thread/Thread.h
 *
 * Threads implementation using Win32 threads.
 *
 * This works by recieving a function in the constructor that is the function
 * to call when the new thread is created.  The function is stored internally
 * to the class, then the class is "boot-strapped" by spawning a call to the
 * startThread() function with in turn will call the previously set thread
 * function.
 *
 * This is typedef'd to vpr::Thread.
 *
 * @note This class was originally part of VR Juggler 1.0 and was brought back
 *       into VPR in version 1.1.36.
 */
class VPR_CLASS_API ThreadWin32 : public vpr::BaseThread
{
public:
   /**
    * Non-spawning constructor.  This will not start a thread.
    *
    * @param priority  The priority for this thread. This parameter is
    *                  optional and defaults to VPR_PRIORITY_NORMAL if not
    *                  specified.
    * @param scope     The scheduling scope of this thread. This parameter is
    *                  optional and defaults to VPR_GLOBAL_THREAD if not
    *                  specified. This parameter is ignored in this
    *                  implementation. All Win32 threads are global scope.
    * @param state     The joinable state of this thread. This parameter is
    *                  optional and defaults to VPR_JOINABLE_THREAD if not
    *                  specified. This parameter is ignored in this
    *                  implementation. All Win32 threads are joinable.
    * @param stackSize The default stack size for this thread. This parameter
    *                  is optional and defaults to 0 (indicating that the
    *                  default stack size should be used) if not specified.
    *                  Note that this parameter is only honored on platforms
    *                  that implement the stack size attribute for Win32
    *                  threads.
    *
    * @see start()
    */
   ThreadWin32(VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
               VPRThreadScope scope = VPR_GLOBAL_THREAD,
               VPRThreadState state = VPR_JOINABLE_THREAD,
               size_t stackSize = 0);

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
    *                  specified. This parameter is ignored in this
    *                  implementation. All Win32 threads are global scope.
    * @param state     The joinable state of this thread. This parameter is
    *                  optional and defaults to VPR_JOINABLE_THREAD if not
    *                  specified. This parameter is ignored in this
    *                  implementation. All Win32 threads are joinable.
    * @param stackSize The default stack size for this thread. This parameter
    *                  is optional and defaults to 0 (indicating that the
    *                  default stack size should be used) if not specified.
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
   ThreadWin32(const vpr::thread_func_t& func,
               VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
               VPRThreadScope scope = VPR_GLOBAL_THREAD,
               VPRThreadState state = VPR_JOINABLE_THREAD,
               size_t stackSize = 0);

   /**
    * Destructor.
    *
    * @post This thread is removed from the thread table and from the local
    *       thread hash.
    */
   virtual ~ThreadWin32();

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
    *
    * @throw vpr::IllegalArgumentException is thrown if an invalid attribute
    *        is given to pthread_create(3) or if no functor has been set for
    *        this thread object.
    * @throw vpr::ResourceException is thrown if a thread could not be
    *        allocated.
    * @throw vpr::Exception is thrown if anything else went wrong during the
    *        creation of the thread.
    */
   virtual void start();

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
    *      function.
    *
    */
   virtual void resume();

   /**
    * Suspends the execution of this thread.
    *
    */
   virtual void suspend();

   /**
    * Gets this thread's priority.
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
    */
   virtual void setPrio(const VPRThreadPriority prio);

   /**
    * Yields execution of the calling thread to allow a different blocked
    * thread to execute.
    *
    * @post The caller yields its execution control to another thread or
    *       process.
    */
   static void yield()
   {
      // Sleep for 0 ms. This gives up our time-slice.
      Sleep(0);
   }

   /**
    * Sends the specified signal to this thread (not necessarily \c SIGKILL).
    *
    * @param signum The signal to send to the specified thread.
    *
    * @note This method does nothing. Use kill() instead.
    */
   virtual void kill(const int signum)
   {
      boost::ignore_unused_variable_warning(signum);
      std::cerr << "vpr::ThreadWin32::kill() is not implemented!"
                << std::endl;
   }

   /**
    * Kills (cancels) this thread.
    *
    * @post This thread is cancelled.
    */
   virtual void kill()
   {
      CloseHandle(mThreadHandle);
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
   HANDLE            mThreadHandle;  /**< HANDLE* data structure for this thread */
   DWORD             mThreadTID;     /**< The Win32 thread ID */
   VPRThreadPriority mPriority;
   size_t            mStackSize;

   vpr::UncaughtThreadException mException;
   bool                         mCaughtException;

   /** Flag for signaling when thread start is completed. */
   bool mThreadStartCompleted;

   /** Condition variable for thread starting. */
   vpr::CondVarWin32 mThreadStartCondVar;

   /**
    * Functor used for calling back vpr::ThreadPosix::startThread().  This
    * technique helps to ensure proper thread registration.
    */
   vpr::thread_func_t mStartFunctor;

   /** Converts a VPR thread priority to its Win32 equivalent. */
   int vprThreadPriorityToWin32(const VPRThreadPriority priority);

   /** Converts a Win32 thread priority to its VPR equivalent. */
   VPRThreadPriority win32ThreadPriorityToVPR(const int priority);

   struct staticWrapper
   {
      staticWrapper()
         : mStaticsInitialized(1221)
         , mThreadIdKey(NULL)
      {;}

      unsigned       mStaticsInitialized;    // Just a debug helper to help find when called before initialized
      ThreadKeyWin32 mThreadIdKey;           // Key for the id of the local thread
   };

   static ThreadKeyWin32& threadIdKey()
   {
      return statics.mThreadIdKey;
   }

   static staticWrapper statics;
};

} // End of vpr namespace


#endif   /* _VPR_THREAD_WIN32_H_ */
