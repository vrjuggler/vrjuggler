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

/// TOP OF TEST_C
#include <stdlib.h>
#include <iostream.h>
#include <stdio.h>

#ifdef VJ_OS_IRIX
#   include <ulocks.h>
#endif

#include <math.h>
#include <time.h>
#include <sys/time.h>

//#include <Sync/vjMutex.h>
//#include <Sync/vjSemaphore.h>
//#include <Sync/vjBarrier.h>
//#include <Sync/NullMutex.h>
//#include <Threads/ThreadSGI.h>
#include <Threads/vjThreadPool.h>
#include <SharedMem/vjMemory.h>
#include <Kernel/vjDebug.h>

void doIt(void*);

long counter;
vjMutex counterMutex;

const int NUMTHREADS = 16;

	///---//  Beginning of main
void main()
{
    vjSharedPool myPool(65536, 16);    // size, num threads
    vjThreadPool* thePool = new(&myPool) vjThreadPool(NUMTHREADS);
    
    DebugLock.acquire();
	cout << "Hello there\n\n" << flush;
	cout << "Start:\n\n";
    
	counter = 0;
	cout << "Counter: " << counter << endl;
    DebugLock.release();
    
        
    for (float i=0;i<70;i++) {
	thePool->startFunc((THREAD_FUNC)doIt);      
    }
    thePool->barrier();
  
    cerr << "\n\nPast the barrier:" << counter << "\n\n" << flush;
    
    counter = 0;
    
    for (float z=0;z<30;z++) {
	thePool->startFunc((THREAD_FUNC)doIt);      
    }
    thePool->barrier();
  
    DebugLock.acquire();
	cout << "\n\nCounter: " << counter << endl << flush;
	
	cout << "End:\n\n";
	cout << "\n" << flush;
	cerr << "\n\nDone:" << endl;
    DebugLock.release();

}

void doIt(void* param)
{
    counterMutex.acquire();
	counter = counter+1;
	cout << vjThread::self() << " Counter:" << counter << endl;
    counterMutex.release();
    
    float pdq = 3.14;
    long iters = (1000000*drand48());
    for(float q=0;q<iters;q += .34)
    {
	pdq = sin(sin(q)*q*cos(q*pdq));
    }
    	
}

