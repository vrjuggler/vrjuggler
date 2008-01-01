/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include <vpr/Thread/Thread.h>
#include <vpr/Thread/TSObjectProxy.h>
#include <vpr/Thread/TSObject.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Debug.h>

int num_threads_to_create = 6;
int threads_created = 0;
int threads_exited = 0;
vpr::Mutex create_mutex;

void doFunc(vpr::TSObjectProxy<int>&);

int main()
{
   vpr::TSObjectProxy<int> tsCounter;     // Allocate a thread specific counter

   vpr::Thread* new_thread =
      new vpr::Thread(boost::bind(doFunc, boost::ref(tsCounter)));

   create_mutex.acquire();
      threads_created++;
   create_mutex.release();

   while ( true )
   {
      ;
   }

   return 0;
}

void doFunc(vpr::TSObjectProxy<int>& tsCounter)
{
   create_mutex.acquire();
      num_threads_to_create -= 1;
   create_mutex.release();

   for ( int i = 0; i < 3; ++i )
   {
      create_mutex.acquire();
         // All the threads we want have been created.
         if ( num_threads_to_create == 0 )
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << "All " << threads_created << " threads created\n"
               << vprDEBUG_FLUSH;
         }
         else if ( num_threads_to_create > 0 )
         {
            vpr::Thread* new_thread =
               new vpr::Thread(boost::bind(doFunc, boost::ref(tsCounter)));
            threads_created++;
         }
      create_mutex.release();
   }

   (*tsCounter) = 0;
   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << "Starting with: " << *tsCounter << std::endl << vprDEBUG_FLUSH;

   while ( *tsCounter < 1000 )
   {
      for ( int i = 0; i < 1000; ++i )
      {
         float x = sin((float) i);
      }
      (*tsCounter)++;
   }

   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << "Finished counting: Final val:" << *tsCounter << std::endl
      << vprDEBUG_FLUSH;

   create_mutex.acquire();
      threads_exited++;

      if ( threads_exited == threads_created )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "All threads have finished execution\n" << vprDEBUG_FLUSH;
      }
   create_mutex.release();
}
