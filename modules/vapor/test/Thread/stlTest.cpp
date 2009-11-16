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
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#include <vector>
#include <iostream>
#include <stdio.h>

#include <math.h>
#include <time.h>
#include <sys/time.h>

#include <boost/bind.hpp>

#include <vpr/Thread/ThreadPool.h>
#include <vpr/Util/Debug.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/System.h>

void doIt(const int);

template<class T>
void dumpVector(std::vector<T>& theVector);


const int NUMTHREADS = 2;

std::vector<int> intVector;

int main()
{
   vpr::ThreadPool* thePool = new vpr::ThreadPool(NUMTHREADS);
   vpr::Mutex DebugLock;

   std::cout << "\nMax: " << intVector.max_size() << std::endl;

   int params[1000];
   for ( int z = 0; z < 1000; ++z )
   {
      params[z] = z;
   }

//   for ( int j = 0; j < 10; ++j )
//   {
//      for ( int x = 0; x < 5000 ; ++x )
//      {
//         intVector.push_back(x);
//      }
//      cerr << intVector.size() << endl;
//   }

//-------------------------------------------------
   // Timing variables
   struct timeval startTime, endTime;		    // Used to calculate time
   double startSecond, endSecond, totalSeconds;    // performance

   gettimeofday(&startTime, NULL);	    // Get the starting time
//--------------------------------------------------

///*
   for ( int i = 0; i < 15; ++i )
   {
      thePool->startFunc(boost::bind(doIt, params[i]));
   }
   thePool->wait();
//*/
//--------------------------------------------------------
   gettimeofday(&endTime, NULL);       // Get ending time
   startSecond = (startTime.tv_sec + (startTime.tv_usec * 0.000001));
   endSecond = endTime.tv_sec + (endTime.tv_usec * 0.000001);
   totalSeconds = (endSecond - startSecond);

//   std::cout << "\nDone: It took... " << totalSeconds << " seconds"
//             << std::flush;
//---------------------------------------------------------

   vpr::System::usleep(100);

   DebugLock.acquire();
      dumpVector(intVector);
   DebugLock.release();

   return 0;
}

void doIt(const int x)
{
   for ( int i = 0; i < 100; ++i )
   {
      intVector.push_back(i + (x * 100));
   }
}

template<class T>
void dumpVector(std::vector<T>& theVector)
{
   typename std::vector<T>::iterator i;
   for ( i = theVector.begin(); i != theVector.end(); ++i )
   {
      std::cout << "\n" << *i;
   }
   std::cout << std::endl;
}
