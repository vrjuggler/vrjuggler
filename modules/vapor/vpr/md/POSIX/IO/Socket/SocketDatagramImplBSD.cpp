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
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/md/POSIX/IO/Socket/SocketDatagramImplBSD.h>


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
SocketDatagramImplBSD::recvfrom (void* msg, const size_t length,
                                 const int flags, InetAddr& from,
                                 ssize_t& bytes_read, const vpr::Interval timeout)
{
    socklen_t fromlen;
    Status retval;

    retval = m_handle->isReadable(timeout);

    if ( retval.success() ) {
        m_blocking_fixed = true;

        fromlen    = from.size();
        bytes_read = ::recvfrom(m_handle->m_fdesc, msg, length, flags,
                                (struct sockaddr*) &from.m_addr, &fromlen);

        if ( bytes_read == -1 ) {
            if ( errno == EAGAIN && getNonBlocking() ) {
                retval.setCode(Status::InProgress);
            }
            else {
                fprintf(stderr,
                        "[vpr::SocketDatagramImplBSD] ERROR: Could not read from socket (%s:%hu): %s\n",
                        m_remote_addr.getAddressString().c_str(),
                        m_remote_addr.getPort(), strerror(errno));
                retval.setCode(Status::Failure);
            }
        }
    }

    return retval;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
Status
SocketDatagramImplBSD::sendto (const void* msg, const size_t length,
                               const int flags, const InetAddr& to,
                               ssize_t& bytes_sent, const vpr::Interval timeout)
{
    Status retval;

    retval = m_handle->isWriteable(timeout);

    if ( retval.success() ) {
        m_blocking_fixed = true;

        bytes_sent = ::sendto(m_handle->m_fdesc, msg, length, flags,
                              (struct sockaddr*) &to.m_addr, to.size());

        if ( bytes_sent == -1 ) {
            if ( errno == EAGAIN && getNonBlocking() ) {
                retval.setCode(Status::InProgress);
            }
            else {
                fprintf(stderr,
                        "[vpr::SocketDatagramImplBSD] ERROR: Could not send to %s:%hu on socket (%s:%hu): %s\n",
                        to.getAddressString().c_str(), to.getPort(),
                        m_remote_addr.getAddressString().c_str(),
                        m_remote_addr.getPort(), strerror(errno));
                retval.setCode(Status::Failure);
            }
        }
    }

    return retval;
}

}; // End of vpr namespace
