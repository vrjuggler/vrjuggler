/*
 * vjTimedUpdate.h
 * class definition of vjTimedUpdate and its children,
 *   vjTrackerTimedUpdate
 *
 * 6/9/98
 *
 * author: Christopher Just
 */


// TimedUpdate is probably gonna be a parent class of a whole bunch of
// different timed updates, for input devices, performance monitoring,
// and so on.

#ifndef _VJ_TIMED_UPDATE_H_
#define _VJ_TIMED_UPDATE_H_

#include <vjConfig.h>

class vjTimedUpdate {

public:
    virtual void write (ostream& out);

    virtual std::string getName ();
};


#endif
