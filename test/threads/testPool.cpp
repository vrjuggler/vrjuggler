/// TOP OF TEST_C
#include <stdlib.h>
#include <iostream.h>
#include <stdio.h>

#ifdef C2_OS_SGI
#   include <ulocks.h>
#endif

#include <math.h>
#include <time.h>
#include <sys/time.h>

//#include <Sync/C2Mutex.h>
//#include <Sync/C2Semaphore.h>
//#include <Sync/C2Barrier.h>
//#include <Sync/NullMutex.h>
//#include <Threads/ThreadSGI.h>
#include <Threads/C2ThreadPool.h>
#include <SharedMem/C2Memory.h>
#include <Kernel/C2Debug.h>

void doIt(void*);

long counter;
C2Mutex counterMutex;

const int NUMTHREADS = 16;

	///---//  Beginning of main
void main()
{
    C2SharedPool myPool(65536, 16);    // size, num threads
    C2ThreadPool* thePool = new(&myPool) C2ThreadPool(NUMTHREADS);
    
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
	cout << C2Thread::self() << " Counter:" << counter << endl;
    counterMutex.release();
    
    float pdq = 3.14;
    long iters = (1000000*drand48());
    for(float q=0;q<iters;q += .34)
    {
	pdq = sin(sin(q)*q*cos(q*pdq));
    }
    	
}

