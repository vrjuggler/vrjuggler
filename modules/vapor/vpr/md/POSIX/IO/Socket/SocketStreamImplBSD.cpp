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
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include <vpr/Util/Debug.h>
#include <vpr/md/POSIX/IO/Socket/SocketStreamImplBSD.h>


namespace vpr
{

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Listen on the socket for incoming connection requests.
// ----------------------------------------------------------------------------
vpr::ReturnStatus SocketStreamImplBSD::listen (const int backlog)
{
   vpr::ReturnStatus retval;

   // Put the socket into listning mode.  If that fails, print an error and
   // return error status.
   if ( ::listen(mHandle->mFdesc, backlog) == -1 )
   {
      fprintf(stderr,
              "[vpr::SocketStreamImplBSD] Cannot listen on socket: %s\n",
              strerror(errno));
      retval.setCode(ReturnStatus::Fail);
   }

   return retval;
}

// ----------------------------------------------------------------------------
// Accept an incoming connection request.
// ----------------------------------------------------------------------------
vpr::ReturnStatus SocketStreamImplBSD::accept (SocketStreamImplBSD& sock,vpr::Interval timeout)
{
   int accept_sock;
   vpr::ReturnStatus retval;
   InetAddr addr;
#if defined(VPR_OS_IRIX) || defined(VPR_OS_HPUX)
   int addrlen;
#else
   socklen_t addrlen;
#endif

   retval = mHandle->isReadable(timeout);

   if ( retval.success() )
   {
      mBlockingFixed = true;

      // Accept an incoming connection request.
      addrlen = addr.size();
      accept_sock = ::accept(mHandle->mFdesc,
                             (struct sockaddr*) &addr.mAddr, &addrlen);

      // If accept(2) failed, print an error message and return error stauts.
      if ( accept_sock == -1 )
      {
         if ( errno == EWOULDBLOCK && getNonBlocking() )
         {
            retval.setCode(ReturnStatus::WouldBlock);
         }
         else
         {
            fprintf(stderr,
                    "[vpr::SocketStreamImplBSD] Error while accepting "
                    "incoming connection: %s\n", strerror(errno));
            retval.setCode(ReturnStatus::Fail);
         }
      }
      // Otherwise, put the new socket in the passed socket object.
      else
      {
         sock.setRemoteAddr(addr);
         sock.mHandle          = new FileHandleImplUNIX(addr.getAddressString());
         sock.mHandle->mFdesc = accept_sock;
         sock.mOpen            = true;

         // Inherit the blocking state from the accepting socket.  This
         // must be done after mOpen is set to true to satisfy the
         // pre-condition.
         if ( getNonBlocking() )
         {
            sock.enableNonBlocking();
         }

         sock.mBound          = true;
         sock.mConnected      = true;
         sock.mBlockingFixed = true;
      }
   }

   return retval;
}

} // End of vpr namespace
