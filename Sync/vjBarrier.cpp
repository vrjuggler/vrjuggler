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
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>
#include <Sync/vjBarrier.h>


// ---------------------------------------------------------------------------
// Block the caller until all <count> threads have called <wait> and then
// allow all the caller threads to continue in parallel.
// ---------------------------------------------------------------------------
int
vjBarrier::wait (void) {
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
