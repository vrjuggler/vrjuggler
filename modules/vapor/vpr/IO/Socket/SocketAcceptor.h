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

#ifndef _VPR_SOCK_ACCEPTOR_H_
#define _VPR_SOCK_ACCEPTOR_H_

#include <vpr/vprConfig.h>
#include <vpr/Util/ReturnStatus.h>
#include <vpr/IO/Socket/SocketStream.h>


namespace vpr
{

/**
 * Socket connection acceptor factory.
 * Wraps all the nitty-gritty details of accepting a connection.
 */
template<class SockConfig_ = vpr::AcceptorConnectorConfiguration>
class SocketAcceptor_t
{
public:
   typedef SockConfig_ Config;
   typedef typename Config::SocketStreamImpl SocketStreamImpl;
   typedef typename Config::InetAddrImpl     InetAddrImpl;
   typedef typename Config::IntervalImpl     IntervalImpl;
   typedef typename Config::IOSysImpl        IOSysImpl;

public:
    /**
     * Default constructor.  This does nothing.  If this constructor is used
     * when creating an acceptor, the <code>open</code> method must be
     * called.
     *
     * @see open
     */
    SocketAcceptor_t()
    {;}

    /**
     * Destructor.  This makes sure that the accepting socket gets closed.
     */
    ~SocketAcceptor_t()
    {
        if(mSocket.isOpen())
            mSocket.close();
    }

    /**
     * Constructs Acceptor to accept connections on the given address.
     * - Opens the socket automatically
     *
     * @post This acceptor is open and bound to the given address.  It is ready
     *       to accept incoming connections.
     *
     * @param addr      The local address to which this acceptor will be bound.
     * @param reuseAddr A flag stating whether or not to set the "reuse
     *                  address" option for this socket.  This parameter is
     *                  optional and defaults to true.
     * @param backlog   The maximum allowed size for the queue of pending
     *                  connections.
     */
    SocketAcceptor_t(const InetAddrImpl& addr, bool reuseAddr = true,
                     const int backlog = 5)
    {
       open(addr, reuseAddr, backlog);
    }

    /**
     * Opens the socket for accepting a connection.
     * - Binds the connection and starts listening.
     *
     * @pre  This acceptor is not already open.
     * @post This acceptor is open and bound to the given address.  It is ready
     *       to accept incoming connections.
     *
     * @param addr      The local address to which this acceptor will be bound.
     * @param reuseAddr A flag stating whether or not to set the "reuse
     *                  address" option for this socket.  This parameter is
     *                  optional and defaults to true.
     * @param backlog   The maximum allowed size for the queue of pending
     *                  connections.
     */
    vpr::ReturnStatus open (const InetAddrImpl& addr, bool reuseAddr = true,
                            const int backlog = 5)
    {
       vpr::ReturnStatus ret_val;

       vprASSERT((!mSocket.isOpen()) && "Trying to re-open socket that has already been opened");

        mSocket.setLocalAddr(addr);

        ret_val = mSocket.open();
        if(ret_val.failure())
            return ret_val;

        mSocket.setReuseAddr(reuseAddr);

        ret_val = mSocket.bind();
        if(ret_val.failure())
        {
            mSocket.close();
            return ret_val;
        }

        ret_val = mSocket.listen(backlog);
        if(ret_val.failure())
        {
            mSocket.close();
            return ret_val;
        }
        return ret_val;
    }

    /**
     * Accepts a new connection.  Creates a new socket on the connection and
     * returns it.
     *
     * @pre This acceptor is open and listening for incoming connections.
     * @post A connected socket is returned via the by-reference parameter
     *       <code>sock</code> if a connection is successfully accepted.
     *
     * @param sock    A reference to a vpr::SocketStream object that will be
     *                used as storage for the connected socket object if a
     *                connection can be established.
     * @param timeout The amount of time to wait for an incoming connection
     *                request.  This argument is optional and default to
     *                vpr::Interval::NoTimeout.
     *
     * @return vpr::ReturnStatus::Succeed is returned when a connection is
     *         completed successfully.  In this case, the sock argument
     *         returned is the newly created connected socket.<br>
     *         vpr::ReturnStatus::WouldBlock is returned if this is a
     *         non-blocking acceptor and there are no pending connections.<br>
     *         vpr::ReturnStatus::Fail is returned if the socket failed to
     *         accept.
     */
    vpr::ReturnStatus accept(SocketStreamImpl& sock,
                             IntervalImpl timeout = IntervalImpl::NoTimeout)
    {
       vprASSERT(mSocket.isOpen());

       return mSocket.accept(sock, timeout);
    }

    /**
     * Closes the accepting socket.
     */
    vpr::ReturnStatus close()
    { return mSocket.close(); }

    /**
     * Gets the member socket that is being used.
     */
    SocketStreamImpl& getSocket()
    { return mSocket; }

    typename IOSysImpl::Handle getHandle()
    { return mSocket.getHandle(); }

private:
    SocketStreamImpl    mSocket;
};

typedef SocketAcceptor_t<> SocketAcceptor;

}


#endif /* _VPR_SOCK_ACCEPTOR_H_ */
