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

#include <VPR/md/POSIX/vjSocketStreamImpBSD.h>


// ============================================================================
// External global variables.
// ============================================================================
extern int errno;


// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
// ----------------------------------------------------------------------------
vjSocketStreamImpBSD::vjSocketStreamImpBSD (void)
    : vjSocketImpBSD(), vjSocketStreamImp()
{
fprintf(stderr, "vjSocketStreamImpBSD default constructor\n");
    /* Do nothing. */ ;
}

// ----------------------------------------------------------------------------
// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
// ----------------------------------------------------------------------------
vjSocketStreamImpBSD::vjSocketStreamImpBSD (const std::string& address,
                                              const unsigned short port)
    : vjSocketImpBSD(address, port, vjSocketTypes::INET,
                      vjSocketTypes::STREAM),
      vjSocketStreamImp(address, port, vjSocketTypes::INET)
{
    m_name = address;
    m_addr.setPort(port);
    m_addr.setFamily(vjSocketTypes::INET);
    m_type = vjSocketTypes::STREAM;
fprintf(stderr, "vjSocketStreamImpBSD(address, port) constructor\n");
fprintf(stderr, "    Address: %s -> %s\n", address.c_str(), m_name.c_str());
fprintf(stderr, "    Port: %hu -> %hu\n", port, m_addr.getPort());
fprintf(stderr, "    Domain: %d\n", m_addr.getFamily());
fprintf(stderr, "    Type: %d\n", m_type);
}

// ----------------------------------------------------------------------------
// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
// ----------------------------------------------------------------------------
vjSocketStreamImpBSD::vjSocketStreamImpBSD (const std::string& address,
                                            const unsigned short port,
                                            const vjSocketTypes::Domain domain)
    : vjSocketImpBSD(address, port, domain, vjSocketTypes::STREAM),
      vjSocketStreamImp(address, port, domain)
{
    m_name = address;
    m_addr.setPort(port);
    m_addr.setFamily(domain);
    m_type = vjSocketTypes::STREAM;
fprintf(stderr, "vjSocketStreamImpBSD(address, port, domain) constructor\n");
fprintf(stderr, "    Address: %s -> %s\n", address.c_str(), m_name.c_str());
fprintf(stderr, "    Port: %hu -> %hu\n", port, m_addr.getPort());
fprintf(stderr, "    Domain: %d -> %d\n", domain, m_addr.getFamily());
fprintf(stderr, "    Type: %d\n", m_type);
}

// ----------------------------------------------------------------------------
// Destructor.  This currently does nothing.
// ----------------------------------------------------------------------------
vjSocketStreamImpBSD::~vjSocketStreamImpBSD () {
    /* Do nothing. */ ;
}

// ----------------------------------------------------------------------------
// Listen on the socket for incoming connection requests.
// ----------------------------------------------------------------------------
bool
vjSocketStreamImpBSD::listen (const int backlog) {
    bool retval;

    // Put the socket into listning mode.  If that fails, print an error and
    // return error status.
    if ( ::listen(m_handle->m_fdesc, backlog) == -1 ) {
        fprintf(stderr, "[vjSocketStreamImpBSD] Cannot listen on socket: %s\n",
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
// Accept an incoming connection request.
// ----------------------------------------------------------------------------
vjSocketStreamImp*
vjSocketStreamImpBSD::accept () {
    int accept_sock;
    vjInetAddr addr;
    socklen_t addrlen;
    vjSocketStreamImp* new_sock;

    // Accept an incoming connection request.
    addrlen = addr.size();
    accept_sock = ::accept(m_handle->m_fdesc, (struct sockaddr*) &addr.m_addr,
                           &addrlen);

    // If accept(2) failed, print an error message and return error stauts.
    if ( accept_sock == -1 ) {
        fprintf(stderr,
                "[vjSocketStreamImpBSD] Error while accepting incoming connection: %s\n",
                strerror(errno));
        new_sock = NULL;
    }
    // Otherwise, create a new vjSocketStreamImpBSD object using what the
    // operating system gave us through accept(2).
    else {
        new_sock = new vjSocketStreamImpBSD(accept_sock, addr);
    }

    return new_sock;
}

// ============================================================================
// Protected methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Protected constructor.  This is used when the socket is created by the
// operating system, typically by the accept(2) system call.
// ----------------------------------------------------------------------------
vjSocketStreamImpBSD::vjSocketStreamImpBSD (const int sock,
                                            vjInetAddr& host_addr)
    : vjSocketImpBSD(), vjSocketStreamImp()
{
fprintf(stderr, "Protected vjSocketStreamImpBSD constructor\n");
// XXX: Merge
    std::string addr = host_addr.getAddressString();
fprintf(stderr, "Client addr: %s:%hu\n", addr.c_str(), host_addr.getPort());
    m_handle          = new vjFileHandleUNIX(addr);
    m_handle->m_fdesc = sock;
    m_type            = vjSocketTypes::STREAM;

    // Copy the given vjInetAddr to the new object's member variable.
    m_addr = host_addr;
}
