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



#ifndef _JCCL_TIMESTAMP_NONE_
#define _JCCL_TIMESTAMP_NONE_

#include <jccl/jcclConfig.h>

namespace jccl {

//-----------------------------------------
//:Dummy time recorder
//
//     This version of TimeStamp is simply a dummy,
//     meant for use on systems where neither the SGI
//     or Posix versions will work, or where absolutely
//     no performance-measurement overhead is acceptable.
//     All functions are empty and inlined, and should
//     be completely eliminated by any decent compiler.
//
//     TimeStampPosix should never be instantiated directly.
//     Instead, use TimeStamp, which will be typedefed to
//     the correct implementation.
//
// @author  Christopher Just
//-----------------------------------------

class VJ_CLASS_API TimeStampNone {

public:

    //: Called at application initialization to open the counter
    inline static void initialize() {;}


    //: Constructor
    inline TimeStampNone() {;}


    //: does nothing - this is the null performance monitor, remember?
    inline void set() {;}


    //: returns 0.0
    inline float operator - (const TimeStampNone& t2) const {
	return 0.0;
    }


    //: returns 0.0
    inline float usecs() {
	return 0.0;
    }


    //: assignment operator
    inline TimeStampNone& operator= (TimeStampNone& t2) {
	return *this;
    }


    inline friend std::ostream& operator << (std::ostream& out,
                                             TimeStampNone& ts)
    {
	return out;
    }


    //: returns resolution of timer in microseconds
    inline float getResolution() {
	return 0.0;
    }

};

}; // namespace jccl

#endif
