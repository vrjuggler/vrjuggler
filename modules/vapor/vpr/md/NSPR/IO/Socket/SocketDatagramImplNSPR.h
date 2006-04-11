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

#ifndef _VPR_SOCKET_DATAGRAM_IMPL_NSPR_H_
#define _VPR_SOCKET_DATAGRAM_IMPL_NSPR_H_

#include <vpr/vprConfig.h>

#include <vpr/md/NSPR/IO/Socket/SocketImplNSPR.h>


namespace vpr
{

/** \class SocketDatagramImplNSPR SocketDatagramImplNSPR.h vpr/IO/Socket/SocketDatagram.h
 *
 * NSPR implementation of the datagram socket interface.  This is used in
 * conjunction with vpr::SocketConfiguration to create the typedef
 * vpr::SocketDatagram.
 */
class VPR_CLASS_API SocketDatagramImplNSPR : public SocketImplNSPR
{
public:
   typedef SocketImplNSPR Parent;

   // ========================================================================
   // vpr::SocketDatagram implementation.
   // ========================================================================

   /**
    * Default constructor.
    *
    * @post The member variables are initialized to default values.  The
    *       socket type is set to vpr::SocketTypes::DATAGRAM.
    */
   SocketDatagramImplNSPR() : SocketImplNSPR(SocketTypes::DATAGRAM)
   {
      /* Do nothing. */ ;
   }

   /**
    * Constructor.
    *
    * @post The member variables are initialized to default values.  The
    *       socket type is set to vpr::SocketTypes::DATAGRAM.
    *
    * @param localAddr  The local address to which this socket will be bound.
    * @param remoteAddr The remote address whith which this socket will
    *                   communicate.
    */
   SocketDatagramImplNSPR(const InetAddr& localAddr,
                          const InetAddr& remoteAddr)
      : SocketImplNSPR(localAddr, remoteAddr, SocketTypes::DATAGRAM)
   {
      /* Do nothing. */ ;
   }

   /** Copy constructor. */
   SocketDatagramImplNSPR(const SocketDatagramImplNSPR& sock)
      : SocketImplNSPR(sock)
   {
      mLocalAddr  = sock.mLocalAddr;
      mRemoteAddr = sock.mRemoteAddr;
   }

   vpr::ReturnStatus recvfrom(void* msg, const vpr::Uint32 length,
                              vpr::InetAddr& from, vpr::Uint32& bytesRead,
                              const vpr::Interval timeout = vpr::Interval::NoTimeout);

   vpr::ReturnStatus sendto(const void* msg, const vpr::Uint32 length,
                            const vpr::InetAddr& to, vpr::Uint32& bytesSent,
                            const vpr::Interval timeout = vpr::Interval::NoTimeout);
};

} // End of namespace


#endif  /* _VPR_SOCKET_DATAGRAM_IMPL_NSPR_H_ */
