/***************** <Plexus heading BEGIN do not edit this line> ***************
 *
 * Plexus
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ***************** <Plexus heading END do not edit this line> ****************/

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

#ifndef _VPR_SOCKET_DATAGRAM_IMPL_SIM_H_
#define _VPR_SOCKET_DATAGRAM_IMPL_SIM_H_

#include <vpr/vprConfig.h>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/md/SIM/IO/Socket/SocketImplSIM.h>


namespace vpr
{

class SocketDatagramImplSIM : public vpr::SocketImplSIM
{
public:
   SocketDatagramImplSIM (void)
      : SocketImplSIM(vpr::SocketTypes::DATAGRAM)
   {
      /* Do nothing. */ ;
   }

   SocketDatagramImplSIM (const vpr::InetAddr& local_addr,
                          const vpr::InetAddr& remote_addr)
      : SocketImplSIM(local_addr, remote_addr, vpr::SocketTypes::DATAGRAM)
   {
      /* Do nothing. */ ;
   }

   vpr::ReturnStatus recvfrom(void* msg, const vpr::Uint32 length,
                              const int flags, vpr::InetAddr& from,
                              vpr::Uint32& bytes_read,
                              const vpr::Interval timeout = vpr::Interval::NoTimeout);

   vpr::ReturnStatus sendto(const void* msg, const vpr::Uint32 length,
                            const int flags, const vpr::InetAddr& to,
                            vpr::Uint32& bytes_sent,
                            const vpr::Interval timeout = vpr::Interval::NoTimeout);

   virtual vpr::ReturnStatus isReadReady() const;

   virtual vpr::ReturnStatus isWriteReady() const;
};

} // End of vpr namespace


#endif /* _VPR_SOCKET_DATAGRAM_IMPL_SIM_H_ */
