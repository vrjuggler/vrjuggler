#ifndef _VJ_TIMER_H_
#define _VJ_TIMER_H_

#ifndef WIN32
#include <sys/time.h>
#endif

//: Simple class to take timings and give averages
// NOTE: All times are in seconds
// XXX: Should use vjTimeStamp for the time info in the future
class vjTimer
{
public:
	vjTimer()
	{
		totalTime = 0;
		timeCount = 0;
		startTime = 0;
		endTime = 0;
	}
	
	//: Start a timing
	void startTiming()
	{ startTime = wallclock(); }
	
	//: Stop current timing
	//! PRE: Timing has been started
	void stopTiming()
	{ 
		endTime = wallclock();
		if (startTime == 0)	// If we haven't called start then ignore
		{ totalTime = 0; }
		else
		{
		    lastTiming = (endTime - startTime);
		    totalTime += lastTiming;
		    timeCount++;
		}
	}

	// Get the average timing
	double getTiming()
	{ return (totalTime/(double)timeCount); }

	double getLastTiming()
	{ return lastTiming; }
	
	//: Returns the number of timings collected
	long getTimeCount()
	{ return timeCount; }
	
	void reset()
	{
	    totalTime = 0;
	    timeCount = 0;
	    startTime = 0;
	    endTime = 0;
	}   
    
#ifdef WIN32
   double wallclock()
   {
   	return ((double)GetTickCount())/1000.0f;
   }
#else
   double wallclock()
   {
       double v;
       struct timeval tv;
   
       gettimeofday(&tv, 0);
   
       v = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;
       return v;
   }
#endif

private:		// Timing stuff
	double  startTime;	    // The times
	double  endTime;
	double  lastTiming;		 // The last timing recieved
	double  totalTime;	    // The total time
	long    timeCount;	    // The count on the number of times	
};

#endif 
