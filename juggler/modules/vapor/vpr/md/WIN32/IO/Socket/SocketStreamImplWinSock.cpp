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

#include <vprConfig.h>

#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#include <md/WIN32/SocketStreamImpWinSock.h>


namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
// ----------------------------------------------------------------------------
SocketStreamImpWinSock::SocketStreamImpWinSock (void)
    : SocketImpWinSock()
{
fprintf(stderr, "vpr::SocketStreamImpWinSock default constructor\n");
    /* Do nothing. */ ;
}

// ----------------------------------------------------------------------------
// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
// ----------------------------------------------------------------------------
SocketStreamImpWinSock::SocketStreamImpWinSock (const InetAddr& local_addr,
                                                const InetAddr& remote_addr)
    : SocketImpWinSock(local_addr, remote_addr, SocketTypes::STREAM)
{
fprintf(stderr, "vpr::SocketStreamImpWinSock(local, remote) constructor\n");
fprintf(stderr, "    Local Address: %s -> %s\n",
        local_addr.getAddressString().c_str(),
        m_local_addr.getAddressString().c_str());
fprintf(stderr, "    Local Port: %hu -> %hu\n", local_addr.getPort(),
        m_local_addr.getPort());
fprintf(stderr, "    Remote Address: %s -> %s\n",
        remote_addr.getAddressString().c_str(),
        m_remote_addr.getAddressString().c_str());
fprintf(stderr, "    Remote Port: %hu -> %hu\n", remote_addr.getPort(),
        m_remote_addr.getPort());
fprintf(stderr, "    Domain: %d\n", m_local_addr.getFamily());
fprintf(stderr, "    Type: %d\n", m_type);
}

// ----------------------------------------------------------------------------
// Listen on the socket for incoming connection requests.
// ----------------------------------------------------------------------------
Status
SocketStreamImpWinSock::listen (const int backlog) {
    Status retval;

    // Put the socket into listning mode.  If that fails, print an error and
    // return error status.
    if ( ::listen(m_sockfd, backlog) == -1 ) {
        fprintf(stderr,
                "[vpr::SocketStreamImpWinSock] Cannot listen on socket: %s\n",
                strerror(errno));
        retval.setCode(Status::Failure);
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Accept an incoming connection request.
// ----------------------------------------------------------------------------
Status
SocketStreamImpWinSock::accept (SocketStreamImpWinSock& sock) {
    SOCKET accept_sock;
    InetAddr addr;
    int addrlen;
    Status retval;

    // Accept an incoming connection request.
    addrlen = addr.size();
    accept_sock = ::accept(m_sockfd, (struct sockaddr*) &addr.m_addr,
                           &addrlen);

    // If accept(2) failed, print an error message and return error stauts.
    if ( accept_sock == -1 ) {
        fprintf(stderr,
                "[vpr::SocketStreamImpWinSock] Error while accepting incoming connection: %s\n",
                strerror(errno));
        retval.setCode(Status::Failure);
    }
    // Otherwise, create a new vpr::SocketStreamImpWinSock object using what
    // the operating system gave us through accept(2).
    else {
        sock.setRemoteAddr(addr);
        sock.m_sockfd    = accept_sock;
        sock.m_type      = SocketTypes::STREAM;
        sock.m_open      = true;
        sock.m_bound     = true;
        sock.m_connected = true;
    }

    return retval;
}

}; // End of vpr namespace
