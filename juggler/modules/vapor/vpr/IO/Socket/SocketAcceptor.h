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

#ifndef _VPR_SOCK_ACCEPTOR_H_
#define _VPR_SOCK_ACCEPTOR_H_

#include <vpr/vprConfig.h>
#include <vpr/IO/Socket/SocketStream.h>
#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/Util/Status.h>

namespace vpr
{

// Socket connection acceptor factory
// Wraps all the nitty-gritty details of accepting a connection
class VPR_CLASS_API SocketAcceptor
{
public:
    // Default constructor
    SocketAcceptor()
    {;}

    ~SocketAcceptor()
    {
        if(mSocket.isOpen())
            mSocket.close();
    }

    // Construct Acceptor to accept connections on the given address
    // - Opens the socket automatically
    SocketAcceptor( const InetAddr& addr, bool reuseAddr = true, const int backlog = 5);

    // Open the socket for accepting a connection
    // - Binds the connection and starts listening
    Status open(const InetAddr& addr, bool reuseAddr = true, const int backlog = 5);

    /**
     * Accept a new connection.  Creates a new socket on the connection and returns it
     *
     * @return Ptr to the new socket for the connection  <br>
     *         NULL - Failed to accept
     * NOTE: This call is blocking.  It blocks until there is a connection to accept
     */
    Status accept(SocketStream& sock, vpr::Interval timeout = vpr::Interval::NoTimeout);

    // Close the accepting socket
    Status close()
    { return mSocket.close(); }

    // Get the member socket that is being used
    SocketStream& getSocket()
    { return mSocket; }
    IOSys::Handle getHandle()
    { return mSocket.getHandle(); }

private:
    SocketStream    mSocket;
};

}


#endif /* _VPR_SOCK_ACCEPTOR_H_ */
