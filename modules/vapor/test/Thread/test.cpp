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
#include <vpr/Thread/TSObjectProxy.h>
#include <vpr/Thread/TSObject.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Debug.h>

int num_threads_to_create = 6;
int threads_created = 0;
int threads_exited = 0;
vpr::Mutex create_mutex;

void doFunc(void*);

int main(void)
{
   vpr::TSObjectProxy<int>    tsCounter;     // Allocate a thread specific counter

   vpr::Thread* new_thread = new vpr::Thread(doFunc, (void*)(&tsCounter));

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
   vpr::TSObjectProxy<int>&  ts_counter = *(static_cast<vpr::TSObjectProxy<int>*>(TSCounterPtr));

   create_mutex.acquire();
      num_threads_to_create -= 1;
   create_mutex.release();

   for ( i = 0; i < 3; i++ ) {
      create_mutex.acquire();
         // All the threads we want have been created.
         if ( num_threads_to_create == 0 ) {
               vprDEBUG(vprDBG_ALL, 0) << "All " << threads_created
                                       << " threads created\n"
                                       << vprDEBUG_FLUSH;
         }
         else if ( num_threads_to_create > 0 ) {
            vpr::Thread* new_thread = new vpr::Thread(doFunc, TSCounterPtr);
            threads_created++;
         }
      create_mutex.release();
   }

   (*ts_counter) = 0;
   vprDEBUG(vprDBG_ALL, 0) << "Starting with: " << *ts_counter << std::endl
                           << vprDEBUG_FLUSH;

   while((*ts_counter) < 1000)
   {
      for(i=0;i<1000;i++)
         float x = sin((float) i);
      (*ts_counter)++;
   }

   vprDEBUG(vprDBG_ALL, 0) << "Finished counting: Final val:" << *ts_counter
                           << std::endl << vprDEBUG_FLUSH;

   create_mutex.acquire();
      threads_exited++;

      if ( threads_exited == threads_created ) {
          vprDEBUG(vprDBG_ALL, 0) << "All threads have finished execution\n"
                                  << vprDEBUG_FLUSH;
      }
   create_mutex.release();
}
