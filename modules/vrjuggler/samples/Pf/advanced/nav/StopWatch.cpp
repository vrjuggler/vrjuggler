/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <vpr/System.h>
#include "StopWatch.h"

///////////////////////////////////
// Implementation:
///////////////////////////////////

//: Default constructor.
StopWatch::StopWatch(const int averageFpsRefreshRate)
   : timeAverage(0.0)
   , timeInstant(0.0)
   , count(0)
   , fpsAverage(0.0)
   , fpsInstant(0.0)
   , refreshRate(averageFpsRefreshRate)
   , startTime(0.0)
   , stopTime(0.0)
   , _timeAccumulator(0.0)
{;}

//: get the current time
//  useful for profiling
//  returns - a number of any scalar type (see top for time format)
// TODO: does compiler optimize the
//       case where T = double???, if not, then un-template this.
void StopWatch::getTime(double& num)
{
   vpr::TimeVal tv;
   vpr::System::gettimeofday(&tv);

   // compose sec with microsec for sec.millisec
   double goodPrecision = static_cast<double>(tv.tv_sec) +
                             (static_cast<double>(tv.tv_usec) / 1000000.0);

   // cast the high-precision number down to what user wants.
   num = goodPrecision;
}

//: Starts the stopwatch
//  result - sets the value startTime
void StopWatch::start()
{
   getTime(startTime);
}

//: Stops the stopwatch
//  results - sets the value stopTime,      <BR>
//            sets the value time,     <BR>
//            sets the value fpsAverage (every [refreshRate] times that stop is called),    <BR>
//            sets the value fpsInstant,    <BR>
//            sets the value count.    <BR>
void StopWatch::stop()
{
   getTime(stopTime);

   // get the time for this one frame.
   timeInstant = stopTime - startTime;

   // every [refreshRate] frames, calc the average FPS
   if (count % refreshRate == 0)
   {
      double refreshrate = refreshRate;
      fpsAverage = refreshrate / _timeAccumulator;
      timeAverage = _timeAccumulator / refreshrate;

      // reset the accumulator
      _timeAccumulator = 0.0;
   }

   // accumulate the frame times to later calc
   // the average FPS time.
   _timeAccumulator += timeInstant;

   // calculate the instantaneous FPS value (1 frame/sec)
   fpsInstant = 1.0 / timeInstant;

   // Increment the number of frames elapsed
   ++count;
}

//: Reset the stopwatch
//  result - Resets every param except for [refreshRate]
void StopWatch::reset()
{
   startTime = 0.0;
   stopTime = 0.0;
   _timeAccumulator = 0.0;

   count = 0;
   fpsAverage = 0.0;
   fpsInstant = 0.0;
   timeInstant = 0.0;
}
