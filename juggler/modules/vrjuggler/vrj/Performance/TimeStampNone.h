
#ifndef _VJ_TIMESTAMP_NONE_
#define _VJ_TIMESTAMP_NONE_

#include <vjConfig.h>

//-----------------------------------------
//:Dummy time recorder
//
//     This version of vjTimeStamp is simply a dummy,
//     meant for use on systems where neither the SGI
//     or Posix versions will work, or where absolutely
//     no performance-measurement overhead is acceptable.
//     All functions are empty and inlined, and should
//     be completely eliminated by any decent compiler.
//
//     vjTimeStampPosix should never be instantiated directly.
//     Instead, use vjTimeStamp, which will be typedefed to
//     the correct implementation.
//
// @author  Christopher Just
//-----------------------------------------

class vjTimeStampNone {

public:

    //: Called at application initialization to open the counter
    inline static void initialize() {;}


    //: Constructor
    inline vjTimeStampNone() {;}


    //: does nothing - this is the null performance monitor, remember?
    inline void set() {;}


    //: returns 0.0
    inline float operator - (const vjTimeStampNone& t2) const {
	return 0.0;
    }


    //: returns 0.0
    inline float usecs() {
	return 0.0;
    }


    //: assignment operator
    inline vjTimeStampNone& operator= (vjTimeStampNone& t2) {
	return *this;
    }


    inline friend ostream& operator << (ostream& out, vjTimeStampNone& ts); {
	return out;
    }


    //: returns resolution of timer in microseconds
    inline float getResolution() {
	return 0.0;
    }

};

#endif
