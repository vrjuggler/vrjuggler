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

#ifndef _VPR_SOCKET_STREAM_IMPL_WINSOCK_H_
#define _VPR_SOCKET_STREAM_IMPL_WINSOCK_H_

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/md/WIN32/IO/Socket/SocketImplWinSock.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/IO/Socket/SocketStreamOpt.h>


namespace vpr {

class SocketStreamImplWinSock : public SocketStreamOpt,
                                public SocketImplWinSock
{
public:
    // ========================================================================
    // vpr::SocketStream implementation.
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
    SocketStreamImplWinSock(void);

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    SocketStreamImplWinSock(const InetAddr& local_addr,
                            const InetAddr& remote_addr);

    // ------------------------------------------------------------------------
    // Constructor.  This takes the address (either hostname or IP address) of
    // a remote site and a port and stores the values for later use in the
    // member variables of the object.
    //
    // PRE: None.
    // POST: The member variables are initialized with the m_type variable in
    //       particular set to SOCK_STREAM.
    //
    // Arguments:
    //     address - The hostname or IP address of the site to which we will
    //               connect.
    //     port    - The port on the remote site to which we will connect.
    // ------------------------------------------------------------------------
    SocketStreamImplWinSock(const std::string& address,
                            const unsigned short port,
                            const SocketTypes::Domain domain);

    // ------------------------------------------------------------------------
    // Destructor.  This currently does nothing.
    //
    // PRE: None.
    // POST: None.
    // ------------------------------------------------------------------------
    virtual ~SocketStreamImplWinSock (void) {
        /* Do nothing. */ ;
    }

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
    virtual Status listen(const int backlog = 5);

    // ------------------------------------------------------------------------
    // Accept an incoming connection request.
    //
    // PRE: The socket is open and is in a listening state.
    // POST: When a connection is established, a new
    //       vpr::SocketStreamImplWinSock object will be created that can be
    //       used for further communication with the remote site.
    //
    // Returns:
    //     Non-NULL - A new vpr::SocketStreamImplWinSock object that can be
    //                used to communicate with the remote site.
    //     NULL     - A socket could not be created to establish communication
    //                with the remote site.  An error message is printed
    //                explaining what went wrong.
    //
    // Note:
    //     This is a blocking call and will block until a connection is
    //     established.
    // ------------------------------------------------------------------------
    virtual Status accept(SocketStreamImplWinSock& sock);

protected:
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status
    getOption (const SocketOptions::Types option,
               struct SocketOptions::Data& data)
    {
        return SocketImplWinSock::getOption(option, data);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual Status
    setOption (const SocketOptions::Types option,
               const struct SocketOptions::Data& data)
    {
        return SocketImplWinSock::setOption(option, data);
    }
};

}; // End of vpr namespace


#endif	/* _VPR_SOCKET_STREAM_IMPL_WINSOCK_H_ */
