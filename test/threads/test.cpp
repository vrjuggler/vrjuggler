#include <iostream.h>
#include <stdio.h>
#include <math.h>

#include <Threads/vjThread.h>
#include <Threads/vjTSObjectProxy.h>
#include <Threads/vjTSObject.h>
#include <Sync/vjMutex.h>
#include <Kernel/vjDebug.h>

int num_threads_to_create = 6;
int threads_created = 0;
int threads_exited = 0;
vjMutex create_mutex;

void doFunc(void*);

int main(void)
{
   vjTSObjectProxy<int>    tsCounter;     // Allocate a thread specific counter

   vjThread* new_thread = new vjThread(doFunc, (void*)(&tsCounter));

   create_mutex.acquire();
      threads_created++;
   create_mutex.release();

   while(1)
   {
      ;
   }
}


void doFunc(void* TSCounterPtr)
{
   int i;
   vjTSObjectProxy<int>&  ts_counter = *(static_cast<vjTSObjectProxy<int>*>(TSCounterPtr));

   create_mutex.acquire();
      num_threads_to_create -= 1;
   create_mutex.release();

   for ( i = 0; i < 3; i++ ) {
      create_mutex.acquire();
         // All the threads we want have been created.
         if ( num_threads_to_create == 0 ) {
               vjDEBUG(vjDBG_ALL, 0) << "All " << threads_created
                                     << " threads created\n" << vjDEBUG_FLUSH;
         }
         else if ( num_threads_to_create > 0 ) {
            vjThread* new_thread = new vjThread(doFunc, TSCounterPtr);
            threads_created++;
         }
      create_mutex.release();
   }

   (*ts_counter) = 0;
   vjDEBUG(vjDBG_ALL, 0) << "Starting with: " << *ts_counter << endl
                         << vjDEBUG_FLUSH;

   while((*ts_counter) < 1000)
   {
      for(i=0;i<1000;i++)
         float x = sin(i);
      (*ts_counter)++;
   }

   vjDEBUG(vjDBG_ALL, 0) << "Finished counting: Final val:" << *ts_counter
                         << endl << vjDEBUG_FLUSH;

   create_mutex.acquire();
      threads_exited++;

      if ( threads_exited == threads_created ) {
          vjDEBUG(vjDBG_ALL, 0) << "All threads have finished execution\n"
                                << vjDEBUG_FLUSH;
      }
   create_mutex.release();

   (vjThread::self())->exit();
}
