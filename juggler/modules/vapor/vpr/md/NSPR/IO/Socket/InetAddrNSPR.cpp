/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vprConfig.h>

#include <stdio.h>

#ifdef VPR_OS_Win32
#define MAXHOSTNAMELEN INTERNET_MAX_HOST_NAME_LENGTH
#else
#include <sys/param.h>
#endif

#include <md/NSPR/InetAddrNSPR.h>
#include <md/NSPR/NSPRHelpers.h>

namespace vpr {

const InetAddrNSPR InetAddrNSPR::AnyAddr;      // Default constructor defaults to ANY addr

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
bool
InetAddrNSPR::setAddress (const std::string& address) {
   bool retval;
   PRStatus ret_status;
   PRHostEnt host_entry;
   char buffer[MAXHOSTNAMELEN];

   ret_status = PR_GetHostByName(address.c_str(), buffer, sizeof(buffer),
                                 &host_entry);

   if(ret_status == PR_FAILURE)
   {
      setAddressValue(0);           // Error on lookup, so zero the address
      std::string error_msg("InetAddrNSPR::setAddress: Failure to look up host: ");
      error_msg += address;

      NSPR_PrintError(error_msg);
      return false;
   }

   retval = (-1 == PR_EnumerateHostEnt(0, &host_entry, 0, &mAddr));

   return retval;
}

}; // End of vpr namespace
