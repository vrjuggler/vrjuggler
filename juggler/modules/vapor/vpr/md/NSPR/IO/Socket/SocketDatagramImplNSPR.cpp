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

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <prio.h>
#include <prinrval.h>

#include <Utils/Assert.h>
#include <md/NSPR/SocketDatagramImpNSPR.h>


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
SocketDatagramImpNSPR::recvfrom (void* msg, const size_t length,
                                 const int flags, InetAddr& from,
                                 ssize_t& bytes_read)
{
    Status retval;

    bytes_read = PR_RecvFrom(m_handle, msg, length, flags, from.getPRNetAddr(),
                             PR_INTERVAL_NO_TIMEOUT);

    if ( bytes_read == -1 ) {
        NSPR_PrintError("SocketDatagramImpNSPR::recvfrom: Could not read from socket");
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImpNSPR::recvfrom (std::string& msg, const size_t length,
                                 const int flags, InetAddr& from,
                                 ssize_t& bytes_read)
{
    msg.resize(length);
    memset(&msg[0], '\0', msg.size());

    return recvfrom((void*) &msg[0], msg.size(), flags, from, bytes_read);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImpNSPR::recvfrom (std::vector<vpr::Uint8>& msg,
                                 const size_t length, const int flags,
                                 InetAddr& from, ssize_t& bytes_read)
{
    Status retval;

    msg.resize(length);

    memset(&msg[0], '\0', msg.size());
    retval = recvfrom((void*) &msg[0], msg.size(), flags, from, bytes_read);

    // Size it down if needed, if (bytes_read==length), then resize does
    // nothing.
    if ( bytes_read >= 0 ) {
        msg.resize(bytes_read);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImpNSPR::sendto (const void* msg, const size_t length,
                               const int flags, const InetAddr& to,
                               ssize_t& bytes_sent)
{
    Status retval;

    bytes_sent = PR_SendTo(m_handle, msg, length, flags, to.getPRNetAddr(),
                           PR_INTERVAL_NO_TIMEOUT);

    if ( bytes_sent == -1 ) {
        NSPR_PrintError("SocketDatagramImpNSPR::sendto: Could not send message");
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImpNSPR::sendto (const std::string& msg, const size_t length,
                               const int flags, const InetAddr& to,
                               ssize_t& bytes_sent)
{
    vprASSERT(length <= msg.size() && "Length is bigger than data given");
    return sendto(msg.c_str(), length, flags, to, bytes_sent);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImpNSPR::sendto (const std::vector<vpr::Uint8>& msg,
                               const size_t length, const int flags,
                               const InetAddr& to, ssize_t& bytes_sent)
{
    vprASSERT(length <= msg.size() && "Length is bigger than data given");
    return sendto((const void*) &msg[0], length, flags, to, bytes_sent);
}

} // End of vpr namespace
