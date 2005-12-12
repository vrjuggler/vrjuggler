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
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include <boost/concept_check.hpp>

#include <vpr/md/POSIX/IO/Socket/InetAddrBSD.h>
#include <vpr/Util/Debug.h>

#ifndef INADDR_NONE
#  define INADDR_NONE 0xffffffff   /* -1 return */
#endif


namespace vpr
{

const InetAddrBSD InetAddrBSD::AnyAddr;      // Default constructor defaults to ANY addr

InetAddrBSD::InetAddrBSD()
{
   memset(&mAddr, 0, sizeof(mAddr));
   setAddressValue(INADDR_ANY);
   setPort(0);
   setFamily(SocketTypes::INET);
}

vpr::ReturnStatus InetAddrBSD::getLocalHost(vpr::InetAddrBSD& hostAddr)
{
   char local_host_name[MAXHOSTNAMELEN + 1];
   vpr::ReturnStatus status;

   bzero(local_host_name, sizeof(local_host_name));

   if ( gethostname(local_host_name, MAXHOSTNAMELEN) == 0 )
   {
      hostAddr.setAddress(std::string(local_host_name), 0);
   }
   else
   {
      status.setCode(vpr::ReturnStatus::Fail);
   }

   return status;
}

/**
 * Constructs an address object using the given address.  It must be of the
 * form <address>:<port> where <address> can be a hostname or a
 * dotted-decimal IP address.
 */
vpr::ReturnStatus InetAddrBSD::setAddress(const std::string& address)
{
   std::string::size_type pos;
   std::string host_addr, host_port;
   vpr::Uint16 port;
   vpr::ReturnStatus retval;

   // Extract the address and the port number from the given string.
   pos       = address.find(":");
   host_addr = address.substr(0, pos);
   host_port = address.substr(pos + 1);
   port      = (Uint16) atoi(host_port.c_str());

   retval = lookupAddress(host_addr);
   setPort(port);
   setFamily(vpr::SocketTypes::INET);

   return retval;
}

vpr::ReturnStatus InetAddrBSD::setAddress(const std::string& address,
                                          const Uint16 port)
{
   vpr::ReturnStatus retval;

   retval = lookupAddress(address);
   setPort(port);
   setFamily(SocketTypes::INET);

   return retval;
}

vpr::ReturnStatus InetAddrBSD::setAddress(const vpr::Uint32 address,
                                          const vpr::Uint16 port)
{
   setAddressValue(address);
   setPort(port);
   setFamily(SocketTypes::INET);
   return ReturnStatus();
}

unsigned char InetAddrBSD::getLength() const
{
#ifdef HAVE_SIN_LEN
   return mAddr.sin_len;
#else
   return 0;
#endif
}

void InetAddrBSD::setLength(const unsigned char length)
{
#ifdef HAVE_SIN_LEN
   mAddr.sin_len = length;
#else
   boost::ignore_unused_variable_warning(length);
#endif
}

/**
 * Gets the protocol family of this address structure.
 */
vpr::SocketTypes::Domain InetAddrBSD::getFamily() const
{
   vpr::SocketTypes::Domain family;

   switch ( mAddr.sin_family )
   {
#ifdef PF_LOCAL
      case PF_LOCAL:
#else
      case PF_UNIX:
#endif
         family = SocketTypes::LOCAL;
         break;
      case PF_INET:
         family = SocketTypes::INET;
         break;
#ifdef PF_INET6
      case PF_INET6:
         family = SocketTypes::INET6;
         break;
#endif
#if defined(PF_LINK) || defined(PF_RAW)
#ifdef PF_LINK
      case PF_LINK:
#else
      case PF_RAW:
#endif
         family = SocketTypes::LINK;
         break;
#endif
      default:
         family = SocketTypes::INET;
         break;
   }

   return family;
}

/**
 * Sets the protocol family of this address structure.
 */
void InetAddrBSD::setFamily(const vpr::SocketTypes::Domain family)
{
   switch ( family )
   {
      case SocketTypes::LOCAL:
#ifdef PF_LOCAL
         mAddr.sin_family = PF_LOCAL;
#else
         mAddr.sin_family = PF_UNIX;
#endif
         break;
      case SocketTypes::INET:
         mAddr.sin_family = PF_INET;
         break;
      case SocketTypes::INET6:
#ifdef PF_INET6
         mAddr.sin_family = PF_INET6;
#else
         fprintf(stderr,
                 "[vpr::InetAddrBSD] WARNING: IPv6 not supported on this host!\n");
#endif
         break;
#if defined(PF_LINK) || defined(PF_RAW)
      case SocketTypes::LINK:
#ifdef PF_LINK
         mAddr.sin_family = PF_LINK;
#else
         mAddr.sin_family = PF_RAW;
#endif
         break;
#endif
      default:
         fprintf(stderr,
                 "[vpr::InetAddrBSD] ERROR: Unknown socket family value %d\n",
                 family);
         break;
   }
}

vpr::Uint16 InetAddrBSD::getPort() const
{
   return ntohs(mAddr.sin_port);
}

void InetAddrBSD::setPort(const vpr::Uint16 port)
{
   mAddr.sin_port = htons(port);
}

vpr::Uint32 InetAddrBSD::getAddressValue() const
{
   return ntohl(mAddr.sin_addr.s_addr);
}

/**
 * Get the IP address associated with this structure as a human-readable
 * string.
 */
std::string InetAddrBSD::getAddressString() const
{
   char ip_addr[sizeof("255.255.255.255")];
   std::string ip_str;
   union
   {
      char c[sizeof(vpr::Uint32)];
      vpr::Uint32 value;
   } addr;

   addr.value = mAddr.sin_addr.s_addr;

   snprintf(ip_addr, sizeof(ip_addr), "%u.%u.%u.%u", (Uint8) addr.c[0],
            (Uint8) addr.c[1], (Uint8) addr.c[2], (Uint8) addr.c[3]);
   ip_str = ip_addr;

   return ip_str;
}

vpr::ReturnStatus InetAddrBSD::getHostname(std::string& hostname) const
{
   vpr::ReturnStatus status;
   struct hostent* entry;

   entry = gethostbyaddr((const char*) &mAddr.sin_addr,
                         sizeof(mAddr.sin_addr), mAddr.sin_family);

   if ( NULL == entry )
   {
      status.setCode(vpr::ReturnStatus::Fail);
      const char* error_str = hstrerror(h_errno);
      vprDEBUG(vprDBG_VPR, vprDBG_CRITICAL_LVL) 
         << "[InetAddrBSD::getHostname()] ERROR: " << error_str
         << std::endl << vprDEBUG_FLUSH;
   }
   else
   {
      hostname = entry->h_name;
   }

   return status;
}

std::vector<std::string> InetAddrBSD::getHostnames() const
{
   std::vector<std::string> names;
   struct hostent* entry;

   entry = gethostbyaddr((const char*) &mAddr.sin_addr,
                         sizeof(mAddr.sin_addr), mAddr.sin_family);

   if ( NULL != entry )
   {
      names.push_back(std::string(entry->h_name));

      for ( char** ptr = entry->h_aliases; *ptr != NULL; ptr++ )
      {
         names.push_back(std::string(*ptr));
      }
   }

   return names;
}

// Overloaded equality operator.
bool InetAddrBSD::operator== (const InetAddrBSD& addr) const
{
   return (mAddr.sin_addr.s_addr == addr.mAddr.sin_addr.s_addr) &&
          (mAddr.sin_port == addr.mAddr.sin_port) &&
          (mAddr.sin_family == addr.mAddr.sin_family);
}

void InetAddrBSD::copyAddressValue(const char* addrValue)
{
   vprASSERT(addrValue != NULL);
   memcpy((void*) &mAddr.sin_addr.s_addr, (void*) addrValue,
          sizeof(mAddr.sin_addr.s_addr));
}

void InetAddrBSD::setAddressValue(const vpr::Uint32 addrValue)
{
   mAddr.sin_addr.s_addr = htonl(addrValue);
}

size_t InetAddrBSD::size() const
{
   return sizeof(mAddr);
}

size_t InetAddrBSD::addressSize() const
{
   return sizeof(mAddr.sin_addr.s_addr);
}

void InetAddrBSD::setSockaddr(const struct sockaddr* addr)
{
   memcpy((void*) &mAddr, (void*) addr, sizeof(mAddr));
}

struct sockaddr_in InetAddrBSD::toSockaddrInet()
{
   return mAddr;
}

void InetAddrBSD::copy(const InetAddrBSD& addr)
{
   memcpy((void*) &mAddr, (void*) &addr.mAddr, sizeof(mAddr));
}

/**
 * Look up the given address and store the address in mAddr.
 */
vpr::ReturnStatus InetAddrBSD::lookupAddress(const std::string& address)
{
   vpr::ReturnStatus retval;
   struct hostent* host_entry;

   // First, try looking the host up by name.
   host_entry = gethostbyname(address.c_str());

   // If that succeeded, put the result in mRemoteAddr.
   if ( host_entry != NULL )
   {
      copyAddressValue(host_entry->h_addr);
   }
   // If gethostbyname(3) failed, the address string may be an IP address.
   else
   {
      in_addr_t addr;

      // Try looking it up with inet_addr(3).
      addr = inet_addr(address.c_str());

      // If the address string could not be found using inet_addr(3), then
      // return error status.
      if ( addr == INADDR_NONE )
      {
         fprintf(stderr,
                 "[vpr::InetAddrBSD] Could not find address for '%s': %s\n",
                 address.c_str(), strerror(errno));
         retval.setCode(ReturnStatus::Fail);
      }
      // Otherwise, we found the integer address successfully.
      else
      {
         setAddressValue(addr);
      }
   }

   return retval;
}

} // End of vpr namespace
