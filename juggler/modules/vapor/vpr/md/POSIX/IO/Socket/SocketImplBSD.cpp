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
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include <md/POSIX/SocketImpBSD.h>

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff	/* -1 return */
#endif


// ============================================================================
// External global variables.
// ============================================================================
extern int errno;

namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Open the socket.  This creates a new socket using the domain and type
// options set through member variables.
// ----------------------------------------------------------------------------
bool
SocketImpBSD::open () {
    int domain, type, sock;
    bool retval;

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
#ifdef PF_INET6
        domain = PF_INET6;
#else
        fprintf(stderr,
                "[vpr::SocketImpBSD] WARNING: IPv6 not supported on this host!\n");
#endif
        break;
      case SocketTypes::LINK:
#if defined(PF_LINK)
        domain = PF_LINK;
#elif defined(PF_RAW)
        domain = PF_RAW;
#else
        fprintf(stderr,
                "[vpr::SocketImpBSD] WARNING: Cannot use LINK sockets. domain %d\n",
                m_local_addr.getFamily());
#endif
        break;
      default:
        fprintf(stderr,
                "[vpr::SocketImpBSD] ERROR: Unknown socket domain value %d\n",
                m_local_addr.getFamily());
        break;
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
      default:
        fprintf(stderr,
                "[vpr::SocketImpBSD] ERROR: Unknown socket type value %d\n",
                m_local_addr.getFamily());
        break;
    }

    // Attempt to create a new socket using the values in m_local_addr and
    // m_type.
    sock = socket(domain, type, 0);

    // If socket(2) failed, print an error message and return error status.
    if ( sock == -1 ) {
        fprintf(stderr,
                "[vpr::SocketImpBSD] Could not create socket (%s): %s\n",
                m_name.c_str(), strerror(errno));
        retval = false;
    }
    // Otherwise, return success.
    else {
        m_handle->m_fdesc = sock;
        retval = true;
    }

    if ( retval != 0 ) {
        // If the value in m_local_addr is the string "INADDR_ANY", use the
        // INADDR_ANY constant.
        if ( m_name == "INADDR_ANY" || m_name == "" ) {
            m_local_addr.setAddressValue(INADDR_ANY);
        }
        // Otherwise, look up the address and save the return status as the
        // return value for this method.
        else {
            retval = lookupAddress();
        }
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Bind this socket to the address in the host address member variable.
// ----------------------------------------------------------------------------
bool
SocketImpBSD::bind () {
    bool retval;
    int status;

    // Bind the socket to the address in m_local_addr.
    status = ::bind(m_handle->m_fdesc, (struct sockaddr*) &m_local_addr.m_addr,
                    m_local_addr.size());

    // If that fails, print an error and return error status.
    if ( status == -1 ) {
        fprintf(stderr,
                "[vpr::SocketImpBSD] Cannot bind socket to address: %s\n",
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
SocketImpBSD::connect () {
    bool retval;
    int status;

    // Attempt to connect to the address in m_addr.
    status = ::connect(m_handle->m_fdesc,
                       (struct sockaddr*) &m_remote_addr.m_addr,
                       m_remote_addr.size());

    // If connect(2) failed, print an error message explaining why and return
    // error status.
    if ( status == -1 ) {
        fprintf(stderr, "[vpr::SocketImpBSD] Error connecting to %s: %s\n",
                m_name.c_str(), strerror(errno));
        retval = false;
    }
    // Otherwise, return success.
    else {
        retval = true;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
ssize_t
SocketImpBSD::recv (void* buffer, const size_t length, const int flags) {
    return ::recv(m_handle->m_fdesc, buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
ssize_t
SocketImpBSD::recv (unsigned char* buffer, const size_t length,
                      const int flags)
{
    return recv((void*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
ssize_t
SocketImpBSD::recv (char* buffer, const size_t length, const int flags) {
    return recv((void*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
ssize_t
SocketImpBSD::recv (std::string& buffer, const size_t length,
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
SocketImpBSD::recv (std::vector<char>& buffer, const size_t length,
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
SocketImpBSD::send (const void* buffer, const size_t length, const int flags) {
    return ::send(m_handle->m_fdesc, buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
SocketImpBSD::send (const unsigned char* buffer, const size_t length,
                    const int flags)
{
    return send((void*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
SocketImpBSD::send (const char* buffer, const size_t length, const int flags) {
    return send((void*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
SocketImpBSD::send (const std::string& buffer, const size_t length,
                    const int flags)
{
    return send(buffer.c_str(), length, flags);
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
SocketImpBSD::send (const std::vector<char>& buffer, const size_t length,
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
SocketImpBSD::SocketImpBSD ()
    : SocketImp_i(), m_handle(NULL)
{
fprintf(stderr, "vpr::SocketImpBSD default constructor\n");
    /* Do nothing. */ ;
}

// ----------------------------------------------------------------------------
// Standard constructor.  This takes the given address (a string containing a
// hostname or an IP address), port, domain and type and stores the values in
// the member variables for use when opening the socket and performing
// communications.
// ----------------------------------------------------------------------------
SocketImpBSD::SocketImpBSD (const InetAddr& local_addr,
                            const InetAddr& remote_addr,
                            const SocketTypes::Type sock_type)
    : SocketImp_i(local_addr, remote_addr, sock_type)
{
fprintf(stderr, "vpr::SocketImpBSD(local, remote) constructor\n");
fprintf(stderr, "    Local Address: %s\n",
        m_local_addr.getAddressString().c_str());
fprintf(stderr, "    Local Port: %hu -> %hu\n", local_addr.getPort(),
        m_local_addr.getPort());
fprintf(stderr, "    Remote Address: %s\n",
        m_remote_addr.getAddressString().c_str());
fprintf(stderr, "    Remote Port: %hu -> %hu\n", remote_addr.getPort(),
        m_remote_addr.getPort());
fprintf(stderr, "    Domain: %d -> %d\n", local_addr.getFamily(),
        m_local_addr.getFamily());
fprintf(stderr, "    Type: %d -> %d\n", sock_type, m_type);
    m_handle = new FileHandleUNIX(m_name);
}

// ----------------------------------------------------------------------------
// Destructor.  This currently does nothing.
// ----------------------------------------------------------------------------
SocketImpBSD::~SocketImpBSD () {
    if ( m_handle != NULL ) {
        delete m_handle;
        m_handle = NULL;
    }
}

// ----------------------------------------------------------------------------
// Look up the address in m_name and store the address in the m_remote_addr
// object.
// ----------------------------------------------------------------------------
bool
SocketImpBSD::lookupAddress () {
    bool retval;
    struct hostent* host_entry;

    // First, try looking the host up by name.
    host_entry = gethostbyname(m_name.c_str());

    // If that succeeded, put the result in m_remote_addr.
    if ( host_entry != NULL ) {
        m_remote_addr.copyAddressValue(host_entry->h_addr);
        retval = true;
    }
    // If gethostbyname(3) failed, the address string may be an IP address.
    else {
        unsigned long addr;

        // Try looking it up with inet_addr(3).
        addr = inet_addr(m_name.c_str());

        // If the address string could not be found using inet_addr(3), then
        // return error status.
        if ( addr == INADDR_NONE ) {
            fprintf(stderr,
                    "[vpr::SocketImpBSD] Could not find address for '%s': %s\n",
                    m_name.c_str(), strerror(errno));
            retval = false;
        }
        // Otherwise, we found the integer address successfully.
        else {
            m_remote_addr.setAddressValue(addr);
            retval = true;
        }
    }

    return retval;
}

}; // End of vpr namespace
