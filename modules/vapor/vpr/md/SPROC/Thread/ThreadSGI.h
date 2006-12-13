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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/Thread/BaseThread.h>


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
   /** Non-spawning constructor.  This will not start a new thread. */
   ThreadSGI(BaseThread::VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
             BaseThread::VPRThreadScope scope = VPR_GLOBAL_THREAD,
             BaseThread::VPRThreadState state = VPR_JOINABLE_THREAD,
             size_t stackSize = 0);

   /**
    * Spawning constructor.
    * This will actually start a new thread that will execute the specified
    * function.
    */
   ThreadSGI(thread_func_t func, void* arg = 0,
             BaseThread::VPRThreadPriority priority = VPR_PRIORITY_NORMAL,
             BaseThread::VPRThreadScope scope = VPR_GLOBAL_THREAD,
             BaseThread::VPRThreadState state = VPR_JOINABLE_THREAD,
             size_t stackSize = 0);

   /**
    * Spawning constructor with arguments (functor version).
    * This will start a new thread that will execute the specified
    * function.
    */
   ThreadSGI(BaseThreadFunctor* functorPtr,
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
   virtual void setFunctor(BaseThreadFunctor* functorPtr);

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
    * @return A non-zero value is returned upon successful thread creation.
    *         -1 is returned if an error occurred.
    */
   vpr::ReturnStatus spawn(BaseThreadFunctor* functorPtr);

   /**
    * Called by the spawn routine to start the user thread function.
    *
    * @pre Called ONLY by a new thread
    * @post The new thread will have started the user thread function.
    */
   void startThread(void* null_param);

private:
   /**
    * The functor to call from startThread.
    */
   vpr::BaseThreadFunctor* mUserThreadFunctor;

   /**
    * Flag indicating if we allocated mUserThreadFunctor and therefore must
    * delete it ourselves.
    */
   bool mDeleteFunctor;

   vpr::ThreadMemberFunctor<vpr::ThreadSGI>* mStartFunctor;

   bool               mRunning;
   VPRThreadPriority  mPriority;

public:

   /**
    * Makes the calling thread wait for the termination of the specified
    * thread.
    *
    * @return 0 is returned on successful completion.  -1 is returned on
    *         failure.
    */
   virtual int join(void** = 0);

   /**
    * Resumes the execution of a thread that was previously suspended using
    * suspend().
    *
    * @pre The specified thread was previously suspended using the
    *       suspend() member function.
    * @post The specified thread is sent the SIGCONT signal and is allowed
    *        to begin executing again.
    *
    * @return 0 is returned on successful completion.  -1 is returned on
    *         failure.
    */
   virtual int resume()
   {
      return ::kill(mThreadPID, SIGCONT);
   }

   /**
    * Suspends the execution of this thread.
    *
    * @post This thread is sent the SIGSTOP signal and is thus suspended
    *        from execution until the member function resume() is called.
    *
    * @return 0 is returned on successful completion.  -1 is returned on
    *         failure.
    */
   virtual int suspend()
   {
      return ::kill(mThreadPID, SIGSTOP);
   }

   /**
    * Gets this thread's priority.
    *
    * @post The priority of this thread is returned in the integer pointer
    *        variable.
    *
    * @param prio  Pointer to an int variable that will have the thread's
    *               priority stored in it.
    *
    * @return 0 is returned on successful completion.  -1 is returned on
    *         failure.
    */
   virtual int getprio(int* prio)
   {
      *prio = getpriority(PRIO_PROCESS, mThreadPID);

      if ((*prio)== -1)
      {
         return -1;
      }
      else
      {
         return 0;
      }
   }

   /**
    * Sets this thread's priority.
    *
    * @post This thread has its priority set to the specified value.
    *
    * @param prio  The new priority of the specified thread.
    *
    * @return 0 is returned on successful completion.  -1 is returned on
    *         failure.
    */
   int setprio(int prio)
   {
      return setpriority(PRIO_PROCESS, mThreadPID, prio);
   }

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
    * @return 0 is returned on successful completion.  -1 is returned on
    *         failure.
    */
   virtual int kill(int signum)
   {
      return ::kill(mThreadPID, signum);
   }

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
