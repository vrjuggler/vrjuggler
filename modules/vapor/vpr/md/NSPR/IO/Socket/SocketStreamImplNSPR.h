/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#include <vpr/IO/Socket/NoPushWriter.h>
#include <vpr/md/NSPR/IO/Socket/SocketImplNSPR.h>


namespace vpr
{

/** \class SocketStreamImplNSPR SocketStreamImplNSPR.h vpr/IO/Socket/SocketStream.h
 *
 * NSPR implementation of the stream-oriented socket interface.  This is used
 * in conjunction with vpr::SocketConfiguration to create the typedef
 * vpr::SocketStream.
 *
 * @see vpr::NoPushWriter
 */
class VPR_CLASS_API SocketStreamImplNSPR : public SocketImplNSPR
{
public:
   // ========================================================================
   // vpr::SocketStreamImp implementation.
   // ========================================================================

   /**
    * Default constructor.  This initializes the member variables.
    */
   SocketStreamImplNSPR();

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
   SocketStreamImplNSPR(const vpr::InetAddr& localAddr,
                        const vpr::InetAddr& remoteAddr);

   /**
    * Copy constructor.
    */
   SocketStreamImplNSPR(const SocketStreamImplNSPR& sock);

   virtual ~SocketStreamImplNSPR();

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
    * @throw vpr::SocketException If the socket could not be put into a
    *                             listening state.
    *
    * @see open, bind
    */
   void listen(const int backlog = 5);

   /**
    * Accepts an incoming connection request and returns the connected socket
    * to the caller in the given socket object reference.
    *
    * @pre This socket is open, bound, and in a listening state.
    * @post When a connection is established, the given socket \p sock will
    *       be set up to communicate with the newly accepted connection.
    *
    * @param sock    A reference to a vpr::SocketStream object that will be
    *                used to return the newly connected socket.
    * @param timeout The length of time to wait for the accept call to return.
    *
    * @throw vpr::WouldBlockException
    *           If this is a non-blocking socket, and there are no waiting
    *           connection requests.
    * @throw vpr::TimeoutException
    *           If no connection requests arrived within the given timeout
    *           period.
    * @throw vpr::SocketException
    *           If the connection was not accepted because of an error.
    *
    * @see open, bind, listen
    */
   void accept(SocketStreamImplNSPR& sock,
               const vpr::Interval& timeout = vpr::Interval::NoTimeout);

   /**
    * Implementation of the write() template method. This writes the buffer
    * to the socket. If currently in the no-push (corked) state, then the
    * write is delayed until the state is changed back to the regular
    * uncorked state.
    *
    * @pre The socket is open for writing.
    * @post If not corked, the given buffer is written to the I/O socket. If
    *       corked, the buffer will be queued for later transmission. The
    *       number of bytes written successfully is returned to the caller.
    *
    * @param buffer  A pointer to the buffer to be written.
    * @param length  The length of the buffer.
    * @param timeout The maximum amount of time to wait for data to be
    *                available for writing.
    *
    * @return The number of bytes written to the socket is returned.
    *
    * @throw vpr::ConnectionResetException
    *           Thrown if connection is reset.
    * @throw vpr::NoRouteToHostException
    *           Thrown if a route to host does not exist.
    * @throw vpr::UnknownHostException
    *           Thrown if host does not exist.
    * @throw vpr::IOException
    *           Thrown if the network is down.
    * @throw vpr::WouldBlockException
    *           Thrown if the handle is in non-blocking mode, and the write
    *           operation could not be completed.
    * @throw vpr::TimeoutException
    *           Thrown if the write could not begin within the timeout
    *           interval.
    * @throw vpr::SocketException
    *           Thrown if the write operation failed.
    * @throw vpr::IOException
    *           Thrown if the file handle write operation failed.
    * @throw vpr::Exception
    *           Thrown if corking is enabled and, in the event that resizing
    *           the corking buffer is necessary, the new buffer allocation
    *           size returned by the allocation strategy is insufficiently
    *           large.
    * @throw std::bad_alloc
    *
    * @see cork()
    * @see vpr::NoPushWriter::write()
    *
    * @since 2.1.9
    */
   vpr::Uint32 write_i(const void* buffer, const vpr::Uint32 length,
                       const vpr::Interval& timeout);

   /**
    * @name TCP Corking Interface
    *
    * @note These methods are not meant to be called by user-level code and
    *       may at some point be changed to have private visiblity. Since this
    *       class itself is not supposed to be used directly be user-level
    *       code, this is not seen as a high priority change, however.
    */
   //@{
   /**
    * Corks this socket for platforms that lack support for TCP corking as a
    * socket option. Until this socket is uncorked, all write/send operations
    * will queue up the data instead of putting it on the wire. On all other
    * platforms, this method does nothing.
    *
    * @post \c mCorked is set to true.
    *
    * @since 2.1.9
    */
   void cork();

   /**
    * Uncorks this socket and writes all pending data to the wire.
    *
    * @post \c mCorked is set to false and \c mCorkedWriter is flushed.
    *
    * @since 2.1.9
    */
   void uncork();

   /**
    * Changes the strategy used for determining hwo much memory to allocate
    * for the corking buffer.
    *
    * @param strategy A callable object used to determine how much memory to
    *                 allocate when the corking buffer needs to grow.
    *
    * @since 2.1.9
    */
   void setCorkAllocStrategy(const NoPushAllocStrategy& strategy)
   {
      mCorkedWriter.setAllocStrategy(strategy);
   }
   //@}

private:
   bool mCorked;        /**< "Corked" state for the internal TCP socket */

   NoPushWriter mCorkedWriter;
};

} // End of vpr namespace


#endif   /* _VJ_SOCKET_STREAM_IMPL_NSPR_H_ */
