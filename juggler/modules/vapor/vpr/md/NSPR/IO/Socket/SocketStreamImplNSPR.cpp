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

#include <md/NSPR/SocketStreamImpNSPR.h>
#include <md/NSPR/NSPRHelpers.h>


namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
// ----------------------------------------------------------------------------
SocketStreamImpNSPR::SocketStreamImpNSPR (void)
   : SocketImpNSPR()
{
    /* Do nothing. */ ;
}

// ----------------------------------------------------------------------------
// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
// ----------------------------------------------------------------------------
SocketStreamImpNSPR::SocketStreamImpNSPR (const InetAddr& local_addr,
                                        const InetAddr& remote_addr)
    : SocketImpNSPR(local_addr, remote_addr, SocketTypes::STREAM)
{;}

// ----------------------------------------------------------------------------
// Listen on the socket for incoming connection requests.
// ----------------------------------------------------------------------------
bool
SocketStreamImpNSPR::listen (const int backlog)
{
    bool retval;
    PRStatus status;

    if(!m_bound)        // To listen, we must be bound
    {
        vprDEBUG(0,0) << "SocketStreamImpNSPR::listen: Trying to listen on an unbound socket.\n" << vprDEBUG_FLUSH;
        return false;
    }

    // Put the socket into listning mode.  If that fails, print an error and
    // return error status.
    status = PR_Listen(m_handle, backlog);

    if (PR_FAILURE == status) {
       NSPR_PrintError("SocketStreamImpNSPR::listen: Cannon listen on socket: ");
       retval = false;
    }
    retval = true;

    return retval;
}

// ----------------------------------------------------------------------------
// Accept an incoming connection request.
// ----------------------------------------------------------------------------
SocketStreamImpNSPR*
SocketStreamImpNSPR::accept () {
    PRFileDesc* accept_sock(NULL);
    InetAddr addr;
    SocketStreamImpNSPR* new_sock(NULL);

    if(!m_bound)        // To listen, we must be bound
    {
        vprDEBUG(0,0) << "SocketStreamImpNSPR::accept: Trying to accept on an unbound socket.\n" << vprDEBUG_FLUSH;
        return false;
    }

    // Accept an incoming connection request.
    vprASSERT(m_handle != NULL);
    accept_sock = PR_Accept(m_handle, addr.getPRNetAddr(), PR_INTERVAL_NO_TIMEOUT);

    if (NULL == accept_sock) {
       NSPR_PrintError("SocketStreamImpNSPR::accept: Cannot accept on socket: ");
       return NULL;
    }

    // Otherwise, create a new vpr::SocketStreamImp object
    else {
        new_sock = new SocketStreamImpNSPR(accept_sock, addr);
        vprASSERT(new_sock != NULL);
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
SocketStreamImpNSPR::SocketStreamImpNSPR (PRFileDesc* sock,
                                        const InetAddr& remote_addr)
    : SocketImpNSPR()
{
   m_handle = sock;
   m_type = SocketTypes::STREAM;
   m_type = SocketTypes::STREAM;

    // Copy the given vpr::InetAddr to the new object's member variable.
    m_remote_addr = remote_addr;

    // The socket is open and bound (since it was created and is connected)
    m_open = true;
    m_bound = true;
}

}; // End of vpr namespace
