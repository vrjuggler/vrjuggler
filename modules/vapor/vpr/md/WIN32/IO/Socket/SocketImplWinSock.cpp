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
#include <stdlib.h>
#include <string.h>

#include <md/WIN32/SocketImpWinSock.h>


namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Open the socket.  This creates a new socket using the domain and type
// options set through member variables.
// ----------------------------------------------------------------------------
bool
SocketImpWinSock::open () {
    bool retval;
    int domain, type;

    switch (m_local_addr.getFamily()) {
      case SocketTypes::LOCAL:
#ifdef PF_LOCAL
        domain = PF_LOCAL;
#else
        domain = PF_UNIX;
#endif
        break;
      case SocketTypes::INET:
        domain = PF_INET;
        break;
      case SocketTypes::INET6:
        domain = PF_INET6;
        break;
/*
      case SocketTypes::LINK:
        domain = PF_LINK;
        break;
*/
    }

    switch (m_type) {
      case SocketTypes::STREAM:
        type = SOCK_STREAM;
        break;
      case SocketTypes::DATAGRAM:
        type = SOCK_DGRAM;
        break;
      case SocketTypes::RAW:
        type = SOCK_RAW;
        break;
    }

    m_sockfd = socket(domain, type, 0);

    if ( m_sockfd == INVALID_SOCKET ) {
        fprintf(stderr,
                "[vpr::SocketImpWinSock] Could not create socket (%s): %s\n",
                m_name.c_str(), strerror(errno));
        retval = false;
    }
    else {
        retval = true;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Close the socket.
// ----------------------------------------------------------------------------
bool
SocketImpWinSock::close (void) {
    bool retval;

    if ( ::closesocket(m_sockfd) == 0 ) {
        m_open = false;
        retval = true;
    }
    else {
        fprintf(stderr,
                "[vpr::SocketImpWinSock] Could not close socket (%s): %s\n",
                m_name.c_str(), strerror(errno));
        retval = false;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Bind this socket to the address in the host address member variable.
// ----------------------------------------------------------------------------
bool
SocketImpWinSock::bind () {
    bool retval;
    int status;

    // Bind the socket to the address in m_addr.
    status = ::bind(m_sockfd, (struct sockaddr*) &m_local_addr.m_addr,
                    m_local_addr.size());

    // If that fails, print an error and return error status.
    if ( status == -1 ) {
        fprintf(stderr,
                "[vpr::SocketImpWinSock] Cannot bind socket to address: %s\n",
                strerror(errno));
        retval = false;
    }
    // Otherwise, return success.
    else {
        retval = true;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Connect the socket on the client side to the server side.  For a datagram
// socket, this makes the address given to the constructor the default
// destination for all packets.  For a stream socket, this has the effect of
// establishing a connection with the destination.
// ----------------------------------------------------------------------------
bool
SocketImpWinSock::connect () {
    bool retval;
    int status;

    // Attempt to connect to the address in m_addr.
    status = ::connect(m_sockfd, (struct sockaddr*) &m_remote_addr.m_addr,
                       m_remote_addr.size());

    // If connect(2) failed, print an error message explaining why and return
    // error status.
    if ( status == -1 ) {
        fprintf(stderr, "[vpr::SocketImpWinSock] Error connecting to %s: %s\n",
                m_name.c_str(), strerror(errno));
        retval = false;
    }
    // Otherwise, return success.
    else {
        retval = true;
    }

    return retval;
}

// ============================================================================
// Protected methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Default constructor.  This just initializes member variables to reasonable
// defaults.
// ----------------------------------------------------------------------------
SocketImpWinSock::SocketImpWinSock ()
    : BlockIO(std::string("INADDR_ANY")), m_sockfd(-1)
{
    init();
}

// ----------------------------------------------------------------------------
// Standard constructor.  This takes the given address (a string containing
// a hostname or an IP address), port, domain and type and stores the values
// in the member variables for use when opening the socket and performing
// communications.
// ----------------------------------------------------------------------------
SocketImpWinSock::SocketImpWinSock (const InetAddr& local_addr,
                                    const InetAddr& remote_addr,
                                    const SocketTypes::Type type)
    : BlockIO(std::string("INADDR_ANY")), m_sockfd(-1),
      m_local_addr(local_addr), m_remote_addr(remote_addr)
{
    init();
}

// ----------------------------------------------------------------------------
// Destructor.  This currently does nothing.
// ----------------------------------------------------------------------------
SocketImpWinSock::~SocketImpWinSock () {
    init();
}

// ----------------------------------------------------------------------------
// Do the WinSock initialization required before any socket stuff can happen.
// This is copied from a similar function given on page 279 of _Effective
// TCP/IP Programming_ by Jon C. Snader.
// ----------------------------------------------------------------------------
void
SocketImpWinSock::init () {
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
ssize_t
SocketImpWinSock::recv(void* buffer, const size_t length, const int flags) {
    return ::recv(m_sockfd, (char*) buffer, length, flags);
}


// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
SocketImpWinSock::send (const void* buffer, const size_t length,
                        const int flags)
{
    return ::send(m_sockfd, (char*) buffer, length, flags);
}



}; // End of vpr namespace
