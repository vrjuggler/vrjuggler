/// TOP OF TEST_CPP

#include <iostream.h>
#include <stdio.h>
#include <ulocks.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include <sprocGroup.h>
#include <Sync/vjMutex.h>
#include <Sync/vjSemaphore.h>
#include <Sync/NullMutex.h>

void doIt(void*);
void printArray(void);

long counter;
//NullMutex myMutex;
vjMutex* myMutex;
vjSemaphore* mySemaphore;

///---//  Beginning of main
void main()
{
    mySemaphore = new vjSemaphore;
    myMutex = new vjMutex;
    //myMutex = new NullMutex;
    cout << "Hello there\n\n" << flush;
    cout << "Start:\n\n";

    counter = 0;
    cout << "Counter: " << counter << endl;
    
    
    sprocInit(NULL);
    sginap(100);	

//-------------------------------------------------
	    // Timing variables
    struct timeval startTime, endTime;		    // Used to calculate time
    double startSecond, endSecond, totalSeconds;    // performance
    
    gettimeofday(&startTime, NULL);	    // Get the starting time
//--------------------------------------------------
    
    for (float i=0;i<10000;i++) {
	sprocFunc(doIt,NULL,NULL);      
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
    cout << "\n\nCounter: " << counter << endl << flush;
    
    cout << "End:\n\n";
    cout << "\n" << flush;
    
    // -- Clean up --- //
    delete mySemaphore;
    delete myMutex;    
}

void doIt(void* param)
{
///---//  Beginning of doIt

    //mySemaphore->acquire();
    //myMutex.acquire();
    mySemaphore->acquire();
	int tempCounter = counter;
	
	for(int j=0;j<1000;j++)
	    counter++; 
	
	counter = tempCounter + 1;
	//cerr << "Counter now: " << counter << endl << flush;
    mySemaphore->release();
    //myMutex.release();
    //mySemaphore->release();
}

