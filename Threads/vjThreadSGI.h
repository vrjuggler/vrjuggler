#ifndef _THREAD_SGI_H_
#define _THREAD_SGI_H_

// NOTE: This file(vjThreadSGI.h) MUST be included by vjThread.h.
// Not the other way around

#include <config.h>
#include <iostream.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <signal.h>
#include <sched.h>

#include <Threads/vjThreadFunctor.h>
#include <Threads/vjBaseThread.h>

//: Threads implementation using multiple processes created with sproc(2).

class vjThreadSGI : public vjBaseThread
{
public:
   /***** CONSTRUCTORS ******/

   //: Spawning constructor
   //  This will actually start a new thread
   //+ that will execute the specified function.
   vjThreadSGI (THREAD_FUNC func, void* arg = 0, long flags = 0,
                 u_int priority = 0, void* stack_addr = NULL,
                 size_t stack_size = 0);


   //: Spawning constructor with arguments (functor version).
   //   This will start a new
   //+  thread that will execute the specified function.
   vjThreadSGI ( vjBaseThreadFunctor* functorPtr, long flags = 0,
                  u_int priority = 0, void* stack_addr = NULL,
                  size_t stack_size = 0);


   virtual ~vjThreadSGI()
   { mThreadTable.removeThread(mThreadPID);}

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
   spawn ( vjBaseThreadFunctor* functorPtr, long flags = 0, u_int priority = 0,
           void* stack_addr = NULL, size_t stack_size = 0)
   {
      mThreadPID = sproc(THREAD_FUNC(&ThreadFunctorFunction), PR_SADDR, functorPtr);
      return mThreadPID;
   }


   // -----------------------------------------------------------------------
   //: Make the calling thread wait for the termination of the specified
   //+ thread.
   //! NOTE:  Not implemented.
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
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
   { sched_yield();}


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
   //: Output the state of the object
   // -----------------------------------------------------------------------
   ostream& outStream(ostream& out)
   {
      out << "p:" << mThreadPID << " ";
      vjBaseThread::outStream(out);
      return out;
   }

private:
   pid_t mThreadPID;      //: pid_t data structure for this thread


   // --------  STATICS ------ //
   // This data is used to maintain a thread table in the system
   // It is here because the self() function needs to use system specific information
public:
   //: Get a ptr to the thread we are in
   //
   //!RETURNS: NULL - Thread is not in global table
   //!RETURNS: NonNull - Ptr to the thread that we are running within
   static vjBaseThread* self()
   {
      pid_t cur_pid = getpid();
      vjBaseThread* cur_thread = mThreadTable.getThread(cur_pid);

      return cur_thread;
   }
private:
   static vjThreadTable<pid_t>    mThreadTable;
};


#endif	/* _THREAD_SGI_H_ */
