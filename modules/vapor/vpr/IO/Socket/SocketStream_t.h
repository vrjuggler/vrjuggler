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
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SOCKET_STREAM_BRIDGE_H_
#define _VPR_SOCKET_STREAM_BRIDGE_H_
// NOTE: this is the bridge class for use with SocketStream.h

#include <vpr/vprConfig.h>

#include <vpr/IO/Socket/Socket_t.h> /* base bridge class.. */

#include <boost/smart_ptr.hpp>


namespace vpr
{

/** \class SocketStream_t SocketStream_t.h vpr/IO/Socket/SocketStream_t.h
 *
 * Cross-platform stream socket class.  Given a SocketConfiguration
 * declaration, this class is typedef'd to vpr::SocketStream.
 *
 * @param SocketConfig_ The SocketConfiguration type that matches the
 *                      platform-specific socket implementation to use.
 *
 * @see vpr::SocketStreamImplNSPR, vpr::SocketStreamImplBSD
 *
 * @note vpr::SocketStreamOpt was folded into this class in version 2.1.10.
 *       User-level code will most likely not be affected by this difference.
 */
//template<class RealSocketStreamImpl, class RealSocketStreamImplParent, class IO_STATS_STRATEGY = NullIOStatsStrategy>
//class SocketStream_t : public Socket_t<RealSocketStreamImplParent, IO_STATS_STRATEGY>,
template<class SocketConfig_>
class SocketStream_t : public Socket_t<SocketConfig_>
{
public:
   typedef SocketConfig_ Config;
   typedef typename Config::SocketStreamImpl SocketStreamImpl;

public:
   /**
    * Default constructor.
    */
   SocketStream_t()
   {
      mSocketStreamImpl = boost::shared_ptr<SocketStreamImpl>( new SocketStreamImpl );
      this->mSocketImpl = mSocketStreamImpl;
   }

   /**
    * Constructor.  This takes a reference to a vpr::InetAddr object giving
    * the local socket address and a reference to a vpr::InetAddr object
    * giving the remote address.
    *
    * @pre \p localAddr and \p remoteAddr are references to valid vpr::InetAddr
    *      objects.
    * @post A socket is created using the contents of \p localAddr and
    *       \p remoteAddr.
    *
    * @param localAddr  A reference to a vpr::InetAddr object for the
    *                   local socket address.
    * @param remoteAddr A reference to a vpr::InetAddr object for the
    *                   remote socket address.
    */
   SocketStream_t(const vpr::InetAddr& localAddr,
                  const vpr::InetAddr& remoteAddr)
   {
      mSocketStreamImpl =
         boost::shared_ptr<SocketStreamImpl>(new SocketStreamImpl(localAddr,
                                                                  remoteAddr));
      this->mSocketImpl = mSocketStreamImpl;
   }

   /**
    * Copy constructor.
    *
    * @param sock The source stream socket to be copied into this object.
    */
   SocketStream_t(const SocketStream_t& sock)
      : Socket_t<SocketConfig_>()
      , mSocketStreamImpl(sock.mSocketStreamImpl)
   {
      this->mSocketImpl = mSocketStreamImpl;
   }

   /**
    * Destructor.  This currently does nothing.
    */
   virtual ~SocketStream_t()
   {
      ;  // When obj is destroyed, then member mSocketStreamImpl will be destroyed
   }

   /**
    * Puts this socket into the listening state where it listens for
    * incoming connection requests.
    *
    * @pre The socket has been opened and bound to the address in \c mAddr.
    * @post The socket is in a listening state waiting for incoming
    *       connection requests.
    *
    * @param backlog The maximum length of th queue of pending connections.
    *
    * @throw vpr::SocketException
    *           Thrown if the socket could not be put into a listening state.
    */
   void listen(const int backlog = 5)
   {
      mSocketStreamImpl->listen(backlog);
   }

   /**
    * Accepts an incoming connection request and returns the connected socket
    * to the caller in the given socket object reference.
    *
    * @pre This socket is open, bound, and in a listening state.
    * @post When a connection is established, the given socket \p sock will
    *       be set up to communicate with the newly accepted connection.
    *
    * @param sock    A reference to a vpr::SocketStream object that will
    *                be used to return the newly connected socket.
    * @param timeout The length of time to wait for the accept call to
    *                return.
    *
    * @throw vpr::WouldBlockException
    *           Thrown if this is a non-blocking socket, and there are no
    *           waiting connection requests.
    * @throw vpr::TimeoutException
    *           Thrown if no connection requests arrived within the given
    *           timeout period.
    * @throw vpr::SocketException
    *           Thrown if the connection was not accepted because of an error.
    *
    * @see open, bind, listen
    */
   void accept(SocketStream_t& sock,
               const vpr::Interval& timeout = vpr::Interval::NoTimeout)
   {
      mSocketStreamImpl->accept(*(sock.mSocketStreamImpl), timeout);
   }

   /**
    * Opens a socket to be used for the server side communications.  This is
    * provided to automate the repeated tasks executed when a server is
    * set up.  It handles opening the socket, binding the address and going
    * into listening mode.
    *
    * @pre The member variables have been initialized properly.
    * @post The socket is in the listening state waiting for incoming
    *       connection requests.
    *
    * @param reuseAddr Enable or disable reuse of the address being bound.
    *                  This argument is optional and defaults to true.
    * @param backlog   The maximum length of the pending connection queue.
    *
    * @throw vpr::WouldBlockException
    *           Thrown if this is a non-blocking socket, and there are no
    *           waiting connection requests.
    * @throw vpr::TimeoutException
    *           Thrown if no connection requests arrived within the given
    *           timeout period.
    * @throw vpr::SocketException
    *           Thrown if the connection was not accepted because of an error.
    */
   void openServer(const bool reuseAddr = true,
                   const int backlog = 5)
   {

      // First, open the socket.
      this->open();
      this->setReuseAddr(reuseAddr);
      this->bind();

      // If that succeeded, bind to the internal address.
      // Finally, if that succeeded, go into listening mode.
      this->listen(backlog);
   }

   /** @name Socket Options */
   //@{
   /**
    *
    */
   size_t getMaxSegmentSize() const
   {
      vpr::SocketOptions::Data option;

      this->getOption(vpr::SocketOptions::MaxSegment, option);
      return option.max_segment;
   }

   /**
    *
    */
   void setMaxSegmentSize(const vpr::Int32 size)
   {
      vpr::SocketOptions::Data option;
      option.max_segment = size;
      this->setOption(vpr::SocketOptions::MaxSegment, option);
   }

   /** @name Nagle Algorithm Control */
   //@{
   /**
    * Gets the current no-delay status for this socket.  If no-delay is true,
    * then the Nagle algorithm has been disabled.
    *
    * @return \c true is returned if the Nabel algorithm is \em not being used
    *         to delay the transmission of TCP segments. Otherwise, the Nagle
    *         algorithm is delaying the transmission.
    */
   bool getNoDelay() const
   {
      vpr::SocketOptions::Data option;

      this->getOption(vpr::SocketOptions::NoDelay, option);
      return option.no_delay;
   }

   /**
    * Sets the current no-delay status for this socket.  If no-delay is true,
    * then the Nagle algorithm will be disabled.
    *
    * @param enableVal The Boolean enable/disable state for no-delay on this
    *                  socket.
    */
   void setNoDelay(const bool enableVal)
   {
      vpr::SocketOptions::Data option;
      option.no_delay = enableVal;
      this->setOption(vpr::SocketOptions::NoDelay, option);
   }
   //@}

   /** @name TCP Corking Interface */
   //@{
   /**
    * Gets the state of the "no push" or "corking" for this socket. While a
    * TCP socket is in the corked state, write operations are queued for
    * later transmission when the socket is uncorked (that is, when the
    * no-push state is disabled).
    *
    * @return true is returned if this socket is currently in the no-push
    *         (corked) state. false is returned otherwise.
    *
    * @since 2.1.9
    */
   bool getNoPush() const
   {
#if defined(HAVE_CORKABLE_TCP)
      vpr::SocketOptions::Data option;

      this->getOption(vpr::SocketOptions::NoPush, option);
      return option.no_push;
#else
      return mCorked;
#endif
   }

   /**
    * Enables or disables the no-push (or "corked") state.
    *
    * @post If this socket was previously in the corked state and the value of
    *       \p enableVal is false, then all queued buffer writes will be
    *       performed.
    *
    * @param enableVal A boolean value indicating whether this socket should
    *                  be in the no-push (corked) state.
    *
    * @since 2.1.9
    */
   void setNoPush(const bool enableVal)
   {
// NSPR sockets are not corkable, but the OS may still support TCP corking.
#if defined(HAVE_CORKABLE_TCP) && VPR_IO_DOMAIN_INCLUDE != VPR_DOMAIN_NSPR
      vpr::SocketOptions::Data option;
      option.no_push = enableVal;
      this->setOption(vpr::SocketOptions::NoPush, option);
#else
      if ( enableVal != mCorked )
      {
         // Changing from the uncorked state to the corked state.
         if ( enableVal )
         {
            mSocketStreamImpl->cork();
         }
         // Changing from the corked state to the uncorked state.
         else
         {
            mSocketStreamImpl->uncork();
         }
      }
#endif

      mCorked = enableVal;
   }

   /**
    * Changes the strategy used for determining hwo much memory to allocate
    * for the corking buffer.
    *
    * @param strategy A callable object used to determine how much memory to
    *                 allocate when the corking buffer needs to grow.
    *
    * @note This only has bearing on platforms (such as Windows) that do not
    *       implement the no-push ("corking") option on TCP sockets.
    *
    * @since 2.1.9
    */
   void setCorkAllocStrategy(const NoPushAllocStrategy& strategy)
   {
      mSocketStreamImpl->setCorkAllocStrategy(strategy);
   }
   //@}

   //@}

protected:
   /**
    * Constructor.  Create a vpr::SocketStream object using the given
    * SocketStreamImpl object pointer.  This is needed by accept().
    *
    * @pre \p sockImp points to a valid SocketStreamImpl object.
    * @post \p sockImp is copied into mSocketStreamImpl.
    *
    * @param sockImp A pointer to a SocketStreamImpl object.
    */
   SocketStream_t(SocketStreamImpl* sockImp)
      : Socket_t<Config>()
      , mSocketStreamImpl(sockImp)
   {
      this->mSocketImpl = mSocketStreamImpl;
   }

   /// Platform-specific stream socket implementation
   boost::shared_ptr<SocketStreamImpl> mSocketStreamImpl;

private:
   /**
    * @name TCP Corking Interface Implementation
    *
    * More about TCP corking can be found here: http://www.baus.net/on-tcp_cork
    */
   //@{
   bool mCorked;
   //@}
};

} // End of vpr namespace


#endif // _VPR_SOCKET_STREAM_BRIDGE_H_
