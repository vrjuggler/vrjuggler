#ifndef _VJ_BASE_THREAD_H_
#define _VJ_BASE_THREAD_H_
// --------------------------------------------------------------------------
// vjBaseThread.h
// $Revision$
// $Date$
// --------------------------------------------------------------------------
// NOTES:
//    - This is used as the base class for all thread classes.
// --------------------------------------------------------------------------

#include <config.h>
#include <iostream.h>
#include <hash_map.h>
#include <Threads/vjThreadFunctor.h>

class vjBaseThread
{
public:
   vjBaseThread()
      : mThreadId(0)
   {;}

   virtual ~vjBaseThread()
   {
      ;
   }

protected:
   //: After the object has been created, call this routine to complete initialization.
   // Done this way, because I need to call this based on stuff that happens
   // in derived class's constructor
   //! POST: Thread is setup correctly to run
   //+       The thread has been registered with the system
   //! ARGS: successfulCreation - Did the thread get created correctly
   void registerThread(bool succesfulCreation);

public:
   // -----------------------------------------------------------------------
   //: Create a new thread that will execute functorPtr.
   //
   //! PRE: None.
   //! POST: A thread (with any specified attributes) is created that begins
   //+       executing func().
   //
   //! ARGS: functorPtr - Function to be executed by the thread.
   //! ARGS: flags - Flags for the thread--not currently used in this
   //+               implementation (optional).
   //! ARGS: priority - Priority of created thread (optional).
   //! ARGS: stack_addr - Alternate address for thread's stack (optional).
   //! ARGS: stack_size - Size for thread's stack (optional).
   //
   //! RETURNS:  0 - Successful thread creation
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   virtual int spawn ( vjBaseThreadFunctor* functorPtr, long flags = 0,
                        u_int priority = 0, void* stack_addr = NULL,
                        size_t stack_size = 0)
   {
      cerr << "Base spawn.  Should NOT be called." << endl;
      return -1;
   }


   // -----------------------------------------------------------------------
   //: Make the calling thread wait for the termination of this thread.
   //
   //! PRE: None.
   //! POST: The caller blocks until this thread finishes its execution
   //+       (i.e., calls the exit() method).  This routine may return
   //+       immediately if this thread has already exited.
   //
   //! ARGS: status - Status value of the terminating thread when that
   //+                thread calls the exit routine (optional).
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   virtual int join (void** status = 0)
   {return -1;}

   // -----------------------------------------------------------------------
   //: Wait for one or more threads to exit.
   //
   //! PRE: None.
   //! POST: The caller blocks until the specified threads finish their
   //+       execution (i.e., call the exit() method).  This routine may
   //+       return immediately if the specified threads have already
   //+       exited.
   //
   //! ARGS: thread_array - Array of one or more thread IDs to be joined.
   //! ARGS: n - The number of threads to join.
   //! ARGS: ret_val - Storage for return value of an exiting thread
   //+                 (optional).
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   int join (vjBaseThread* thread_array[], int n, void** ret_val = 0)
   { return -1;}

   // -----------------------------------------------------------------------
   //: Resume the execution of a thread that was previously suspended using
   //+ suspend().
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   //
   //! NOTE: This is not currently supported on HP-UX 10.20.
   // -----------------------------------------------------------------------
   virtual int resume()
   {return -1;}

   // -----------------------------------------------------------------------
   //: Suspend the execution of this thread.
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   //
   //! NOTE: This is not currently supported on HP-UX 10.20.
   // -----------------------------------------------------------------------
   virtual int suspend()
   {return -1;}


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
   //
   // -----------------------------------------------------------------------
   virtual int getPrio(int* prio)
   { return -1;}

   // -----------------------------------------------------------------------
   //: Set this thread's priority.
   //
   //! ARGS: prio - The new priority for this thread.
   //
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   virtual int setPrio(int prio)
   { return -1;}

   // -----------------------------------------------------------------------
   //: Yield execution of the calling thread to allow a different blocked
   //+ thread to execute.
   // -----------------------------------------------------------------------
   virtual void yield()
   {;}

   // -----------------------------------------------------------------------
   //: Get the "thread ID" of this vjThreadPosix object.  This is a unique
   //+ integer value assigned when the thread was created.
   //
   //! RETURNS: -1 - Bad thread. Creation error.
   //! RETURNS: other - id of thread in Juggler
   // -----------------------------------------------------------------------
   long getTID()
   { return mThreadId; }

   //: Is this a valid thread?
   //
   //! RETURNS: true - object represents a thread that has been created corectly
   bool valid()
   {
      return (mThreadId != -1);
   }
   // -----------------------------------------------------------------------
   //: Send the specified signal to this thread (not necessarily SIGKILL).
   //
   //! POST: This thread receives the specified signal.
   //! ARGS: signum - The signal to send to the specified thread.
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
   virtual int kill (int signum)
   { return -1;}

   // -----------------------------------------------------------------------
   //: Kill (cancel) this thread.
   //
   //! PRE: None.
   //! POST: This thread is cancelled. Immediate
   //+       cancellation is not guaranteed.
   // -----------------------------------------------------------------------
   virtual void kill()
   {;}


   // -----------------------------------------------------------------------
   //: Ouput the state of the object
   // -----------------------------------------------------------------------
   virtual ostream& outStream(ostream& out)
   {
      out << "t:" << mThreadId << " ";
      return out;
   }


protected:
   // Initialize the state of the object
   void create_thread_id()
   {
      mThreadId = getNextThreadId();
   }

private:    // Don't allow copy
   vjBaseThread(vjBaseThread& other)
   {;}

private:
   long  mThreadId;     // The local id for the thread.

   // --- STATICS ---- //

private:
   long getNextThreadId()
   {
      return mNextThreadId++;
   }

   static long mNextThreadId;    // Initialized to 0
};

// Ouput operator
ostream& operator<<(ostream& out, vjBaseThread* threadPtr);


//: Helper class that maintains a table of the threads allocated in the system
//
// Used internally because we can have many types of indexes for the thread list
// depending upon the type of threads being used.
template <class IdxType>
class vjThreadTable
{
public:
   //: Add a thread to the list
   void addThread(vjBaseThread* threadPtr, IdxType index)
   {
      mThreadMap[index] = threadPtr;
   }

   //: Get a thread from the list
   vjBaseThread* getThread(IdxType index)
   {
      hash_map<IdxType, vjBaseThread*>::iterator i;
      i = mThreadMap.find(index);
      if(i == mThreadMap.end())
         return NULL;
      else
         return (*i).second;
   }

   //: Remove a thread from the list
   void removeThread(IdxType index)
   {
      mThreadMap.erase(index);
   }

private:
   hash_map<IdxType, vjBaseThread*> mThreadMap;
};

#endif
