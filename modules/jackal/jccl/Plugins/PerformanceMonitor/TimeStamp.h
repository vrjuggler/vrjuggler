
/* vjTimeStamp... a simple class to enclose a timestamp */

#include <iostream.h>


#ifndef _VJ_TIMESTAMP_
#define _VJ_TIMESTAMP_

#define _VJ_SGI_TIMING_

#ifdef _VJ_SGI_TIMING_
/* This version of vjTimeStamp uses the SGI system cycle counter
 * to collect timing information.  The precision of this hardware
 * timer varies on different machines.  On Onyx systems it's
 * 21 nanoseconds, and on Octanes it's 80 ns.
 */

#include <stddef.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/syssgi.h>
#include <unistd.h>


class vjTimeStamp {

public:

    //: Called at application initialization to open the counter
    //! PRE: true
    //! POST: cycle counter is initialized
    //! RETURNS: true - succes
    //! RETURNS: false - failure to open counter. Timestamp set()
    //+          will set to the value 0.
    static void initialize();



    //: Constructor
    //! PRE: initialize() has been called.
    //! POST: self is created and set to the current counter val.
    vjTimeStamp() {
	set();
    }



    //: stamps the timestamp with the current time
    //! PRE: true
    //! POST: self's value is the current time
    void inline set() {
	if (cyclecntrsize == 64)
	    val = *(unsigned long long*)iotimer_addr;
	else 
	    val = *(unsigned int*)iotimer_addr;
	val = (val >= initval)?
	    val - initval
	    :val + (maxval - initval);
    }



    //: returns number of microseconds between self and t2
    //! PRE: t2 is stamped with an earlier time than self
    //! 
    float operator - (const vjTimeStamp& t2) const {
	/* returns self - t2, in usecs */
	return (val - t2.val)*resolution;
    }



    float usecs();

    friend ostream& operator << (ostream& out, vjTimeStamp& ts) {
	out << ts.usecs();
	return out;
    }

    //: returns resolution of timer in microseconds
    float getResolution() {
	return resolution;
    }

private:

    static __psunsigned_t phys_addr, raddr;
    //  static unsigned int cycleval;
    static volatile void* iotimer_addr;
    static volatile unsigned long long longcount;
    static int fd, poffmask;
    static float resolution; // in usecs.
    static int cyclecntrsize;  // either 32 or 64 bits. depends on hardware
    static long long initval;
    static long long maxval;

    long long val; // (in clockticks; resolution*clocktics = time in usecs

};


#else
/*
 * This version of vjTimeStamp uses the gettimeofday system call
 * to collect timing information.  It's probably not as high
 * resolution as the SGI-specific version, but should be portable
 * across all unix platforms.
 */

#include <sys/time.h>



class vjTimeStamp {

public:

    static void initialize() {
	struct timeval t;
	gettimeofday (&t, 0);
	//cout << "time is " << tp.tv_sec << ", " << tp.tv_usec << "\n";
        initval = t.tv_sec * 1000000 + t.tv_usec;
    }



    vjTimeStamp() {
	set();
    }



    void set() {
	gettimeofday (&tp, 0);
        //cout << "time is " << tp.tv_sec << ", " << tp.tv_usec << "\n";
        val = tp.tv_sec * 1000000 + tp.tv_usec;
    }



    float usecs() {
	return val - initval;
    }



    float diff (vjTimeStamp& ts) {
	return ts.val - val;
    }



    friend ostream& operator << (ostream& out, vjTimeStamp& ts) {
	out << ts.usecs();
	return out;
    }



private:

    static float initval;
    struct timeval tp;
    float val;
};



#endif
#endif
