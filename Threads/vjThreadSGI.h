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


#ifndef _THREAD_SGI_H_
#define _THREAD_SGI_H_
//#pragma once

// NOTE: This file(vjThreadSGI.h) MUST be included by vjThread.h.
// Not the other way around

#include <vjConfig.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <signal.h>
#include <sched.h>
#include <unistd.h>

#include <Threads/vjThreadFunctor.h>
#include <Threads/vjBaseThread.h>

//: Threads implementation using multiple processes created with sproc(2).

//!PUBLIC_API:
class vjThreadSGI : public vjBaseThread
{
public:
   /***** CONSTRUCTORS ******/

   // -----------------------------------------------------------------------
   //: Spawning constructor.
   //  This will actually start a new thread that will execute the specified
   //  function.
   // -----------------------------------------------------------------------
   vjThreadSGI(vj_thread_func_t func, void* arg = 0, long flags = 0,
               u_int priority = 0, void* stack_addr = NULL,
               size_t stack_size = 0);

   // -----------------------------------------------------------------------
   //: Spawning constructor with arguments (functor version).
   //  This will start a new thread that will execute the specified
   //  function.
   // -----------------------------------------------------------------------
   vjThreadSGI(vjBaseThreadFunctor* functorPtr, long flags = 0,
               u_int priority = 0, void* stack_addr = NULL,
               size_t stack_size = 0);

   virtual ~vjThreadSGI()
   {;}

   // -----------------------------------------------------------------------
   //: Spawns a new thread that will execute functorPtr.
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
   //
   //! RETURNS: non-zero - Successful thread creation
   //! RETURNS:       -1 - Error
   // -----------------------------------------------------------------------
   virtual int
   spawn (vjBaseThreadFunctor* functorPtr, long flags = 0, u_int priority = 0,
          void* stack_addr = NULL, size_t stack_size = 0)
   {
      mThreadPID = sproc(vj_thread_func_t(&ThreadFunctorFunction), PR_SADDR,
                         functorPtr);
      return mThreadPID;
   }

   // Called by the spawn routine to start the user thread function
    // PRE: Called ONLY by a new thread
    // POST: The new thread will have started the user thread function
    void startThread(void* null_param);

private:
   // The functor to call from startThread
   vjBaseThreadFunctor* mUserThreadFunctor;

public:

   // -----------------------------------------------------------------------
   //: Make the calling thread wait for the termination of the specified
   //+ thread.
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   //
   //! NOTE:  Not implemented.
   // -----------------------------------------------------------------------
   virtual int
   join (void** = 0);

   // -----------------------------------------------------------------------
   //: Resume the execution of a thread that was previously suspended using
   //+ suspend().
   //
   //! PRE: The specified thread was previously suspended using the
   //+      suspend() member function.
   //! POST: The specified thread is sent the SIGCONT signal and is allowed
   //+       to begin executing again.
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   virtual int resume (void)
   { return ::kill(mThreadPID, SIGCONT);}

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
   virtual int suspend (void)
   { return ::kill(mThreadPID, SIGSTOP);}

   // -----------------------------------------------------------------------
   //: Get this thread's priority.
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
   virtual int getprio (int* prio)
   {
      *prio = getpriority(PRIO_PROCESS, mThreadPID);

      if ((*prio)== -1)
         return -1;
      else
         return 0;
   }

   // -----------------------------------------------------------------------
   //: Set this thread's priority.
   //
   //! PRE: None.
   //! POST: This thread has its priority set to the specified value.
   //
   //! ARGS: prio - The new priority of the specified thread.
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   inline int
   setprio (int prio)
   {
      return setpriority(PRIO_PROCESS, mThreadPID, prio);
   }

   // -----------------------------------------------------------------------
   //: Yield execution of the calling thread to allow a different blocked
   //+ thread to execute.
   //
   //! PRE: None.
   //! POST: The caller yields its execution control to another thread or
   //+       process.
   // -----------------------------------------------------------------------
   virtual void yield (void)
   { sginap(0);}


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
   // -----------------------------------------------------------------------
   virtual int kill (int signum)
   { return ::kill(mThreadPID, signum);}

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
   // -----------------------------------------------------------------------
   virtual void kill (void)
   { kill(SIGKILL);}


   // -----------------------------------------------------------------------
   //: Output the state of the object.
   // -----------------------------------------------------------------------
   std::ostream& outStream(std::ostream& out)
   {
      out.setf(std::ios::right);
      out << std::setw(7) << std::setfill('0') << mThreadPID << "/";
      out.unsetf(std::ios::right);
      vjBaseThread::outStream(out);
      out << std::setfill(' ');
      return out;
   }

private:
   pid_t mThreadPID;      //: pid_t data structure for this thread

   // --------  STATICS ------ //
   // This data is used to maintain a thread table in the system
   // It is here because the self() function needs to use system specific
   // information.
public:
   // -----------------------------------------------------------------------
   //: Get a ptr to the thread we are in.
   //
   //! RETURNS: NULL - Thread is not in global table
   //! RETURNS: NonNull - Ptr to the thread that we are running within
   // -----------------------------------------------------------------------
   static vjBaseThread* self()
   {
      return getLocalThreadPtr();
   }

private:
   // Set the thread ptr stored in the local PRDA area
   // NOTE: PRDA is a memory address that each thread has a seperate copy of.
   static void setLocalThreadPtr(vjThreadSGI* threadPtr)
   {
      ((ThreadInfo*)PRDA->usr_prda.fill)->mThreadPtr = threadPtr;
   }

   // Get the thread ptr stored in the local PRDA area
   static vjThreadSGI* getLocalThreadPtr()
   {
      return ((ThreadInfo*)PRDA->usr_prda.fill)->mThreadPtr;
   }

   // Structure for storing thread specific information
   struct ThreadInfo
   {
      vjThreadSGI* mThreadPtr;
   };
};


#endif   /* _THREAD_SGI_H_ */
