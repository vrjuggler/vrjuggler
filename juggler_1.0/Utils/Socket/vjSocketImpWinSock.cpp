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

#include <Utils/Socket/vjSocketImpWinSock.h>


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
    /* Do nothing. */ ;
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
//    bzero((void*) &m_host_addr, sizeof(m_host_addr));
}

// ----------------------------------------------------------------------------
// Destructor.  This currently does nothing.
// ----------------------------------------------------------------------------
vjSocketImpWinSock::~vjSocketImpWinSock () {
    /* Do nothing. */ ;
}
