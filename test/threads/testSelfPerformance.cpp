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

#include <iostream.h>
#include <stdio.h>
#include <math.h>

#include <Threads/vjThread.h>
#include <Sync/vjMutex.h>
#include <Kernel/vjDebug.h>
#include <Utils/vjTimer.h>

#define NUM_THREADS 4

int thread_count = 0;
vjMutex thread_count_mutex;        // Protect the count of number of threads running

vjTimer timers[NUM_THREADS];        // Timers to get the times to call self()

const long num_reps = 100000;          // Number of times to call

void doFunc(void*);

int main(void)
{
   for(int thread_num=0;thread_num < NUM_THREADS;thread_num++)
   {
      vjThread* new_thread = new vjThread(doFunc, (void*)(thread_num));

      // Increment the thread count
      thread_count_mutex.acquire();
         thread_count++;
      thread_count_mutex.release();
   }

   // Wait until all threads exited
   // NOTE: Don't have to guard because I am just looking
   while(thread_count != 0)
   {;}

   double total_avg(0.0f);
   for(int x=0;x<NUM_THREADS;x++)
   {
      total_avg += timers[x].getTiming();
   }

   total_avg /= (double)NUM_THREADS;

   vjDEBUG(vjDBG_ALL,0) << "AVERAGE TIMING: " << total_avg*1000.0f << "ms" << endl << vjDEBUG_FLUSH;
}


void doFunc(void* void_thread_num)
{
   int thread_num = (int)void_thread_num;

   vjDEBUG(vjDBG_ALL, 0) << "Thread: " << thread_num << ": Entering.\n" << vjDEBUG_FLUSH;
         
   vjBaseThread* my_thread;

   for(long rep=0;rep<num_reps;rep++)
   {
      timers[thread_num].startTiming();
      my_thread = vjThread::self();
      timers[thread_num].stopTiming();
      my_thread->yield();
   }
   
   vjDEBUG(vjDBG_ALL, 0) << "Thread: " << thread_num << ": Exiting: Avg Time of: "
                         << (timers[thread_num].getTiming()*1000.0f) << "ms" << endl << vjDEBUG_FLUSH;
   thread_count_mutex.acquire();
      thread_count--;               // Removing us from the number there
   thread_count_mutex.release();
}
