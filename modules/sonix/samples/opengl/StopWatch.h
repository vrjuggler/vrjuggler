
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
    StopWatch( const int& averageFpsRefreshRate = 15 );

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
       
// StopWatch settings:
public:
    //: the refresh rate for mFpsAverage and mTimeAverage (in number of frames)
    //  mFpsInstant is averaged over [mRefreshRate] number of 
    //    start/stop cycles
    //  see also - mFpsAverage for the average frames per second value.
    //  see also - mTimeAverage for the average time value.
    //  TODO: make this based on time rather than cycles
    inline void    setRefreshRate( const int& rate ) 
    { 
       mRefreshRate = rate; 
    }


// Time metrics:   
public:
    //: the time between the last start/stop cycle averaged 
    //: over timeRefreshRate start/stop cycles
    //  format - see NOTE at top for time format
    //  result - undefined if used before stop() is called
    inline const double&  timeAverage() const { return mTimeAverage; }

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

// Frames per second metrics (start/stop per second actually):
public:
    //: the number start/stop cycles
    //  Number start/stop cycles that have been
    //  performed since the last reset() or since construction
    inline const unsigned long&  count() const { return mCount; }

    //: The average frames per second over [mRefreshRate] start/stop cycles
    inline const double&  fpsAverage() const { return mFpsAverage; }
    
    //: the frames per second between the last start/stop cycle
    inline const double&  fpsInstant() const { return mFpsInstant; }    
    
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
    double         mTimeAverage;
    double         mTimeInstant; 
    double         mTimeStarted;
    double         mTimeStopped;

// FPS metrics
protected:
    unsigned long  mCount;
    double         mFpsAverage;
    double         mFpsInstant;

// hidden/implementation members
private:
    double         mTimeAccumulator;
};

///////////////////////////////////
// Implementation:
///////////////////////////////////

//: Default constructor.
inline StopWatch::StopWatch( const int& averageFpsRefreshRate ) : 
		mRefreshRate( averageFpsRefreshRate ), 
		mTimeAverage( 0.0 ), 
		mTimeInstant( 0.0 ),
                  mTimeStarted( 0.0 ), 
		mTimeStopped( 0.0 ), 
		mCount( 0 ), 
		mFpsAverage( 0.0 ), 
		mFpsInstant( 0.0 ), 
		mTimeAccumulator( 0.0 )
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
    
    // get the time for this one frame.
    mTimeInstant = mTimeStopped - mTimeStarted;
    
    // every [mRefreshRate] frames, calc the average FPS
    assert( mRefreshRate != 0 && "StopWatch: refresh rate of 0 will cause a math error" );
    if (mCount % mRefreshRate == 0)
    {
      mFpsAverage = mRefreshRate / mTimeAccumulator;
      mTimeAverage = mTimeAccumulator / mRefreshRate;

      // reset the accumulator
      mTimeAccumulator = 0.0;
    }
    
    // accumulate the frame times to later calc 
    // the average FPS time.
    mTimeAccumulator += mTimeInstant;
    
    // calculate the instantaneous FPS value (1 frame/sec)
    mFpsInstant = 1.0 / mTimeInstant;
    
    // Increment the number of frames elapsed
    ++mCount;
}

//: Reset the stopwatch
//  result - Resets every param except for [mRefreshRate]
inline void StopWatch::reset()
{
    mTimeStarted = 0.0;
    mTimeStopped = 0.0;
    mTimeAccumulator = 0.0;
    
    mCount = 0;
    mFpsAverage = 0.0;
    mFpsInstant = 0.0; 
    mTimeInstant = 0.0;
}

//: do a stop then a start.  usually called once per frame.
inline void StopWatch::pulse()
{
   this->stop();
   this->start();
}
#endif
