/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include <VPR/Sync/vjMutex.h>
//#include <VPR/Sync/vjSemaphore.h>
//#include <VPR/Sync/vjBarrier.h>
//#include <VPR/Sync/vjNullMutex.h>
//#include <VPR/md/SPROC/vjThreadSGI.h>
#include <VPR/Threads/vjThreadPool.h>
#include <VPR/SharedMem/vjMemPool.h>
#include <Utils/vjDebug.h>
#include <VPR/vjSystem.h>

void doIt(void*);

long counter;
vjMutex counterMutex;

const int NUMTHREADS = 16;

	///---//  Beginning of main
int main(void )
{
    vjSharedPool myPool(65536, 16);    // size, num threads
    vjThreadPool* thePool = new(&myPool) vjThreadPool(NUMTHREADS);
    vjMutex DebugLock;
    
    DebugLock.acquire();
	std::cout << "Hello there\n\n" << std::flush;
	std::cout << "Start:\n\n";
    
	counter = 0;
	std::cout << "Counter: " << counter << std::endl;
    DebugLock.release();
    
        
    for (float i=0;i<70;i++) {
	thePool->startFunc((THREAD_FUNC)doIt);      
    }
    thePool->barrier();

    std::cerr << "\n\nPast the barrier:" << counter << "\n\n" << std::flush;

    counter = 0;

    for (float z=0;z<30;z++) {
	thePool->startFunc((THREAD_FUNC)doIt);
    }
    thePool->barrier();
  
    DebugLock.acquire();
	std::cout << "\n\nCounter: " << counter << std::endl << std::flush;
	
	std::cout << "End:\n\n";
	std::cout << "\n" << std::flush;
	std::cerr << "\n\nDone:" << std::endl;
    DebugLock.release();

}

void doIt(void* param)
{
    counterMutex.acquire();
	counter = counter+1;
	std::cout << vjThread::self() << " Counter:" << counter << std::endl;
    counterMutex.release();
    
    float pdq = 3.14;
    long iters = (1000000*drand48());
    for(float q=0;q<iters;q += .34)
    {
	pdq = vjSystem::sin(vjSystem::sin(q)*q*vjSystem::cos(q*pdq));
    }
    	
}
