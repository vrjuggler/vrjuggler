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


#ifndef SOCK_ACCEPTOR_H
#define SOCK_ACCEPTOR_H

#include <vprConfig.h>
#include <IO/Socket/SocketStream.h>
#include <IO/Socket/InetAddr.h>

namespace vpr
{

// Socket connection acceptor factory
// Wraps all the nitty-gritty details of accepting a connection
class SocketAcceptor
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
    bool open(const InetAddr& addr, bool reuseAddr = true, const int backlog = 5);
    
    /**
     * Accept a new connection.  Creates a new socket on the connection and returns it
     * 
     * @return Ptr to the new socket for the connection  <br>
     *         NULL - Failed to accept
     * NOTE: This call is blocking.  It blocks until there is a connection to accept
     */
    SocketStream* accept();

    // Close the accepting socket
    bool close()
    { return mSocket.close(); }

    // Get the member socket that is being used
    SocketStream& getSocket()
    { return mSocket; }

private:   
    SocketStream    mSocket; 
};

SocketAcceptor::SocketAcceptor(const InetAddr& addr, bool reuseAddr, int backlog)
{
    open(addr, reuseAddr, backlog);
}

bool SocketAcceptor::open(const InetAddr& addr, bool reuseAddr, int backlog)
{
    vprASSERT((!mSocket.isOpen()) && "Trying to re-open socket that has already been opened");

    mSocket.setLocalAddr(addr);

    if(!mSocket.open())
        return false;

    mSocket.setReuseAddr(reuseAddr);

    if(!mSocket.bind())
    {
        mSocket.close();
        return false;
    }
    
    if(!mSocket.listen(backlog))
    {
        mSocket.close();
        return false;
    }
    return true;
}

SocketStream* SocketAcceptor::accept()
{
    SocketStream* new_socket(NULL);

    vprASSERT(mSocket.isOpen());

    new_socket = mSocket.accept();

    return new_socket;
}



}


#endif //SOCK_ACCEPTOR_H

