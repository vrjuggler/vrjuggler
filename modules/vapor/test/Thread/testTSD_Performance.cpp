/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vpr.h>

#include <iostream>
#include <stdio.h>
#include <math.h>

#include <vpr/Thread/Thread.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Timer.h>

#include <vpr/Thread/TSObjectProxy.h>
#include <vpr/Thread/TSObject.h>


#define MAX_NUM_THREADS 1000

int thread_count = 0;
vpr::Mutex thread_count_mutex;        // Protect the count of number of threads running

vpr::Timer timers[MAX_NUM_THREADS];        // Timers to get the times to call self()

const long num_reps = 100000;          // Number of times to call

vpr::TSObjectProxy<int>    tsDataItem;     // Allocate a thread specific int

void doFunc(void*);

int main(int argc, char* argv[])
{
   if(argc <= 1)
   {
      std::cout << "Usage: testTSD_Performance <num_threads>\n" << std::flush;
      exit(1);
   }

   int num_threads = atoi(argv[1]);
   std::cout << "Using num_threads: " << num_threads << std::endl;

   // Run in primordial thread first.
   int temp_val = 0;
   doFunc((void*) temp_val);
   thread_count++;

   // Then run in num_threads -1 spawned threads.
   for(int thread_num=1;thread_num < num_threads;thread_num++)
   {
      vpr::Thread* new_thread = new vpr::Thread(doFunc, (void*)(thread_num));

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
   for(int x=0;x<num_threads;x++)
   {
      total_avg += (timers[x].getTiming()/(double)num_reps);
   }

   total_avg /= (double)num_threads;

   vprDEBUG(vprDBG_ALL,0) << "AVERAGE TIMING: " << total_avg*1000.0f << "ms"
                          << std::endl << vprDEBUG_FLUSH;
}


void doFunc(void* void_thread_num)
{
   int thread_num = (int)void_thread_num;

   vprDEBUG(vprDBG_ALL, 0) << "Thread: " << thread_num << ": Entering.\n"
                           << vprDEBUG_FLUSH;

   timers[thread_num].startTiming();
   for(long rep=0;rep<num_reps;rep++)
   {
     (*tsDataItem) = rep;
   }
   timers[thread_num].stopTiming();

   vprDEBUG(vprDBG_ALL, 0) << "Thread: " << thread_num
                           << ": Exiting: Avg Time of: "
                           << ((timers[thread_num].getTiming()/(double)num_reps)*1000.0f)
                           << "ms" << std::endl << vprDEBUG_FLUSH;
   thread_count_mutex.acquire();
      thread_count--;               // Removing us from the number there
   thread_count_mutex.release();
}
