#include <iostream.h>
#include <stdio.h>
#include <math.h>

#include <Threads/vjThread.h>
#include <Threads/vjTSObjectProxy.h>
#include <Threads/vjTSObject.h>
#include <Kernel/vjDebug.h>

int num_threads_to_create = 6;

void doFunc(void*);

int main(void)
{
   vjTSObjectProxy<int>    tsCounter;     // Allocate a thread specific counter

   vjThread* new_thread = new vjThread(doFunc, (void*)(&tsCounter));


   while(1)
   {
      ;
   }
}


void doFunc(void* TSCounterPtr)
{
   vjTSObjectProxy<int>&  ts_counter = *(static_cast<vjTSObjectProxy<int>*>(TSCounterPtr));

   num_threads_to_create -= 1;
   if(num_threads_to_create > 0)
      vjThread* new_thread = new vjThread(doFunc, TSCounterPtr);
   if(num_threads_to_create > 0)
      vjThread* new_thread2 = new vjThread(doFunc, TSCounterPtr);
   if(num_threads_to_create > 0)
      vjThread* new_thread3 = new vjThread(doFunc, TSCounterPtr);

   (*ts_counter) = 0;
   vjDEBUG(0) << "Starting with: " << *ts_counter << endl << flush << vjDEBUG_FLUSH;

   while((*ts_counter) < 1000)
   {
      for(int i=0;i<1000;i++)
         float x = sin(i);
      (*ts_counter)++;
   }

   vjDEBUG(0) << "Finished counting: Final val:" << *ts_counter << endl << vjDEBUG_FLUSH;
}

