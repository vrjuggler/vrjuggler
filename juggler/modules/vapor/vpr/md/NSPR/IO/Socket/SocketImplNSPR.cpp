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
#include <errno.h>

#include <md/POSIX/SocketImpBSD.h>

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff   /* -1 return */
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
SocketImpNSPR::open () {
    bool retval;
    PRFileDesc* new_sock;

    // NSPR has not concept of domain in socket creation
    // switch (m_local_addr.getFamily())

    switch (m_type) {
      case SocketTypes::STREAM:
        new_sock = PR_NewTCPSocket();
        break;
      case SocketTypes::DATAGRAM:
        new_sock = PR_NewUDPSocket();
        break;
      default:
        fprintf(stderr,
                "[vpr::SocketImpNSPR] ERROR: Unknown socket type value %d\n",
                m_local_addr.getFamily());
        break;
    }

    // If socket(2) failed, print an error message and return error status.
    if ( new_sock == NULL ) {
        fprintf(stderr,
                "[vpr::SocketImpNSPR] Could not create socket: err %d \n",
                , strerror(errno));
        retval = false;
    }
    // Otherwise, return success.
    else {
        m_handle = new_sock;
        m_open = true;
        retval = true;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Bind this socket to the address in the host address member variable.
// ----------------------------------------------------------------------------
bool
SocketImpNSPR::bind () {
    bool retval;
    PRStatus status;

    // Bind the socket to the address in m_local_addr.
    status = PR_Bind(m_handle, m_local_addr.getPRNetAddr());

    // If that fails, print an error and return error status.
    if ( status == PR_FAILURE )
    {
       NSPR_PrintError("SocketImpNSPR::bind: Failed to bind.");
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
    PRStatus status;

    // Attempt to connect to the address in m_addr.
    status = PR_Connect(m_handle, m_remote_addr.getPRNetAddr(), PR_INTERVAL_NO_TIMEOUT);

    if ( status == PR_FAILURE )
    {
       NSPR_PrintError("SocketImpNSPR::connect: Failed to connect.");
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
SocketImpNSPR::SocketImpNSPR ()
    : BlockIO(std::string("INADDR_ANY")), m_handle(NULL)
{
    /* Do nothing. */ ;
}

// ----------------------------------------------------------------------------
// Standard constructor.  This takes the given address (a string containing a
// hostname or an IP address), port, domain and type and stores the values in
// the member variables for use when opening the socket and performing
// communications.
// ----------------------------------------------------------------------------
SocketImpNSPR::SocketImpNSPR (const InetAddr& local_addr,
                            const InetAddr& remote_addr,
                            const SocketTypes::Type sock_type)
    : BlockIO(std::string("INADDR_ANY")), m_handle(NULL),
      m_local_addr(local_addr), m_remote_addr(remote_addr), m_type(sock_type)
{;}

// ----------------------------------------------------------------------------
// Destructor.  This currently does nothing.
// ----------------------------------------------------------------------------
SocketImpBSD::~SocketImpBSD ()
{
    if ( m_handle != NULL ) {
       PRClose(m_handle);
       m_handle = NULL;
    }
}

}; // End of vpr namespace
