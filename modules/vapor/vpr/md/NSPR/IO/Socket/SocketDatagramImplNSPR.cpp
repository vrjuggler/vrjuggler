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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
#include <prinrval.h>

#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/WouldBlockException.h>
#include <vpr/IO/Socket/ConnectionResetException.h>
#include <vpr/Util/Error.h>
#include <vpr/md/NSPR/IO/Socket/SocketDatagramImplNSPR.h>


namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

void SocketDatagramImplNSPR::recvfrom(void* msg, const vpr::Uint32 length,
                                      vpr::InetAddr& from,
                                      vpr::Uint32& bytesRead,
                                      const vpr::Interval timeout)
   throw (IOException)
{
   PRInt32 bytes;

   bytes = PR_RecvFrom(mHandle, msg, length, 0, from.getPRNetAddr(),
                       NSPR_getInterval(timeout));

   if ( bytes > 0 )
   {
      bytesRead = bytes;
   }
   else if ( bytes == -1 )
   {
      PRErrorCode err_code = PR_GetError();

      bytesRead = 0;

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
         msg_stream << "recvfrom operation timed out";

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         throw TimeoutException(msg_stream.str(), VPR_LOCATION);
      }
      else
      {
         msg_stream << "[vpr::SocketDatagramImplNSPR::recvfrom()] Could not "
                    << "read from socket";
         vpr::Error::outputCurrentError(std::cerr, msg_stream.str());

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         throw SocketException(msg_stream.str(), VPR_LOCATION);
      }
   }
   else if ( bytes == 0 )      // Not connected
   {
      bytesRead = bytes;

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
}

void SocketDatagramImplNSPR::sendto(const void* msg, const vpr::Uint32 length,
                                    const vpr::InetAddr& to,
                                    vpr::Uint32& bytesSent,
                                    const vpr::Interval timeout)
   throw (IOException)
{
   PRInt32 bytes;

   bytes = PR_SendTo(mHandle, msg, length, 0, to.getPRNetAddr(),
                     NSPR_getInterval(timeout));

   if ( bytes == -1 )
   {
      PRErrorCode err_code = PR_GetError();

      bytesSent = 0;

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
         msg_stream << "Write operation timed out";

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         throw TimeoutException(msg_stream.str(), VPR_LOCATION);
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
         vpr::Error::outputCurrentError(std::cerr, msg_stream.str());

         if ( ! nspr_err_msg.empty() )
         {
            msg_stream << ": " << nspr_err_msg;
         }

         throw SocketException(msg_stream.str(), VPR_LOCATION);
      }
   }
   else
   {
      bytesSent = bytes;
   }
}

} // End of vpr namespace
