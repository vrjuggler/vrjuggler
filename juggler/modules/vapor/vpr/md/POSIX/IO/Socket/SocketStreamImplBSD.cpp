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

#include <vpr/md/POSIX/IO/Socket/SocketStreamImplBSD.h>
#include <vpr/Util/Debug.h>


// ============================================================================
// External global variables.
// ============================================================================
extern int errno;


namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Listen on the socket for incoming connection requests.
// ----------------------------------------------------------------------------
Status
SocketStreamImplBSD::listen (const int backlog) {
    Status retval;

    // Put the socket into listning mode.  If that fails, print an error and
    // return error status.
    if ( ::listen(m_handle->m_fdesc, backlog) == -1 ) {
        fprintf(stderr,
                "[vpr::SocketStreamImplBSD] Cannot listen on socket: %s\n",
                strerror(errno));
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Accept an incoming connection request.
// ----------------------------------------------------------------------------
Status
SocketStreamImplBSD::accept (SocketStreamImplBSD& sock,vpr::Interval timeout) {
    int accept_sock;
    Status retval;
    InetAddr addr;
    socklen_t addrlen;

    retval = m_handle->isReadable(timeout);

    if ( retval.success() ) {
        m_blocking_fixed = true;

        // Accept an incoming connection request.
        addrlen = addr.size();
        accept_sock = ::accept(m_handle->m_fdesc,
                               (struct sockaddr*) &addr.m_addr, &addrlen);

        // If accept(2) failed, print an error message and return error stauts.
        if ( accept_sock == -1 ) {
            if ( errno == EWOULDBLOCK && ! m_blocking ) {
                retval.setCode(Status::WouldBlock);
            }
            else {
                fprintf(stderr,
                        "[vpr::SocketStreamImplBSD] Error while accepting "
                        "incoming connection: %s\n", strerror(errno));
                retval.setCode(Status::Failure);
            }
        }
        // Otherwise, put the new socket in the passed socket object.
        else {
            sock.setRemoteAddr(addr);
            sock.m_handle          = new FileHandleImplUNIX(addr.getAddressString());
            sock.m_handle->m_fdesc = accept_sock;
            sock.m_open            = true;

            // Inherit the blocking state from the accepting socket.  This
            // must be done after m_open is set to true to satisfy the
            // pre-condition.
            if ( ! m_blocking ) {
                sock.enableNonBlocking();
            }

            sock.m_bound          = true;
            sock.m_connected      = true;
            sock.m_blocking_fixed = true;
        }
    }

    return retval;
}

}; // End of vpr namespace
