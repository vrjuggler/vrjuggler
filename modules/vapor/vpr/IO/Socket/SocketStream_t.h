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

#ifndef _VPR_SOCKET_STREAM_BRIDGE_H_
#define _VPR_SOCKET_STREAM_BRIDGE_H_
// NOTE: this is the bridge class for use with SocketStream.h

#include <vpr/vprConfig.h>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/IO/Socket/SocketStreamOpt.h>
#include <vpr/IO/Socket/Socket_t.h> /* base bridge class.. */

#include <boost/smart_ptr.hpp>


namespace vpr
{

/**
 * Cross-platform stream socket class.
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
    * @pre addr is a reference to a valid vpr::InetAddr object.
    * @post A socket is created using the contents of addr.
    *
    * @param local_addr  A reference to a vpr::InetAddr object for the
    *                     local socket address.
    * @param remote_addr A reference to a vpr::InetAddr object for the
    *                     remote socket address.
    */
   SocketStream_t(vpr::InetAddr local_addr, vpr::InetAddr remote_addr)
   {
      mSocketStreamImpl = boost::shared_ptr<SocketStreamImpl>(new SocketStreamImpl(local_addr, remote_addr));
      this->mSocketImpl = mSocketStreamImpl;
   }

   /**
    * Copy constructor.
    *
    * @param sock The source stream socket to be copied into this object.
    */
   SocketStream_t(const SocketStream_t& sock)
      : Socket_t<SocketConfig_>(), SocketStreamOpt(), mSocketStreamImpl(sock.mSocketStreamImpl)
   {
      this->mSocketImpl = mSocketStreamImpl;
   }

   /**
    * Destructor.  This currently does nothing.
    *
    * @pre None.
    * @post Delete impl
    */
   virtual ~SocketStream_t()
   {
      ;  // When obj is destroyed, then member mSocketStreamImpl will be destroyed
   }

   /**
    * Puts this socket into the listening state where it listens for
    * incoming connection requests.
    *
    * @pre The socket has been opened and bound to the address in mAddr.
    * @post The socket is in a listening state waiting for incoming
    *       connection requests.
    *
    * @param backlog The maximum length of th queue of pending connections.
    *
    * @return vpr::ReturnStatus::Succeed is returned if this socket is now in a
    *         listening state.<br>
    *         vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus listen (const int backlog = 5)
   {
      return mSocketStreamImpl->listen(backlog);
   }

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
    * @return vpr::ReturnStatus::Succeed is returned if the incoming request has
    *         been handled, and the given SocketStream object is a valid,
    *         connected socket.<br>
    *         vpr::ReturnStatus::WouldBlock is returned if this is a non-blocking
    *         socket, and there are no waiting connection requests.<br>
    *         vpr::ReturnStatus::Timeout is returned when no connections requests
    *         arrived within the given timeout period.<br>
    *         vpr::ReturnStatus::Fail is returned if the accept failed.  The
    *         given vpr::SocketStream object is not modified in this case.
    */
   vpr::ReturnStatus accept (SocketStream_t& sock,
                             const vpr::Interval timeout = vpr::Interval::NoTimeout)
   {
      return mSocketStreamImpl->accept(*(sock.mSocketStreamImpl), timeout);
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
    * @param reuse_addr Enable or disable reuse of the address being bound.
    *                   This argument is optional and defaults to true.
    * @param backlog    The maximum length of the pending connection queue.
    *
    * @return <code>vpr::ReturnStatus::Succeed</code> is returned if the server
    *         socket is in the listening state and ready to accept incoming
    *         connection requests.<br>
    *         <code>vpr::ReturnStatus::Fail</code> is returned if the server
    *         socket could not be set up.
    */
   vpr::ReturnStatus openServer (const bool reuse_addr = true,
                                 const int backlog = 5)
   {
      vpr::ReturnStatus status;

      // First, open the socket.
      status = open();

      if ( status.success() )
      {
         status = setReuseAddr(reuse_addr);

         if ( status.success() )
         {
            status = bind();

            // If that succeeded, bind to the internal address.
            if ( status.success() )
            {
               // Finally, if that succeeded, go into listening mode.
               status = listen(backlog);
            }
         }
      }

      return status;
   }

protected:
   /**
    * Constructor.  Create a vpr::SocketStream object using the given
    * vpr::SocketStreamImpl object pointer.  This is needed by accept().
    *
    * @pre sock_imp points to a valid vpr::SocketStreamImpl object.
    * @post sock_imp is copied into mSocketStreamImpl.
    *
    * @param sock_imp A pointer to a vpr::SocketStreamImpl object.
    */
   SocketStream_t (SocketStreamImpl* sock_imp)
      : Socket_t<Config>(), mSocketStreamImpl(sock_imp)
   {
      this->mSocketImpl = mSocketStreamImpl;
   }

   virtual vpr::ReturnStatus getOption (const vpr::SocketOptions::Types option,
                                        struct vpr::SocketOptions::Data& data)
   {
      return mSocketStreamImpl->getOption(option, data);
   }

   virtual vpr::ReturnStatus setOption (const vpr::SocketOptions::Types option,
                                        const struct vpr::SocketOptions::Data& data)
   {
      return mSocketStreamImpl->setOption(option, data);
   }

// Put in back door for simulator
#if VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_SIMULATOR
public:
#endif
   /// Platform-specific stream socket implementation
   //SocketStreamImpl mSocketStreamImpl;
   boost::shared_ptr<SocketStreamImpl> mSocketStreamImpl;
};

} // End of vpr namespace


#endif // _VPR_SOCKET_STREAM_BRIDGE_H_
