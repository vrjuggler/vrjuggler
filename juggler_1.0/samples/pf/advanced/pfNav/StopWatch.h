#ifndef STOP_WATCH_INCLUDED
#define STOP_WATCH_INCLUDED

#ifndef WIN32
    #include <sys/time.h>
#endif

//: A StopWatch
//  This class monitors frame rate, and general time performance metrics 
//  NOTE: All times expressed in seconds (in 64bit floating point)
//        The getTime func is expressed as seconds since midnight (00:00) 
//        Coordinated Universal Time (UTC), January 1, 1970.
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
    StopWatch(const int& averageFpsRefreshRate = 15);
    
    //: get the current time
	//  useful for profiling
	//  returns - a double (see top for time format)
	static void    getTime( double& num );
	
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

///////////////////////////////////
// Implementation:
///////////////////////////////////

//: Default constructor.
inline StopWatch::StopWatch(const int& averageFpsRefreshRate) : 
		timeAverage(0.0), 
		timeInstant(0.0),
      count(0), 
		fpsAverage(0.0), 
		fpsInstant(0.0), 
		refreshRate(averageFpsRefreshRate), 
		startTime(0.0), 
		stopTime(0.0), 
		_timeAccumulator(0.0)
{;}



//: get the current time
//  useful for profiling
//  returns - a number of any scalar type (see top for time format)
// TODO: does compiler optimize the 
//       case where T = double???, if not, then un-template this.
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

//: Starts the stopwatch
//  result - sets the value startTime
inline void StopWatch::start()
{
    getTime( startTime );
}

//: Stops the stopwatch
//  results - sets the value stopTime,      <BR>
//            sets the value time,     <BR>
//            sets the value fpsAverage (every [refreshRate] times that stop is called),    <BR>
//            sets the value fpsInstant,    <BR>
//            sets the value count.    <BR>
inline void StopWatch::stop()
{
    getTime( stopTime );
    
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
inline void StopWatch::reset()
{
    startTime = 0.0;
    stopTime = 0.0;
    _timeAccumulator = 0.0;
    
    count = 0;
    fpsAverage = 0.0;
    fpsInstant = 0.0; 
    timeInstant = 0.0;
}

#endif
