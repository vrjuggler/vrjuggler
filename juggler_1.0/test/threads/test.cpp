/// TOP OF TEST_C
#include <iostream.h>
#include <stdio.h>

#ifdef C2_OS_SGI
#   include <ulocks.h>
#endif

#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include <Sync/C2Mutex.h>
#include <Sync/C2Semaphore.h>
#include <Sync/C2Barrier.h>
#include <Sync/NullMutex.h>
#include <Threads/C2Thread.h>

void doIt(void*);
void printArray(void);

long counter;
C2Mutex myMutex;
C2Semaphore mySema;

const int NUMTHREADS = 8;
C2Barrier myBarrier(1);

///---//  Beginning of main
int main()
{
    cout << "Hello there\n\n" << flush;
    cout << "Start:\n\n";

    counter = 0;
    cout << "Counter: " << counter << endl;
    
    
//-------------------------------------------------
	    // Timing variables
    struct timeval startTime, endTime;		    // Used to calculate time
    double startSecond, endSecond, totalSeconds;    // performance
    
    gettimeofday(&startTime, NULL);	    // Get the starting time
//--------------------------------------------------
    
    for (float i=0;i<NUMTHREADS;i++) {
	//sprocFunc(doIt,NULL,NULL);
	C2Thread::spawn(doIt);      
    }
    myBarrier.wait();
//--------------------------------------------------------
    gettimeofday(&endTime, NULL);	    // Get ending time
    startSecond = (startTime.tv_sec + (startTime.tv_usec * 0.000001));
    endSecond = endTime.tv_sec + (endTime.tv_usec * 0.000001);
    totalSeconds = (endSecond - startSecond);
    
    cout << "\nDone: It took... " << totalSeconds << " seconds" << flush;
//---------------------------------------------------------
    
    usleep(33333300);
    cout << "\n\nCounter: " << counter << endl << flush;
    
    cout << "End:\n\n";
    cout << "\n" << flush;
    
     float x = 123.45;
     
//     while(1)
//	x = x*-1;
    
    // -- Clean up --- //
    //    delete mySemaphore;
//    delete myMutex;    

    return 0;
}

void doIt(void* param)
{
///---//  Beginning of doIt
    float x = 1000.0034;
//    for(long k=0;k<10000;k++)
//	x = (k*x)/(3.14*k);
    myBarrier.addProcess();
    //mySemaphore->acquire();
    //myMutex.acquire();
    mySema.acquire();
	cerr << "\n-------------------------------------------\n"
	     << "\tMy Id: " << C2Thread::self() << endl;
	counter++;
	cerr << "Counter now: " << counter << endl << flush;
    mySema.release();
    //myMutex.release();
    //mySemaphore->release();
    myBarrier.wait();
    
//    while(1)
//	x = x*-1;
	
}

