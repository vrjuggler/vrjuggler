/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#ifndef _VJBarrier_h_
#define _VJBarrier_h_

#include <vjConfig.h>

#ifdef VJ_IRIX_SPROC    // ---- SGI IPC Barrier ------ //
#   include <ulocks.h>
#   include <Sync/vjBarrierSGI.h>
    typedef  vjBarrierSGI vjBarrier;
#else

#include <Sync/vjCond.h>
#include <Sync/vjMutex.h>
#include <Sync/vjGuard.h>


//---------------------------------------------------------------------
//: Helper class for vjBarrier
//---------------------------------------------------------------------
class vjSubBarrier
{
public:
    // ------------------------------------------------------------------------
    //: Initialization.
    // ------------------------------------------------------------------------
    vjSubBarrier (int count, vjMutex* lock) : runningThreads(count), barrierFinished(lock)
    {}

    vjCond barrierFinished;   // True if this generation of the barrier is done.

    int runningThreads;  //: Number of threads that are still running.

    // ------------------------------------------------------------------------
    //: Dump the state of an object.
    // ------------------------------------------------------------------------
    void dump (void)
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
//!PUBLIC_API:
//---------------------------------------------------------------------
class vjBarrier
{
public:
    // ------------------------------------------------------------------------
    //: Initialize the barrier to synchronize <count> threads.
    // ------------------------------------------------------------------------
    vjBarrier (int count) : currentGeneration(0),
        count(count),
        subBarrier1(count, &mutex),
        subBarrier2(count, &mutex)
    {
        //cerr << "vjBarrier::vjBarrier: Entering." << endl;
        subBarrier[0] = &subBarrier1;
        subBarrier[1] = &subBarrier2;
    }

    // -----------------------------------------------------------------------
    //: Block the caller until all <count> threads have called <wait> and
    //+ then allow all the caller threads to continue in parallel.
    // -----------------------------------------------------------------------
    int wait(void);

    // -----------------------------------------------------------------------
    //: Tell the barrier to increase the count of the number of threads to
    //+ syncronize.
    // -----------------------------------------------------------------------
    void addProcess()
    {
        cerr << "vjBarrier::addProcess: Not implemented yet." << endl;
    }

    // -----------------------------------------------------------------------
    //: Tell the barrier to decrease the count of the number of threads to
    //+ syncronize.
    // -----------------------------------------------------------------------
    void removeProcess()
    {
        cerr << "vjBarrier::removeProcess: Not implemented yet." << endl;
    }

    // -----------------------------------------------------------------------
    //: Dump the state of an object.
    // -----------------------------------------------------------------------
    void dump (void) const {}

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


#endif	/* VJ_IRIX_SPROC */
#endif	/* _VJBarrier_h_ */
