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

#include <vpr/md/SIM/DNS.h>         /* for simulated DNS lookups... */
#include <vpr/md/SIM/IO/Socket/InetAddrSIM.h>
#include <vpr/System.h>


namespace vpr
{

  const InetAddrSIM InetAddrSIM::AnyAddr;

  vpr::ReturnStatus InetAddrSIM::getLocalHost(vpr::InetAddrSIM& host_addr)
  {
     return vpr::ReturnStatus::Fail;
  }

  vpr::ReturnStatus InetAddrSIM::setAddress( const std::string& address )
  {
      std::string::size_type pos;
      std::string host_addr, host_port;
      vpr::Uint32 port;

      // Extract the address and the port number from the given string.
      pos       = address.find( ":" );
      host_addr = address.substr( 0, pos );
      host_port = address.substr( pos + 1 );
      port      = (vpr::Uint32) atoi( host_port.c_str() );

      //setAddress( host_addr );
      mAddress = vpr::sim::DNS::instance()->lookupAddress( host_addr );
      setPort( port );
      setFamily( vpr::SocketTypes::INET );
      setDebugData();

      return vpr::ReturnStatus();
   }

   vpr::ReturnStatus InetAddrSIM::setAddress( const std::string& address,
                                              const vpr::Uint32 port )
   {
      mAddress = vpr::sim::DNS::instance()->lookupAddress( address );
      setPort( port );
      setFamily( vpr::SocketTypes::INET );
      setDebugData();
      return vpr::ReturnStatus();
   }

   std::string InetAddrSIM::getAddressString () const
   {
      char buffer[sizeof("255.255.255.255")];

      union
      {
         vpr::Uint8 bytes[sizeof(vpr::Uint32)];
         vpr::Uint32 value;
      } addr;

      addr.value = vpr::System::Htonl(mAddress);
      memset(buffer, '\0', sizeof(buffer));
      sprintf(buffer, "%u.%u.%u.%u", addr.bytes[0], addr.bytes[1],
              addr.bytes[2], addr.bytes[3]);
      //std::string result = buffer;

      return std::string(buffer);
   }

} // End of vpr namespace
