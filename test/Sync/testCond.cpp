/// TOP OF TEST_C

#include <iostream.h>
#include <stdio.h>
#include <ulocks.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include <sprocGroup.h>
#include <Sync/C2Cond.h>

void decrementIt(void*);
void checkIt(void* param);

void printArray(void);

const int ITERATIONS = 25;
long counter;
long num;
long exitFlag;

C2Cond myCond;
///---//  Beginning of main
void main()
{
    cout << "Hello there\n\n" << flush;
    cout << "Start:\n\n";

    counter = 1000;
    num = 0;
    exitFlag = 0;
    cerr << "Counter: " << counter << endl;
    
    
    sprocInit(NULL);
    sginap(100);	

//-------------------------------------------------
	    // Timing variables
    struct timeval startTime, endTime;		    // Used to calculate time
    double startSecond, endSecond, totalSeconds;    // performance
    
    gettimeofday(&startTime, NULL);	    // Get the starting time
//--------------------------------------------------
    
    sprocFunc(decrementIt, NULL, NULL);
    
    for (float i=0;i<ITERATIONS;i++) {
	sprocFunc(checkIt,NULL,NULL);      
    }
    
    sprocBarrier();
    
//--------------------------------------------------------
    gettimeofday(&endTime, NULL);	    // Get ending time
    startSecond = (startTime.tv_sec + (startTime.tv_usec * 0.000001));
    endSecond = endTime.tv_sec + (endTime.tv_usec * 0.000001);
    totalSeconds = (endSecond - startSecond);
    
    cerr << "\nDone: It took... " << totalSeconds << " seconds" << flush;
//---------------------------------------------------------
    
    sginap(100);
    cout << "\n\nCounter: " << counter << endl << flush;
    
    cout << "End:\n\n";
    cout << "\n" << flush;
    
    // -- Clean up --- //
    //    delete mySemaphore;
//    delete myMutex;    
}

void decrementIt(void* param)
{
///---//  Beginning of doIt

    while(exitFlag == 0)
    {
	myCond.acquire();
	    if (counter > 0)
		counter--;
	    
	    myCond.signal();	    
	myCond.release();
    }
}

void checkIt(void* param)
{
    myCond.acquire();
	    // Want to wait for counter == 0
	while(counter != 0)
	    myCond.wait();
	    
	cerr << "We are done with ...." << ++num << endl << flush;
	counter = 1000;
	if (num >= ITERATIONS) {
	    cerr << "Setting exit flag" << endl;
	    exitFlag = 1;
	    }
    myCond.release();
}
