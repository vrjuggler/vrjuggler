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
 *    - This file (vprSystemNSPR.h) MUST be included by vprSystemBase.h,
 *      not the other way around.
 * --------------------------------------------------------------------------
 */

#ifndef _VPR_SYSTEM_NSPR_H_
#define _VPR_SYSTEM_NSPR_H_

#include <vprConfig.h>

#include <stdlib.h>
#include <string.h>
#include <prtime.h>

#include <SystemBase.h>

#ifndef HAVE_GETTIMEOFDAY
struct timeval {
    long tv_sec;           /* seconds since Jan. 1, 1970 */
    long tv_usec;          /* and microseconds */
};

struct timezone {
    int tv_minuteswest;    /* minutes west of Greenwich */
    int tv_dsttime;        /* type of dst correction */
};
#else
#include <sys/time.h>
#endif


namespace  {

class SystemNSPR : public SystemBase {
public:
    inline static int
    gettimeofday (struct timeval* tp, struct timezone* tzp = NULL) {
        PRExplodedTime time;
        memset((void*) &time, 0, sizeof(PRExplodedTime));
        PR_ExplodeTime(PR_Now(), PR_LocalTimeParameters, &time);
        tp->tv_sec  = time.tm_sec;
        tp->tv_usec = time.tm_usec;

        return 0;
    }
};

}; // End of vpr namespace


#endif   /* _VPR_SYSTEM_H_ */
