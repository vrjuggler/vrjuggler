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
