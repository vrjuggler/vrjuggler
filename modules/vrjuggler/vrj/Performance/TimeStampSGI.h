/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/



#ifndef _VJ_TIMESTAMP_SGI_
#define _VJ_TIMESTAMP_SGI_

#include <vjConfig.h>
#include <sys/types.h>

namespace vrj
{
 
//-----------------------------------------
//:Time recorder for SGI systems
//
//     This version of TimeStaMp uses the SGI system cycle
//     counter to collect timing information.  The precision
//     of this hardware timer varies on different machines.
//     For example, on an SGI Onyx it's 21 nanoseconds, while
//     it's 80 ns on an Octane.
//
//     TimeStaMpPosix should never be instantiated directly.
//     Instead, use TimeStaMp, which will be typedefed to
//     the correct implementation.
//
// @author  Christopher Just
//-----------------------------------------

class TimeStaMpSGI {

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
    TimeStaMpSGI();



    //: stamps the timestamp with the current time
    //! PRE: true
    //! POST: self's value is the current time
    inline void set() {
	val = (cyclecntrsize == 64)
	    ? *(unsigned long long*) iotimer_addr
	    : *(unsigned int*) iotimer_addr;
	val = (val >= initval)
	    ? val - initval
	    : val + (maxval - initval);
    }



    //: assignment operator
    TimeStaMpSGI& operator= (const TimeStaMpSGI& t2) {
	val = t2.val;
	return *this;
    }



    //: returns number of microseconds between self and t2
    //! PRE: t2 is stamped with an earlier time than self
    //! 
    float operator - (const TimeStaMpSGI& t2) const;



    float usecs();

    friend std::ostream& operator << (std::ostream& out, TimeStaMpSGI& ts);

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

};

#endif
