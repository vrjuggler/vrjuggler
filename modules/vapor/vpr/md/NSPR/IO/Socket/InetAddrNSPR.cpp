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

#include <stdio.h>
#include <prsystem.h>

#include <vpr/md/NSPR/IO/Socket/InetAddrNSPR.h>
#include <vpr/Util/Error.h>


namespace vpr
{

vpr::ReturnStatus getIfAddrs(std::vector<vpr::InetAddrNSPR>& hostAddrs,
                             const bool withLoopback);

const InetAddrNSPR InetAddrNSPR::AnyAddr;      // Default constructor defaults to ANY addr

vpr::ReturnStatus InetAddrNSPR::getLocalHost(vpr::InetAddrNSPR& hostAddr)
{
   vpr::ReturnStatus status(vpr::ReturnStatus::Fail);
   char local_host_name[257];
   memset(local_host_name, 0, 257);

   if ( PR_GetSystemInfo(PR_SI_HOSTNAME, local_host_name, 256) == PR_SUCCESS )
   {
      hostAddr.setAddress(std::string(local_host_name), 0);
      status.setCode(vpr::ReturnStatus::Succeed);
   }

   return status;
}

vpr::ReturnStatus
InetAddrNSPR::getAllLocalAddrs(std::vector<vpr::InetAddrNSPR>& hostAddrs,
                               const bool withLoopback)
{
   return vpr::getIfAddrs(hostAddrs, withLoopback);
}

// Set the address for this object using the given address.  It must be of the
// form <address>:<port> where <address> can be a hostname or a dotted-decimal
// IP address.
vpr::ReturnStatus InetAddrNSPR::setAddress(const std::string& address)
{
   std::string::size_type pos;
   std::string host_addr, host_port;
   vpr::Uint16 port;
   vpr::ReturnStatus retval;

   // Extract the address and the port number from the given string.
   pos       = address.find(":");
   host_addr = address.substr(0, pos);
   host_port = address.substr(pos + 1);
   port      = (vpr::Uint16) atoi(host_port.c_str());

   retval = lookupAddress(host_addr);
   setPort(port);
   setFamily(vpr::SocketTypes::INET);
   return retval;
}

// Get the protocol family of this address structure.
vpr::SocketTypes::Domain InetAddrNSPR::getFamily() const
{
   vpr::SocketTypes::Domain family;

   switch ( PR_NetAddrFamily(&mAddr) )
   {
      case PR_AF_INET:
         family = vpr::SocketTypes::INET;
         break;
      case PR_AF_LOCAL:
         family = vpr::SocketTypes::LOCAL;
         break;
      case PR_AF_INET6:
         family = vpr::SocketTypes::INET6;
         break;
   }

   return family;
}

// Set the protocol family of this address structure.
void InetAddrNSPR::setFamily(const vpr::SocketTypes::Domain family)
{
   switch ( family )
   {
      case vpr::SocketTypes::LOCAL:
         PR_NetAddrFamily(&mAddr) = PR_AF_LOCAL;
         break;
      case vpr::SocketTypes::INET:
         PR_NetAddrFamily(&mAddr) = PR_AF_INET;
         break;
      case vpr::SocketTypes::INET6:
         PR_NetAddrFamily(&mAddr) = PR_AF_INET6;
         break;
      default:
         fprintf(stderr,
                 "[vpr::InetAddrNSPR] ERROR: Unknown socket family value %d\n",
                 family);
         break;
   }
}

// Get the IP address associated with this structure as a human-readable
// string.
std::string InetAddrNSPR::getAddressString() const
{
   char ip_str[256];
   memset(ip_str, 0, 256);
   PR_NetAddrToString(&mAddr, ip_str, sizeof(PRNetAddr));
   std::string temp(ip_str);
   return temp;
}

vpr::ReturnStatus InetAddrNSPR::getHostname(std::string& hostname) const
{
   vpr::ReturnStatus status;
   char buffer[PR_NETDB_BUF_SIZE];
   memset(buffer, 0, PR_NETDB_BUF_SIZE);
   PRStatus ret_status;
   PRHostEnt hostent;

   ret_status = PR_GetHostByAddr(&mAddr, buffer, sizeof(buffer), &hostent);

   if ( ret_status == PR_FAILURE )
   {
      vpr::Error::outputCurrentError(std::cerr, "[InetAddrNSPR::getHostname] Failed to get host by address");
      status.setCode(vpr::ReturnStatus::Fail);
   }
   else
   {
      hostname = hostent.h_name;
   }

   return status;
}

std::vector<std::string> InetAddrNSPR::getHostnames() const
{
   std::vector<std::string> names;
   char buffer[PR_NETDB_BUF_SIZE];
   PRStatus ret_status;
   PRHostEnt hostent;

   ret_status = PR_GetHostByAddr(&mAddr, buffer, sizeof(buffer), &hostent);

   if ( ret_status != PR_FAILURE )
   {
      names.push_back(std::string(hostent.h_name));

      for ( char** ptr = hostent.h_aliases; *ptr != NULL; ptr++ )
      {
         names.push_back(std::string(*ptr));
      }
   }

   return names;
}

// Look up the address in mName and store the address in mAddr.
vpr::ReturnStatus InetAddrNSPR::lookupAddress(const std::string& address)
{
   vpr::ReturnStatus retval;
   PRStatus ret_status;
   PRHostEnt host_entry;
   char buffer[PR_NETDB_BUF_SIZE];

   ret_status = PR_GetHostByName(address.c_str(), buffer, sizeof(buffer),
                                 &host_entry);

   if ( ret_status == PR_FAILURE )
   {
      setAddressValue(0);           // Error on lookup, so zero the address
      std::string error_msg("[InetAddrNSPR::lookupAddress] Fail to look up host: ");
      error_msg += address;

      vpr::Error::outputCurrentError(std::cerr, error_msg);
      retval.setCode(ReturnStatus::Fail);
   }
   else
   {
      if ( PR_EnumerateHostEnt(0, &host_entry, 0, &mAddr) == -1 )
      {
         retval.setCode(ReturnStatus::Fail);
      }

      if ( retval.failure() )
      {
         vpr::Error::outputCurrentError(std::cerr, "[InetAddrNSPR::lookupAddress] Could not enumerate host entry");
      }
   }

   return retval;
}

} // End of vpr namespace
