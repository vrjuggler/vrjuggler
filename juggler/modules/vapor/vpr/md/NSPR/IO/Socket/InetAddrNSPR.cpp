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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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

#include <vpr/md/NSPR/IO/Socket/InetAddrNSPR.h>
#include <vpr/md/NSPR/NSPRHelpers.h>

namespace vpr {

const InetAddrNSPR InetAddrNSPR::AnyAddr;      // Default constructor defaults to ANY addr

// ----------------------------------------------------------------------------
// Set the address for this object using the given address.  It must be of the
// form <address>:<port> where <address> can be a hostname or a dotted-decimal
// IP address.
// ----------------------------------------------------------------------------
Status
InetAddrNSPR::setAddress (const std::string& address) {
   std::string::size_type pos;
   std::string host_addr, host_port;
   Uint16 port;
   Status retval;

   // Extract the address and the port number from the given string.
   pos       = address.find(":");
   host_addr = address.substr(0, pos);
   host_port = address.substr(pos + 1);
   port      = (Uint16) atoi(host_port.c_str());

   retval = lookupAddress(host_addr);
   setPort(port);
   setFamily(SocketTypes::INET);
   return retval;
}

// ----------------------------------------------------------------------------
// Get the protocol family of this address structure.
// ----------------------------------------------------------------------------
SocketTypes::Domain
InetAddrNSPR::getFamily (void) const
{
    SocketTypes::Domain family;

    switch ( PR_NetAddrFamily(&mAddr))
    {
    case PR_AF_INET:
        family = SocketTypes::INET;
        break;
    case PR_AF_LOCAL:
       family = SocketTypes::LOCAL;
       break;
    case PR_AF_INET6:
       family = SocketTypes::INET6;
       break;
    }

    return family;
}

// ----------------------------------------------------------------------------
// Set the protocol family of this address structure.
// ----------------------------------------------------------------------------
void
InetAddrNSPR::setFamily (const SocketTypes::Domain family) {
    switch (family) {
      case SocketTypes::LOCAL:
         PR_NetAddrFamily(&mAddr) = PR_AF_LOCAL;
         break;
      case SocketTypes::INET:
        PR_NetAddrFamily(&mAddr) = PR_AF_INET;
        break;
      case SocketTypes::INET6:
        PR_NetAddrFamily(&mAddr) = PR_AF_INET6;
        break;
      default:
        fprintf(stderr,
                "[vpr::InetAddrNSPR] ERROR: Unknown socket family value %d\n",
                family);
        break;
    }
}

// ----------------------------------------------------------------------------
// Get the IP address associated with this structure as a human-readable
// string.
// ----------------------------------------------------------------------------
std::string
InetAddrNSPR::getAddressString (void) const {
    char ip_str[256];
    PR_NetAddrToString(&mAddr, ip_str, sizeof(PRNetAddr));
    return std::string(ip_str);
}

// ----------------------------------------------------------------------------
// Look up the address in m_name and store the address in m_remote_addr.
// ----------------------------------------------------------------------------
Status
InetAddrNSPR::lookupAddress (const std::string& address) {
   Status retval;
   PRStatus ret_status;
   PRHostEnt host_entry;
   char buffer[PR_NETDB_BUF_SIZE];

   ret_status = PR_GetHostByName(address.c_str(), buffer, sizeof(buffer),
                                 &host_entry);

   if(ret_status == PR_FAILURE)
   {
      setAddressValue(0);           // Error on lookup, so zero the address
      std::string error_msg("[InetAddrNSPR::setAddress] Failure to look up host: ");
      error_msg += address;

      NSPR_PrintError(error_msg);
      retval.setCode(Status::Failure);
   }
   else {
      if ( PR_EnumerateHostEnt(0, &host_entry, 0, &mAddr) == -1 ) {
         retval.setCode(Status::Failure);
      }

      if ( retval.failure() ) {
         NSPR_PrintError(std::string("[InetAddrNSPR::lookupAddress] Could not enumerate host entry"));
      }
   }

   return retval;
}

}; // End of vpr namespace
