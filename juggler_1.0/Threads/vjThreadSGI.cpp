/*
 * ---------------------------------------------------------------------------
 * vjThreadSGI.cpp
 * $Revision$
 * $Date$
 * ---------------------------------------------------------------------------
 */

#include <iostream.h>
#include <sys/types.h>
#include <sys/prctl.h>

#include <Threads/vjThread.h>
#include <Threads/vjThreadSGI.h>
#include <Threads/vjThreadManager.h>

vjThreadTable<pid_t> vjThreadSGI::mThreadTable;



//: Spawning constructor
//  This will actually start a new thread
//+ that will execute the specified function.
vjThreadSGI::vjThreadSGI (THREAD_FUNC func, void* arg, long flags,
             u_int priority, void* stack_addr, size_t stack_size)
{
   vjThreadManager::instance()->lock();
   {
      vjThreadNonMemberFunctor* NonMemFunctor = new vjThreadNonMemberFunctor(func, arg);
      int retVal = spawn(NonMemFunctor, flags, priority, stack_addr, stack_size);
      if (retVal > 0)
      {
         registerThread(true);
         mThreadTable.addThread(this, mThreadPID);
      }
      else
         registerThread(false);     // Failed to create
   }
   vjThreadManager::instance()->unlock();
}


//: Spawning constructor with arguments (functor version).
//   This will start a new
//+  thread that will execute the specified function.
vjThreadSGI::vjThreadSGI ( vjBaseThreadFunctor* functorPtr, long flags,
              u_int priority, void* stack_addr, size_t stack_size)
{
   vjThreadManager::instance()->lock();
   {
      int retVal = spawn(functorPtr, flags, priority, stack_addr, stack_size);
      if (retVal > 0)
      {
         registerThread(true);
         mThreadTable.addThread(this, mThreadPID);
      }
      else
         registerThread(false);     // Failed to create));
   }
   vjThreadManager::instance()->unlock();
}


// -----------------------------------------------------------------------
   //: Make the calling thread wait for the termination of the specified
   //+ thread.
   //! NOTE:  Not implemented.
   //! RETURNS:  0 - Successful completion
   //! RETURNS: -1 - Error
   // -----------------------------------------------------------------------
    int
   vjThreadSGI::join (void** arg)
   {
      cerr << "vjThreadSGI::join() not implemented yet!\n";
      return -1;
   }

/*
// ---------------------------------------------------------------------------
// Create n new threads that execute func() with the argument arg if given.
//
// PRE: The thread_ids[] array should not have anything stored in it.  Its
//      contents are destroyed in the routine.
// POST: The thread ID's created in this routine are stored and returned in
//       the array thread_ids[] so that the caller may access them.
// ---------------------------------------------------------------------------
int
vjThreadSGI::create_n (vjThreadSGI thread_ids[], int n, THREAD_FUNC func,
                     void* arg, long flags, u_int priority,
                     void* stack_addr[], size_t stack_size[])
{
    for ( int i = 0; i < n; i++ ) {
        vjThreadSGI* newThread = vjThread::spawn(func, arg, flags, priority,
                                               (stack_addr == 0) ? 0 : stack_addr[i],
                                               (stack_size == 0) ? 0 : stack_size[i]);

        thread_ids[i] = *newThread;
    }

    return i;
}

*/
