/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
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
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>
#include <new.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
//#include <unistd.h>

#ifndef WIN32
#   include <sys/resource.h>
#endif

#include <Threads/vjThreadPool.h>
#include <Threads/vjThread.h>
#include <SharedMem/vjMemory.h>
#include <Sync/vjGuard.h>
#include <Kernel/vjDebug.h>

// XXX: There is a bug that causes a memory leak
//   - We never delete the Functors

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
vjThreadPool::vjThreadPool (int numToStartWith) : readyThreads(0) {
    DebugLock.acquire();
      vjDEBUG(vjDBG_ALL,6) << "vjThreadPool::vjThreadPool: Entering.\n" << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_ALL,5) << "\tvjThreadPool::vjThreadPool: Number threads: "
                 << numToStartWith << endl << vjDEBUG_FLUSH;
    DebugLock.release();

    listHead = NULL;
    workingCount = 0;
    listLock.release();	    // release threadList
    finishedLock.release();	    // Initialize if to threads being done

       //-- Start the initial # of threads ---//
    for (int index=0;index < numToStartWith;index++)
    {
        addThread();
    }
}


/* Why did I have to hard code number in sproc group?
  Why aren't I tracking the number of threads started? */

// ---------------------------------------------------------------------------
// Body of a general-purpose child process. The argument, which must be
// declared void* to match the func prototype, is the vjOneThread structure
// that represents this process.   The contents of that struct, in
// particular threadWait, MUST be initialized by the parent.
// ---------------------------------------------------------------------------
void
vjThreadPool::threadLoop(void* theThreadAsVoid) {
   DebugLock.acquire();
   vjDEBUG(vjDBG_ALL,6) << vjThread::self() << " vjThreadPool::threadLoop: Entering."
   << endl << vjDEBUG_FLUSH;
//      vjDEBUG(vjDBG_ALL,5) << vjThread::self()
//      << " vjThreadPool::threadLoop: theThreadAsVoid:"
//      << theThreadAsVoid << endl << vjDEBUG_FLUSH;
   DebugLock.release();

   listLock.acquire();
   listLock.release();     // Do this to make sure addThread is done

   vjOneThread* myThread = (vjOneThread*)theThreadAsVoid;

#ifdef VJ_IRIX_SPROC
   // --- SIGNAL Handlers ---- //
   prctl(PR_TERMCHILD);       // What should I do with. FIX - Allen
#endif

   for (;;)
   {
      // --- WAIT FOR WORK --- //
      threadSleep(myThread);
      // ASSERT:  We now have work to do...
      // --- PROCESS ENTRY OVERHEAD --- //
      workingCountLock.acquire();     // Get access to the working thread count
      if (workingCount == 0)
      {
         finishedLock.acquire();       // Now there are threads working
      }
      workingCount = workingCount + 1;    // Update thread count
      workingCountLock.release();

      // --- DO THE WORK --- //
      myThread->functor->operator()();

      // --- PROCESS EXIT OVERHEAD --- //
      workingCountLock.acquire();     // Get access to the working count
      workingCount = workingCount - 1;
      if (workingCount == 0)
      {
         finishedLock.release();       // Now there are no threads working
      }
      workingCountLock.release();
   }
}

// ---------------------------------------------------------------------------
// Wait for work to do.  Put a vjOneThread structure on the ready list and
// sleep on it.  Called by a child process when its work is done.
// ---------------------------------------------------------------------------
void
vjThreadPool::threadSleep(vjOneThread* theThread) {
    listLock.acquire();		      // acquire exclusive rights to threadList
        theThread->next = listHead;   // put self on head of the list
        listHead = theThread;
    listLock.release();		      // release threadList

    readyThreads.release();	      // notify master, at least 1 on the list

    theThread->threadWait.acquire();  // sleep until master needs/releases me
}

// ---------------------------------------------------------------------------
// Get a process to run.  Acquire a vjOneThread structure from the ready list,
// waiting if necessary.  Called by the master process as part of
// dispatching a thread.
// ---------------------------------------------------------------------------
vjOneThread*
vjThreadPool::getThread (void) {
    vjOneThread* theThread;

    readyThreads.acquire();	    // wait until at least 1 thread is free

    listLock.acquire();		    // acquire exclusive rights to threadList
        theThread = listHead;           // get address of first free vjOneThread
        listHead = theThread->next;     // make next in list, the head of list
    listLock.release();		    // release threadList/

    return theThread;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void
vjThreadPool::printList (void) {
    vjOneThread* curThread = listHead;
    int counter = 0;

    cerr << "----- Thread List -----\n";

    while(curThread != NULL) {
        cerr << "Thread: " << counter++ << endl;
        cerr << "\tpid: " << *curThread << endl;
        curThread = curThread->next;
    }
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
vjOneThread* vjThreadPool::addThread (void)
{
    static int numTimes = 0;
    DebugLock.acquire();
      vjDEBUG(vjDBG_ALL,6) << vjThread::self() << " vjThreadPool::addThread: Entering: "
                 << ++numTimes << endl << vjDEBUG_FLUSH;
    DebugLock.release();

    vjGuard<vjMutex> guard(listLock);   // Protect the head

    vjOneThread* newThread = new (this->getMyMemPool()->allocate(sizeof(vjOneThread))) vjOneThread;	// Used placement new
    newThread->next = NULL;

//    vjThreadMemberFunctor<vjThreadPool>* memberFunctor = new vjThreadMemberFunctor<vjThreadPool>(this, vjThreadPool::threadLoop, (void*)newThread);
    vjThreadMemberFunctor<vjThreadPool>* memberFunctor = new vjThreadMemberFunctor<vjThreadPool>(this, &vjThreadPool::threadLoop, (void*)newThread);

    newThread->thread = new vjThread(memberFunctor, 0);

    DebugLock.acquire();
        vjDEBUG(vjDBG_ALL,5) << newThread->thread
                   << " vjThreadPool::addThread: List at end\n" << vjDEBUG_FLUSH;
        printList();
    DebugLock.release();

    return listHead;
}

ostream& operator<< (ostream& outfile, vjOneThread& thread)
{
    outfile << thread.thread;
    return outfile;
}
