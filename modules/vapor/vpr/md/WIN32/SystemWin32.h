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
 *    - This file (vprSystemWin32.h) MUST be included by vprSystemBase.h,
 *      not the other way around.
 * --------------------------------------------------------------------------
 */

#ifndef _VPR_SYSTEM_WIN32_H_
#define _VPR_SYSTEM_WIN32_H_

#include <vpr/vprConfig.h>

#include <stdlib.h>
#include <string>
#include <windows.h>
#include <winsock2.h>

#include <vpr/Util/Status.h>
#include <vpr/SystemBase.h>


#ifndef HAVE_GETTIMEOFDAY
struct timezone {
    int tv_minuteswest;    /* minutes west of Greenwich */
    int tv_dsttime;        /* type of dst correction */
};
#endif

namespace vpr {

class SystemWin32 : public SystemBase {
public:
    static VPR_API(int) gettimeofday(struct timeval* tp,
                                     struct timezone* tzp = NULL);

    // ----- Host to network byte order conversions ---- //
    inline static vpr::Uint16
    Ntohs (vpr::Uint32 conversion) {
        return ntohs(conversion);
    }

    inline static vpr::Uint32
    Ntohl (vpr::Uint32 conversion) {
        return ntohl(conversion);
    }

    inline static vpr::Uint16
    Htons (vpr::Uint16 conversion) {
        return htons(conversion);
    }

    inline static vpr::Uint32
    Htonl (vpr::Uint32 conversion) {
        return htonl(conversion);
    }

    static VPR_API(Status) getenv(const std::string& name, std::string& result);
};

}; // End of vpr namespace


#endif   /* _VPR_SYSTEM_WIN32_H_ */
