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

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <string.h>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include <prio.h>
#include <prinrval.h>

#include <vpr/Util/Assert.h>
#include <vpr/md/NSPR/IO/Socket/SocketDatagramImplNSPR.h>


// ============================================================================
// External global variables.
// ============================================================================
extern int errno;

namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImplNSPR::recvfrom (void* msg, const vpr::Uint32 length,
                                  const int flags, InetAddr& from,
                                  vpr::Uint32& bytes_read,
                                  const vpr::Interval timeout)
{
    Status retval;
    PRInt32 bytes;

    bytes = PR_RecvFrom(m_handle, msg, length, flags, from.getPRNetAddr(),
                        NSPR_getInterval(timeout));

    if ( bytes == -1 ) {
        PRErrorCode err_code = PR_GetError();

        bytes_read = 0;

        if ( err_code == PR_WOULD_BLOCK_ERROR ) {
            retval.setCode(vpr::Status::WouldBlock);
        }
        else if ( err_code == PR_IO_TIMEOUT_ERROR ) {
            retval.setCode(Status::Timeout);
        }
        else {
            NSPR_PrintError("SocketDatagramImplNSPR::recvfrom: Could not read from socket");
            retval.setCode(Status::Failure);
        }
    }
    else {
        bytes_read = bytes;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImplNSPR::sendto (const void* msg, const vpr::Uint32 length,
                                const int flags, const InetAddr& to,
                                vpr::Uint32& bytes_sent,
                                const vpr::Interval timeout)
{
    Status retval;
    PRInt32 bytes;

    bytes = PR_SendTo(m_handle, msg, length, flags, to.getPRNetAddr(),
                      NSPR_getInterval(timeout));

    if ( bytes_sent == -1 ) {
        PRErrorCode err_code = PR_GetError();

        bytes_sent = 0;

        if ( err_code == PR_WOULD_BLOCK_ERROR ) {
            retval.setCode(vpr::Status::WouldBlock);
        }
        else if ( err_code == PR_IO_TIMEOUT_ERROR ) {
            retval.setCode(Status::Timeout);
        }
        else {
            NSPR_PrintError("SocketDatagramImplNSPR::sendto: Could not send message");
            retval.setCode(Status::Failure);
        }
    }
    else {
        bytes_sent = bytes;
    }

    return retval;
}

} // End of vpr namespace
