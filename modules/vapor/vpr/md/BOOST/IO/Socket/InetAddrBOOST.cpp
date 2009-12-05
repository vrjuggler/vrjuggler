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
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include <boost/concept_check.hpp>

#include <vpr/md/BOOST/IO/Socket/InetAddrBOOST.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>

#ifndef INADDR_NONE
#  define INADDR_NONE 0xffffffff   /* -1 return */
#endif


namespace vpr
{

std::vector<vpr::InetAddrBOOST> getIfAddrs(const bool withLoopback,
                                           const bool withDown);

const InetAddrBOOST InetAddrBOOST::AnyAddr;      // Default constructor defaults to ANY addr

InetAddrBOOST::InetAddrBOOST()
{
   setAddressValue(INADDR_ANY);
   setPort(0);
   setFamily(SocketTypes::INET);
}

vpr::InetAddrBOOST InetAddrBOOST::getLocalHost()
{
   vpr::InetAddrBOOST localhost;

   boost::system::error_code ec;
   std::string local_host_name = boost::asio::ip::host_name(ec);

   if ( !ec )
   {
      localhost.setAddress(local_host_name, 0);
   }
   else
   {
      throw UnknownHostException(
         "No IP address for could be found for localhost.", VPR_LOCATION
      );
   }

   return localhost;
}

std::vector<vpr::InetAddrBOOST>
InetAddrBOOST::getAllLocalAddrs(const bool withLoopback, const bool withDown)
{
   return vpr::getIfAddrs(withLoopback, withDown);
}

/**
 * Constructs an address object using the given address.  It must be of the
 * form <address>:<port> where <address> can be a hostname or a
 * dotted-decimal IP address.
 */
void InetAddrBOOST::setAddress(const std::string& address)
{
   std::string::size_type pos;
   std::string host_addr, host_port;
   vpr::Uint16 port;

   // Extract the address and the port number from the given string.
   pos       = address.find(":");
   host_addr = address.substr(0, pos);
   host_port = address.substr(pos + 1);
   port      = (Uint16) atoi(host_port.c_str());

   if ( ! host_addr.empty() )
   {
      lookupAddress(host_addr);
   }

   setPort(port);
   setFamily(vpr::SocketTypes::INET);
}

void InetAddrBOOST::setAddress(const std::string& address,
                               const Uint16 port)
{
   if ( ! address.empty() )
   {
      lookupAddress(address);
   }

   setPort(port);
   setFamily(SocketTypes::INET);
}

void InetAddrBOOST::setAddress(const vpr::Uint32 address,
                               const vpr::Uint16 port)
{
   setAddressValue(address);
   setPort(port);
   setFamily(SocketTypes::INET);
}

/**
 * Gets the protocol family of this address structure.
 */
vpr::SocketTypes::Domain InetAddrBOOST::getFamily() const
{
   return mFamily;
}

/**
 * Sets the protocol family of this address structure.
 */
void InetAddrBOOST::setFamily(const vpr::SocketTypes::Domain family)
{
   mFamily = family;
}

vpr::Uint16 InetAddrBOOST::getPort() const
{
   return mTcpAddr.port();
}

void InetAddrBOOST::setPort(const vpr::Uint16 port)
{
   mTcpAddr.port(port);
   mUdpAddr.port(port);
}

vpr::Uint32 InetAddrBOOST::getAddressValue() const
{
   boost::asio::ip::address boostAddr = mTcpAddr.address();
   vpr::Uint32 addr = boostAddr.to_v4().to_ulong();
   return addr;
}

/**
 * Get the IP address associated with this structure as a human-readable
 * string.
 */
std::string InetAddrBOOST::getAddressString() const
{
   boost::asio::ip::address boostAddr = mTcpAddr.address();
   return boostAddr.to_string();
}

std::string InetAddrBOOST::getHostname() const
{
   boost::asio::io_service io_service;
   boost::asio::ip::tcp::resolver resolver(io_service);
   boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(mTcpAddr);
   
   return (*iter).host_name();
}

std::vector<std::string> InetAddrBOOST::getHostnames() const
{
   std::vector<std::string> names;
   boost::asio::io_service io_service;
   boost::asio::ip::tcp::resolver resolver(io_service);
   boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(mTcpAddr);
   boost::asio::ip::tcp::resolver::iterator end;
   while (iter != end)
   {
      names.push_back((*iter).host_name());
      ++iter;
   }

   return names;
}

// Overloaded equality operator.
bool InetAddrBOOST::operator== (const InetAddrBOOST& addr) const
{
   return (mTcpAddr.address() == addr.mUdpAddr.address()) &&
          (mTcpAddr.port() == addr.mTcpAddr.port()) &&
          (mUdpAddr.address() == addr.mUdpAddr.address()) &&
          (mUdpAddr.port() == addr.mUdpAddr.port()) &&
          (mFamily == addr.mFamily);
}

/**
 * Set address to addrValue
 */

void InetAddrBOOST::setAddressValue(const vpr::Uint32 addrValue)
{
   boost::asio::ip::address_v4 addr;
   if (addrValue == INADDR_ANY)
   {
      addr = boost::asio::ip::address_v4::address_v4();
   }
   else
   {
      addr = boost::asio::ip::address_v4::address_v4(addrValue);
   }
   mTcpAddr.address(addr);
   mUdpAddr.address(addr);
}

void InetAddrBOOST::copy(const InetAddrBOOST& addr)
{
   mTcpAddr = addr.mTcpAddr;
   mUdpAddr = addr.mUdpAddr;
   mFamily = addr.mFamily;
}

/**
 * Look up the given address and store the address in mAddr.
 */
void InetAddrBOOST::lookupAddress(const std::string& address)
{
   boost::asio::io_service io_service;
   boost::asio::ip::tcp::resolver resolver(io_service);
   boost::asio::ip::tcp::resolver::query query(address, "http");
   boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
   boost::asio::ip::tcp::endpoint ep = *iterator;
   mTcpAddr.address(ep.address());
   mUdpAddr.address(ep.address());
}

} // End of vpr namespace
