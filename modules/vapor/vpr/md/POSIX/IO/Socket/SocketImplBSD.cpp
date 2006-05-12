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
#include <boost/concept_check.hpp>

#include <vpr/md/POSIX/IO/Socket/SocketImplBSD.h>
#include <vpr/IO/Socket/ConnectionResetException.h>
#include <vpr/IO/Socket/ConnectionRefusedException.h>
#include <vpr/IO/Socket/UnknownHostException.h>
#include <vpr/IO/Socket/NoRouteToHostException.h>
#include <vpr/md/POSIX/IO/SelectorImplBSD.h>
#include <vpr/Util/Debug.h>

namespace
{

// Given an error number (or errno) build up an exception with the
// correct type and error string and throw it.
//
// This helper comes in handy since we have to throw exceptions from so many places in
// the socket implementation.
void buildAndThrowException(std::string prefix, std::string location, int error_number=-2)
{
   if (-2 == error_number)
   {
      error_number = errno;
   }

   /** Build error string. */
   const char* err_str(strerror(error_number));
   std::string err_string;

   if ( NULL != err_str )
   {
      err_string = std::string(err_str);
   }
   else
   {
      err_string = std::string("");
   }

   // Build and throw exception
   if(ECONNREFUSED == error_number)
   {
      throw vpr::ConnectionRefusedException(prefix + "Connection refused: " + err_string, location);
   }
   else if(ECONNRESET == error_number)
   {
      throw vpr::ConnectionResetException(prefix + "Connection reset: " + err_string, location);
   }
   else if (EHOSTUNREACH == error_number)
   {
      throw vpr::NoRouteToHostException(prefix + "No route to host: " + err_string, location);
   }
   else if (EHOSTDOWN == error_number)
   {
      throw vpr::SocketException(prefix + "Host down: " + err_string, location);
   }
   else if (ENETDOWN == error_number)
   {
      throw vpr::SocketException(prefix + "Network is down: " + err_string, location);
   }
   else
   {
      throw vpr::SocketException(prefix + "Error: " + err_string, location);
   }

}

}

namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

const std::string& SocketImplBSD::getName() const
{
   return mHandle->getName();
}

// Open the socket.  This creates a new socket using the domain and type
// options set through member variables.
void SocketImplBSD::open() throw (IOException)
{
   int domain, type, sock;

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
      buildAndThrowException("[vpr::CocketImplBSD::open] (" + getName() + ") ", VPR_LOCATION);
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

      // Since socket(2) cannot open a socket in non-blocking mode, we call
      // vpr::FileHandleUNIX::setBlocking() directly.
      mHandle->setBlocking(mOpenBlocking);
   }
}

void SocketImplBSD::close() throw (IOException)
{
   mHandle->close();

   // Reset the local state tracking to initial state
   // since we are not connected or bound any more.
   // This allows the socket to be reused.
   mBound = false;
   mConnectCalled = false;
   mBlockingFixed = false;
}

// Reconfigures the socket so that it is in blocking mode.
void SocketImplBSD::setBlocking(bool blocking) throw (IOException)
{
   vprASSERT(! mBlockingFixed &&
             "Cannot change blocking state after a blocking call!");

   if ( ! mBlockingFixed )
   {
      mHandle->setBlocking(blocking);

      if ( ! isOpen() )
      {
         mOpenBlocking = blocking;
      }
   }
   else
   {
      throw SocketException("Cannot change blocking state after a blocking call.",
         VPR_LOCATION);
   }
}

// Bind this socket to the address in the host address member variable.
void SocketImplBSD::bind() throw (SocketException)
{
   int status;

   // Bind the socket to the address in mLocalAddr.
   status = ::bind(mHandle->mFdesc, (struct sockaddr*) &mLocalAddr.mAddr,
                   mLocalAddr.size());

   // If that fails, print an error and return error status.
   if ( status == -1 )
   {
      buildAndThrowException("[vpr::SocketImplBSD::bind] ", VPR_LOCATION);
   }
   else
   {
      mBound = true;
   }
}

// Connect the socket on the client side to the server side.  For a datagram
// socket, this makes the address given to the constructor the default
// destination for all packets.  For a stream socket, this has the effect of
// establishing a connection with the destination.
void SocketImplBSD::connect(vpr::Interval timeout) throw (vpr::Exception)
{
   int status;

   // Attempt to connect to the address in mAddr.
   status = ::connect(mHandle->mFdesc,
                      (struct sockaddr*) &mRemoteAddr.mAddr,
                      mRemoteAddr.size());

   // If connect(2) failed, print an error message explaining why and return
   // error status.
   if ( status == -1 )
   {
      // If this is a non-blocking connection, return normally with the
      // post condition that users must call isConnected() after calling connect
      // when using non-blocking sockets.
      // NOTE: I'm not sure if it's safe to set mConnectCalled and
      // mBlockingFixed at this point, but they have to be set sometime.
      if ( errno == EINPROGRESS && ! isBlocking() )
      {
         if ( vpr::Interval::NoWait != timeout )
         {
            // Force socket to wait for timeout interval to expire
            // before returning. This provides a way for the caller
            // to specify that they want the connection process to
            // block even with a non-blocking socket.

            try
            {
               // Wait for read/write on the socket
               SelectorImplBSD selector;
               selector.addHandle(getHandle(), SelectorBase::Read | SelectorBase::Write);
               vpr::Uint16 num_events(0);
               selector.select(num_events, timeout);

               // Check for error state on socket
               vpr::SocketOptions::Data opt_data;
               getOption(SocketOptions::Error, opt_data);
               int sock_error = opt_data.error;

               if(sock_error)
               {
                  close();
                  buildAndThrowException("[vpr::SocketImplBSD] Async-connection error: ", VPR_LOCATION, sock_error);
               }
               else  // Completed in time
               {
                  mBound = true;
                  mConnectCalled = true;
                  mBlockingFixed = true;
               }

            }
            catch(TimeoutException& te)      // Select timed out, so the connect timed out
            {
               close();
               throw TimeoutException("Timeout while connecting.", VPR_LOCATION);
            }

         }
         else   // non-blocking connect started
         {
            mBound         = true;
            mConnectCalled = true;
            mBlockingFixed = true;
         }
      }
      else
      {
         buildAndThrowException("[vpr::SocketImplBSD::Connect] ", VPR_LOCATION);
      }
   }
   // Otherwise, return success.
   else
   {
      mBound            = true;
      mConnectCalled    = true;
      mBlockingFixed    = true;
   }

   // Fill in the local address if has not already been assigned.
   if ( mConnectCalled && vpr::InetAddr::AnyAddr == mLocalAddr )
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
}


// Idea:
// - If have read or write and there are no socket errors, then we are connected
bool SocketImplBSD::isConnected() const throw ()
{
   bool connected(false);

   if ( isOpen() && mConnectCalled )
   {
      //vpr::Int32 bytes;

      bool readable = mHandle->isReadable(vpr::Interval::NoWait);
      bool writable = mHandle->isWriteable(vpr::Interval::NoWait);
      boost::ignore_unused_variable_warning(readable);
      boost::ignore_unused_variable_warning(writable);

      try
      {
         SelectorImplBSD selector;
         selector.addHandle(getHandle(), SelectorBase::Read | SelectorBase::Write);
         vpr::Uint16 num_events(0);
         selector.select(num_events, vpr::Interval::NoWait);
      }
      catch(TimeoutException& te)
      {
         return false;
      }

      vpr::SocketOptions::Data opt_data;
      getOption(SocketOptions::Error, opt_data);
      int sock_error = opt_data.error;

      if(sock_error)
      {
         //close();
         buildAndThrowException("[vpr::SocketImplBSD::isConnected] Error: ", VPR_LOCATION, sock_error);
      }

      // No error, so we are connected
      connected = true;
   }

   return connected;
}

void SocketImplBSD::setLocalAddr(const InetAddr& addr) throw (SocketException)
{
   if ( mBound || mConnectCalled )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "SocketImplBSD::setLocalAddr: Can't set address of a "
         << "bound or connected socket.\n" << vprDEBUG_FLUSH;

      throw SocketException(std::string("SocketImplBSD::setLocalAddr: Can't set address of a ")
         + std::string("bound or connected socket."), VPR_LOCATION);
   }
   else
   {
      mLocalAddr = addr;
   }
}

void SocketImplBSD::setRemoteAddr(const InetAddr& addr) throw (SocketException)
{
   if ( mConnectCalled )
   {
      throw SocketException("Can not set remote address on a connected socket.",
         VPR_LOCATION);
   }
   else
   {
      mRemoteAddr = addr;
   }
}

void SocketImplBSD::read_i(void* buffer,
                           const vpr::Uint32 length,
                           vpr::Uint32& bytesRead,
                           const vpr::Interval timeout)
   throw (IOException)
{
   mBlockingFixed = true;
   mHandle->read_i(buffer, length, bytesRead, timeout);

   // If read returns 0, then other side shut down cleanly
   // note: bytesRead is also 0 when non-blocking and can't get data
   //       but in that case we throw WouldBlockException
   if ( 0 == bytesRead )
   {
      throw SocketException("Socket disconnected cleanly.", VPR_LOCATION);
   }
}

void SocketImplBSD::readn_i(void* buffer,
                            const vpr::Uint32 length,
                            vpr::Uint32& bytesRead,
                            const vpr::Interval timeout)
   throw (IOException)
{
   mBlockingFixed = true;
   mHandle->readn_i(buffer, length, bytesRead, timeout);

   // XXX: Should never happen.
   if ( bytesRead == 0 )
   {
      throw SocketException("Socket not connected.", VPR_LOCATION);
   }
}

void SocketImplBSD::write_i(const void* buffer,
                            const vpr::Uint32 length,
                            vpr::Uint32& bytesWritten,
                            const vpr::Interval timeout)
   throw (IOException)
{
   mBlockingFixed = true;

   //try
   //{
      mHandle->write_i(buffer, length, bytesWritten, timeout);
   //}
   //catch (IOException& ex)
   //{
   //   // Build up the socket based error
   //   buildAndThrowException("[vpr::SocketImplBSD::write_i] ", VPR_LOCATION);
   //}
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
   Int32          error;
};

void SocketImplBSD::getOption(const vpr::SocketOptions::Types option,
                              struct vpr::SocketOptions::Data& data) const
   throw (SocketException)
{
   int opt_name, opt_level, status;
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
      case vpr::SocketOptions::Broadcast:
         opt_level = SOL_SOCKET;
         opt_name  = SO_BROADCAST;
         opt_size  = sizeof(opt_data.enabled);
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

      // BSD specific
      case vpr::SocketOptions::Error:
         opt_level = SOL_SOCKET;
         opt_name  = SO_ERROR;
         opt_size  = sizeof(opt_data.error);
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
         case vpr::SocketOptions::Broadcast:
            data.broadcast = (opt_data.enabled != 0 ? true : false);
            break;
         case vpr::SocketOptions::MaxSegment:
            data.max_segment = opt_data.size;
            break;
         case vpr::SocketOptions::AddMember:
         case vpr::SocketOptions::DropMember:
            /** Do nothing */
            break;
         case vpr::SocketOptions::Error:
            data.error = opt_data.error;
            break;
      }
   }
   else
   {
      fprintf(stderr,
              "[vpr::SocketImplBSD] ERROR: Could not get socket option for socket %s: %s\n",
              mHandle->getName().c_str(), strerror(errno));

      throw SocketException("[vpr::SocketImplBSD] ERROR: Could not get socket option for socket "
         + mHandle->getName() + ": " + std::string(strerror(errno)), VPR_LOCATION);
   }
}

void SocketImplBSD::setOption(const vpr::SocketOptions::Types option,
                              const struct vpr::SocketOptions::Data& data)
   throw (SocketException)
{
   int opt_name, opt_level;
#if defined(VPR_OS_IRIX) || defined(VPR_OS_HPUX)
   int opt_size;
#else
   socklen_t opt_size;
#endif
   union sockopt_data opt_data;

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
      case vpr::SocketOptions::Broadcast:
         opt_level = SOL_SOCKET;
         opt_name  = SO_BROADCAST;
         opt_data.enabled = (data.broadcast ? 1 : 0);
         opt_size  = sizeof(int);
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
               // XXX: Should this be an invalid argument exception
               //      or should we ignore it.
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

      // Unsetable
      case vpr::SocketOptions::Error:
         break;
   }

   if ( ::setsockopt(mHandle->mFdesc, opt_level, opt_name, &opt_data, opt_size) != 0 )
   {
      throw SocketException("[vpr::SocketImplBSD] ERROR: Could not set socket option for socket "
         + mHandle->getName() + ": " + std::string(strerror(errno)), VPR_LOCATION);
   }
}

SocketImplBSD::~SocketImplBSD()
{
   if ( mHandle != NULL )
   {
      delete mHandle;
      mHandle = NULL;
   }
}

// ============================================================================
// Protected methods.
// ============================================================================

SocketImplBSD::SocketImplBSD(const vpr::SocketTypes::Type sockType)
   : mOpenBlocking(true)
   , mBound(false)
   , mConnectCalled(false)
   , mBlockingFixed(false)
   , mHandle(NULL)
   , mType(sockType)
{
   mHandle = new FileHandleImplUNIX();
}

SocketImplBSD::SocketImplBSD(const vpr::InetAddr& localAddr,
                             const vpr::InetAddr& remoteAddr,
                             const vpr::SocketTypes::Type sockType)
   : mOpenBlocking(true)
   , mBound(false)
   , mConnectCalled(false)
   , mBlockingFixed(false)
   , mHandle(NULL)
   , mLocalAddr(localAddr)
   , mRemoteAddr(remoteAddr)
   , mType(sockType)
{
   mHandle = new FileHandleImplUNIX(remoteAddr.getAddressString());
}

} // End of vpr namespace
