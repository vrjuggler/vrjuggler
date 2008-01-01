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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _VPR_INET_ADDR_SIM_H_
#define _VPR_INET_ADDR_SIM_H_

#include <vpr/vprConfig.h>

#include <string>
#include <vector>
#include <vpr/IO/Socket/SocketTypes.h>
#include <vpr/IO/Socket/InetAddrBase.h> // my base class...
#include <vpr/IO/Socket/UnknownHostException.h>
#include <vpr/Util/IllegalArgumentException.h>

// XXX: Extend to work with IPv6.

namespace vpr
{

// Forward declare this.  It is defined in vpr/IO/Socket/InetAddr.h which may
// not be included by this file.
struct InetAddrHash;

/** \class InetAddrSIM InetAddrSIM.h vpr/IO/Socket/InetAddr.h
 *
 * Simulated Internet address structures.  This is typedef'd to vpr::InetAddr
 * when simulated sockets are used.
 */
class VPR_CLASS_API InetAddrSIM : public vpr::InetAddrBase
{
public:
   static const InetAddrSIM AnyAddr;

   typedef vpr::InetAddrHash hash;

   /**
    * Returns the local host's address via the given object reference.
    *
    * @throw UnknownHostException If no IP address for the host could be found.
    *
    * @return A vpr::InetAddr object holding the local host's default address.
    */
   static vpr::InetAddrSIM getLocalHost();

   /**
    * Constructs an address object using the given address.  It must be of
    * the form \c address:port where \c address can be a hostname or a
    * dotted-decimal IP address.
    *
    * @param address A string giving the address and port number separated
    *                by a colon.  The address can be a hostname or a
    *                dotted-decimal IP address.
    */
   void setAddress( const std::string& address );

   /**
    * Constructs an address object using the given address and port number.
    * The address string can be a hostname or a dotted-decimal IP address.
    *
    * @param address A string giving the address (either hostname or IP
    *                address).
    * @param port    The port to associate with the IP address.
    */
   void setAddress( const std::string& address,
                    const vpr::Uint32 port );

   /**
    * Constructs an address that is associated with the given port number.
    * The address will be set to a wildcard.
    */
   InetAddrSIM()
   {
      this->copy( InetAddrSIM::AnyAddr );
      setAddress(0x7F000001, 0);   // 127.0.0.1:port
      setFamily( vpr::SocketTypes::INET );
      setDebugData();
   }

   /**
    * Constructs an address object using the given address and port number.
    * The address must be the actual 32-bit integer value.
    *
    * @param address An integer IP address.
    * @param port    The port to associate with the IP address.
    */
   void setAddress( const vpr::Uint32 address,
                    const vpr::Uint32 port )
   {
      setAddressValue( address );
      setPort( port );
      setFamily( vpr::SocketTypes::INET );
      setDebugData();
   }

   /**
    * Copy constructor.
    *
    * @post A copy of the given vpr::InetAddrSIM object is made in this
    *       object.
    *
    * @param addr The vpr::InetAddrSIM object to be copied into this object.
    */
   InetAddrSIM( const InetAddrSIM& addr )
   {
      copy( addr );
      setDebugData();
   }

   /**
    * Gets the protocol family of this address.
    *
    * @pre The protocol family has been properly initialized.
    * @post The protocol family is returned as a vpr::SocketTypes::Domain
    *       value.
    *
    * @return A vpr::SocketTypes::Domain value representing this object's
    *         protocol family.
    */
   vpr::SocketTypes::Domain getFamily() const
   {
      return mFamily;
   }

   /**
    * Sets the protocol family of this address.
    *
    * @post The given protocol family (a vpr::SocketTypes::Domain value) is
    *       mapped to the appropriate platform-specific protocol family
    *       value and stored.
    *
    * @param family The protocol family value.
    */
   void setFamily( const vpr::SocketTypes::Domain family )
   {
      mFamily = family;
   }

   /**
    * Gets this address' port in host byte order.
    *
    * @pre The port has been initialized properly in network byte order.
    * @post The port associated with this address is returned to the caller
    *       in host byte order.
    *
    * @return An unsigned 16-bit integer  giving the port for this address
    *          in host byte order.
    */
   vpr::Uint32 getPort() const
   {
      return mPort;
   }

   /**
    * Sets this address' port.  The given port must be in host byte order.
    *
    * @pre The given port number is in host byte order.
    * @post The given port number is stored in the address.
    *
    * @param port An unsigned 16-bit integer port number for this address
    *             in host byte order.
    */
   void setPort( const vpr::Uint32 port )
   {
      mPort = port;
      setDebugData();
   }

   /**
    * Gets this address's Internet address in host byte order.
    *
    * @pre The IP address has been initialized properly in network byte order.
    * @post The IP address associated with this address structure is
    *       returned to the caller in host byte order.
    *
    * @return An unsigned 32-bit integer giving the IP address for this
    *         object in host byte order.
    */
   vpr::Uint32 getAddressValue() const
   {
      return mAddress;
   }

   /**
    * Gets the IP address associated with this object as a human-readable
    * string.
    *
    * @pre The object contains a valid IP address.
    * @post The integer IP address is converted to dotted-decmial notation
    *       and returned as a string.
    *
    * @return A std::string object containing this structure's IP address
    *         in the human-readable "dotted-decimal" notation.
    */
   std::string getAddressString() const;

   /**
    * Returns the fully qualified hostname for this address.
    */
   void getHostname(std::string& hostname) const
   {
      hostname = getAddressString();
   }

   /**
    * Returns the fully qualified primary hostname for this address and all
    * known aliases.
    */
   std::vector<std::string> getHostnames() const
   {
      std::vector<std::string> vec(1);
      vec[0] = getAddressString();
      return vec;
   }

   /**
    * Overloaded assignment operator to ensure that assignments work
    * correctly.
    *
    * @post A copy of the given vpr::InetAddrSIM object is made in this object
    *       which is then returned to the caller.
    *
    * @param addr The vpr::InetAddrSIM object to be copied into this object.
    *
    * @return A reference to this object.
    */
   InetAddrSIM& operator=( const InetAddrSIM& addr )
   {
      this->copy( addr );
      return *this;
   }

   /**
    * Overloaded equality operator.
    */
   bool operator==( const InetAddrSIM& addr ) const
   {
      return this->equals( addr );
   }

   /**
    * Overloaded inequality operator.
    */
   bool operator!=( const InetAddrSIM& addr ) const
   {
      return ! (*this == addr);
   }

protected:

   /**
    * Sets this objects's IP address.  The given address must be in host byte
    * order.
    *
    * @pre The given IP address is in host byte order.
    * @post The given IP address is stored.
    *
    * @param addrValue An unsigned 32-bit integer IP address for this object in
    *                  host byte order.
    */
   void setAddressValue(const vpr::Uint32 addrValue)
   {
      mAddress = addrValue;
   }

   /**
    * Make a copy of the given vpr::InetAddrSIM object in this object.
    *
    * @post The memory in mAddr is overwritten with that of the given
    *       object's mAddr structure.
    *
    * @param addr The vpr::InetAddrSIM object to be copied into this object.
    */
   void copy( const InetAddrSIM& addr )
   {
      mPort = addr.mPort;
      mAddress = addr.mAddress;
      mFamily = addr.mFamily;
      setDebugData();
   }

   bool equals( const InetAddrSIM& addr ) const
   {
      return (mPort == addr.mPort && mAddress == addr.mAddress);
   }

   void setDebugData()
   {
#ifdef VPR_DEBUG
      mDebugStringAddress = getAddressString();
#endif
   }

   vpr::Uint32 mPort;
   vpr::Uint32 mAddress;
   vpr::SocketTypes::Domain mFamily;

#ifdef VPR_DEBUG
   std::string mDebugStringAddress;
#endif
};

} // End of vpr namespace


#endif   /* _VPR_INET_ADDR_SIM_H_ */
