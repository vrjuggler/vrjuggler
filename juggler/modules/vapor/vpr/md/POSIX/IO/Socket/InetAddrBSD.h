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

#ifndef _VPR_INET_ADDR_BSD_H_
#define _VPR_INET_ADDR_BSD_H_

#include <vpr/vprConfig.h>

#include <string.h>
#include <string>
#include <sys/types.h>
#include <netinet/in.h>

// I don't know why this is necessary, but I think something is being defined
// incorrectly somewhere.
#ifdef VPR_OS_IRIX
#  include <sys/endian.h>
#endif

#include <vpr/IO/Socket/InetAddrBase.h>
#include <vpr/IO/Socket/SocketTypes.h>


// XXX: Extend to work with IPv6.

namespace vpr
{

/**
 * Cross-platform abstraction to Internet address structures.
 */
class InetAddrBSD : public InetAddrBase
{
public:
   static const InetAddrBSD AnyAddr;

   /**
    * Default constructor.  This initializes the memory for the encapsulated
    * address structure.
    *
    * @pre None.
    * @post The m_addr structure has its memory zeroed, and the port and
    *       internet address are set to wildcard values.
    *
    * @param port An unsigned 16-bit integer port number for this address in
    *             host byte order.
    */
   InetAddrBSD (const Uint16 port = 0)
   {
      memset(&m_addr, 0, sizeof(m_addr));
      setAddressValue(INADDR_ANY);
      setPort(port);
      setFamily(SocketTypes::INET);
   }

   /**
    * Copy constructor.
    *
    * @pre None.
    * @post A copy of the given vpr::InetAddrBSD object is made in this
    *       object.
    *
    * @param addr The vpr::InetAddrBSD object to be copied into this object.
    */
   InetAddrBSD (const InetAddrBSD& addr)
   {
      copy(addr);
   }

   /**
    * Sets the address for this object using the given address.  It must be
    * of the form <address>:<port> where <address> can be a hostname or a
    * dotted-decimal IP address.
    *
    * @param address A string giving the address and port number separated by
    *                a colon.  The address can be a hostname or a
    *                dotted-decimal IP address.
    */
   vpr::ReturnStatus setAddress(const std::string& address);

   /**
    * Sets the address for this object using the given address and port
    * number.  The address string can be a hostname or a dotted-decimal IP
    * address.
    *
    * @param address A string giving the address (either hostname or IP
    *                address).
    * @param port    The port to associate with the IP address.
    */
   vpr::ReturnStatus setAddress (const std::string& address, const Uint16 port)
   {
      vpr::ReturnStatus retval;

      retval = lookupAddress(address);
      setPort(port);
      setFamily(SocketTypes::INET);

      return retval;
   }

   /**
    * Sets the address for this object using the given address and port
    * number.  The address must be the actual 32-bit integer value.
    *
    * @param address A 32-bit integer IP address.
    * @param port    The port to associate with the IP address.
    */
   vpr::ReturnStatus setAddress (const vpr::Uint32 address,
                                 const vpr::Uint16 port)
   {
      setAddressValue(address);
      setPort(port);
      setFamily(SocketTypes::INET);
      return ReturnStatus();
   }

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
   unsigned char getLength (void) const
   {
#ifdef _HAVE_SIN_LEN
      return m_addr.sin_len;
#else
      return 0;
#endif
   }

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
   void setLength (const unsigned char length)
   {
#ifdef _HAVE_SIN_LEN
      m_addr.sin_len = length;
#endif
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
   vpr::SocketTypes::Domain getFamily(void) const;

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
   void setFamily(const vpr::SocketTypes::Domain family);

   /**
    * Gets this address' port in host byte order.
    *
    * @pre The port has been initialized properly in network byte order.
    * @post The port associated with this address is returned to the caller
    *       in host byte order.
    *
    * @return An unsigned 16-bit integer  giving the port for this address
    *         in host byte order.
    */
   vpr::Uint16 getPort (void) const
   {
      return ntohs(m_addr.sin_port);
   }

   /**
    * Sets this address' port.  The given port must be in host byte order.
    *
    * @pre The given port number is in host byte order.
    * @post The given port number is stored in the address.
    *
    * @param port An unsigned 16-bit integer port number for this address in
    *             host byte order.
    */
   void setPort (const vpr::Uint16 port)
   {
      m_addr.sin_port = htons(port);
   }

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
   vpr::Uint32 getAddressValue (void) const
   {
      return ntohl(m_addr.sin_addr.s_addr);
   }

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
   std::string getAddressString(void) const;

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
   bool operator== (const InetAddrBSD& addr) const
   {
      return((m_addr.sin_addr.s_addr == addr.m_addr.sin_addr.s_addr) &&
             (m_addr.sin_port == addr.m_addr.sin_port) &&
             (m_addr.sin_family == addr.m_addr.sin_family));
   }

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
    * @post The memory pointed to by addr is copied into m_addr.
    *
    * @param addr A pointer to a sockaddr struct containing a valid address.
    */
   InetAddrBSD (const struct sockaddr* addr)
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
    * @param addr_value The A record contianing an IP address in network
    *                   byte order.
    */
   void copyAddressValue (const char* addr_value)
   {
      vprASSERT(addr_value != NULL);
      memcpy((void*) &m_addr.sin_addr.s_addr, (void*) addr_value,
             sizeof(m_addr.sin_addr.s_addr));
   }

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
   void setAddressValue (const vpr::Uint32 addr_value)
   {
      m_addr.sin_addr.s_addr = htonl(addr_value);
   }

   /**
    * Gets the size of this object's encapsulated address structure.
    *
    * @pre None.
    * @post The static size of m_addr is returned to the caller.
    *
    * @return A value of type size_t giving the size of the encapsualted
    *         address structure.
    */
   size_t size (void) const
   {
      return sizeof(m_addr);
   }

   /**
    * Gets the size of this object's IP address value.
    *
    * @pre None.
    * @post The static size of the IP address is returned to the caller.
    *
    * @return A value of type size_t giving the size of this object's IP
    *         address.
    */
   size_t addressSize (void) const
   {
      return sizeof(m_addr.sin_addr.s_addr);
   }

   /**
    * Initializes the internal socket address structure using the given
    * sockaddr* object.  This will overwrite a previously set address.
    *
    * @post The memory pointed to by the given object is copied into this
    *       object's m_addr structure.
    *
    * @param addr A pointer to the sockaddr object being used to initialize
    *             this object's address.
    */
   void setSockaddr (const struct sockaddr* addr)
   {
      memcpy((void*) &m_addr, (void*) addr, sizeof(m_addr));
   }

   /**
    * Converts this object to a sockaddr_in struct.
    *
    * @pre None.
    * @post A copy of a sockaddr_in struct equivalent to this object is
    *       returned to the caller.
    */
   struct sockaddr_in toSockaddrInet (void)
   {
      return m_addr;
   }

   /**
    * Makes a copy of the given vpr::InetAddrBSD object in this object.
    *
    * @pre None.
    * @post The memory in m_addr is overwritten with that of the given
    *       object's m_addr structure.
    *
    * @param addr The vpr::InetAddrBSD object to be copied into this object.
    */
   void copy (const InetAddrBSD& addr)
   {
      memcpy((void*) &m_addr, (void*) &addr.m_addr, sizeof(m_addr));
   }

   /**
    * Looks up the given address and store the value in m_addr.
    *
    * @pre None.
    * @post The given address string is converted into a 32-bit INET
    *       address.  The m_addr member variable is populated accordingly.
    *
    * @return vpr::ReturnStatus::Succeed is returned if the address lookup was
    *         successful.  vpr::ReturnStatus::Fail is returned otherwise.
    */
   vpr::ReturnStatus lookupAddress(const std::string& addr);

   struct sockaddr_in m_addr;    /**< The Ineternet address structure */
};

} // End of vpr namespace


#endif   /* _VPR_INET_ADDR_H_ */
