/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef STOP_WATCH_INCLUDED
#define STOP_WATCH_INCLUDED


//: A StopWatch
//  This class monitors frame rate, and general time performance metrics
//  NOTE: The times are expressed as seconds (in 64bit floating point)
//        since midnight (00:00) Coordinated Universal Time (UTC),
//        January 1, 1970.
class StopWatch
{
// Time metrics:
public:
   //: the time between the last start/stop cycle averaged
   //: over timeRefreshRate start/stop cycles
   //  format - see NOTE at top for time format
   //  result - undefined if used before stop() is called
   double         timeAverage;

   //: the time between the last start/stop cycle
   //: (time = stopTime - startTime)
   //  format - see NOTE at top for time format
   //  result - undefined if used before stop() is called
   double         timeInstant;

// Frames per second metrics:
public:
   //: the number start/stop cycles
   //  Number start/stop cycles that have been
   //  performed since the last reset() or since construction
   unsigned long  count;

   //: The average frames per second over [refreshRate] start/stop cycles
   double         fpsAverage;

   //: the frames per second between the last start/stop cycle
   double         fpsInstant;

// StopWatch settings:
public:
   //: the refresh rate for fpsAverage and timeAverage (in number of frames)
   //  fpsInstant is averaged over [refreshRate] number of
   //    start/stop cycles
   //  see also - fpsAverage for the average frames per second value.
   //  see also - timeAverage for the average time value.
   //  TODO: make this based on time rather than cycles
   int            refreshRate;

// Start/stop values:
public:
   //: the time at which start() was invoked last
   // format: see NOTE at top for time format
   double         startTime;

   //: the time at which stop() was invoked last
   // format - see NOTE at top for time format
   double         stopTime;

private:
   double         _timeAccumulator;

public:
   //: Default constructor.
   StopWatch(const int averageFpsRefreshRate = 15);

   //: get the current time
   //  useful for profiling
   //  returns - a double (see top for time format)
   static void    getTime(double& num);

   //: Starts the stopwatch
   //  result - sets the value startTime
   void           start();

   //: Stops the stopwatch
   //  results - sets the value stopTime,      <BR>
   //            sets the value time,     <BR>
   //            sets the value fpsAverage (every [refreshRate] times that stop is called),    <BR>
   //            sets the value fpsInstant,    <BR>
   //            sets the value count.    <BR>
   void           stop();

   //: Reset the stopwatch
   //  result - Resets every param except for [refreshRate]
   void           reset();
};

#endif
