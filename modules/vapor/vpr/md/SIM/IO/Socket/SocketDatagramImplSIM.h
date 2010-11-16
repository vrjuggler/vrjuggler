/***************** <Plexus heading BEGIN do not edit this line> ***************
 *
 * Plexus
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert
 *
 ***************** <Plexus heading END do not edit this line> ****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _VPR_SOCKET_DATAGRAM_IMPL_SIM_H_
#define _VPR_SOCKET_DATAGRAM_IMPL_SIM_H_

#include <vpr/vprConfig.h>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/md/SIM/IO/Socket/SocketImplSIM.h>


namespace vpr
{

/** \class SocketDatagramImplSIM SocketDatagramImplSIM.h vpr/IO/Socket/SocketDatagram.h
 *
 * Implementation of datagram sockets using simulated sockets.  This is used
 * in conjunction with vpr::SocketConfiguration to create the typedef
 * vpr::SocketDatagram.
 */
class SocketDatagramImplSIM : public vpr::SocketImplSIM
{
public:
   /**
    * Default constructor.
    *
    * @post The member variables are initialized to default values.  The
    *       socket type is set to vpr::SocketTypes::DATAGRAM.
    */
   SocketDatagramImplSIM()
      : SocketImplSIM(vpr::SocketTypes::DATAGRAM)
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
   SocketDatagramImplSIM(const vpr::InetAddr& localAddr,
                         const vpr::InetAddr& remoteAddr)
      : SocketImplSIM(localAddr, remoteAddr, vpr::SocketTypes::DATAGRAM)
   {
      /* Do nothing. */ ;
   }

   vpr::Uint32 recvfrom(void* msg, const vpr::Uint32 length,
                        vpr::InetAddr& from,
                        const vpr::Interval timeout = vpr::Interval::NoTimeout);

   vpr::Uint32 sendto(const void* msg, const vpr::Uint32 length,
                      const vpr::InetAddr& to,
                      const vpr::Interval timeout = vpr::Interval::NoTimeout);

   virtual vpr::ReturnStatus isReadReady() const;

   virtual vpr::ReturnStatus isWriteReady() const;
};

} // End of vpr namespace


#endif /* _VPR_SOCKET_DATAGRAM_IMPL_SIM_H_ */
