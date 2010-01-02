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

#ifndef _VPR_INET_ADDR_BSD_H_
#define _VPR_INET_ADDR_BSD_H_

#include <vpr/vprConfig.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <string>
#include <vector>

#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketTypes.h>
#include <vpr/IO/Socket/UnknownHostException.h>
#include <vpr/Util/IllegalArgumentException.h>


// XXX: Extend to work with IPv6.

namespace vpr
{

// Forward declare this.  It is defined in vpr/IO/Socket/InetAddr.h which may
// not be included by this file.
struct InetAddrHash;

/** \class InetAddrBSD InetAddrBSD.h vpr/IO/Socket/InetAddr.h
 *
 * Cross-platform abstraction to Internet address structures.  This is
 * typedef'd to vpr::InetAddr.
 */
class VPR_CLASS_API InetAddrBSD
{
public:
   static const InetAddrBSD AnyAddr;

   typedef vpr::InetAddrHash hash;

   /**
    * Default constructor.  This initializes the memory for the encapsulated
    * address structure.
    *
    * @post The mAddr structure has its memory zeroed, and the port and
    *       internet address are set to wildcard values.
    */
   InetAddrBSD();

   /**
    * Copy constructor.
    *
    * @post A copy of the given vpr::InetAddrBSD object is made in this
    *       object.
    *
    * @param addr The vpr::InetAddrBSD object to be copied into this object.
    */
   InetAddrBSD(const InetAddrBSD& addr)
   {
      copy(addr);
   }

   /**
    * Returns the local host's default address.
    *
    * @throw UnknownHostException if no IP address for the host could be
    *        found.
    *
    * @return A vpr::InetAddr object holding the local host's default address.
    */
   static vpr::InetAddrBSD getLocalHost();

   /**
    * Retrieves all the IPv4 addresses associated with the local machine,
    * including the loopback address (127.0.0.1) if so indicated.
    *
    * @param withLoopback A flag indicating whether to include the loopback
    *                     address (127.0.0.1) in the returned collection. This
    *                     parameter is optional and defaults to false.
    * @param withDown     A flag indicating whether the address for interfaces
    *                     in the down state should be included. This parameter
    *                     is optional and defaults to false.
    *
    * @note This method currently supports only IPv4.
    * @note The \p withDown parameter was added in version 1.1.43.
    *
    * @throw vpr::Exception is thrown if a fatal error occurs that prevents
    *        discovery of the local machine's addresses.
    *
    * @return A vector containing vpr::InetAddr objects holding all the local
    *         IPv4 addresses for the local machine.
    *
    * @since 1.1.12
    */
   static std::vector<vpr::InetAddrBSD>
      getAllLocalAddrs(const bool withLoopback = false,
                       const bool withDown = false);

   /**
    * Sets the address for this object using the given address.  It must be
    * of the form \c address:port where \c address can be a hostname or a
    * dotted-decimal IP address.
    *
    * @param address A string giving the address and port number separated by
    *                a colon.  The address can be a hostname or a
    *                dotted-decimal IP address.
    *
    * @throw UnknownHostException if no IP address for the host could be found.
    */
   void setAddress(const std::string& address);

   /**
    * Sets the address for this object using the given address and port
    * number.  The address string can be a hostname or a dotted-decimal IP
    * address.
    *
    * @param address A string giving the address (either hostname or IP
    *                address).
    * @param port    The port to associate with this IP address.
    *
    * @throw UnknownHostException if no IP address for the host could be
    *        found.
    */
   void setAddress(const std::string& address, const Uint16 port);

   /**
    * Sets the address for this object using the given address and port
    * number.  The address must be the actual 32-bit integer value.
    *
    * @param address A 32-bit integer IP address.
    * @param port    The port to associate with this IP address.
    */
   void setAddress(const vpr::Uint32 address,
                   const vpr::Uint16 port);

   /**
    * Gets the length of the address structure (if supported by the host OS).
    *
    * @pre None.
    * @post The length of the address structure is returned if the OS
    *       supports that feature.  Otherwise, 0 is returned.
    *
    * @return A value greather than 0 is return to indicate the length of the
    *         encapsulated address.  0 is returned if the OS does not support
    *         address structure length.
    */
   unsigned char getLength() const;

   /**
    * Sets the length of the address structure (if the host OS allows such an
    * operation).
    *
    * @pre None.
    * @post The address strcture's length is set to the given value if the
    *       OS supports it.  Otherwise, this is a no-op.
    *
    * @param length The length of the address structure.
    */
   void setLength(const unsigned char length);

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
   vpr::SocketTypes::Domain getFamily() const;

   /**
    * Sets the protocol family of this address.
    *
    * @pre None.
    * @post The given protocol family (a vpr::SocketTypes::Domain value) is
    *       mapped to the appropriate platform-specific protocol family
    *       value and stored.
    *
    * @param family The protocol family value.
    *
    * @throw IllegalArgumentException is thrown if \p family is not a valid
    *        protocol family or if the identified family is not supported.
    */
   void setFamily(const vpr::SocketTypes::Domain family);

   /**
    * Gets this address' port in host byte order.
    *
    * @pre The port has been initialized properly in network byte order.
    * @post The port associated with this address is returned to the caller
    *       in host byte order.
    *
    * @return An unsigned 16-bit integer giving the port for this address
    *         in host byte order.
    */
   vpr::Uint16 getPort() const;

   /**
    * Sets this address' port.  The given port must be in host byte order.
    *
    * @pre The given port number is in host byte order.
    * @post The given port number is stored in the address.
    *
    * @param port An unsigned 16-bit integer port number for this address in
    *             host byte order.
    */
   void setPort(const vpr::Uint16 port);

   /**
    * Gets this address's Internet address in host byte order.
    *
    * @pre The IP address has been initialized properly in network byte
    *      order.
    * @post The IP address associated with this address structure is
    *       returned to the caller in host byte order.
    *
    * @return An unsigned 32-bit integer giving the IP address for this
    *         object in host byte order.
    */
   vpr::Uint32 getAddressValue() const;

   /**
    * Gets the IP address associated with this object as a human-readable
    * string.
    *
    * @pre The object contains a valid IP address.
    * @post The integer IP address is converted to dotted-decmial notation
    *       and returned as a string.
    *
    * @return A std::string object containing this structure's IP address in
    *         the human-readable "dotted-decimal" notation.
    */
   std::string getAddressString() const;

   /**
    * Returns the fully qualified hostname for this address.
    * @throws UnknownHostException when host can not be found.
    */
   std::string getHostname() const;

   /**
    * Returns the fully qualified primary hostname for this address and all
    * known aliases.
    * @throws UnknownHostException when host can not be found.
    */
   std::vector<std::string> getHostnames() const;

   /**
    * Overloaded assignment operator to ensure that assignments work
    * correctly.
    *
    * @pre None.
    * @post A copy of the given vpr::InetAddrBSD object is made in this
    *       object which is then returned to the caller.
    *
    * @param addr The vpr::InetAddrBSD object to be copied into this object.
    *
    * @return A reference to this object.
    */
   InetAddrBSD& operator= (const InetAddrBSD& addr)
   {
      copy(addr);
      return *this;
   }

   /**
    * Overloaded equality operator.
    */
   bool operator== (const InetAddrBSD& addr) const;

   /**
    * Overloaded inequality operator.
    */
   bool operator!= (const InetAddrBSD& addr) const
   {
      return ! (*this == addr);
   }

protected:
   friend class vpr::SocketImplBSD;
   friend class vpr::SocketDatagramImplBSD;
   friend class vpr::SocketStreamImplBSD;

   /**
    * Constructs a vpr::InetAddrBSD object from a pointer to a sockaddr
    * struct.  This is not typically useful to users but may be very
    * important internally.
    *
    * @pre The given pointer points to a valid sockaddr struct.
    * @post The memory pointed to by addr is copied into mAddr.
    *
    * @param addr A pointer to a sockaddr struct containing a valid address.
    */
   InetAddrBSD(const struct sockaddr* addr)
   {
      setSockaddr(addr);
   }

   /**
    * Copies the given array of bytes (an A record) into this structure's IP
    * address value.  The record must be in network byte order.  This method
    * is useful when working with host entries returned by gethostname(3).
    *
    * @pre The given array of bytes represents an A record in network byte
    *      order.
    * @post The bytes are copied into this structure's IP address value.
    *
    * @param addrValue The A record contianing an IP address in network
    *                  byte order.
    */
   void copyAddressValue(const char* addrValue);

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
   void setAddressValue(const vpr::Uint32 addrValue);

   /**
    * Gets the size of this object's encapsulated address structure.
    *
    * @pre None.
    * @post The static size of mAddr is returned to the caller.
    *
    * @return A value of type size_t giving the size of the encapsualted
    *         address structure.
    */
   size_t size() const;

   /**
    * Gets the size of this object's IP address value.
    *
    * @pre None.
    * @post The static size of the IP address is returned to the caller.
    *
    * @return A value of type size_t giving the size of this object's IP
    *         address.
    */
   size_t addressSize() const;

   /**
    * Initializes the internal socket address structure using the given
    * sockaddr* object.  This will overwrite a previously set address.
    *
    * @post The memory pointed to by the given object is copied into this
    *       object's mAddr structure.
    *
    * @param addr A pointer to the sockaddr object being used to initialize
    *             this object's address.
    */
   void setSockaddr(const struct sockaddr* addr);

   /**
    * Converts this object to a sockaddr_in struct.
    *
    * @pre None.
    * @post A copy of a sockaddr_in struct equivalent to this object is
    *       returned to the caller.
    */
   struct sockaddr_in toSockaddrInet();

   /**
    * Makes a copy of the given vpr::InetAddrBSD object in this object.
    *
    * @pre None.
    * @post The memory in mAddr is overwritten with that of the given
    *       object's mAddr structure.
    *
    * @param addr The vpr::InetAddrBSD object to be copied into this object.
    */
   void copy(const InetAddrBSD& addr);

   /**
    * Looks up the given address and store the value in mAddr.
    *
    * @pre None.
    * @post The given address string is converted into a 32-bit INET
    *       address.  The mAddr member variable is populated accordingly.
    *
    * @throw UnknownHostException if address lookup failed.
    */
   void lookupAddress(const std::string& addr);

   struct sockaddr_in mAddr;    /**< The Ineternet address structure */
};

} // End of vpr namespace


#endif   /* _VPR_INET_ADDR_H_ */
