
#ifndef _VJ_TIMESTAMP_POSIX_
#define _VJ_TIMESTAMP_POSIX_

/*
 * This version of vjTimeStampPosix uses the gettimeofday system call
 * to collect timing information.  It's probably not as high
 * resolution as the SGI-specific version, but should be portable
 * across all unix platforms.
 */

#include <iostream.h>
#include <sys/time.h>



class vjTimeStampPosix {

public:

    static void initialize() {
	struct timeval t;
	gettimeofday (&t, 0);
        initval = t.tv_sec * 1000000 + t.tv_usec;
    }



    vjTimeStampPosix() {
	set();
    }



    vjTimeStampPosix& operator= (const vjTimeStamp& t2);


    void set() {
	gettimeofday (&tp, 0);
        val = tp.tv_sec * 1000000 + tp.tv_usec;
    }



    float usecs() {
	return val - initval;
    }



    float diff (vjTimeStampPosix& ts) {
	return ts.val - val;
    }



    friend ostream& operator << (ostream& out, vjTimeStampPosix& ts) {
	out << ts.usecs();
	return out;
    }



private:

    static float initval;
    struct timeval tp;
    float val;
};


#endif
