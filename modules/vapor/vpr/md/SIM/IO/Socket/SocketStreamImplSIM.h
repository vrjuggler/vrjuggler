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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _VPR_SOCKET_STREAM_IMPL_SIMULATED_H_
#define _VPR_SOCKET_STREAM_IMPL_SIMULATED_H_

#include <vpr/vprConfig.h>

#include <string>
#include <queue>
#include <utility>

#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Interval.h>
#include <vpr/md/SIM/IO/Socket/SocketImplSIM.h>


namespace vpr
{

/**
 * Simulated stream sockets.
 *
 * @author Kevin Meinert
 * @author Patrick Hartling
 */
class VPR_CLASS_API SocketStreamImplSIM : public vpr::SocketImplSIM
{
public:
   // =========================================================================
   // vpr::SocketStreamImp implementation.
   // =========================================================================

   /**
    * Default constructor.  This initializes the member variables.
    */
   SocketStreamImplSIM (void)
      : SocketImplSIM(vpr::SocketTypes::STREAM), mNoDelay(false)
   {
      /* Do nothing. */ ;
   }

   /**
    * Constructor.  This initializes the member variables.  In particular,
    * the local and remote addresses for this socket are initialized using
    * the given addresses.
    *
    * @post The member variables are initialized with the mType variable in
    *       particular set to SOCK_STREAM.
    *
    * @param local_addr  The local address for this socket (the one it is
    *                    bound to).
    * @param remote_addr The remote address for this socket.  This is used for
    *                    a socket that will connect to another socket.
    */
   SocketStreamImplSIM (const vpr::InetAddr& local_addr,
                        const vpr::InetAddr& remote_addr)
      : SocketImplSIM(local_addr, remote_addr, vpr::SocketTypes::STREAM),
        mNoDelay( false )
   {
      mLocalAddr  = local_addr;
      mRemoteAddr = remote_addr;
   }

   /**
    * Destructor.  This currently does nothing but act as a placeholder.
    */
   virtual ~SocketStreamImplSIM (void)
   {
      /* Do nothing. */ ;
   }

   /**
    * Puts this socket into the listening state where it listens for
    * incoming connection requests.
    *
    * @pre The socket has been opened and bound to the address in mLocalAddr.
    * @post The socket is in a listening state waiting for incoming
    *       connection requests.
    *
    * @param backlog The maximum length of the queue of pending connections.
    *
    * @return vpr::ReturnStatus::Success is returned if this socket is now in a
    *         listening state.<br>
    *         vpr::ReturnStatus::Failure is returned otherwise.
    */
   vpr::ReturnStatus listen( const int backlog = 5 );

   /**
    * Accepts an incoming connection request and return the connected socket
    * to the caller in the given socket object reference.
    *
    * @pre The socket is open and is in a listening state.
    * @post When a connection is established, the given vpr::SocketStream
    *       object is assigned the newly connected socket.
    *
    * @param sock    A reference to a vpr::SocketStream object that will
    *                be used to return the connected socket created.
    * @param timeout The length of time to wait for the accept call to
    *                return.
    *
    * @return vpr::ReturnStatus::Success is returned if the incoming request has
    *         been handled, and the given SocketStream object is a valid,
    *         connected socket.<br>
    *         vpr::ReturnStatus::WouldBlock is returned if this is a non-blocking
    *         socket, and there are no waiting connection requests.<br>
    *         vpr::ReturnStatus::Timeout is returned when no connections requests
    *         arrived within the given timeout period.<br>
    *         vpr::ReturnStatus::Failure is returned if the accept failed.  The
    *         given vpr::SocketStream object is not modified in this case.
    */
   vpr::ReturnStatus accept( SocketStreamImplSIM& client_sock,
                             vpr::Interval timeout = vpr::Interval::NoTimeout );

   /**
    * Sets the current no-delay status for this socket.  If no-delay is true,
    * then the Nagel algorithm will be disabled.  Of course, there is no
    * Nagel algorithm for sim sockets.
    *
    * @param enable_val The Boolean enable/disable state for no-delay on this
    *                   socket.
    */
   vpr::ReturnStatus setNoDelay( bool setting )
   {
      mNoDelay = setting;
      return vpr::ReturnStatus();
   }

   /**
    * @param connector The socket making the connection.
    */
   vpr::ReturnStatus addConnector(vpr::SocketImplSIM* peerSock);

   /**
    * Gets the current number of connectors waiting to connect to this
    * socket.
    */
   vpr::Uint32 getConnectorCount (void) const
   {
      return mConnectorQueue.size();
   }

   virtual vpr::ReturnStatus isReadReady() const;

   virtual vpr::ReturnStatus isWriteReady() const;

protected:
   /**< Queue of pending connection requests.This is a list of the requesting connectors */
   std::queue<SocketStreamImplSIM*> mConnectorQueue;
   vpr::Mutex              mConnectorQueueMutex; /**< Mutex for connector queue */

   bool mNoDelay;
};

} // End of vpr namespace


#endif  /* _VPR_SOCKET_STREAM_IMPL_SIMULATED_H_ */
