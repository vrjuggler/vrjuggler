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

#include <iostream.h>
#include <stdio.h>
#include <ulocks.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include <sprocGroup.h>
#include <Sync/vjBarrier.h>

void doIt(void* param);

void printArray(void);

const int NUMTHREADS = 10;
vjBarrier myBarrier(NUMTHREADS);

///---//  Beginning of main
void main()
{
    float x;
    cout << "Hello there\n\n" << flush;
    cout << "Start:\n\n";

    
    sprocInit(NULL);
    sginap(100);	

//-------------------------------------------------
	    // Timing variables
    struct timeval startTime, endTime;		    // Used to calculate time
    double startSecond, endSecond, totalSeconds;    // performance
    
    gettimeofday(&startTime, NULL);	    // Get the starting time
//--------------------------------------------------
    
    for (float i=0;i<NUMTHREADS;i++) {
	sprocFunc(doIt,NULL,NULL);
	for(long i=0;i<10000;i++)
	    x = (i*i)/11.0;
	          
    }
    
    sprocBarrier();
    
//--------------------------------------------------------
    gettimeofday(&endTime, NULL);	    // Get ending time
    startSecond = (startTime.tv_sec + (startTime.tv_usec * 0.000001));
    endSecond = endTime.tv_sec + (endTime.tv_usec * 0.000001);
    totalSeconds = (endSecond - startSecond);
    
    cout << "\nDone: It took... " << totalSeconds << " seconds" << flush;
//---------------------------------------------------------
    
    sginap(100);
    
    cout << "End:\n\n";
    cout << "\n" << flush;
    
    // -- Clean up --- //
    //    delete mySemaphore;
//    delete myMutex;    
}


void doIt(void* param)
{
    cerr << "Entering doIt...\n" << endl;
    myBarrier.wait();
    cerr << "Exiting doIt...\n" << endl;
}
