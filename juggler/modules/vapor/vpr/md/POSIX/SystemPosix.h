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
 *    - This file (vprSystemPosix.h) MUST be included by vprSystemBase.h,
 *      not the other way around.
 * --------------------------------------------------------------------------
 */

#ifndef _VPR_SYSTEM_POSIX_H_
#define _VPR_SYSTEM_POSIX_H_

#include <vpr/vprConfig.h>

#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/time.h>

#include <vpr/Util/Status.h>
#include <vpr/SystemBase.h>

namespace vpr {

class SystemPosix : public SystemBase {
public:
    inline static int
    gettimeofday (struct timeval* tp, struct timezone* tzp = NULL) {
        return ::gettimeofday(tp, tzp);
    }

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

    inline static Status
    getenv (const std::string& name, std::string& result) {
        char* val;
        Status status;

        val = ::getenv(name.c_str());

        if ( val != NULL ) {
            result = val;
        }
        else {
            status.setCode(Status::Failure);
        }

        return status;
    }
};

};


#endif   /* _VPR_SYSTEM_POSIX_H_ */
