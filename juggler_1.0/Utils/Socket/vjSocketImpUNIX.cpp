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

#include <Utils/Socket/vjSocketImpUNIX.h>

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff	/* -1 return */
#endif


// ============================================================================
// External global variables.
// ============================================================================
extern int errno;


// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Open the socket.  This creates a new socket using the domain and type
// options set through member variables.
// ----------------------------------------------------------------------------
bool
vjSocketImpUNIX::open () {
    int domain, type, sock;
    bool retval;

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
#ifdef PF_INET6
        domain = PF_INET6;
#else
        fprintf(stderr,
                "[vjSocketImpUNIX] WARNING: IPv6 not supported on this host!\n");
#endif
        break;
      case vjSocketTypes::LINK:
#ifdef PF_LINK
        domain = PF_LINK;
#else
        domain = PF_RAW;
#endif
        break;
      default:
        fprintf(stderr,
                "[vjSocketImpUNIX] ERROR: Unknown socket domain value %d\n",
                m_addr.getFamily());
        break;
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
      default:
        fprintf(stderr,
                "[vjSocketImpUNIX] ERROR: Unknown socket type value %d\n",
                m_addr.getFamily());
        break;
    }

    // Attempt to create a new socket using the values in m_addr and m_type.
    sock = socket(domain, type, 0);

    // If socket(2) failed, print an error message and return error status.
    if ( sock == -1 ) {
        fprintf(stderr, "[vjSocketImpUNIX] Could not create socket (%s): %s\n",
                m_name.c_str(), strerror(errno));
        retval = false;
    }
    // Otherwise, return success.
    else {
        m_handle->m_fdesc = sock;
        retval = true;
    }

    if ( retval ) {
        // If the value in m_address is the string "INADDR_ANY", use the
        // INADDR_ANY constant.
        if ( m_name == "INADDR_ANY" || m_name == "" ) {
            m_addr.setAddressValue(INADDR_ANY);
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
vjSocketImpUNIX::bind () {
    bool retval;
    int status;

    // Bind the socket to the address in m_addr.
    status = ::bind(m_handle->m_fdesc, (struct sockaddr*) &m_addr.m_addr,
                    m_addr.size());

    // If that fails, print an error and return error status.
    if ( status == -1 ) {
        fprintf(stderr, "[vjSocketImpUNIX] Cannot bind socket to address: %s\n",
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
vjSocketImpUNIX::connect () {
    bool retval;
    int status;

    // Attempt to connect to the address in m_addr.
    status = ::connect(m_handle->m_fdesc, (struct sockaddr*) &m_addr.m_addr,
                       m_addr.size());

    // If connect(2) failed, print an error message explaining why and return
    // error status.
    if ( status == -1 ) {
        fprintf(stderr, "[vjSocketImpUNIX] Error connecting to %s: %s\n",
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
vjSocketImpUNIX::recv(void* buffer, const size_t length, const int flags) {
    return ::recv(m_handle->m_fdesc, buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpUNIX::recv (unsigned char* buffer, const size_t length,
                    const int flags)
{
    return recv((void*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpUNIX::recv (char* buffer, const size_t length, const int flags) {
    return recv((void*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Receive the specified number of bytes from the remote site to which the
// local side is connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpUNIX::recv (std::vector<char>& buffer, const size_t length,
                       const int flags)
{
    ssize_t bytes;

    // If length is 0, determine the size of buffer using the size() method.
    if ( length == 0 ) {
        bytes = recv((void*) &(buffer[0]), buffer.size(), flags);
    }
    // Otherwise, just use length.
    else {
        bytes = recv((void*) &(buffer[0]), length, flags);
    }

    return bytes;
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpUNIX::send (const void* buffer, const size_t length,
                       const int flags)
{
    return ::send(m_handle->m_fdesc, buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpUNIX::send (const unsigned char* buffer, const size_t length,
                       const int flags)
{
    return send((void*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpUNIX::send (const char* buffer, const size_t length,
                       const int flags)
{
    return send((void*) buffer, length, flags);
}

// ----------------------------------------------------------------------------
// Send the specified number of bytes contained in the given buffer from the
// local side to the remote site to which we are connected.
// ----------------------------------------------------------------------------
ssize_t
vjSocketImpUNIX::send (const std::vector<char>& buffer, const size_t length,
                       const int flags)
{
    ssize_t bytes;

    // If length is 0, determine the size of buffer using the size() method.
    if ( length == 0 ) {
        bytes = send((void*) &(buffer[0]), buffer.size(), flags);
    }
    // Otherwise, just use length.
    else {
        bytes = send((void*) &(buffer[0]), length, flags);
    }

    return bytes;
}

// ============================================================================
// Protected methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Default constructor.  This just initializes member variables to reasonable
// defaults.
// ----------------------------------------------------------------------------
vjSocketImpUNIX::vjSocketImpUNIX ()
    : vjSocketImp(), m_handle(NULL)
{
fprintf(stderr, "vjSocketImpUNIX default constructor\n");
    /* Do nothing. */ ;
}

// ----------------------------------------------------------------------------
// Standard constructor.  This takes the given address (a string containing a
// hostname or an IP address), port, domain and type and stores the values in
// the member variables for use when opening the socket and performing
// communications.
// ----------------------------------------------------------------------------
vjSocketImpUNIX::vjSocketImpUNIX (const std::string& address,
                                  const unsigned short port,
                                  const vjSocketTypes::Domain domain,
                                  const vjSocketTypes::Type type)
    : vjSocketImp(address, port, domain, type)
{
    m_name = address;
    m_addr.setPort(port);
    m_addr.setFamily(domain);
    m_type = type;
fprintf(stderr, "vjSocketImpUNIX(address, port, domain, type) constructor\n");
fprintf(stderr, "    Address: %s -> %s\n", address.c_str(), m_name.c_str());
fprintf(stderr, "    Port: %hu -> %hu\n", port, m_addr.getPort());
fprintf(stderr, "    Domain: %d -> %d\n", domain, m_addr.getFamily());
fprintf(stderr, "    Type: %d -> %d\n", type, m_type);
    m_handle = new vjFileHandleUNIX(address);
}

// ----------------------------------------------------------------------------
// Destructor.  This currently does nothing.
// ----------------------------------------------------------------------------
vjSocketImpUNIX::~vjSocketImpUNIX () {
    if ( m_handle != NULL ) {
        delete m_handle;
        m_handle = NULL;
    }
}

// ----------------------------------------------------------------------------
// Look up the address in m_name and store the address in the m_addr
// object.
// ----------------------------------------------------------------------------
bool
vjSocketImpUNIX::lookupAddress () {
    bool retval;
    struct hostent* host_entry;

    // First, try looking the host up by name.
    host_entry = gethostbyname(m_name.c_str());

    // If that succeeded, put the result in m_addr.
    if ( host_entry != NULL ) {
        m_addr.copyAddressValue(host_entry->h_addr);
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
                    "[vjSocketImpUNIX] Could not find address for '%s': %s\n",
                    m_name.c_str(), strerror(errno));
            retval = false;
        }
        // Otherwise, we found the integer address successfully.
        else {
            m_addr.setAddressValue(addr);
            retval = true;
        }
    }

    return retval;
}
