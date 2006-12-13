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

#ifndef _VPR_THREAD_SGI_H_
#define _VPR_THREAD_SGI_H_
//#pragma once

// NOTE: This file(vprThreadSGI.h) MUST be included by vprThread.h.
// Not the other way around

#include <vpr/vprConfig.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <signal.h>
#include <sched.h>
#include <unistd.h>

#include <vpr/vprTypes.h>
#include <vpr/Thread/BaseThread.h>
#include <vpr/Thread/UncaughtThreadException.h>

namespace vpr
{

/** \class ThreadSGI ThreadSGI.h vpr/Thread/Thread.h
 *
 * Threads implementation using multiple processes created with sproc(2).
 * This is typedef'd to vpr::Thread.
 */
class ThreadSGI : public BaseThread
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
    *                  This parameter is ignored for this implementation.
    *
    * @see start()
    */
   ThreadSGI(BaseThread::VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
             BaseThread::VPRThreadScope scope = VPR_GLOBAL_THREAD,
             BaseThread::VPRThreadState state = VPR_JOINABLE_THREAD,
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
    *                  specified.
    * @param state     The joinable state of this thread. This parameter is
    *                  optional and defaults to VPR_JOINABLE_THREAD if not
    *                  specified.
    * @param stackSize The default stack size for this thread. This parameter
    *                  is optional and defaults to 0 (indicating that the
    *                  default stack size should be used) if not specified.
    *                  This parameter is ignored for this implementation.
    *
    * @throw vpr::IllegalArgumentException is thrown if an invalid attribute
    *        is given to sproc(2) or if no functor has been set for this
    *        thread object.
    * @throw vpr::ResourceException is thrown if a thread could not be
    *        allocated.
    * @throw vpr::Exception is thrown if anything else went wrong during the
    *        creation of the thread.
    *
    * @see start()
    */
   ThreadSGI(const vpr::thread_func_t& func,
             BaseThread::VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
             BaseThread::VPRThreadScope scope = VPR_GLOBAL_THREAD,
             BaseThread::VPRThreadState state = VPR_JOINABLE_THREAD,
             size_t stackSize = 0);
   //@}

   virtual ~ThreadSGI();

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
    *        is given to sproc(2) or if no functor has been set for this
    *        thread object.
    * @throw vpr::ResourceException is thrown if a thread could not be
    *        allocated.
    * @throw vpr::Exception is thrown if anything else went wrong during the
    *        creation of the thread.
    */
   virtual void start();

private:
   /**
    * Spawns a new thread that will execute functorPtr.
    *
    * @post A thread (with any specified attributes) is created that begins
    *        executing func().  Depending on the scheduler, it may being
    *        execution immediately, or it may block for a short time before
    *        beginning execution.
    *
    * @param functorPtr Function to be executed by the thread.
    *
    * @throw vpr::IllegalArgumentException is thrown if an invalid attribute
    *        is given to sproc(2).
    * @throw vpr::ResourceException is thrown if a thread could not be
    *        allocated.
    * @throw vpr::Exception is thrown if anything else went wrong during the
    *        creation of the thread.
    */
   void spawn();

   /**
    * Called by the spawn routine to start the user thread function.
    *
    * @pre Called ONLY by a new thread
    * @post The new thread will have started the user thread function.
    */
   void startThread();

private:
   /**
    * The functor to call from startThread().
    */
   vpr::thread_func_t mUserThreadFunctor;

   vpr::thread_func_t mStartFunctor;

   bool               mRunning;
   VPRThreadPriority  mPriority;
   vpr::UncaughtThreadException mException;
   bool                         mCaughtException;

public:

   /**
    * Makes the calling thread wait for the termination of the specified
    * thread.
    *
    * @return 0 is returned on successful completion.  -1 is returned on
    *         failure.
    *
    * @throw vpr::UncaughtThreadException is thrown if an exception was
    *        thrown by code executing in this thread and was not caught.
    */
   virtual void join(void** status = NULL);

   /**
    * Resumes the execution of a thread that was previously suspended using
    * suspend().
    *
    * @pre The specified thread was previously suspended using the
    *       suspend() member function.
    * @post The specified thread is sent the \c SIGCONT signal and is allowed
    *        to begin executing again.
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread and thus cannot receive a signal or if the given signal
    *        number is invalid.
    * @throw vpr::Exception is thrown if the calling thread does not have
    *        permission to kill this thread.
    */
   virtual void resume()
   {
      this->kill(SIGCONT);
   }

   /**
    * Suspends the execution of this thread.
    *
    * @pre This is a valid thread.
    * @post This thread is sent the \c SIGSTOP signal and is thus suspended
    *        from execution until the member function resume() is called.
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread and thus cannot receive a signal or if the given signal
    *        number is invalid.
    * @throw vpr::Exception is thrown if the calling thread does not have
    *        permission to kill this thread.
    */
   virtual void suspend()
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
    */
   virtual VPRThreadPriority getPrio();

   /**
    * Sets this thread's priority.
    *
    * @pre This is a valid thread.
    * @post This thread has its priority set to the specified value.
    *
    * @param prio The new priority of the specified thread.
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread (and thus cannot have its scheduling changed) or if the
    *        given priority is invalid.
    * @throw vpr::Exception is thrown if the calling thread does not have
    *        permission to set the priority of this thread.
    */
   void setPrio(const VPRThreadPriority prio);

   /**
    * Yields execution of the calling thread to allow a different blocked
    * thread to execute.
    *
    * @post The caller yields its execution control to another thread or
    *       process.
    */
   static void yield()
   {
      sginap(0);
   }

   /**
    * Sends the specified signal to this thread (not necessarily SIGKILL).
    *
    * @post This thread receives the specified signal.
    *
    * @param signum The signal to send to the specified thread.
    *
    * @throw vpr::IllegalArgumentException is thrown if this is not a valid
    *        thread and thus cannot receive a signal or if the given signal
    *        number is invalid.
    * @throw vpr::Exception is thrown if the calling thread does not have
    *        permission to kill this thread.
    */
   virtual void kill(const int signum);

   /**
    * Kills (cancels) this thread.
    *
    * @post This thread is cancelled.  Depending on the cancellation
    *       attributes of the specified thread, it may terminate
    *       immediately, it may wait until a pre-defined cancel point to
    *       stop or it may ignore the cancel altogether.  Thus, immediate
    *       cancellation is not guaranteed.
    */
   virtual void kill()
   {
      kill(SIGKILL);
   }

   /**
    * Outputs the state of the object.
    */
   std::ostream& outStream(std::ostream& out);

private:
   pid_t mThreadPID;      /**<  pid_t data structure for this thread */

   // --------  STATICS ------ //
   // This data is used to maintain a thread table in the system
   // It is here because the self() function needs to use system specific
   // information.
public:
   /**
    * Get a pointer to the thread we are in.
    *
    * @return NULL is returned if this thread is not in the global table.
    * @return A non-NULL pointer is returned that points to the thread in
    *         which we are currently running.
    */
   static Thread* self()
   {
      return getLocalThreadPtr();
   }

private:
   int vprThreadPriorityToUNIX(const VPRThreadPriority priority);

   VPRThreadPriority unixThreadPriorityToVPR(const int priority);

   /**
    * Sets the thread ptr stored in the local PRDA area.
    *
    * @note PRDA is a memory address that each thread has a seperate copy of.
    */
   static void setLocalThreadPtr(ThreadSGI* threadPtr)
   {
      ((ThreadInfo*)PRDA->usr2_prda.fill)->mThreadPtr = threadPtr;
   }

   /**
    * Gets the thread ptr stored in the local PRDA area.
    */
   static ThreadSGI* getLocalThreadPtr()
   {
      return ((ThreadInfo*)PRDA->usr2_prda.fill)->mThreadPtr;
   }

   /**
    * Structure for storing thread specific information.
    */
   struct ThreadInfo
   {
      ThreadSGI* mThreadPtr;
   };
};

} // End of vpr namespace


#endif   /* _VPR_THREAD_SGI_H_ */
