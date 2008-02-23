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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <vpr/vprConfig.h>

#include <cstdlib>
#include <boost/bind.hpp>

#include <vpr/Thread/ThreadPool.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Guard.h>
#include <vpr/Util/Debug.h>


namespace vpr
{

OneThread::OneThread()
   : thread(NULL)
   , next(NULL)
   , threadWait(0)
{
   /* Do nothing. */ ;
}

OneThread::~OneThread()
{
   /* Do nothing. */ ;
}

// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
ThreadPool::ThreadPool(const int numToStartWith)
   : mReadyThreads(0)
{
   //DebugLock.acquire();
   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
      << "[vpr::ThreadPool::ThreadPool()] Entering.\n" << vprDEBUG_FLUSH;
   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
      << "\tNumber of threads: " << numToStartWith << std::endl
      << vprDEBUG_FLUSH;
   //DebugLock.release();

   mListHead = NULL;
   mWorkingCount = 0;
   mListLock.release();             // release threadList
   mFinishedLock.release();         // Initialize if to threads being done

   //-- Start the initial # of threads ---//
   for ( int index = 0; index < numToStartWith; ++index )
   {
      addThread();
   }
}

ThreadPool::~ThreadPool()
{
   OneThread* cur_thread = mListHead;
   while ( cur_thread != NULL )
   {
      delete cur_thread->thread;
      cur_thread->thread = NULL;
      OneThread* old_thread = cur_thread;
      cur_thread = cur_thread->next;
      delete old_thread;
   }
}

/* Why did I have to hard code number in sproc group?
  Why aren't I tracking the number of threads started? */

// ----------------------------------------------------------------------------
// Body of a general-purpose child process. The argument, which must be
// declared void* to match the func prototype, is the vpr::OneThread structure
// that represents this process.   The contents of that struct, in
// particular threadWait, MUST be initialized by the parent.
// ----------------------------------------------------------------------------
void ThreadPool::threadLoop(OneThread* myThread)
{
//   DebugLock.acquire();
   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
      << "[vpr::ThreadPool::threadLoop()] Entering." << std::endl
      << vprDEBUG_FLUSH;
//      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
//      << "[vpr::ThreadPool::threadLoop()] theThreadAsVoid:"
//      << theThreadAsVoid << endl << vprDEBUG_FLUSH;
//   DebugLock.release();

   mListLock.acquire();
   mListLock.release();     // Do this to make sure addThread is done

   for ( ;; )
   {
      // --- WAIT FOR WORK --- //
      threadSleep(myThread);
      // ASSERT:  We now have work to do...
      // --- PROCESS ENTRY OVERHEAD --- //
      mWorkingCountLock.acquire();     // Get access to the working thread count
      if ( mWorkingCount == 0 )
      {
         mFinishedLock.acquire();       // Now there are threads working
      }
      ++mWorkingCount;                  // Update thread count
      mWorkingCountLock.release();

      // --- DO THE WORK --- //
      myThread->functor();

      // --- PROCESS EXIT OVERHEAD --- //
      mWorkingCountLock.acquire();     // Get access to the working count
      --mWorkingCount;
      if ( mWorkingCount == 0 )
      {
         mFinishedLock.release();       // Now there are no threads working
      }
      mWorkingCountLock.release();
   }
}

// ---------------------------------------------------------------------------
// Wait for work to do.  Put a vpr::OneThread structure on the ready list and
// sleep on it.  Called by a child process when its work is done.
// ---------------------------------------------------------------------------
void ThreadPool::threadSleep(OneThread* theThread)
{
   mListLock.acquire();              // acquire exclusive rights to threadList
   theThread->next = mListHead;      // put self on head of the list
   mListHead = theThread;
   mListLock.release();              // release threadList

   mReadyThreads.release();          // notify master, at least 1 on the list

   theThread->threadWait.acquire();  // sleep until master needs/releases me
}

// ---------------------------------------------------------------------------
// Get a process to run.  Acquire a vpr::OneThread structure from the ready
// list, waiting if necessary.  Called by the master process as part of
// dispatching a thread.
// ---------------------------------------------------------------------------
OneThread* ThreadPool::getThread()
{
   OneThread* the_thread;

   mReadyThreads.acquire();        // wait until at least 1 thread is free

   mListLock.acquire();            // acquire exclusive rights to threadList
   the_thread = mListHead;         // get address of first free OneThread
   mListHead = the_thread->next;   // make next in list, the head of list
   mListLock.release();            // release threadList

   return the_thread;
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void ThreadPool::printList() const
{
   OneThread* cur_thread(mListHead);
   int counter(0);

   std::cerr << "----- Thread List -----\n";

   while ( cur_thread != NULL )
   {
      std::cerr << "Thread: " << counter++ << std::endl;
      std::cerr << "\tpid: " << *cur_thread << std::endl;
      cur_thread = cur_thread->next;
   }
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
OneThread* ThreadPool::addThread()
{
   static int numTimes = 0;
//    DebugLock.acquire();
   vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
      << "[vpr::ThreadPool::addThread()] Entering: " << ++numTimes
      << std::endl << vprDEBUG_FLUSH;
//    DebugLock.release();

   Guard<Mutex> guard(mListLock);   // Protect the head

   //OneThread* newThread = new (this->getMyMemPool()->allocate(sizeof(OneThread))) OneThread;    // Used placement new
   OneThread* newThread = new OneThread;
   newThread->next = NULL;
   newThread->thread = new Thread(boost::bind(&ThreadPool::threadLoop, this,
                                              newThread));

//    DebugLock.acquire();
   vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
      << newThread->thread << " [vprThreadPool::addThread()] List at end\n"
      << vprDEBUG_FLUSH;
   printList();
//    DebugLock.release();

   return mListHead;
}

std::ostream& operator<<(std::ostream& outfile, vpr::OneThread& thread)
{
   outfile << thread.thread;
   return outfile;
}

} // End of vpr namespace
