/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <Threads/vjThread.h>
#include <Threads/vjThreadWin32.h>


vjThreadTable<DWORD> vjThreadWin32::mThreadTable;


//: Spawning constructor
//  This will actually start a new thread
//  that will execute the specified function.
vjThreadWin32::vjThreadWin32 (THREAD_FUNC func, void* arg, long flags,
             u_int priority, void* stack_addr, size_t stack_size)
{
   vjThreadManager::instance()->lock();
   {
      vjThreadNonMemberFunctor* NonMemFunctor = new vjThreadNonMemberFunctor(func, arg);
      int retVal = spawn(NonMemFunctor, flags, priority, stack_addr, stack_size);
      if (retVal > 0)
      {
         registerThread(true);
         mThreadTable.addThread(this, mThreadTID);
      }
      else
         registerThread(false);     // Failed to create
   }
   vjThreadManager::instance()->unlock();
}


//: Spawning constructor with arguments (functor version).
//   This will start a new
//   thread that will execute the specified function.
vjThreadWin32::vjThreadWin32 ( vjBaseThreadFunctor* functorPtr, long flags,
              u_int priority, void* stack_addr, size_t stack_size)
{
   vjThreadManager::instance()->lock();
   {
      int retVal = spawn(functorPtr, flags, priority, stack_addr, stack_size);
      if (retVal > 0)
      {
         registerThread(true);
         mThreadTable.addThread(this, mThreadTID);
      }
      else
         registerThread(false);     // Failed to create));
   }
   vjThreadManager::instance()->unlock();
}


// ---------------------------------------------------------------------------
// Create a new thread that will execute functorPtr.
//
// PRE: None.
// POST: A thread (with any specified attributes) is created that begins
//       executing func().  Depending on the scheduler, it may being
//       execution immediately, or it may block for a short time before
//       beginning execution.
// ---------------------------------------------------------------------------
int
vjThreadWin32::spawn ( vjBaseThreadFunctor* functorPtr, long flags, u_int priority,
                     void* stack_addr, size_t stack_size)
{
    
	 unsigned int thread_id;
    mThreadHandle = (HANDLE) _beginthreadex(NULL,0, &ThreadFunctorFunction, functorPtr, 0, &thread_id);
    if (0 == mThreadHandle)
    {
       cerr << "ERROR: Could not create thread." << endl; 
		 return -1;
    }

    mThreadTID = thread_id;
	 

    return 1;
}



