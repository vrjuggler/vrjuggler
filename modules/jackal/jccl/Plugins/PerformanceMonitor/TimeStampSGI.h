
#ifndef _VJ_TIMESTAMP_SGI_
#define _VJ_TIMESTAMP_SGI_

/* This version of vjTimeStamp uses the SGI system cycle counter
 * to collect timing information.  The precision of this hardware
 * timer varies on different machines.  On Onyx systems it's
 * 21 nanoseconds, and on Octanes it's 80 ns.
 */

#include <iostream.h>
#include <stddef.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/syssgi.h>
#include <unistd.h>


class vjTimeStampSGI {

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
    vjTimeStampSGI();



    //: stamps the timestamp with the current time
    //! PRE: true
    //! POST: self's value is the current time
    inline void set() {
	if (cyclecntrsize == 64)
	    val = *(unsigned long long*)iotimer_addr;
	else 
	    val = *(unsigned int*)iotimer_addr;
	val = (val >= initval)?
	    val - initval
	    :val + (maxval - initval);
    }



    //: assignment operator
    vjTimeStampSGI& operator= (const vjTimeStampSGI& t2) {
	val = t2.val;
	return *this;
    }



    //: returns number of microseconds between self and t2
    //! PRE: t2 is stamped with an earlier time than self
    //! 
    float operator - (const vjTimeStampSGI& t2) const;



    float usecs();

    friend ostream& operator << (ostream& out, vjTimeStampSGI& ts);

    //: returns resolution of timer in microseconds
    float getResolution();

private:

    static __psunsigned_t phys_addr, raddr;
    static volatile void* iotimer_addr;
    static volatile unsigned long long longcount;
    static int fd, poffmask;
    static float resolution; // in usecs.
    static int cyclecntrsize;  // either 32 or 64 bits. depends on hardware
    static long long initval;
    static long long maxval;

    long long val; // (in clockticks; resolution*clocktics = time in usecs

};

#endif
