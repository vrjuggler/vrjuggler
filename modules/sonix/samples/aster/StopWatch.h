
//////////////////////////////////////////////////////////////////
//
//                   -=    StopWatch    =-
//
// Definition: "cross platform timer"
//
///////////////// <auto-copyright BEGIN do not edit this line> /////////////////
//
//   $RCSfile$
//   $Date$
//   $Revision$
//   Copyright (C) 1998, 1999, 2000  Kevin Meinert, kevin@vrsource.org
//
//   This library is free software; you can redistribute it and/or
//   modify it under the terms of the GNU Library General Public
//   License as published by the Free Software Foundation; either
//   version 2 of the License, or (at your option) any later version.
//
//   This library is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//   Library General Public License for more details.
//
//   You should have received a copy of the GNU Library General Public
//   License along with this library; if not, write to the Free
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
///////////////// <auto-copyright END do not edit this line> ///////////////////
#ifndef STOP_WATCH_INCLUDED
#define STOP_WATCH_INCLUDED

#if ! defined(WIN32) || ! defined(WIN64)
#  include <sys/time.h>
#endif
#include <assert.h>

/** A cross platform timer in the form of a StopWatch.
 *  Measures simple time metrics: absolute system time, time deltas.
 *  NOTE: All times expressed in seconds (64bit floating point)
 */
class StopWatch
{
public:
   /** Default constructor. */
   StopWatch();

/** @name StopWatch controls */
//@{
   /** Starts the stopwatch.
    *  @post sets the value mTimeStarted
    */
   void         start();
   
   /** Stops the stopwatch
    *  @post sets the value mTimeStopped
    *         sets the value time
    */
   void         stop();
   
   /**  Reset the stopwatch.
    *  @post Resets every param
    */
   void         reset();
   
   /** Pulse: executes a stop() immediately followed a start().
    *  you can call this as a simple way to get time deltas between pulses.
    */
   void         pulse();
//@}
         
/** @name Time metrics */
//@{
   /** the time between the last start/stop (or the last two pulses).
    *  @post undefined if used before stop() is called
    *        returns timeStop() - timeStart()
    *        time is expressed in seconds (64bit floating point)
    */
   inline const double&  timeDelta() const { return mTimeDelta; }

   /** the time at which start() was invoked last.
    *  format: see NOTE at top for time format
    */
   inline const double&  timeStart() const { return mTimeStarted; }

   /** the time at which stop() was invoked last.
    *  @post time is expressed in seconds (64bit floating point)
    */
   inline const double&  timeStop() const { return mTimeStopped; }
//@}
   
   /** getTime function.
    *  useful for profiling, returns current time in seconds.
    *  precision is to usec on some systems, msec on others.
    *  @post returns a 64bit float expressed as seconds since midnight (00:00)
    *        Coordinated Universal Time (UTC), January 1, 1970.
    */
   static double         getTime();

protected:
/** @name time metrics */
//@{
   double       mTimeDelta; 
   double       mTimeStarted;
   double       mTimeStopped;
//@}
};

///////////////////////////////////
// Implementation:
///////////////////////////////////

/** Default constructor. */
inline StopWatch::StopWatch() : mTimeDelta( 0.0 ),
                        mTimeStarted( 0.0 ), 
                        mTimeStopped( 0.0 ) 
{
}

/** getTime function.
 *  useful for profiling, returns current time in seconds.
 *  precision is to usec on some systems, msec on others.
 *  @post returns a 64bit float expressed as seconds since midnight (00:00)
 *        Coordinated Universal Time (UTC), January 1, 1970.
 */
#if ! defined(WIN32) && ! defined(WIN64)
   inline double StopWatch::getTime()
   {
     struct timeval tv;
     gettimeofday( &tv, 0 );

     // compose sec with microsec
     return static_cast<double>( tv.tv_sec )
        + ( static_cast<double>( tv.tv_usec ) / 1000000.0 );
   }
#else
   #include <sys/types.h>
   #include <sys/timeb.h>
   inline double StopWatch::getTime()
   {
     struct _timeb tv;
     _ftime( &tv );

     // compose sec with millisec
     return static_cast<double>( tv.time )
        + ( static_cast<double>( tv.millitm ) / 1000.0 );
   }
#endif

/** Starts the stopwatch.
 *  @post sets the value mTimeStarted
 */
inline void StopWatch::start()
{
   mTimeStarted = StopWatch::getTime();
}

/** Stops the stopwatch
 *  @post sets the value mTimeStopped
 *         sets the value time
 */
inline void StopWatch::stop()
{
   mTimeStopped = StopWatch::getTime();
   mTimeDelta = mTimeStopped - mTimeStarted;
}

/**  Reset the stopwatch.
 *  @post Resets every param
 */
inline void StopWatch::reset()
{
   mTimeStarted = 0.0;
   mTimeStopped = 0.0;
   mTimeDelta = 0.0;
}

/** Pulse: executes a stop() immediately followed a start().
 *  you can call this as a simple way to get time deltas between pulses.
 */
inline void StopWatch::pulse()
{
   this->stop();
   this->start();
}
#endif
