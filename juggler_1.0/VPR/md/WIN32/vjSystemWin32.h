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

/*
 * --------------------------------------------------------------------------
 * NOTES:
 *    - This file (vjSystemPosix.h) MUST be included by vjSystemBase.h, 
 *      not the other way around.
 * --------------------------------------------------------------------------
 */

/* this define is the same as the Win32/Posix/NSPR version */
/* in order to protect against including both */
#ifndef _VJ_SYSTEM_H_
#define _VJ_SYSTEM_H_

#include <vjConfig.h>

#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>


#ifndef HAVE_GETTIMEOFDAY
struct timezone {
    int tv_minuteswest;    /* minutes west of Greenwich */
    int tv_dsttime;        /* type of dst correction */
};
#endif

class VJ_CLASS_API vjSystem : public vjSystemBase {
public:
    // ------------------------------------------------------------------------
    // NOTE:
    //     The manual construction of the struct timeval pointer's contents is
    //     based on ACE_OS::gettimeofday().
    // ------------------------------------------------------------------------
    inline static int
    gettimeofday (struct timeval* tp, struct timezone* tzp = NULL) {
#ifdef HAVE_GETTIMEOFDAY
        return ::gettimeofday(tp, tzp);
#else
        int retval;
        FILETIME file_time, tfile;
        GetSystemTimeAsFileTime(&tfile);

        //  Initializes the ACE_Time_Value object from a Win32 FILETIME
        ULARGE_INTEGER _100ns =
        {
            file_time.dwLowDateTime,
            file_time.dwHighDateTime
        };

        // Static constant to remove time skew between FILETIME and POSIX
        // time.
        _100ns.QuadPart -= 0x19db1ded53e8000;

        // Convert 100ns units to seconds;
        tp->tv_sec = (long) (_100ns.QuadPart / (10000 * 1000));
        // Convert remainder to microseconds;
        tp->tv_usec = (long) ((_100ns.QuadPart % (10000 * 1000)) / 10);

        return 0;
#endif
    }
};
 

#endif   /* _VJ_SYSTEM_H_ */
