/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

#ifndef _VPR_Barrier_h_
#define _VPR_Barrier_h_

#include <vpr/vprConfig.h>

#if VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_IRIX_SPROC
#   include <ulocks.h>
#   include <vpr/md/SPROC/Sync/BarrierSGI.h>
#elif (VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_NSPR) || \
      (VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_POSIX)
#include <vpr/Sync/CondVar.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>


namespace vpr {

//---------------------------------------------------------------------
//: Helper class for vpr::Barrier
//---------------------------------------------------------------------
class SubBarrier
{
public:
    // ------------------------------------------------------------------------
    //: Initialization.
    // ------------------------------------------------------------------------
    SubBarrier (int count, Mutex* lock)
       : barrierFinished(lock), runningThreads(count)
    {}

    CondVar barrierFinished; // True if this generation of the barrier is done.

    int runningThreads;  //: Number of threads that are still running.

    // ------------------------------------------------------------------------
    //: Dump the state of an object.
    // ------------------------------------------------------------------------
    void dump (void)
    {
        std::cerr << "vpr::SubBarrier::dump" << std::endl;
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
class Barrier
{
public:
    // ------------------------------------------------------------------------
    //: Initialize the barrier to synchronize <count> threads.
    // ------------------------------------------------------------------------
    Barrier (int count) : currentGeneration(0),
        count(count),
        subBarrier1(count, &mutex),
        subBarrier2(count, &mutex)
    {
        //std::cerr << "vpr::Barrier::Barrier: Entering." << std::endl;
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
        std::cerr << "vpr::Barrier::addProcess: Not implemented yet."
                  << std::endl;
    }

    // -----------------------------------------------------------------------
    //: Tell the barrier to decrease the count of the number of threads to
    //+ syncronize.
    // -----------------------------------------------------------------------
    void removeProcess()
    {
        std::cerr << "vpr::Barrier::removeProcess: Not implemented yet."
                  << std::endl;
    }

    // -----------------------------------------------------------------------
    //: Dump the state of an object.
    // -----------------------------------------------------------------------
    void dump (void) const {}

private:
    Mutex mutex;   // Serialize access to the barrier state.

    // Either 0 or 1, depending on whether we are the first generation
    // of waiters or the next generation of waiters.
    int currentGeneration;


    int count; // Total number of threads that can be waiting at any one time.

    SubBarrier subBarrier1;
    SubBarrier subBarrier2;
    SubBarrier* subBarrier[2];
    // We keep two <sub_barriers>, one for the first "generation" of
    // waiters, and one for the next "generation" of waiters.  This
    // efficiently solves the problem of what to do if all the first
    // generation waiters don't leave the barrier before one of the
    // threads calls wait() again (i.e., starts up the next generation
    // barrier).

    // = Prevent assignment and initialization.
    void operator= (const Barrier &) {;}
    Barrier (const Barrier &)
       : subBarrier1(0, &mutex), subBarrier2(0, &mutex)
    {;}
};

}; // End of vpr namespace

#endif	/* ! VPR_USE_IRIX_SPROC */

#endif	/* _VPR_Barrier_h_ */
