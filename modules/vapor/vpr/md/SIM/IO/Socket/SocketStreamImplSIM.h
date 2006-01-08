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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

/** \class SocketStreamImplSIM SocketStreamImplSIM.h vpr/IO/Socket/SocketStream.h
 *
 * Implementation of stream sockets using simulated sockets.  This is used in
 * conjunction with vpr::SocketConfiguration to create the typedef
 * vpr::SocketStream.
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
   SocketStreamImplSIM()
      : SocketImplSIM(vpr::SocketTypes::STREAM), mNoDelay(false)
   {
      /* Do nothing. */ ;
   }

   /**
    * Constructor.  This takes the local and remote addresses for this socket.
    * The local address is the address to which this socket will be bound.
    * The remote address is the address which with this socket will
    * communicate.
    *
    * @post The member variables are initialized to default values.  The
    *       socket type is set to vpr::SocketTypes::STREAM.
    *
    * @param localAddr  The local address to which this socket will be bound.
    * @param remoteAddr The remote address whith which this socket will
    *                   communicate.
    *
    * @see bind, connect
    */
   SocketStreamImplSIM(const vpr::InetAddr& localAddr,
                       const vpr::InetAddr& remoteAddr)
      : SocketImplSIM(localAddr, remoteAddr, vpr::SocketTypes::STREAM),
        mNoDelay(false)
   {
      mLocalAddr  = localAddr;
      mRemoteAddr = remoteAddr;
   }

   /**
    * Destructor.  This currently does nothing but act as a placeholder.
    */
   virtual ~SocketStreamImplSIM()
   {
      /* Do nothing. */ ;
   }

   /**
    * Listens on the socket for incoming connection requests.
    *
    * @pre The socket has been opened and bound to the address in
    *      \c mLocalAddr.
    * @post The socket is in a listening state waiting for incoming
    *       connection requests.
    *
    * @param backlog The maximum length of the queue of pending connections.
    *
    * @return vpr::ReturnStatus::Succeed is returned if this socket is now in a
    *         listening state.
    * @return vpr::ReturnStatus::Fail is returned if this socket could not be
    *         put into a listening state.  An error message is printed
    *         explaining what went wrong.
    *
    * @see open, bind
    */
   vpr::ReturnStatus listen(const int backlog = 5);

   /**
    * Accepts an incoming connection request and returns the connected socket
    * to the caller in the given socket object reference.
    *
    * @pre This socket is open, bound, and in a listening state.
    * @post When a connection is established, the given socket \p sock will
    *       be set up to communicate with the newly accepted connection.
    *
    * @param clientSock A reference to a vpr::SocketStream object that will be
    *                   used to return the newly connected socket.
    * @param timeout    The length of time to wait for the accept call to
    *                   return.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the new connection was
    *         accepted succesfully.
    * @return vpr::ReturnStatus::WouldBlock is returned if this is a
    *         non-blocking socket, and there are no waiting connection
    *         requests.
    * @return vpr::ReturnStatus::Timeout is returned when no connections
    *         requests arrived within the given timeout period.
    * @return vpr::ReturnStatus::Fail is returned if the connection was not
    *         accepted.  An error message is printed explaining what went
    *         wrong.
    *
    * @see open, bind, listen
    */
   vpr::ReturnStatus accept(SocketStreamImplSIM& clientSock,
                            vpr::Interval timeout = vpr::Interval::NoTimeout);

   /**
    * Sets the current no-delay status for this socket.  If no-delay is true,
    * then the Nagel algorithm will be disabled.  Of course, there is no
    * Nagel algorithm for sim sockets.
    *
    * @param setting The Boolean enable/disable state for no-delay on this
    *                socket.
    */
   vpr::ReturnStatus setNoDelay(bool setting)
   {
      mNoDelay = setting;
      return vpr::ReturnStatus();
   }

   /**
    * @param peerSock The socket making the connection.
    */
   vpr::ReturnStatus addConnector(vpr::SocketImplSIM* peerSock);

   /**
    * Gets the current number of connectors waiting to connect to this socket.
    */
   vpr::Uint32 getConnectorCount() const
   {
      return mConnectorQueue.size();
   }

   virtual vpr::ReturnStatus isReadReady() const;

   virtual vpr::ReturnStatus isWriteReady() const;

protected:
   /**
    * Queue of pending connection requests.This is a list of the requesting
    * connectors.
    */
   std::queue<SocketStreamImplSIM*> mConnectorQueue;
   vpr::Mutex mConnectorQueueMutex;   /**< Mutex for connector queue */

   bool mNoDelay;
};

} // End of vpr namespace


#endif  /* _VPR_SOCKET_STREAM_IMPL_SIMULATED_H_ */
