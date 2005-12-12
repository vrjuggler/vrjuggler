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
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <net/if.h>
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

void getIfAddrs(std::vector<vpr::InetAddrBSD>& hostAddrs,
                const bool withLoopback);

const InetAddrBSD InetAddrBSD::AnyAddr;      // Default constructor defaults to ANY addr

InetAddrBSD::InetAddrBSD()
{
   memset(&mAddr, 0, sizeof(mAddr));
   setAddressValue(INADDR_ANY);
   setPort(0);
   setFamily(SocketTypes::INET);
}

void InetAddrBSD::getLocalHost(vpr::InetAddrBSD& hostAddr)
   throw (UnknownHostException)
{
   char local_host_name[MAXHOSTNAMELEN + 1];

   bzero(local_host_name, sizeof(local_host_name));

   if ( gethostname(local_host_name, MAXHOSTNAMELEN) == 0 )
   {
      hostAddr.setAddress(std::string(local_host_name), 0);
   }
   else
   {
      throw UnknownHostException(
         "No IP address for could be found for localhost.", VPR_LOCATION
      );
   }
}

void InetAddrBSD::getAllLocalAddrs(std::vector<vpr::InetAddrBSD>& hostAddrs,
                                   const bool withLoopback)
{
   vpr::getIfAddrs(hostAddrs, withLoopback);
}

/**
 * Constructs an address object using the given address.  It must be of the
 * form <address>:<port> where <address> can be a hostname or a
 * dotted-decimal IP address.
 */
void InetAddrBSD::setAddress(const std::string& address)
   throw (UnknownHostException)
{
   std::string::size_type pos;
   std::string host_addr, host_port;
   vpr::Uint16 port;

   // Extract the address and the port number from the given string.
   pos       = address.find(":");
   host_addr = address.substr(0, pos);
   host_port = address.substr(pos + 1);
   port      = (Uint16) atoi(host_port.c_str());

   lookupAddress(host_addr);
   setPort(port);
   setFamily(vpr::SocketTypes::INET);
}

void InetAddrBSD::setAddress(const std::string& address,
                             const Uint16 port)
   throw (UnknownHostException)
{
   lookupAddress(address);
   setPort(port);
   setFamily(SocketTypes::INET);
}

void InetAddrBSD::setAddress(const vpr::Uint32 address,
                             const vpr::Uint16 port)
{
   setAddressValue(address);
   setPort(port);
   setFamily(SocketTypes::INET);
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
   throw (IllegalArgumentException)
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
         throw IllegalArgumentException("IPv6 not supported on this host!",
                                        VPR_LOCATION);
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
         {
            std::ostringstream msg_stream;
            msg_stream << "Unknown socket family value "
                       << (unsigned long) family;
            throw IllegalArgumentException(msg_stream.str(), VPR_LOCATION);
         }
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

void InetAddrBSD::getHostname(std::string& hostname) const
   throw (UnknownHostException)
{
   socklen_t salen;
#if defined(HAVE_SIN_LEN)
   salen = mAddr.sin_len;
#else
   salen = sizeof(mAddr);
#endif

   char addr[NI_MAXHOST];
   memset((void*) &addr, 0, sizeof(addr));

   int result = getnameinfo((sockaddr*) &mAddr, salen, addr, sizeof(addr),
                            NULL, 0, NI_NAMEREQD);

   if ( result == 0 )
   {
      hostname = addr;
   }
   else
   {
      // inet_ntoa(3) could change errno if it fails, so we save the current
      // value while we have it.
      const int err_code(errno);
      std::ostringstream msg_stream;
      msg_stream << "Could not find hostname for "
                 << inet_ntoa(mAddr.sin_addr) << ": " << strerror(err_code);
      throw UnknownHostException(msg_stream.str(), VPR_LOCATION);
   }
}

std::vector<std::string> InetAddrBSD::getHostnames() const
   throw (UnknownHostException)
{
   std::vector<std::string> names;
   struct hostent* entry;

   entry = gethostbyaddr((const char*) &mAddr.sin_addr,
                         sizeof(mAddr.sin_addr), mAddr.sin_family);

   if ( NULL == entry )
   {
      std::ostringstream msg_stream;
      msg_stream << "[InetAddrBSD::getHostnames()] Hostname lookup failed: "
                 << hstrerror(h_errno);
      throw UnknownHostException(msg_stream.str(), VPR_LOCATION);
   }
   else
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
void InetAddrBSD::lookupAddress(const std::string& address)
   throw (UnknownHostException)
{
   struct addrinfo* addrs;

   struct addrinfo hints;
   memset((void*) &hints, 0, sizeof(hints));
   hints.ai_flags = AF_INET;

   int result = getaddrinfo(address.c_str(), NULL, &hints, &addrs);

   if ( result == 0 )
   {
      memcpy((void*) &mAddr, addrs->ai_addr, addrs->ai_addrlen);
      freeaddrinfo(addrs);
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << "Could not find address for '" << address << "': "
                 << gai_strerror(result);
      throw UnknownHostException(msg_stream.str(), VPR_LOCATION);
   }
}

} // End of vpr namespace
