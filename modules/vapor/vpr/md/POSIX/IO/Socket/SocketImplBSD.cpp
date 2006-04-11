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

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <errno.h>

#include <vpr/md/POSIX/IO/Socket/SocketImplBSD.h>
#include <vpr/Util/Debug.h>


namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Open the socket.  This creates a new socket using the domain and type
// options set through member variables.
// ----------------------------------------------------------------------------
vpr::ReturnStatus SocketImplBSD::open ()
{
   int domain, type, sock;
   vpr::ReturnStatus retval;

   switch ( mLocalAddr.getFamily() )
   {
      case SocketTypes::LOCAL:
#ifdef PF_LOCAL
         domain = PF_LOCAL;
#else
         domain = PF_UNIX;
#endif
         break;
      case SocketTypes::INET:
         domain = PF_INET;
         break;
      case SocketTypes::INET6:
#ifdef PF_INET6
         domain = PF_INET6;
#else
         fprintf(stderr,
                 "[vpr::SocketImplBSD] WARNING: IPv6 not supported on this host!\n");
#endif
         break;
      case SocketTypes::LINK:
#if defined(PF_LINK)
         domain = PF_LINK;
#elif defined(PF_RAW)
         domain = PF_RAW;
#else
         fprintf(stderr,
                 "[vpr::SocketImplBSD] WARNING: Cannot use LINK sockets. domain %d\n",
                 mLocalAddr.getFamily());
#endif
         break;
      default:
         domain = -1;
         fprintf(stderr,
                 "[vpr::SocketImplBSD] ERROR: Unknown socket domain value %d\n",
                 mLocalAddr.getFamily());
         break;
   }

   switch ( mType )
   {
      case SocketTypes::STREAM:
         type = SOCK_STREAM;
         break;
      case SocketTypes::DATAGRAM:
         type = SOCK_DGRAM;
         break;
      case SocketTypes::RAW:
         type = SOCK_RAW;
         break;
      default:
         type = -1;
         fprintf(stderr,
                 "[vpr::SocketImplBSD] ERROR: Unknown socket type value %d\n",
                 mLocalAddr.getFamily());
         break;
   }

   // Attempt to create a new socket using the values in mLocalAddr and
   // mType.
   sock = socket(domain, type, 0);

   // If socket(2) failed, print an error message and return error status.
   if ( sock == -1 )
   {
      fprintf(stderr,
              "[vpr::SocketImplBSD] Could not create socket (%s): %s\n",
              getName().c_str(), strerror(errno));
      retval.setCode(vpr::ReturnStatus::Fail);
   }
   // Otherwise, return success.
   else
   {
      if ( mHandle == NULL )
      {
         mHandle = new FileHandleImplUNIX(mRemoteAddr.getAddressString());
      }

      mHandle->mFdesc = sock;
      mHandle->mOpen  = true;
      mOpen           = true;

      // Since socket(2) cannot open a socket in non-blocking mode, we call
      // enableNonBlocking() now if the socket is to be opened in
      // non-blocking mode.
      if ( ! mOpenBlocking )
      {
         retval = enableNonBlocking();
      }
   }

   return retval;
}

// ----------------------------------------------------------------------------
// Reconfigures the socket so that it is in blocking mode.
// ----------------------------------------------------------------------------
vpr::ReturnStatus SocketImplBSD::enableBlocking ()
{
   vpr::ReturnStatus status;

   vprASSERT(isOpen() && "precondition says you must open() the socket first");
   vprASSERT(! mBlockingFixed &&
             "Cannot enable blocking after a blocking call!");

   status = mHandle->enableBlocking();

   return status;
}

// ----------------------------------------------------------------------------
// Reconfigures the socket so that it is in non-blocking mode.
// ----------------------------------------------------------------------------
vpr::ReturnStatus SocketImplBSD::enableNonBlocking ()
{
   vpr::ReturnStatus status;

   vprASSERT(isOpen() && "precondition says you must open() the socket first");
   vprASSERT(! mBlockingFixed &&
             "Cannot disable blocking after a blocking call!");

   status = mHandle->enableNonBlocking();

   return status;
}

// ----------------------------------------------------------------------------
// Bind this socket to the address in the host address member variable.
// ----------------------------------------------------------------------------
vpr::ReturnStatus SocketImplBSD::bind ()
{
   vpr::ReturnStatus retval;
   int status;

   // Bind the socket to the address in mLocalAddr.
   status = ::bind(mHandle->mFdesc, (struct sockaddr*) &mLocalAddr.mAddr,
                   mLocalAddr.size());

   // If that fails, print an error and return error status.
   if ( status == -1 )
   {
      fprintf(stderr,
              "[vpr::SocketImplBSD] Cannot bind socket to address: %s\n",
              strerror(errno));
      retval.setCode(vpr::ReturnStatus::Fail);
   }
   else
   {
      mBound = true;
   }

   return retval;
}

// ----------------------------------------------------------------------------
// Connect the socket on the client side to the server side.  For a datagram
// socket, this makes the address given to the constructor the default
// destination for all packets.  For a stream socket, this has the effect of
// establishing a connection with the destination.
// ----------------------------------------------------------------------------
vpr::ReturnStatus SocketImplBSD::connect (vpr::Interval timeout)
{
   vpr::ReturnStatus retval;
   int status;

   // Attempt to connect to the address in mAddr.
   status = ::connect(mHandle->mFdesc,
                      (struct sockaddr*) &mRemoteAddr.mAddr,
                      mRemoteAddr.size());

   // If connect(2) failed, print an error message explaining why and return
   // error status.
   if ( status == -1 )
   {
      // If this is a non-blocking connection, return
      // vpr::ReturnStatus::InProgress to indicate that the connection will
      // complete later.  I'm not sure if it's safe to set mConnected and
      // mBlockingFixed at this point, but they have to be set sometime.
      if ( errno == EINPROGRESS && getNonBlocking() )
      {
         if ( vpr::Interval::NoWait == timeout )
         {
            retval.setCode(vpr::ReturnStatus::InProgress);
         }
         // If we have a timeout value, wait for at most the duration of
         // that interval.  If we time out, tell the caller that the
         // connection is still in progress.
         else if ( ! mHandle->isWriteable(timeout).success() )
         {
            retval.setCode(vpr::ReturnStatus::InProgress);
         }

         mBound          = true;
         mConnected      = true;
         mBlockingFixed = true;
      }
      else
      {
//         fprintf(stderr, "[vpr::SocketImplBSD] Error connecting to %s: %s\n",
//                 mRemoteAddr.getAddressString().c_str(), strerror(errno));
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "[vpr::SocketImplBSD] Error connecting to "
            << mRemoteAddr.getAddressString() << ": " << strerror(errno) << std::endl << vprDEBUG_FLUSH;
         retval.setCode(vpr::ReturnStatus::Fail);
      }
   }
   // Otherwise, return success.
   else
   {
      mBound          = true;
      mConnected      = true;
      mBlockingFixed = true;
   }

   // Fill in the local address if has not already been assigned.
   if ( mConnected && vpr::InetAddr::AnyAddr == mLocalAddr )
   {
      int status;
#if defined(VPR_OS_IRIX) || defined(VPR_OS_HPUX)
      int namelen;
#else
      socklen_t namelen;
#endif
      struct sockaddr temp_addr;

      namelen = sizeof(struct sockaddr);
      status  = getsockname(mHandle->mFdesc, &temp_addr, &namelen);

      if ( status == 0 )
      {
         mLocalAddr.setSockaddr(&temp_addr);
/* XXX: This doesn't compile on IRIX, and I don't know why.
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Connected, local address is "
                                          << mLocalAddr << std::endl
                                          << vprDEBUG_FLUSH;
*/
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Failed to get sock name: "
                                                  << strerror(errno)
                                                  << std::endl
                                                  << vprDEBUG_FLUSH;
      }
   }

   return retval;
}

bool SocketImplBSD::isConnected ()
{
   bool retval;

   retval = false;

   if ( mOpen && mConnected )
   {
      vpr::ReturnStatus status;
      vpr::Int32 bytes;

      status = mHandle->getReadBufferSize(bytes);

      if ( status.success() )
      {
         if ( bytes == 0 )
         {
            // If there are no bytes to read but the OS tells us that the
            // socket is readable, then something is wrong.
            if ( ! mHandle->isReadable(vpr::Interval::NoWait).success() )
            {
               retval = true;
            }
         }
         else
         {
            retval = true;
         }
      }
   }

   return retval;
}

vpr::ReturnStatus SocketImplBSD::setLocalAddr (const InetAddr& addr)
{
   vpr::ReturnStatus status;

   if ( mBound || mConnected )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "SocketImplBSD::setLocalAddr: Can't set address of a "
         << "bound or connected socket.\n" << vprDEBUG_FLUSH;
      status.setCode(vpr::ReturnStatus::Fail);
   }
   else
   {
      mLocalAddr = addr;
   }

   return status;
}

vpr::ReturnStatus SocketImplBSD::setRemoteAddr (const InetAddr& addr)
{
   vpr::ReturnStatus status;

   if ( mConnected )
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }
   else
   {
      mRemoteAddr = addr;
   }

   return status;
}

vpr::ReturnStatus SocketImplBSD::read_i (void* buffer,
                                         const vpr::Uint32 length,
                                         vpr::Uint32& bytes_read,
                                         const vpr::Interval timeout)
{
   vpr::ReturnStatus status;
   mBlockingFixed = true;
   status = mHandle->read_i(buffer, length, bytes_read, timeout);

   if ( bytes_read == 0 )
   {
      status.setCode(vpr::ReturnStatus::NotConnected);
   }

   return status;
}

vpr::ReturnStatus SocketImplBSD::readn_i (void* buffer,
                                          const vpr::Uint32 length,
                                          vpr::Uint32& bytes_read,
                                          const vpr::Interval timeout)
{
   vpr::ReturnStatus status;
   mBlockingFixed = true;
   status = mHandle->readn_i(buffer, length, bytes_read, timeout);

   if ( bytes_read == 0 )
   {
      status.setCode(vpr::ReturnStatus::NotConnected);
   }

   return status;
}

vpr::ReturnStatus SocketImplBSD::write_i (const void* buffer,
                                          const vpr::Uint32 length,
                                          vpr::Uint32& bytes_written,
                                          const vpr::Interval timeout)
{
   vpr::ReturnStatus status;
   mBlockingFixed = true;
   status = mHandle->write_i(buffer, length, bytes_written, timeout);

   if ( status.failure() )
   {
      if ( ECONNRESET == errno || EHOSTUNREACH || EHOSTDOWN || ENETDOWN )
      {
         status.setCode(vpr::ReturnStatus::NotConnected);
      }
   }

   return status;
}

/**
 * Define a simple union used as the optval argument to [gs]etsockopt(2).
 */
union sockopt_data
{
   Int32          enabled;
   size_t         size;
   struct linger  linger_val;
   struct ip_mreq mcast_req;
   struct in_addr mcast_if;
   Uint8          mcast_ttl;
   Uint8          mcast_loop;
};

/**
 *
 */
vpr::ReturnStatus SocketImplBSD::getOption (const vpr::SocketOptions::Types option,
                                            struct vpr::SocketOptions::Data& data)
{
   int opt_name, opt_level, status;
   vpr::ReturnStatus retval;
#if defined(VPR_OS_IRIX) || defined(VPR_OS_HPUX)
   int opt_size;
#else
   socklen_t opt_size;
#endif
   union sockopt_data opt_data;

   opt_name = opt_level = -1;

   switch ( option )
   {
      // Socket-level options.
      case vpr::SocketOptions::Linger:
         opt_level = SOL_SOCKET;
         opt_name  = SO_LINGER;
         opt_size  = sizeof(opt_data.linger_val);
         break;
      case vpr::SocketOptions::ReuseAddr:
         opt_level = SOL_SOCKET;
         opt_name  = SO_REUSEADDR;
         opt_size  = sizeof(opt_data.enabled);
         break;
      case vpr::SocketOptions::KeepAlive:
         opt_level = SOL_SOCKET;
         opt_name  = SO_KEEPALIVE;
         opt_size  = sizeof(opt_data.enabled);
         break;
      case vpr::SocketOptions::RecvBufferSize:
         opt_level = SOL_SOCKET;
         opt_name  = SO_RCVBUF;
         opt_size  = sizeof(opt_data.size);
         break;
      case vpr::SocketOptions::SendBufferSize:
         opt_level = SOL_SOCKET;
         opt_name  = SO_SNDBUF;
         opt_size  = sizeof(opt_data.size);
         break;

      // IP-level options.
      case vpr::SocketOptions::IpTimeToLive:
         opt_level = IPPROTO_IP;
         opt_name  = IP_TTL;
         opt_size  = sizeof(opt_data.size);
         break;
      case vpr::SocketOptions::IpTypeOfService:
         opt_level = IPPROTO_IP;
         opt_name  = IP_TOS;
         opt_size  = sizeof(opt_data.size);
         break;
      case vpr::SocketOptions::AddMember:
         opt_level = IPPROTO_IP;
         opt_name  = IP_ADD_MEMBERSHIP;
         opt_size  = sizeof(opt_data.mcast_req);
         break;
      case vpr::SocketOptions::DropMember:
         opt_level = IPPROTO_IP;
         opt_name  = IP_DROP_MEMBERSHIP;
         opt_size  = sizeof(opt_data.mcast_req);
         break;
      case vpr::SocketOptions::McastInterface:
         opt_level = IPPROTO_IP;
         opt_name  = IP_MULTICAST_IF;
         opt_size  = sizeof(opt_data.mcast_if);
         break;
      case vpr::SocketOptions::McastTimeToLive:
         opt_level = IPPROTO_IP;
         opt_name  = IP_MULTICAST_TTL;
         opt_size  = sizeof(opt_data.mcast_ttl);
         break;
      case vpr::SocketOptions::McastLoopback:
         opt_level = IPPROTO_IP;
         opt_name  = IP_MULTICAST_LOOP;
         opt_size  = sizeof(opt_data.mcast_loop);
         break;

      // TCP-level options.
      case vpr::SocketOptions::NoDelay:
         opt_level = IPPROTO_TCP;
         opt_name  = TCP_NODELAY;
         opt_size  = sizeof(opt_data.enabled);
         break;
      case vpr::SocketOptions::MaxSegment:
         opt_level = IPPROTO_TCP;
         opt_name  = TCP_MAXSEG;
         opt_size  = sizeof(opt_data.size);
         break;
   }

   status = getsockopt(mHandle->mFdesc, opt_level, opt_name,
                       (void*) &opt_data, &opt_size);

   if ( status == 0 )
   {
      // This extracts the information from the union passed to getsockopt(2)
      // and puts it in our friendly vpr::SocketOptions::Data object.  This
      // code depends on the type of that object being a union!
      switch ( option )
      {
         case vpr::SocketOptions::Linger:
            data.linger.enabled = (opt_data.linger_val.l_onoff != 0 ? true
                                                                    : false);
            data.linger.seconds = opt_data.linger_val.l_linger;
            break;
         case vpr::SocketOptions::ReuseAddr:
            data.reuse_addr = (opt_data.enabled != 0 ? true : false);
            break;
         case vpr::SocketOptions::KeepAlive:
            data.keep_alive = (opt_data.enabled != 0 ? true : false);
            break;
         case vpr::SocketOptions::RecvBufferSize:
            data.recv_buffer_size = opt_data.size;
            break;
         case vpr::SocketOptions::SendBufferSize:
            data.send_buffer_size = opt_data.size;
            break;
         case vpr::SocketOptions::IpTimeToLive:
            data.ip_ttl = opt_data.size;
            break;
         case vpr::SocketOptions::IpTypeOfService:
            switch ( opt_data.size )
            {
               case IPTOS_LOWDELAY:
                  data.type_of_service = vpr::SocketOptions::LowDelay;
                  break;
               case IPTOS_THROUGHPUT:
                  data.type_of_service = vpr::SocketOptions::Throughput;
                  break;
               case IPTOS_RELIABILITY:
                  data.type_of_service = vpr::SocketOptions::Reliability;
                  break;
#ifdef IPTOS_LOWCOST
               case IPTOS_LOWCOST:
                  data.type_of_service = vpr::SocketOptions::LowCost;
                  break;
#endif
            }

            break;
         case vpr::SocketOptions::McastInterface:
            data.mcast_if = InetAddr();
            data.mcast_if.setAddress(opt_data.mcast_if.s_addr, 0);
            break;
         case vpr::SocketOptions::McastTimeToLive:
            data.mcast_ttl = opt_data.mcast_ttl;
            break;
         case vpr::SocketOptions::McastLoopback:
            data.mcast_loopback = opt_data.mcast_loop;
            break;
         case vpr::SocketOptions::NoDelay:
            data.no_delay = (opt_data.enabled != 0 ? true : false);
            break;
         case vpr::SocketOptions::MaxSegment:
            data.max_segment = opt_data.size;
            break;
      }
   }
   else
   {
      retval.setCode(vpr::ReturnStatus::Fail);
      fprintf(stderr,
              "[vpr::SocketImplBSD] ERROR: Could not get socket option for socket %s: %s\n",
              mHandle->getName().c_str(), strerror(errno));
   }

   return retval;
}

/**
 *
 */
vpr::ReturnStatus SocketImplBSD::setOption (const vpr::SocketOptions::Types option,
                                            const struct vpr::SocketOptions::Data& data)
{
   int opt_name, opt_level;
#if defined(VPR_OS_IRIX) || defined(VPR_OS_HPUX)
   int opt_size;
#else
   socklen_t opt_size;
#endif
   union sockopt_data opt_data;
   vpr::ReturnStatus retval;

   opt_name = opt_level = -1;
   opt_size = 0;

   switch ( option )
   {
      // Socket-level options.
      case vpr::SocketOptions::Linger:
         opt_level                    = SOL_SOCKET;
         opt_name                     = SO_LINGER;
         opt_data.linger_val.l_onoff  = (data.linger.enabled ? 1 : 0);
         opt_data.linger_val.l_linger = data.linger.seconds;
         opt_size                     = sizeof(struct linger);
         break;
      case vpr::SocketOptions::ReuseAddr:
         opt_level        = SOL_SOCKET;
         opt_name         = SO_REUSEADDR;
         opt_data.enabled = (data.reuse_addr ? 1 : 0);
         opt_size         = sizeof(int);
         break;
      case vpr::SocketOptions::KeepAlive:
         opt_level        = SOL_SOCKET;
         opt_name         = SO_KEEPALIVE;
         opt_data.enabled = (data.keep_alive ? 1 : 0);
         opt_size         = sizeof(int);
         break;
      case vpr::SocketOptions::RecvBufferSize:
         opt_name      = SO_RCVBUF;
         opt_level     = SOL_SOCKET;
         opt_data.size = data.recv_buffer_size;
         opt_size      = sizeof(size_t);
         break;
      case vpr::SocketOptions::SendBufferSize:
         opt_level     = SOL_SOCKET;
         opt_name      = SO_SNDBUF;
         opt_data.size = data.send_buffer_size;
         opt_size      = sizeof(size_t);
         break;

      // IP-level options.
      case vpr::SocketOptions::IpTimeToLive:
         opt_level     = IPPROTO_IP;
         opt_name      = IP_TTL;
         opt_data.size = data.ip_ttl;
         opt_size      = sizeof(size_t);
         break;
      case vpr::SocketOptions::IpTypeOfService:
         opt_level     = IPPROTO_IP;
         opt_name      = IP_TOS;

         switch ( data.type_of_service )
         {
            case vpr::SocketOptions::LowDelay:
               opt_data.size = IPTOS_LOWDELAY;
               break;
            case vpr::SocketOptions::Throughput:
               opt_data.size = IPTOS_THROUGHPUT;
               break;
            case vpr::SocketOptions::Reliability:
               opt_data.size = IPTOS_RELIABILITY;
               break;
            case vpr::SocketOptions::LowCost:
#ifdef IPTOS_LOWCOST
               opt_data.size = IPTOS_LOWCOST;
#else
               fprintf(stderr,
                       "[vpr::SocketImplBSD] WARNING: This platform does not "
                       "support LowCost type of service!\n");
#endif
               break;
         }

         opt_size      = sizeof(size_t);
         break;
      case vpr::SocketOptions::AddMember:
         opt_level = IPPROTO_IP;
         opt_name  = IP_ADD_MEMBERSHIP;
         opt_data.mcast_req.imr_multiaddr.s_addr = data.mcast_add_member.getMulticastAddr().getAddressValue();
         opt_data.mcast_req.imr_interface.s_addr = data.mcast_add_member.getInterfaceAddr().getAddressValue();
         opt_size  = sizeof(struct ip_mreq);
         break;
      case vpr::SocketOptions::DropMember:
         opt_level          = IPPROTO_IP;
         opt_name           = IP_DROP_MEMBERSHIP;
         opt_data.mcast_req.imr_multiaddr.s_addr = data.mcast_drop_member.getMulticastAddr().getAddressValue();
         opt_data.mcast_req.imr_interface.s_addr = data.mcast_drop_member.getInterfaceAddr().getAddressValue();
         opt_size           = sizeof(struct ip_mreq);
         break;
      case vpr::SocketOptions::McastInterface:
         opt_level                = IPPROTO_IP;
         opt_name                 = IP_MULTICAST_IF;
         opt_data.mcast_if.s_addr = data.mcast_if.getAddressValue();
         opt_size                 = sizeof(struct in_addr);
         break;
      case vpr::SocketOptions::McastTimeToLive:
         opt_level          = IPPROTO_IP;
         opt_name           = IP_MULTICAST_TTL;
         opt_data.mcast_ttl = data.mcast_ttl;
         opt_size           = sizeof(Uint8);
         break;
      case vpr::SocketOptions::McastLoopback:
         opt_level           = IPPROTO_IP;
         opt_name            = IP_MULTICAST_LOOP;
         opt_data.mcast_loop = data.mcast_loopback;
         opt_size            = sizeof(Uint8);
         break;

      // TCP-level options.
      case vpr::SocketOptions::NoDelay:
         opt_level        = IPPROTO_TCP;
         opt_name         = TCP_NODELAY;
         opt_data.enabled = (data.no_delay ? 1 : 0);
         opt_size         = sizeof(int);
         break;
      case vpr::SocketOptions::MaxSegment:
         opt_level     = IPPROTO_TCP;
         opt_name      = TCP_MAXSEG;
         opt_data.size = data.max_segment;
         opt_size      = sizeof(size_t);
         break;
   }

   if ( ::setsockopt(mHandle->mFdesc, opt_level, opt_name, &opt_data, opt_size) != 0 )
   {
      retval.setCode(vpr::ReturnStatus::Fail);
   }

   return retval;
}

// ============================================================================
// Protected methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Destructor.  This currently does nothing.
// ----------------------------------------------------------------------------
SocketImplBSD::~SocketImplBSD ()
{
   if ( mHandle != NULL )
   {
      delete mHandle;
      mHandle = NULL;
   }
}

} // End of vpr namespace
