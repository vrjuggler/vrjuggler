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

#ifndef _VPR_INET_ADDR_SIM_H_
#define _VPR_INET_ADDR_SIM_H_

#include <vpr/vprConfig.h>

#include <string>
#include <vpr/IO/Socket/SocketTypes.h>
#include <vpr/IO/Socket/InetAddrBase.h> // my base class...


// XXX: Extend to work with IPv6.

namespace vpr
{

/**
 * Simulated Internet address structures.
 */
class VPR_CLASS_API InetAddrSIM : public vpr::InetAddrBase
{
public:
   static const InetAddrSIM AnyAddr;

   /**
    * Constructs an address object using the given address.  It must be of
    * the form <address>:<port> where <address> can be a hostname or a
    * dotted-decimal IP address.
    *
    * @param address A string giving the address and port number separated
    *                by a colon.  The address can be a hostname or a
    *                dotted-decimal IP address.
    */
   vpr::ReturnStatus setAddress( const std::string& address );

   /**
    * Constructs an address object using the given address and port number.
    * The address string can be a hostname or a dotted-decimal IP address.
    *
    * @param address A string giving the address (either hostname or IP
    *                address).
    * @param port    The port to associate with the IP address.
    */
   vpr::ReturnStatus setAddress( const std::string& address,
                                 const vpr::Uint32 port );

   /**
    * Constructs an address that is associated with the given port number.
    * The address will be set to a wildcard.
    *
    * @param port The port to associate with the IP address.
    */
   InetAddrSIM( const vpr::Uint32 port = 0 )
   {
      this->copy( InetAddrSIM::AnyAddr );
      setAddress(0x7F000001, port);   // 127.0.0.1:port
      setFamily( vpr::SocketTypes::INET );
   }

   /**
    * Constructs an address object using the given address and port number.
    * The address must be the actual 32-bit integer value.
    *
    * @param address An integer IP address.
    * @param port    The port to associate with the IP address.
    */
   vpr::ReturnStatus setAddress( const vpr::Uint32 address,
                                 const vpr::Uint32 port )
   {
      setAddressValue( address );
      setPort( port );
      setFamily( vpr::SocketTypes::INET );
      return vpr::ReturnStatus();
   }

   /**
    * Copy constructor.
    *
    * @pre None.
    * @post A copy of the given vpr::InetAddrSIM object is made in this
    *       object.
    *
    * @param addr The vpr::InetAddrSIM object to be copied into this object.
    */
   InetAddrSIM( const InetAddrSIM& addr )
   {
      copy( addr );
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
   vpr::SocketTypes::Domain getFamily( void ) const
   {
      return mFamily;
   }

   /**
    * Sets the protocol family of this address.
    *
    * @pre None.
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
   vpr::Uint32 getPort( void ) const
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
   }

   /**
    * Gets this address's Internet address in host byte order.
    *
    * @pre The IP address has been initialized properly in network byte order.
    * @psot The IP address associated with this address structure is
    *       returned to the caller in host byte order.
    *
    * @return An unsigned 32-bit integer giving the IP address for this
    *         object in host byte order.
    */
   vpr::Uint32 getAddressValue( void ) const
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
   std::string getAddressString(void) const;

   /**
    * Overloaded assignment operator to ensure that assignments work
    * correctly.
    *
    * @pre None.
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
    * @param port An unsigned 32-bit integer IP address for this object in
    *             host byte order.
    */
   void setAddressValue( const vpr::Uint32 addr_value )
   {
      mAddress = addr_value;
   }

   /**
    * Make a copy of the given vpr::InetAddrSIM object in this object.
    *
    * @pre None.
    * @post The memory in m_addr is overwritten with that of the given
    *       object's m_addr structure.
    *
    * @param addr The vpr::InetAddrSIM object to be copied into this object.
    */
   void copy( const InetAddrSIM& addr )
   {
      mPort = addr.mPort;
      mAddress = addr.mAddress;
      mFamily = addr.mFamily;
   }

   bool equals( const InetAddrSIM& addr ) const
   {
      return (mPort == addr.mPort && mAddress == addr.mAddress);
   }

   vpr::Uint32 mPort;
   vpr::Uint32 mAddress;
   vpr::SocketTypes::Domain mFamily;
};

} // End of vpr namespace


#endif   /* _VPR_INET_ADDR_SIM_H_ */
