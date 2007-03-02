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

#include <vpr/vprConfig.h>

#include <sstream>
#include <prio.h>
#include <private/pprio.h>
#include <prinrval.h>

#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/WouldBlockException.h>
#include <vpr/IO/Socket/ConnectionAbortedException.h>
#include <vpr/IO/Socket/ConnectionResetException.h>
#include <vpr/Util/Error.h>
#include <vpr/md/NSPR/NSPRHelpers.h>
#include <vpr/md/NSPR/IO/Socket/SocketDatagramImplNSPR.h>


namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

vpr::Uint32 SocketDatagramImplNSPR::recvfrom(void* msg,
                                             const vpr::Uint32 length,
                                             vpr::InetAddr& from,
                                             const vpr::Interval timeout)
{
   vpr::Uint32 bytes_read(0);

   PRInt32 bytes = PR_RecvFrom(mHandle, msg, length, 0, from.getPRNetAddr(),
                               NSPR_getInterval(timeout));

   if ( bytes > 0 )
   {
      bytes_read = bytes;
   }
   else if ( bytes == -1 )
   {
      const PRErrorCode err_code = PR_GetError();

      const std::string nspr_err_msg(vpr::Error::getCurrentErrorMsg());
      std::stringstream msg_stream;

      if ( err_code == PR_WOULD_BLOCK_ERROR )
      {
         msg_stream << "Would block while writing";

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         throw WouldBlockException(msg_stream.str(), VPR_LOCATION);
      }
      else if ( err_code == PR_IO_TIMEOUT_ERROR )
      {
#if defined(WINNT)
         // Handle the case of a timeout error on an NT socket. We have to
         // tell NSPR to put the socket back into the right state. We do not
         // need to worry about whether the socket is blocking because the
         // timeout is ignored by non-blocking NSPR sockets.
         PR_NT_CancelIo(mHandle);
#endif

         msg_stream << "recvfrom operation timed out";

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         throw TimeoutException(msg_stream.str(), VPR_LOCATION);
      }
      else if ( err_code == PR_CONNECT_ABORTED_ERROR )
      {
         msg_stream << "recvfrom connection aborted";

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         throw ConnectionAbortedException(msg_stream.str(), VPR_LOCATION);
      }
      else
      {
         msg_stream << "[vpr::SocketDatagramImplNSPR::recvfrom()] Could not "
                    << "read from socket";

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         throw SocketException(msg_stream.str(), VPR_LOCATION);
      }
   }
   else if ( bytes == 0 )      // Not connected
   {
      std::stringstream msg_stream;
      msg_stream << "Connection closed";
      const std::string nspr_err_msg(vpr::Error::getCurrentErrorMsg());

      if ( ! nspr_err_msg.empty() )
      {
         msg_stream << ": " << nspr_err_msg;
      }

      // XXX: Do we need a NotConnectedException?
      throw ConnectionResetException(msg_stream.str(), VPR_LOCATION);
   }

   return bytes_read;
}

vpr::Uint32 SocketDatagramImplNSPR::sendto(const void* msg,
                                           const vpr::Uint32 length,
                                           const vpr::InetAddr& to,
                                           const vpr::Interval timeout)
{
   vpr::Uint32 bytes_sent(0);

#ifdef VPR_OS_Win32
   vprASSERT(vpr::InetAddr::AnyAddr != to && "INADDR_ANY is not a valid desination on win32.");
#endif

   PRInt32 bytes = PR_SendTo(mHandle, msg, length, 0, to.getPRNetAddr(),
                             NSPR_getInterval(timeout));

   if ( bytes == -1 )
   {
      const PRErrorCode err_code = PR_GetError();

      const std::string nspr_err_msg(vpr::Error::getCurrentErrorMsg());
      std::stringstream msg_stream;

      if ( err_code == PR_WOULD_BLOCK_ERROR )
      {
         msg_stream << "Would block while writing";

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         throw WouldBlockException(msg_stream.str(), VPR_LOCATION);
      }
      else if ( err_code == PR_IO_TIMEOUT_ERROR )
      {
#if defined(WINNT)
         // Handle the case of a timeout error on an NT socket. We have to
         // tell NSPR to put the socket back into the right state. We do not
         // need to worry about whether the socket is blocking because the
         // timeout is ignored by non-blocking NSPR sockets.
         PR_NT_CancelIo(mHandle);
#endif

         msg_stream << "Write operation timed out";

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         throw TimeoutException(msg_stream.str(), VPR_LOCATION);
      }
      else if ( err_code == PR_CONNECT_ABORTED_ERROR )
      {
         msg_stream << "Connection aborted while writing";

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         throw ConnectionAbortedException(msg_stream.str(), VPR_LOCATION);
      }
      else if ( err_code == PR_NOT_CONNECTED_ERROR )
      {
         msg_stream << "Connection reset";

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         // XXX: Do we need a NotConnectedException?
         throw ConnectionResetException(msg_stream.str(), VPR_LOCATION);
      }
      else
      {
         msg_stream << "[vpr::SocketDatagramImplNSPR::sendto()] Could not "
                    << "send message";

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         throw SocketException(msg_stream.str(), VPR_LOCATION);
      }
   }
   else
   {
      bytes_sent = bytes;
   }

   return bytes_sent;
}

} // End of vpr namespace
