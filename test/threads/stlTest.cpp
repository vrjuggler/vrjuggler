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

#include <vector>
//#include </home/usr1/allenb/STL/vector.h>
#include <iostream.h>
#include <stdio.h>
#include <ulocks.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include <Threads/vjThreadPool.h>
#include <SharedMem/vjMemory.h>
#include <Kernel/vjDebug.h>

void doIt(void*);

template<class T>
void dumpVector(std::vector<T>& theVector);


const int NUMTHREADS = 2;

std::vector<int> intVector;
    
void main(void)
{
    vjSharedPool myPool(65536, 16);    // size, num threads
    vjThreadPool* thePool = new(&myPool) vjThreadPool(NUMTHREADS);
    
    cout << "\nMax: " << intVector.max_size() << endl;
    
    int params[1000];
    for(int z=0;z<1000;z++)
	params[z] = z;
    
//    for(int j=0;j<10;j++)
//    {
//	for(int x=0;x<5000;x++)
//	    intVector.push_back(x);
//	cerr << intVector.size() << endl;
//    }
	
//-------------------------------------------------
	    // Timing variables
    struct timeval startTime, endTime;		    // Used to calculate time
    double startSecond, endSecond, totalSeconds;    // performance
    
    gettimeofday(&startTime, NULL);	    // Get the starting time
//--------------------------------------------------
    
///*
    for (int i=0;i<15;i++) {
	thePool->startFunc((THREAD_FUNC)doIt, (void*)&(params[i]));      
    }
    thePool->barrier();
//*/
//--------------------------------------------------------
    gettimeofday(&endTime, NULL);	    // Get ending time
    startSecond = (startTime.tv_sec + (startTime.tv_usec * 0.000001));
    endSecond = endTime.tv_sec + (endTime.tv_usec * 0.000001);
    totalSeconds = (endSecond - startSecond);
    
///    cout << "\nDone: It took... " << totalSeconds << " seconds" << flush;
//---------------------------------------------------------

    sginap(100);
    
    DebugLock.acquire();
	dumpVector(intVector);
    DebugLock.release();    
}

void doIt(void* param)
{
    int* x = (int*)param;
    
    for(int i=0;i<100;i++)
	intVector.push_back(i+((*x)*100));    
}

template<class T>
void dumpVector(std::vector<T>& theVector)
{
    for(std::vector<T>::iterator i = theVector.begin(); i != theVector.end(); i++)
	cout << "\n" << *i;
    cout << endl;
}
