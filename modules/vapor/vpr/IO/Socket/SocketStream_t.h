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
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _VPR_SOCKET_STREAM_BRIDGE_H_
#define _VPR_SOCKET_STREAM_BRIDGE_H_
// NOTE: this is the bridge class for use with SocketStream.h

#include <vpr/vprConfig.h>

#include <vpr/IO/Socket/SocketStreamOpt.h>
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
 */
//template<class RealSocketStreamImpl, class RealSocketStreamImplParent, class IO_STATS_STRATEGY = NullIOStatsStrategy>
//class SocketStream_t : public Socket_t<RealSocketStreamImplParent, IO_STATS_STRATEGY>,
template<class SocketConfig_>
class SocketStream_t : public Socket_t<SocketConfig_>, public SocketStreamOpt
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
   SocketStream_t(vpr::InetAddr localAddr, vpr::InetAddr remoteAddr)
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
      , SocketStreamOpt()
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
               const vpr::Interval timeout = vpr::Interval::NoTimeout)
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

   /**
    * @throw vpr::SocketException
    *           Thrown if querying the indiccated option on this socket fails.
    */
   virtual void getOption(const vpr::SocketOptions::Types option,
                          struct vpr::SocketOptions::Data& data)
      const
   {
      mSocketStreamImpl->getOption(option, data);
   }

   /**
    * @throw vpr::SocketException
    *           Thrown if setting the indicated option on this socket fails.
    */
   virtual void setOption(const vpr::SocketOptions::Types option,
                          const struct vpr::SocketOptions::Data& data)
   {
      mSocketStreamImpl->setOption(option, data);
   }

// Put in back door for simulator
#if VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_SIMULATOR
public:
#endif
   /// Platform-specific stream socket implementation
   boost::shared_ptr<SocketStreamImpl> mSocketStreamImpl;
};

} // End of vpr namespace


#endif // _VPR_SOCKET_STREAM_BRIDGE_H_
