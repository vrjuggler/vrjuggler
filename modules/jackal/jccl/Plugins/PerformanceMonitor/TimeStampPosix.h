
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



    vjTimeStampPosix& operator= (const vjTimeStampPosix& t2);



    void set() {
	struct timeval tp;
	gettimeofday (&tp, 0);
        val = tp.tv_sec * 1000000 + tp.tv_usec;
    }



    float usecs() const {
	return val - initval;
    }



    //: returns number of microseconds between self and t2
    //! PRE: t2 is stamped with an earlier time than self
    //! 
    float operator - (const vjTimeStampPosix& t2) const {
	return (usecs() - t2.usecs());
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
    float val;
};


#endif
