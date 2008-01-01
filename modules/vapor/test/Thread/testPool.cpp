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

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include <vpr/Sync/Mutex.h>
//#include <vpr/Sync/Semaphore.h>
//#include <vpr/Sync/Barrier.h>
//#include <vpr/Sync/NullMutex.h>
#include <vpr/Thread/ThreadPool.h>
#include <vpr/Util/Debug.h>
#include <vpr/System.h>

void doIt();

long counter;
vpr::Mutex counterMutex;

const int NUMTHREADS = 16;

///---//  Beginning of main
int main()
{
   vpr::ThreadPool* thePool = new vpr::ThreadPool(NUMTHREADS);
   vpr::Mutex DebugLock;

   DebugLock.acquire();
      std::cout << "Hello there\n\n" << std::flush;
      std::cout << "Start:\n\n";

      counter = 0;
      std::cout << "Counter: " << counter << std::endl;
   DebugLock.release();

   for ( int i = 0; i < 70; ++i )
   {
      thePool->startFunc(doIt);
   }
   thePool->wait();

   std::cerr << "\n\nPast the barrier:" << counter << "\n\n" << std::flush;

   counter = 0;

   for ( int z = 0; z < 30; ++z )
   {
      thePool->startFunc(doIt);
   }
   thePool->wait();

   DebugLock.acquire();
      std::cout << "\n\nCounter: " << counter << std::endl << std::flush;

      std::cout << "End:\n\n";
      std::cout << "\n" << std::flush;
      std::cerr << "\n\nDone:" << std::endl;
   DebugLock.release();

   return 0;
}

void doIt()
{
   counterMutex.acquire();
      counter = counter+1;
      std::cout << vpr::Thread::self() << " Counter:" << counter << std::endl;
   counterMutex.release();

   float pdq = 3.14;
   long iters = 1000000 * drand48();
   for ( float q = 0; q < iters; q += 0.34 )
   {
      pdq = sin(sin(q) * q * cos(q * pdq));
   }
}
