/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <string.h>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include <prio.h>
#include <prinrval.h>

#include <vpr/md/NSPR/IO/Socket/SocketStreamImplNSPR.h>
#include <vpr/md/NSPR/NSPRHelpers.h>


namespace vpr {

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
// ----------------------------------------------------------------------------
SocketStreamImplNSPR::SocketStreamImplNSPR (void)
   : SocketImplNSPR(vpr::SocketTypes::STREAM)
{
    /* Do nothing. */ ;
}

// ----------------------------------------------------------------------------
// Constructor.  This takes the address (either hostname or IP address) of a
// remote site and a port and stores the values for later use in the member
// variables of the object.
// ----------------------------------------------------------------------------
SocketStreamImplNSPR::SocketStreamImplNSPR (const vpr::InetAddr& local_addr,
                                            const vpr::InetAddr& remote_addr)
    : SocketImplNSPR(local_addr, remote_addr, vpr::SocketTypes::STREAM)
{;}

// ----------------------------------------------------------------------------
// Listen on the socket for incoming connection requests.
// ----------------------------------------------------------------------------
vpr::ReturnStatus
SocketStreamImplNSPR::listen (const int backlog)
{
    vpr::ReturnStatus retval;
    PRStatus status;

    if(!m_bound)        // To listen, we must be bound
    {
        vprDEBUG(vprDBG_ALL,0) << "SocketStreamImplNSPR::listen: Trying to listen on an unbound socket.\n"
                      << vprDEBUG_FLUSH;
        retval.setCode(vpr::ReturnStatus::Fail);
    }
    else {
        // Put the socket into listning mode.  If that fails, print an error
        // and return error status.
        status = PR_Listen(m_handle, backlog);

        if (PR_FAILURE == status) {
           NSPR_PrintError("SocketStreamImplNSPR::listen: Cannon listen on socket: ");
           retval.setCode(vpr::ReturnStatus::Fail);
        }
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Accept an incoming connection request.
// ----------------------------------------------------------------------------
vpr::ReturnStatus
SocketStreamImplNSPR::accept (SocketStreamImplNSPR& sock, vpr::Interval timeout)
{
    vpr::ReturnStatus retval;
    vpr::InetAddr addr;

    if(!m_bound)        // To listen, we must be bound
    {
        vprDEBUG(vprDBG_ALL,0) << "SocketStreamImplNSPR::accept: Trying to accept on an unbound socket.\n"
                      << vprDEBUG_FLUSH;
        retval.setCode(vpr::ReturnStatus::Fail);
    }
    else {
       PRFileDesc* accept_sock = NULL;

       // PR_Accept() is a potentially blocking call.
       m_blocking_fixed = true;

       // Accept an incoming connection request.
       vprASSERT(m_handle != NULL);
       accept_sock = PR_Accept(m_handle, addr.getPRNetAddr(),
                               NSPR_getInterval(timeout) );

       if (NULL == accept_sock) {
          PRErrorCode err_code = PR_GetError();

          if ( err_code == PR_WOULD_BLOCK_ERROR ) {
             retval.setCode(vpr::ReturnStatus::WouldBlock);
          }
          else if ( err_code == PR_IO_TIMEOUT_ERROR ) {
             retval.setCode(vpr::ReturnStatus::Timeout);
          }
          else {
             NSPR_PrintError("SocketStreamImplNSPR::accept: Cannot accept on socket: ");
             retval.setCode(vpr::ReturnStatus::Fail);
          }
       }
       // Otherwise, put the new socket in the passed socket object.
       else {
          sock.m_handle = accept_sock;
          sock.setRemoteAddr(addr);
          sock.m_open = true;
          sock.m_bound = true;
          sock.m_connected = true;
          sock.m_blocking = m_blocking;
          sock.m_blocking_fixed = true;
       }
    }

    return retval;
}

}; // End of vpr namespace
