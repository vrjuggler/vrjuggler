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

#ifndef _VPR_SOCKET_STREAM_IMPL_NSPR_H_
#define _VPR_SOCKET_STREAM_IMPL_NSPR_H_

#include <vpr/vprConfig.h>
#include <string>
#include <prio.h>

#include <vpr/md/NSPR/IO/Socket/SocketImplNSPR.h>
#include <vpr/Util/Debug.h>

namespace vpr {

class VPR_CLASS_API SocketStreamImplNSPR : public SocketImplNSPR
{
public:
    // ========================================================================
    // vpr::SocketStreamImp implementation.
    // ========================================================================

    // ------------------------------------------------------------------------
    // Constructor.  This takes the address (either hostname or IP address) of
    // a remote site and a port and stores the values for later use in the
    // member variables of the object.
    //
    // PRE: None.
    // POST: The member variables are initialized with the m_type variable in
    //       particular set to SOCK_STREAM.
    // ------------------------------------------------------------------------
    SocketStreamImplNSPR(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    SocketStreamImplNSPR(const vpr::InetAddr& local_addr,
                         const vpr::InetAddr& remote_addr);

    // ------------------------------------------------------------------------
    // Copy constructor.
    // XXX: We need to have a reference count here
    // ------------------------------------------------------------------------
    SocketStreamImplNSPR (const SocketStreamImplNSPR& sock) : SocketImplNSPR(sock)
    { /* Just call base class */ }

    // ------------------------------------------------------------------------
    // Listen on the socket for incoming connection requests.
    //
    // PRE: The socket has been opened and bound to the address in
    //      m_host_addr.
    // POST: The socket is in a listening state waiting for incoming
    //       connection requests.
    //
    // Arguments:
    //     backlog - The maximum length of th queue of pending connections.
    //
    // Returns:
    //     true  - The socket is in a listening state.
    //     false - The socket could not be put into a listening state.  An
    //             error message is printed explaining what went wrong.
    // ------------------------------------------------------------------------
    vpr::Status listen(const int backlog = 5);

    // ------------------------------------------------------------------------
    // Accept an incoming connection request.
    //
    // PRE: The socket is open and is in a listening state.
    // POST: When a connection is established, a new vpr::SocketStreamImp
    //       object will be created that can be used for further communication
    //       with the remote site.
    //
    // Returns:
    // ------------------------------------------------------------------------
    vpr::Status accept(SocketStreamImplNSPR& sock,
                       const vpr::Interval timeout = vpr::Interval::NoTimeout);
};

}; // End of vpr namespace


#endif   /* _VJ_SOCKET_STREAM_IMPL_NSPR_H_ */
