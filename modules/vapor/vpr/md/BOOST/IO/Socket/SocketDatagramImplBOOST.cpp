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

#include <vpr/vprConfig.h>

#include <string.h>
//#include <strings.h>
#include <sstream>
#include <sys/types.h>
//#include <sys/socket.h>
#include <errno.h>

#include <vpr/IO/Socket/ConnectionResetException.h>
#include <vpr/IO/Socket/UnknownHostException.h>
#include <vpr/IO/Socket/NoRouteToHostException.h>
#include <vpr/IO/IOException.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/WouldBlockException.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/md/BOOST/IO/Socket/SocketDatagramImplBOOST.h>
#include <boost/bind.hpp>
#include <vpr/System.h>


namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

SocketDatagramImplBOOST::SocketDatagramImplBOOST()
   : SocketImplBOOST(SocketTypes::DATAGRAM)
{
   /* Do nothing. */ ;
}

SocketDatagramImplBOOST::SocketDatagramImplBOOST(const InetAddr& localAddr,
                                                 const InetAddr& remoteAddr)
   : SocketImplBOOST(localAddr, remoteAddr, SocketTypes::DATAGRAM)
{
   /* Do nothing. */ ;
}

SocketDatagramImplBOOST::
SocketDatagramImplBOOST(const SocketDatagramImplBOOST& sock)
   : SocketImplBOOST(sock.mLocalAddr, sock.mRemoteAddr, SocketTypes::DATAGRAM)
{
   mUdpSocket = sock.mUdpSocket;
   mTcpSocket = sock.mTcpSocket;
   mLocalAddr = sock.mLocalAddr;
   mRemoteAddr = sock.mRemoteAddr;
   //mIOService = sock.mIOService;
}

vpr::Uint32 SocketDatagramImplBOOST::recvfrom(void* msg,
                                              const vpr::Uint32 length,
                                              vpr::InetAddr& from,
                                              const vpr::Interval& timeout)
{
   mBytesRead = 0;

   // NOTE: It appears that binding the address of a stack variable
   // (timer_result) to an asynchronous callback functor works here because
   // the I/O service is being pumped by this method.
   boost::optional<boost::system::error_code> timer_result;
   boost::asio::deadline_timer timer(mUdpSocket->get_io_service());
   timer.expires_from_now(boost::posix_time::microseconds(timeout.msec()));
   timer.async_wait(boost::bind(&SocketDatagramImplBOOST::setResult, this,
                                &timer_result, _1, -1));

   // Same situation for read_result.
   boost::optional<boost::system::error_code> read_result;
   mUdpSocket->async_receive_from(boost::asio::buffer(msg, length),
                                  from.mUdpAddr,
                                  boost::bind(&SocketDatagramImplBOOST::setResult,
                                              this, &read_result, _1, _2));
   mUdpSocket->get_io_service().reset();
   static bool cancel_supported(true);

   while (mUdpSocket->io_service().run_one())
   {
      if (read_result)
      {
         timer.cancel();
      }
      else if (timer_result)
      {
         if (cancel_supported)
         {
            try
            {
               mUdpSocket->cancel();
            }
            catch (std::exception & ex)
            {
               vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
                  << "[SocketDatagramImplBOOST] caught an exception "
                  << "cancelling UDP socket, switching to pre-Vista mode..."
                  << std::endl << vprDEBUG_FLUSH;
               cancel_supported = false;
            }
         }

         if (! cancel_supported)
         {
            const bool was_bound(isBound());
            this->close();
            this->open();

            if (was_bound)
            {
               this->bind();
            }
         }

         throw TimeoutException("recvfrom operation timed out", VPR_LOCATION);
      }
   }

   return mBytesRead;
}

vpr::Uint32 SocketDatagramImplBOOST::sendto(const void* msg,
                                            const vpr::Uint32 length,
                                            const vpr::InetAddr& to,
                                            const vpr::Interval& timeout)
{
   boost::system::error_code ec;
   vpr::Uint32 bytes_sent(0);

   bytes_sent = mUdpSocket->send_to(boost::asio::buffer(msg, length),
                                    to.mUdpAddr, 0, ec);

   if (ec)
   {
      //TODO handle errors
   }

   return bytes_sent;
}

void SocketDatagramImplBOOST::
setResult(boost::optional<boost::system::error_code>* a,
          const boost::system::error_code b, const std::size_t bytes)
{
   a->reset(b);
   if (bytes != -1)
   {
      mBytesRead = bytes;
   }
}

} // End of vpr namespace
