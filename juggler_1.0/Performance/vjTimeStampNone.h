
#ifndef _VJ_TIMESTAMP_NONE_
#define _VJ_TIMESTAMP_NONE_

#include <iostream.h>


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
