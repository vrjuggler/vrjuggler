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

#include <VPR/md/WIN32/vjSocketImpWinSock.h>


// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Open the socket.  This creates a new socket using the domain and type
// options set through member variables.
// ----------------------------------------------------------------------------
bool
vjSocketImpWinSock::open () {
    bool retval;
    int domain, type;

    switch (m_addr.getFamily()) {
      case vjSocketTypes::LOCAL:
#ifdef PF_LOCAL
        domain = PF_LOCAL;
#else
        domain = PF_UNIX;
#endif
        break;
      case vjSocketTypes::INET:
        domain = PF_INET;
        break;
      case vjSocketTypes::INET6:
        domain = PF_INET6;
        break;
/*
      case vjSocketTypes::LINK:
        domain = PF_LINK;
        break;
*/
    }

    switch (m_type) {
      case vjSocketTypes::STREAM:
        type = SOCK_STREAM;
        break;
      case vjSocketTypes::DATAGRAM:
        type = SOCK_DGRAM;
        break;
      case vjSocketTypes::RAW:
        type = SOCK_RAW;
        break;
    }

    m_sockfd = socket(domain, type, 0);

    if ( m_sockfd == INVALID_SOCKET ) {
        fprintf(stderr, "[vjSocketImpWinSock] Could not create socket (%s): %s\n",
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
vjSocketImpWinSock::close (void) {
    bool retval;

    if ( ::closesocket(m_sockfd) == 0 ) {
        m_open = false;
        retval = true;
    }
    else {
        fprintf(stderr, "[vjSocketImpWinSock] Could not close socket (%s): %s\n",
                m_name.c_str(), strerror(errno));
        retval = false;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Bind this socket to the address in the host address member variable.
// ----------------------------------------------------------------------------
bool
vjSocketImpWinSock::bind () {
    bool retval;
    int status;

    // Bind the socket to the address in m_addr.
    status = ::bind(m_sockfd, (struct sockaddr*) &m_addr.m_addr,
                    m_addr.size());

    // If that fails, print an error and return error status.
    if ( status == -1 ) {
        fprintf(stderr,
                "[vjSocketImpWinSock] Cannot bind socket to address: %s\n",
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
vjSocketImpWinSock::connect () {
    bool retval;
    int status;

    // Attempt to connect to the address in m_addr.
    status = ::connect(m_sockfd, (struct sockaddr*) &m_addr.m_addr,
                       m_addr.size());

    // If connect(2) failed, print an error message explaining why and return
    // error status.
    if ( status == -1 ) {
        fprintf(stderr, "[vjSocketImpWinSock] Error connecting to %s: %s\n",
                m_name.c_str(), strerror(errno));
        retval = false;
    }
    // Otherwise, return success.
    else {
        retval = true;
    }

    return retval;
}

ssize_t
vjSocketImpWinSock::readn (void* buffer, const size_t length) {
    size_t count;
    ssize_t bytes;

    count = length;

    while ( count > 0 ) {
        bytes = recv(buffer, length);

        // Read error.
        if ( bytes < 0 ) {
            break;
        }
        // May have read EOF, so return bytes read so far.
        else if ( bytes == 0 ) {
            bytes = length - count;
        }
        else {
            buffer = (void*) ((char*) buffer + bytes);
            count  -= bytes;
        }
    }

    return bytes;
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpWinSock::recv(void* buffer, const size_t length, const int flags) {
    return ::recv(m_sockfd, (char*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpWinSock::recv (unsigned char* buffer, const size_t length,
                          const int flags)
{
    return recv((void*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpWinSock::recv (char* buffer, const size_t length, const int flags) {
    return recv((void*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpWinSock::recv (std::string& buffer, const size_t length,
                          const int flags)
{
    ssize_t bytes;
    char* temp_buf;

    temp_buf = (char*) malloc(length);
    bytes    = recv(temp_buf, length, flags);

    // If anything was read into temp_buf, copy it into buffer.
    if ( bytes > -1 ) {
        buffer = temp_buf;
    }

    free(temp_buf);

    return bytes;
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpWinSock::recv (std::vector<char>& buffer, const size_t length,
                          const int flags)
{
    ssize_t bytes;
    char* temp_buf;

    temp_buf = (char*) malloc(length);
    bytes    = recv(temp_buf, length, flags);

    // If anything was read into temp_buf, copy it into buffer.
    if ( bytes > -1 ) {
        for ( ssize_t i = 0; i < bytes; i++ ) {
            buffer[i] = temp_buf[i];
        }
    }

    free(temp_buf);

    return bytes;
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpWinSock::send (const void* buffer, const size_t length,
                          const int flags)
{
    return ::send(m_sockfd, (char*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpWinSock::send (const unsigned char* buffer, const size_t length,
                          const int flags)
{
    return send((void*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpWinSock::send (const char* buffer, const size_t length,
                          const int flags)
{
    return send((void*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpWinSock::send (const std::string& buffer, const size_t length,
                          const int flags)
{
    return send(buffer.c_str(), length, flags);
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpWinSock::send (const std::vector<char>& buffer, const size_t length,
                          const int flags)
{
    ssize_t bytes;
    char* temp_buf;

    temp_buf = (char*) malloc(length);

    // Copy the contents of buffer into temp_buf.
    for ( size_t i = 0; i < length; i++ ) {
        temp_buf[i] = buffer[i];
    }

    // Write temp_buf to the file handle.
    bytes = send(temp_buf, length, flags);

    free(temp_buf);

    return bytes;
}

// ============================================================================
// Protected methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Default constructor.  This just initializes member variables to reasonable
// defaults.
// ----------------------------------------------------------------------------
vjSocketImpWinSock::vjSocketImpWinSock ()
    : vjSocketImp(), m_sockfd(-1)
{
    init();
}

// ----------------------------------------------------------------------------
// Standard constructor.  This takes the given address (a string containing
// a hostname or an IP address), port, domain and type and stores the values
// in the member variables for use when opening the socket and performing
// communications.
// ----------------------------------------------------------------------------
vjSocketImpWinSock::vjSocketImpWinSock (const std::string& address,
                                        const unsigned short port,
                                        const vjSocketTypes::Domain domain,
                                        const vjSocketTypes::Type type)
    : vjSocketImp(address, port, domain, type), m_sockfd(-1)
{
    init();
}

// ----------------------------------------------------------------------------
// Destructor.  This currently does nothing.
// ----------------------------------------------------------------------------
vjSocketImpWinSock::~vjSocketImpWinSock () {
    init();
}

// ----------------------------------------------------------------------------
// Do the WinSock initialization required before any socket stuff can happen.
// This is copied from a similar function given on page 279 of _Effective
// TCP/IP Programming_ by Jon C. Snader.
// ----------------------------------------------------------------------------
void
vjSocketImpWinSock::init () {
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
}
