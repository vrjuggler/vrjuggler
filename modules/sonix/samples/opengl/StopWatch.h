
//////////////////////////////////////////////////////////////////
//
//                         -=     StopWatch     =-
//
// Definition: "cross platform timer (fully featured, may not be fastest)"
//
///////////////// <auto-copyright BEGIN do not edit this line> /////////////////
//
//    $RCSfile$
//    $Date$
//    $Revision$
//    Copyright (C) 1998, 1999, 2000  Kevin Meinert, kevin@vrsource.org
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.
//
//    You should have received a copy of the GNU Library General Public
//    License along with this library; if not, write to the Free
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
///////////////// <auto-copyright END do not edit this line> ///////////////////
#ifndef STOP_WATCH_INCLUDED
#define STOP_WATCH_INCLUDED

#ifndef WIN32
    #include <sys/time.h>
#endif
#include <assert.h>

//: A StopWatch
//  This class monitors frame rate, and general time performance metrics 
//  NOTE: All times expressed in seconds (in 64bit floating point)
//        The getTime func is expressed as seconds since midnight (00:00) 
//        Coordinated Universal Time (UTC), January 1, 1970.
class StopWatch
{
public:
    //: Default constructor.
    StopWatch();

// StopWatch controls:
public:
    //: Starts the stopwatch
    //  result - sets the value mTimeStarted
    void           start();
    
    //: Stops the stopwatch
    //  results - sets the value mTimeStopped,      <BR>
    //            sets the value time,     <BR>
    //            sets the value mFpsAverage (every [mRefreshRate] times that stop is called),    <BR>
    //            sets the value mFpsInstant,    <BR>
    //            sets the value mCount.    <BR>
    void           stop();
    
    //: Reset the stopwatch
    //  result - Resets every param except for [mRefreshRate]
    void           reset();
    
    //: do a stop then a start.  usually called once per frame.
    void pulse();
       
// Time metrics:   
public:
    //: the time between the last start/stop cycle
    //: (time = mTimeStopped - mTimeStarted)
    //  format - see NOTE at top for time format
    //  result - undefined if used before stop() is called
    inline const double&  timeInstant() const { return mTimeInstant; }

    //: the time at which start() was invoked last
    // format: see NOTE at top for time format
    inline const double&  timeStart() const { return mTimeStarted; }
    
    //: the time at which stop() was invoked last
    // format - see NOTE at top for time format
    inline const double&  timeStop() const { return mTimeStopped; }

public:
   //: x-platform getTime function
   //  useful for profiling, returns current time in seconds.
   //  returns - a double (see top for time format)
   static void             getTime( double& num );
   static double           getTime();
   
// settings   
protected:
    int            mRefreshRate;

// time metrics
protected:
    double         mTimeInstant; 
    double         mTimeStarted;
    double         mTimeStopped;
};

///////////////////////////////////
// Implementation:
///////////////////////////////////

//: Default constructor.
inline StopWatch::StopWatch() : mTimeInstant( 0.0 ),
                  mTimeStarted( 0.0 ), 
		mTimeStopped( 0.0 ) 
{
}

//: get the current time
//  useful for profiling, or doing time based animation
//  returns - number in seconds...
#ifndef WIN32
	inline void StopWatch::getTime( double& num )
	{
		struct timeval tv;
		gettimeofday( &tv, 0 );

		// compose sec with microsec for sec.millisec
		num = static_cast<double>( tv.tv_sec )
		    + ( static_cast<double>( tv.tv_usec ) 
			  / 1000000.0 );
	}
#else
	#include <sys/types.h>
	#include <sys/timeb.h>
    inline void StopWatch::getTime( double& num )
    {
		struct _timeb tv;
		_ftime( &tv );

		// compose sec with millisec for sec.millisec
		num = static_cast<double>( tv.time )
			+ ( static_cast<double>( tv.millitm ) 
			  / 1000.0 );
    }
#endif

//: a slightly easier to use version for some people ;)
inline double StopWatch::getTime() 
{ 
   double num; 
   StopWatch::getTime( num ); 
   return num; 
}


//: Starts the stopwatch
//  result - sets the value mTimeStarted
inline void StopWatch::start()
{
    StopWatch::getTime( mTimeStarted );
}

//: Stops the stopwatch
//  results - sets the value mTimeStopped,      <BR>
//            sets the value time,     <BR>
//            sets the value mFpsAverage (every [mRefreshRate] times that stop is called),    <BR>
//            sets the value mFpsInstant,    <BR>
//            sets the value mCount.    <BR>
inline void StopWatch::stop()
{
    StopWatch::getTime( mTimeStopped );
    mTimeInstant = mTimeStopped - mTimeStarted;
}

//: Reset the stopwatch
//  result - Resets every param except for [mRefreshRate]
inline void StopWatch::reset()
{
    mTimeStarted = 0.0;
    mTimeStopped = 0.0;
    mTimeInstant = 0.0;
}

//: do a stop then a start.  usually called once per frame.
inline void StopWatch::pulse()
{
   this->stop();
   this->start();
}
#endif
