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
#include <private/pprio.h>
#include <prinrval.h>

#include <vpr/Util/Error.h>
#include <vpr/md/NSPR/NSPRHelpers.h>
#include <vpr/md/NSPR/IO/Socket/SocketDatagramImplNSPR.h>


namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

vpr::ReturnStatus SocketDatagramImplNSPR::recvfrom(void* msg,
                                                   const vpr::Uint32 length,
                                                   vpr::InetAddr& from,
                                                   vpr::Uint32& bytesRead,
                                                   const vpr::Interval timeout)
{
   ReturnStatus retval;
   PRInt32 bytes;

   bytes = PR_RecvFrom(mHandle, msg, length, 0, from.getPRNetAddr(),
                       NSPR_getInterval(timeout));

   if ( bytes > 0 )
   {
      bytesRead = bytes;
   }
   else if ( bytes == -1 )
   {
      const PRErrorCode err_code = PR_GetError();

      bytesRead = 0;

      if ( err_code == PR_WOULD_BLOCK_ERROR )
      {
         retval.setCode(vpr::ReturnStatus::WouldBlock);
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
         retval.setCode(ReturnStatus::Timeout);
      }
      else if ( err_code == PR_CONNECT_ABORTED_ERROR )
      {
         retval.setCode(vpr::ReturnStatus::ConnectionAborted);
      }
      else
      {
         std::ostringstream err_stream;
         vpr::Error::outputCurrentError(err_stream,
                                        "Could not read from socket");
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "[vpr::SocketDatagramImplNSPR::recvfrom()] "
            << err_stream.str() << std::endl << vprDEBUG_FLUSH;
         retval.setCode(ReturnStatus::Fail);
      }
   }
   else if ( bytes == 0 )      // Not connected
   {
      retval.setCode(ReturnStatus::NotConnected);
      bytesRead = bytes;
   }

   return retval;
}

vpr::ReturnStatus SocketDatagramImplNSPR::sendto(const void* msg,
                                                 const vpr::Uint32 length,
                                                 const vpr::InetAddr& to,
                                                 vpr::Uint32& bytesSent,
                                                 const vpr::Interval timeout)
{
   ReturnStatus retval;
   PRInt32 bytes;

#ifdef VPR_OS_Win32
   vprASSERT(vpr::InetAddr::AnyAddr != to && "INADDR_ANY is not a valid desination on win32.");
#endif

   bytes = PR_SendTo(mHandle, msg, length, 0, to.getPRNetAddr(),
                     NSPR_getInterval(timeout));

   if ( bytes == -1 )
   {
      const PRErrorCode err_code = PR_GetError();

      bytesSent = 0;

      if ( err_code == PR_WOULD_BLOCK_ERROR )
      {
         retval.setCode(vpr::ReturnStatus::WouldBlock);
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
         retval.setCode(ReturnStatus::Timeout);
      }
      else if ( err_code == PR_CONNECT_ABORTED_ERROR )
      {
         retval.setCode(vpr::ReturnStatus::ConnectionAborted);
      }
      else if ( err_code == PR_NOT_CONNECTED_ERROR )
      {
         retval.setCode(vpr::ReturnStatus::NotConnected);
      }
      else
      {
         std::ostringstream err_stream;
         vpr::Error::outputCurrentError(err_stream, "Could not send message");
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "[vpr::SocketDatagramImplNSPR::sendto()] " << err_stream.str()
            << std::endl << vprDEBUG_FLUSH;
         retval.setCode(ReturnStatus::Fail);
      }
   }
   else
   {
      bytesSent = bytes;
   }

   return retval;
}

} // End of vpr namespace
