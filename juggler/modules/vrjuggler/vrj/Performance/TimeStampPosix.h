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

#ifndef _VRJ_TIMESTAMP_POSIX_H_
#define _VRJ_TIMESTAMP_POSIX_H_

#include <vrj/vjConfig.h>
#include <sys/time.h>

namespace vrj
{
   
//-----------------------------------------
//:Portable time recorder for Unix/Linux systems
//
//     This version of TimeStaMp uses the gettimeofday()
//     system call to collect timing information.  While
//     it is probably not as high resolution as 
//     TimeStaMpSGI, it should be portable accross all
//     Unix and Unix-like OSes
//
//     TimeStaMpPosix should never be instantiated directly.
//     Instead, use TimeStaMp, which will be typedefed to
//     the correct implementation.
//
// @author  Christopher Just
//-----------------------------------------

class TimeStaMpPosix {

public:

    static void initialize() {
	struct timeval t;
	gettimeofday (&t, 0);
        initval = t.tv_sec * 1000000 + t.tv_usec;
    }



    TimeStaMpPosix() {
	set();
    }



    TimeStaMpPosix& operator= (const TimeStaMpPosix& t2);



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
    float operator - (const TimeStaMpPosix& t2) const {
	return (usecs() - t2.usecs());
    }


    float diff (TimeStaMpPosix& ts) {
	return ts.val - val;
    }



    friend std::ostream& operator << (std::ostream& out, TimeStaMpPosix& ts) {
	out << ts.usecs();
	return out;
    }



private:

    static float initval;
    float val;
};

};

#endif
