#ifndef _VJBarrier_h_
#define _VJBarrier_h_

#include <vjConfig.h>

#ifdef VJ_SGI_IPC    // ---- SGI IPC Barrier ------ //
#   include <ulocks.h>
#   include <Sync/vjBarrierSGI.h>
    typedef  vjBarrierSGI vjBarrier;
#else

#include <Sync/vjCond.h>
#include <Sync/vjMutex.h>
#include <Sync/vjGuard.h>

//: Helper class for vjBarrier
class vjSubBarrier 
{ 
public:
    //: Initialization.
    vjSubBarrier (int count, vjMutex* lock) : runningThreads(count), barrierFinished(lock)
    {}

    vjCond barrierFinished;   // True if this generation of the barrier is done.

    int runningThreads;  // Number of threads that are still running.

    void dump (void)     // Dump the state of an object.
    {
        cerr << "vjSubBarrier::dump" << endl;
        this->barrierFinished.dump();
    }
};

//---------------------------------------------------------------------
//: Implements "barrier synchronization".
//
//     This class allows <count> number of threads to synchronize
//     their completion (so-called "barrier synchronization").  The
//     implementation uses a "sub-barrier generation numbering"
//     scheme to avoid overhead and to ensure that all threads exit
//     the barrier correct.  This code is based on an article from
//     SunOpsis Vol. 4, No. 1 by Richard Marejka
//     (Richard.Marejka@canada.sun.com).
//---------------------------------------------------------------------
class vjBarrier 
{
public:
    // Initialize the barrier to synchronize <count> threads.
    vjBarrier (int count) : currentGeneration(0),
        count(count),
        subBarrier1(count, &mutex),
        subBarrier2(count, &mutex)
    {
        //cerr << "vjBarrier::vjBarrier: Entering." << endl;
        subBarrier[0] = &subBarrier1;
        subBarrier[1] = &subBarrier2;    
    }
      
    // Block the caller until all <count> threads have called <wait> and
    // then allow all the caller threads to continue in parallel.
    int wait (void)  
    {
        vjGuard<vjMutex> guard(mutex);
      
        vjSubBarrier* curvjSubBarrier = this->subBarrier[currentGeneration];
    
        // Check for shutdown...
        if (curvjSubBarrier == NULL) {
            return -1;
        }
    
        if (curvjSubBarrier->runningThreads == 1)
        {
            // We're the last running thread, so swap generations and tell
            // all the threads waiting on the barrier to continue on their
            // way.
    
            curvjSubBarrier->runningThreads = this->count;
	  
            // Swap generations.
            currentGeneration = 1 - this->currentGeneration;	    // Cycles between 0 and 1
            curvjSubBarrier->barrierFinished.broadcast();
        }
        else
        {
            --curvjSubBarrier->runningThreads;
    
            // Block until all the other threads wait().
            while (curvjSubBarrier->runningThreads != count)
                curvjSubBarrier->barrierFinished.wait ();
        }
    
        return 0;  
    }


    //-----------------------------------------------------------
    // addProcess/removeProcess
    // PURPOSE: 
    //	    Tell the barrier to increase/decrease the count of the number
    //	    of threads to syncronize
    //-----------------------------------------------------------
  void addProcess()
  {
    cerr << "vjBarrier::addProcess: Not implemented yet." << endl;
  }
  
  void removeProcess()
  {
    cerr << "vjBarrier::removeProcess: Not implemented yet." << endl;
  }
  
    void dump (void) const {}
    // Dump the state of an object.

private:
    vjMutex mutex;   // Serialize access to the barrier state.

    // Either 0 or 1, depending on whether we are the first generation
    // of waiters or the next generation of waiters.
    int currentGeneration;
  

    int count; // Total number of threads that can be waiting at any one time.
  
    vjSubBarrier subBarrier1;
    vjSubBarrier subBarrier2;
    vjSubBarrier* subBarrier[2];
    // We keep two <sub_barriers>, one for the first "generation" of
    // waiters, and one for the next "generation" of waiters.  This
    // efficiently solves the problem of what to do if all the first
    // generation waiters don't leave the barrier before one of the
    // threads calls wait() again (i.e., starts up the next generation
    // barrier).

    // = Prevent assignment and initialization.
    void operator= (const vjBarrier &) {}
    vjBarrier (const vjBarrier &) : subBarrier1(0, &mutex), subBarrier2(0, &mutex)  {}
};


#endif
#endif
