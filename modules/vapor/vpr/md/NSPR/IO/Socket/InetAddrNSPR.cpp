/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <sstream>
#include <prsystem.h>

#include <vpr/md/NSPR/IO/Socket/InetAddrNSPR.h>
#include <vpr/Util/Error.h>


namespace vpr
{

std::vector<vpr::InetAddrNSPR> getIfAddrs(const bool withLoopback,
                                          const bool withDown);

const InetAddrNSPR InetAddrNSPR::AnyAddr;      // Default constructor defaults to ANY addr

InetAddrNSPR::InetAddrNSPR()
{
   memset(&mAddr, 0, sizeof(mAddr));
   setFamily(SocketTypes::INET);
   setAddressValue(PR_INADDR_ANY);
   setPort(0);
}

InetAddrNSPR::InetAddrNSPR(const InetAddrNSPR& addr)
{
   mAddr = addr.mAddr;
}

InetAddrNSPR::~InetAddrNSPR()
{
#ifdef VPR_DEBUG
   mAddr.inet.family = mAddr.inet.port = mAddr.inet.ip = 1221;
#endif
}

vpr::InetAddrNSPR InetAddrNSPR::getLocalHost()
{
   char local_host_name[257];
   memset(local_host_name, 0, 257);

   vpr::InetAddrNSPR localhost;

   if ( PR_GetSystemInfo(PR_SI_HOSTNAME, local_host_name, 256) == PR_SUCCESS )
   {
      localhost.setAddress(std::string(local_host_name), 0);
   }
   else
   {
      throw UnknownHostException("No IP address for could be found for localhost.",
                                 VPR_LOCATION);
   }

   return localhost;
}

std::vector<vpr::InetAddrNSPR>
InetAddrNSPR::getAllLocalAddrs(const bool withLoopback, const bool withDown)
{
   return vpr::getIfAddrs(withLoopback, withDown);
}

// Set the address for this object using the given address.  It must be of the
// form <address>:<port> where <address> can be a hostname or a dotted-decimal
// IP address.
void InetAddrNSPR::setAddress(const std::string& address)
{
   std::string::size_type pos;
   std::string host_addr, host_port;
   vpr::Uint16 port;

   // Extract the address and the port number from the given string.
   pos       = address.find(":");
   host_addr = address.substr(0, pos);
   host_port = address.substr(pos + 1);
   port      = (vpr::Uint16) atoi(host_port.c_str());

   if ( ! host_addr.empty() )
   {
      lookupAddress(host_addr);
   }

   setPort(port);
   setFamily(vpr::SocketTypes::INET);
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
         {
            std::ostringstream msg_stream;
            msg_stream << "Unknown socket family value " << family;
            throw IllegalArgumentException(msg_stream.str(), VPR_LOCATION);
         }
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

std::string InetAddrNSPR::getHostname() const
{
   char buffer[PR_NETDB_BUF_SIZE];
   memset(buffer, 0, PR_NETDB_BUF_SIZE);
   PRStatus ret_status;
   PRHostEnt hostent;
   std::string hostname;

   ret_status = PR_GetHostByAddr(&mAddr, buffer, sizeof(buffer), &hostent);

   if ( ret_status == PR_FAILURE )
   {
      std::string err_msg(
         "[InetAddrNSPR::getHostname] Failed to get host by address"
      );
      vpr::Error::outputCurrentError(std::cerr, err_msg);
      throw UnknownHostException(err_msg, VPR_LOCATION);
   }
   else
   {
      hostname = hostent.h_name;
   }

   return hostname;
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
   else
   {
      std::stringstream msg_stream;
      msg_stream << "[InetAddrBSD::getHostnames] Hostname lookup failed: "
                 << vpr::Error::getCurrentErrorMsg();
      throw UnknownHostException(msg_stream.str(), VPR_LOCATION);
   }

   return names;
}

// Look up the address in mName and store the address in mAddr.
void InetAddrNSPR::lookupAddress(const std::string& address)
{
   PRStatus ret_status;
   PRHostEnt host_entry;
   char buffer[PR_NETDB_BUF_SIZE];

   ret_status = PR_GetHostByName(address.c_str(), buffer, sizeof(buffer),
                                 &host_entry);

   if ( ret_status == PR_FAILURE )
   {
      setAddressValue(0);           // Error on lookup, so zero the address
      std::stringstream msg_stream;
      msg_stream << "[InetAddrNSPR::lookupAddress] Failed to look up host '"
                 << address << "'";

      vpr::Error::outputCurrentError(std::cerr, msg_stream.str());

      msg_stream << " (" << vpr::Error::getCurrentErrorMsg() << ")";
      throw UnknownHostException(msg_stream.str(), VPR_LOCATION);
   }
   else
   {
      if ( PR_EnumerateHostEnt(0, &host_entry, 0, &mAddr) == -1 )
      {
         std::string err_msg(
            "[InetAddrNSPR::lookupAddress] Could not enumerate host entry"
         );
         vpr::Error::outputCurrentError(std::cerr, err_msg);
         throw UnknownHostException(err_msg, VPR_LOCATION);
      }
   }
}

} // End of vpr namespace
