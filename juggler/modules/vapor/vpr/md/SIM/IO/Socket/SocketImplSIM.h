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

#ifndef _VPR_SOCKET_IMPL_SIMULATED_H_
#define _VPR_SOCKET_IMPL_SIMULATED_H_

#include <vpr/vprConfig.h>

#include <stdlib.h>
#include <string>
#include <queue>
#include <vector>

#include <vpr/vprTypes.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/IO/Socket/SocketOptions.h>
#include <vpr/Util/Interval.h>
#include <vpr/Util/Assert.h>

#include <vpr/md/SIM/IO/IOSysSIM.h>
#include <vpr/md/SIM/IO/Socket/InetAddrSIM.h>

#include <vpr/md/SIM/Network/Message.h>
#include <vpr/md/SIM/Network/MessagePtr.h>
#include <vpr/md/SIM/Network/NetworkGraph.h>
#include <vpr/md/SIM/Network/NetworkNode.h>


namespace vpr
{

namespace sim
{

class SocketManager;

} // End of sim namespace

/**
 * Implementation class for simulated sockets.  This can be used in the
 * vpr::Socket_t bridge template.
 *
 * @author Kevin Meinert
 */
class VPR_CLASS_API SocketImplSIM
{
public:
   /**
    * Destructor.  This will unbind the socket from its network node if it was
    * previously bound to a node.
    */
   virtual ~SocketImplSIM(void);

   // =========================================================================
   // vpr::BlockIO basics.
   // =========================================================================

   /**
    * Gets the "name" of this socket.  It is typically the address of the peer
    * host.
    *
    * @post
    *
    * @return An object containing the "name" of this socket.
    */
   const std::string& getName (void)
   {
      return mName;
   }

   /**
    * Sets the blocking flags so that the socket is opened in blocking mode.
    *
    * @pre None.
    * @post The open flags are updated so that when the socket is opened, it
    *       is opened in blocking mode.  If the socket is already open, this
    *       has no effect.
    */
   void setOpenBlocking (void)
   {
      mOpenBlocking = true;
   }

   /**
    * Sets the blocking flags so that the socket is opened in non-blocking
    * mode.
    *
    * @pre None.
    * @post The open flags are updated so that when the socket is opened, it
    *       is opened in non-blocking mode.  If the socket is already open,
    *       this has no effect.
    */
   void setOpenNonBlocking (void)
   {
      mOpenBlocking = false;
   }

   /**
    * Opens the socket.  This creates a new socket using the domain and type
    * options set through member variables.
    *
    * @pre mLocalAddr and mType have been set to values recognized by the
    *      SimSocketManager::socket() call.
    * @post The open state is set to true.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the socket was opened
    *         successfully.<br>
    *         vpr::ReturnStatus::Fail is returned if the socket could not be
    *         opened.
    */
   vpr::ReturnStatus open( void )
   {
      mOpen = true;

      return vpr::ReturnStatus();
   }

   /**
    * Gets the open state of this socket.
    *
    * @pre None.
    * @post The boolean value giving the open state is returned to the
    *       caller.
    *
    * @return true is returned if this socket is open;
    *         false otherwise.
    */
   bool isOpen() const
   {
      return mOpen;
   }

   /**
    * Closes the socket.
    *
    * @pre The socket is open.
    * @post An attempt is made to close the socket.  The resulting status is
    *       returned to the caller.  If the socket is closed, mOpen is set
    *       to false.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the socket was closed
    *         successfully.<br>
    *         vpr::ReturnStatus::Fail is returned if the socket could not be
    *         closed for some reason.
    */
   vpr::ReturnStatus close( void );

   /// Gets the handle to this socket.
   vpr::IOSysSIM::Handle getHandle ( void )
   {
      return this;
   }

   // set it.
   vpr::ReturnStatus setReuseAddr( bool adr )
   {
      mReuseAddr = adr;
      return vpr::ReturnStatus();
   }

   /**
    * Binds this socket to the address in the host address member variable.
    *
    * @pre The socket is open, and mLocalAddr has been initialized properly.
    * @post The socket is bound to the address in mLocalAddr.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the socket was bound
    *         to the address successfully.<br>
    *         vpr::ReturnStatus::Fail if the socket could not be bound to
    *         the address in mLocalAddr.
    */
   vpr::ReturnStatus bind(void);

   /**
    * Reconfigures the socket so that it is in blocking mode.
    *
    * @pre The socket is open.
    * @post Processes will block when accessing the socket.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the socket's
    *         blocking mode is set to blocking.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus enableBlocking( void )
   {
      mBlocking = true;
      return vpr::ReturnStatus(); // success
   }

   /**
    * Reconfigures the socket so that it is in non-blocking mode.
    *
    * @pre The socket is open.
    * @post Processes will not block when accessing the socket.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the socket's
    *         blocking mode is set to non-blocking.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus enableNonBlocking( void )
   {
      mBlocking = false;
      return vpr::ReturnStatus(); // success
   }

   /**
    * Gets the current blocking state for the socket.
    *
    * @return true is returned if the socket is in blocking
    *         mode.  Otherwise, false is returned.
    */
   bool getBlocking (void) const
   {
      return mBlocking;
   }

   /**
    * Gets the current non-blocking state for the socket.
    *
    * @return true is returned if the socket is in non-blocking
    *         mode.  Otherwise, false is returned.
    */
   bool getNonBlocking (void) const
   {
      return ! mBlocking;
   }

   // =========================================================================
   // vpr::Socket interface implementation.
   // =========================================================================

   /**
    * Connects the socket on the client side to the server side.  For a
    * datagram socket, this makes the address given to the constructor the
    * default destination for all packets.  For a stream socket, this has
    * the effect of establishing a connection with the destination.
    *
    * @pre The socket is open.
    * @post The socket is connected to the address in mLocalAddr.  For a
    *       stream socket, this means that a connection for future
    *       communication has been established.  For a datagram socket, the
    *       default destination for all packets is now mLocalAddr.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the connection was made
    *         and completed.<br>
    *         vpr::ReturnStatus::InProgress is returned if this is a
    *         non-blocking socket and the connection will complete in the
    *         background.<br>
    *         vpr::ReturnStatus::Fail is returned if the connect could not
    *         be made.
    */
   vpr::ReturnStatus connect( vpr::Interval timeout = vpr::Interval::NoTimeout );

   /**
    * Gets the status of a possibly connected socket.
    *
    * @pre This socket is opened and has been connected to a peer.
    *
    * @return true is returned if this socket is still connected.<br>
    *         false is returned if this socket is not currently connected.
    */
   bool isConnected (void)
   {
      return mPeer != NULL;
   }

   /**
    * Gets the type of this socket (e.g., vpr::SocketTypes::STREAM).
    *
    * @pre The socket implementation pointer is valid.
    * @post The socket type for this socket is returned to the caller.
    *
    * @return A vpr::SocketTypes::Type value giving the socket type for this
    *         socket.
    */
   const vpr::SocketTypes::Type& getType( void ) const
   {
      return mType;
   }

   const vpr::InetAddr& getLocalAddr( void ) const
   {
      return mLocalAddr;
   }

   vpr::ReturnStatus setLocalAddr( const vpr::InetAddr& addr )
   {
     vpr::ReturnStatus status;

     if ( mBound == true )
     {
        status.setCode(vpr::ReturnStatus::Fail);
     }
     else
     {
        mLocalAddr = addr;
     }

     return status;
   }

   const vpr::InetAddr& getRemoteAddr( void ) const
   {
      return mRemoteAddr;
   }

   vpr::ReturnStatus setRemoteAddr( const vpr::InetAddr& addr )
   {
      vpr::ReturnStatus status;

      // If we are connected, we cannot change our remote address.
      if (mConnected)
      {
         status.setCode(vpr::ReturnStatus::Fail);
      }
      else
      {
         mRemoteAddr = addr;
      }

      return status;
   }

   /**
    * Implementation of the read template method.  This reads at
    * most the specified number of bytes from the socket into the given
    * buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      length bytes long.
    * @post The given buffer has length bytes copied into it from the device,
    *       and the number of bytes read successfully is returned to the
    *       caller via the bytes_read parameter.
    *
    * @param buffer     A pointer to the buffer where the device's buffer
    *                   contents are to be stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    *                   available for reading.  This argument is optional and
    *                   defaults to vpr::Interval::NoTimeout
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock if the device is in non-blocking
    *         mode, and there is no data to read.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read could not
    *         begin within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the read operation
    *         ailed.
    */
   vpr::ReturnStatus read_i( void* buffer, const vpr::Uint32 length,
                             vpr::Uint32& data_read,
                             vpr::Interval timeout = vpr::Interval::NoTimeout );

   /**
    * Implementation of the readn template method.  This reads
    * exactly the specified number of bytes from the socket into the given
    * buffer.
    *
    * @pre The device is open for reading, and the buffer is at least
    *      length bytes long.
    * @post The given buffer has length bytes copied into
    *       it from the device, and the number of bytes read successfully
    *       is returned to the caller via the bytes_read
    *       parameter.
    *
    * @param buffer     A pointer to the buffer where the device's buffer
    *                   contents are to be stored.
    * @param length     The number of bytes to be read.
    * @param bytes_read The number of bytes read into the buffer.
    * @param timeout    The maximum amount of time to wait for data to be
    *                   available for reading.  This argument is optional and
    *                   defaults to vpr::Interval::NoTimeout
    *
    * @return vpr::ReturnStatus::Succeed is returned if the read operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock if the device is in non-blocking
    *         mode, and there is no data to read.<br>
    *         vpr::ReturnStatus::Timeout is returned if the read could not
    *         begin within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the read operation
    *         failed.
    */
   vpr::ReturnStatus readn_i( void* buffer, const vpr::Uint32 length,
                              vpr::Uint32& data_read,
                              vpr::Interval timeout = vpr::Interval::NoTimeout )
   {
      return read_i(buffer, length, data_read, timeout);
   }

   /**
    * Implementation of the write template method.  This writes
    * the buffer to the socket.
    *
    * @pre The device is open for writing.
    * @post The given buffer is written to the I/O device, and the number
    *       of bytes written successfully is returned to the caller via the
    *       bytes_written parameter.
    *
    * @param buffer        A pointer to the buffer to be written.
    * @param length        The length of the buffer.
    * @param bytes_written The number of bytes written to the device.
    * @param timeout       The maximum amount of time to wait for data to be
    *                      available for writing.  This argument is optional
    *                      and defaults to
    *                      vpr::Interval::NoTimeout.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the write operation
    *         completed successfully.<br>
    *         vpr::ReturnStatus::WouldBlock is returned if the handle is in
    *         non-blocking mode, and the write operation could not be
    *         completed.<br>
    *         vpr::ReturnStatus::Timeout is returned if the write could not
    *         begin within the timeout interval.<br>
    *         vpr::ReturnStatus::Fail is returned if the write operation
    *         failed.
    */
   vpr::ReturnStatus write_i( const void* buffer, const vpr::Uint32 length,
                              vpr::Uint32& data_written,
                              vpr::Interval timeout = vpr::Interval::NoTimeout );

   /**
    * Retrieves the value for the given option as set on the socket.
    *
    * @param option The option to be queried.
    * @param data   A data buffer that will be used to store the value of the
    *               given option.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the value for the given
    *         option was retrieved successfully.<br>
    *         vpr::ReturnStatus;:Fail is returned otherwise.
    */
   vpr::ReturnStatus getOption(const vpr::SocketOptions::Types option,
                               struct vpr::SocketOptions::Data& data);

   /**
    * Sets a value for the given option on the socket using the given data
    * block.
    *
    * @param option The option whose value will be set.
    * @param data   A data buffer containing the value to be used in setting
    *               the socket option.
    */
   vpr::ReturnStatus setOption(const vpr::SocketOptions::Types option,
                               const struct vpr::SocketOptions::Data& data);

   virtual vpr::ReturnStatus addConnector(const vpr::SocketImplSIM* local,
                                          vpr::SocketImplSIM** remote)
   {
      vprASSERT(false && "I shouldn't ever be called!");
      return vpr::ReturnStatus(vpr::ReturnStatus::Fail);
   }

   /**
    * Tests if this socket can read without blocking.
    *
    * @post Depending on the state of the socket, the caller is informed if
    *       a read will block or not.
    *
    * @param timeout The timeout interval to wait for this socket to be ready
    *                for reading.  This is optional, and it defaults to
    *                vpr::Interval::NoWait (poll the socket and return
    *                immediately).
    *
    * @return vpr::ReturnStatus::Succeed is returned if this socket can read
    *         without blocking.  That is, there is data waiting to be read from
    *         its arrival queue.<br>
    *         vpr::ReturnStatus::Timeout is returned if this socket did not
    *         become ready for reading within the timeout period.<br>
    *         vpr::ReturnStatus::Fail is returned if this socket is not
    *         ready for reading.  This can happen if the socket is not open,
    *         not connected, or without any received data.
    */
   virtual vpr::ReturnStatus isReadReady(const vpr::Interval timeout = vpr::Interval::NoWait) const = 0;

   /**
    * Tests if this socket can write without blocking.
    *
    * @post Depending on the state of the socket, the caller is informed if
    *       a write will succeed or not.
    *
    * @param timeout The timeout interval to wait for this socket to be ready
    *                for writing.  This is optional, and it defaults to
    *                vpr::Interval::NoWait (poll the socket and return
    *                immediately).
    *
    * @return vpr::ReturnStatus::Succeed is returned if this socket can write
    *         without blocking.  That is, this socket is in a connected state,
    *         and data can be sent to its peer right away.<br>
    *         vpr::ReturnStatus::Timeout is returned if this socket did not
    *         become ready for writing within the timeout period.<br>
    *         vpr::ReturnStatus::Fail is returned if this socket is not
    *         ready for writing.  This can happen if the socket is not open or
    *         not connected.
    */
   virtual vpr::ReturnStatus isWriteReady(const vpr::Interval timeout = vpr::Interval::NoWait) const = 0;

   /**
    * Tests if this socket is in an exceptional state.
    *
    * @post Depending on the state of the socket, the caller is informed if
    *       this socket is in an exception state or not.
    *
    * @return vpr::ReturnStatus::Succeed is returned if this socket is in an
    *         excptional state.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus inExceptState(void);

   const vpr::sim::NetworkGraph::net_vertex_t& getNetworkNode (void) const
   {
      return mNetworkNode;
   }

   void addArrivedMessage (vpr::sim::MessagePtr msg)
   {
      mArrivedQueueMutex.acquire();
      {
         mArrivedQueue.push(msg);
      }
      mArrivedQueueMutex.release();
   }

   void setPathToPeer (vpr::sim::NetworkGraph::VertexListPtr path)
   {
      mPathToPeer = path;
   }

protected:
   friend class vpr::sim::SocketManager;

   /**
    * @param node The node in the network graph to be asociated with this
    *             socket.
    */
   void setNetworkNode (const vpr::sim::NetworkGraph::net_vertex_t& node)
   {
      mNetworkNode  = node;
      mNodeAssigned = true;
   }

   void disconnect(void);

   /**
    * This just initializes member variables to reasonable defaults.
    *
    * @pre None.
    * @post The member variables are initialized accordingly to reasonable
    *       defaults.
    */
   SocketImplSIM(const vpr::SocketTypes::Type sock_type);

   /**
    * Standard constructor.  This takes two vpr::InetAddr objects, a local
    * address and a remote address.
    *
    * @pre None.
    * @post The member variables are initialized with the given values.
    *
    * @param local_addr  The local address for the socket.
    * @param remote_addr The remote address for the socket.
    * @param sock_type   The type of the socket to be created.
    */
   SocketImplSIM(const vpr::InetAddr& local_addr,
                 const vpr::InetAddr& remote_addr,
                 const vpr::SocketTypes::Type sock_type);

   std::string mName;
   bool        mOpen;
   bool        mBound;
   bool        mConnected;
   bool        mOpenBlocking;
   bool        mBlocking;

   vpr::InetAddr mLocalAddr;  /**< The local site's address structure */
   vpr::InetAddr mRemoteAddr; /**< The remote site's address structure */

   vpr::SocketTypes::Type mType;       /**< STREAM, DATAGRAM, or RAW */

   bool mReuseAddr;  /**< reuse this address? */

   vpr::SocketImplSIM* mPeer; /**< Our peer socket */

   std::queue<vpr::sim::MessagePtr> mArrivedQueue; /**< FIFO of arrived msgs */

   vpr::Mutex mArrivedQueueMutex; /**< Mutex for the arrived message queue */

   vpr::sim::NetworkGraph::net_vertex_t mNetworkNode;
   bool mNodeAssigned;

   ///
   vpr::sim::NetworkGraph::VertexListPtr mPathToPeer;
};

} // End of vpr namespace


#endif  /* _VPR_SOCKET_IMPL_SIMULATED_H_ */
