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

#ifndef _VPR_INET_ADDR_WINSOCK_H_
#define _VPR_INET_ADDR_WINSOCK_H_

#include <vprConfig.h>

#include <string.h>
#include <string>
#include <winsock2.h>

#include <IO/Socket/InetAddrBase.h>
#include <IO/Socket/SocketTypes.h>


// XXX: Extend to work with IPv6.

namespace vpr {

// ----------------------------------------------------------------------------
//: Cross-platform abstraction to Internet address structures.
// ----------------------------------------------------------------------------
//!PUBLIC_API:
class InetAddrWinSock : public InetAddrBase
{
public:
    static const InetAddrWinSock AnyAddr;

    // ------------------------------------------------------------------------
    //: Default constructor.  This initializes the memory for the encapsulated
    //+ address structure.
    //
    //! PRE: None.
    //! POST: The m_addr structure has its memory zeroed, and the port and
    //+       internet address are set to wildcard values.
    // ------------------------------------------------------------------------
    InetAddrWinSock (void) {
        memset(&m_addr, 0, sizeof(m_addr));
        setAddressValue(INADDR_ANY);
        setPort(0);
        setFamily(SocketTypes::INET);
    }

    // ------------------------------------------------------------------------
    //: Construct an address object using the given address.  It must be of
    //+ the form <address>:<port> where <address> can be a hostname or a
    //+ dotted-decimal IP address.
    //
    //! PRE: None.
    //! POST:
    //
    //! ARGS: address - A string giving the address and port number separated
    //+                 by a colon.
    // ------------------------------------------------------------------------
    InetAddrWinSock (const std::string& address) {
        std::string::size_type pos;
        std::string host_addr, host_port;
        Uint16 port;

        pos       = address.find(":");
        host_addr = address.substr(0, pos);
        host_port = address.substr(pos + 1);
        port      = (Uint16) atoi(host_port.c_str());

        setAddress(host_addr);
        setPort(port);
        setFamily(SocketTypes::INET);
    }

    // ------------------------------------------------------------------------
    //: Construct an address object using the given address and port number.
    //+ The address string can be a hostname or a dotted-decimal IP address.
    //
    //! PRE: None.
    //! POST:
    //
    //! ARGS: address - A string giving the address (either hostname or IP
    //+                 address).
    //! ARGS: port    - The port to associate with the IP address.
    // ------------------------------------------------------------------------
    InetAddrWinSock (const std::string& address, const Uint16 port) {
        setAddress(address);
        setPort(port);
        setFamily(SocketTypes::INET);
    }

    // ------------------------------------------------------------------------
    //: Construct an address that is associated with the given port number.
    //+ The address will be set to a wildcard.
    // ------------------------------------------------------------------------
    InetAddrWinSock (const Uint16 port) {
        setAddressValue(INADDR_ANY);
        setPort(port);
        setFamily(SocketTypes::INET);
    }

    // ------------------------------------------------------------------------
    //: Construct an address object using the given address and port number.
    //+ The address must be the actual 32-bit integer value.
    //
    //! PRE: None.
    //! POST:
    //
    //! ARGS: address - A 32-bit integer IP address.
    //! ARGS: port    - The port to associate with the IP address.
    // ------------------------------------------------------------------------
    InetAddrWinSock (const Uint32 address, const Uint16 port) {
        setAddressValue(address);
        setPort(port);
        setFamily(SocketTypes::INET);
    }

    // ------------------------------------------------------------------------
    //: Construct a vpr::InetAddrWinSock object from a pointer to a sockaddr
    //+ struct.
    //
    //! PRE: The given pointer points to a valid sockaddr struct.
    //! POST: The memory pointed to by addr is copied into m_addr.
    //
    //! ARGS: addr - A pointer to a sockaddr struct
    // ------------------------------------------------------------------------
    InetAddrWinSock (const struct sockaddr* addr) {
        memcpy((void*) &m_addr, (void*) addr, sizeof(m_addr));
    }

    // ------------------------------------------------------------------------
    //: Copy constructor.
    //
    //! PRE: None.
    //! POST: A copy of the given vpr::InetAddrWinSock object is made in this
    //+       object.
    //
    //! ARGS: addr - The vpr::InetAddrWinSock object to be copied into this
    //+              object.
    // ------------------------------------------------------------------------
    InetAddrWinSock (const InetAddrWinSock& addr) {
        copy(addr);
    }

    // ------------------------------------------------------------------------
    //: Get the length of the address structure (if supported by the host OS).
    //
    //! PRE: None.
    //! POST: The length of the address structure is returned if the OS
    //+       supports that feature.  Otherwise, 0 is returned.
    //
    //! RETURNS: >0 - The length of the address structure.
    //! RETURNS: 0  - The OS does not support address structure length.
    // ------------------------------------------------------------------------
    inline unsigned char
    getLength (void) const {
#ifdef _HAVE_SIN_LEN
        return m_addr.sin_len;
#else
        return 0;
#endif
    }

    // ------------------------------------------------------------------------
    //: Set the length of the address structure (if the host OS allows such an
    //+ operation).
    //
    //! PRE: None.
    //! POST: The address strcture's length is set to the given value if the
    //+       OS supports it.  Otherwise, this is a no-op.
    //
    //! ARGS: length - The length of the address structure.
    // ------------------------------------------------------------------------
    inline void
    setLength (const unsigned char length) {
#ifdef _HAVE_SIN_LEN
        m_addr.sin_len = length;
#endif
    }

    // ------------------------------------------------------------------------
    //: Get the protocol family of this address.
    //
    //! PRE: The protocol family has been properly initialized.
    //! POST: The protocol family is returned as a vpr::SocketTypes::Domain
    //+       value.
    //
    //! RETURNS: A vpr::SocketTypes::Domain value representing this object's
    //+          protocol family.
    // ------------------------------------------------------------------------
    SocketTypes::Domain getFamily(void) const;

    // ------------------------------------------------------------------------
    //: Set the protocol family of this address.
    //
    //! PRE: None.
    //! POST: The given protocol family (a vpr::SocketTypes::Domain value) is
    //+       mapped to the appropriate platform-specific protocol family
    //+       value and stored.
    //
    //! ARGS: family - The protocol family value.
    // ------------------------------------------------------------------------
    void setFamily(const SocketTypes::Domain family);

    // ------------------------------------------------------------------------
    //: Get this address' port in host byte order.
    //
    //! PRE: The port has been initialized properly in network byte order.
    //! POST: The port associated with this address is returned to the caller
    //+       in host byte order.
    //
    //! RETURNS: A Uint16 (unsigned 16-bit value) giving the port for this
    //+          address structure in host byte order.
    // ------------------------------------------------------------------------
    inline Uint16
    getPort (void) const {
        return ntohs(m_addr.sin_port);
    }

    // ------------------------------------------------------------------------
    //: Set this address' port.  The given port must be in host byte order.
    //
    //! PRE: The given port number is in host byte order.
    //! POST: The given port number is stored in the address structure.
    //
    //! ARGS: port - An unsigned 16-bit integer port number for this address
    //+              in host byte order.
    // ------------------------------------------------------------------------
    inline void
    setPort (const Uint16 port) {
        m_addr.sin_port = htons(port);
    }

    // ------------------------------------------------------------------------
    //: Get this address's Internet address in host byte order.
    //
    //! PRE: The IP address has been initialized properly in network byte
    //+      order.
    //! POST: The IP address associated with this address structure is
    //+       returned to the caller in host byte order.
    //
    //! RETURNS: An unsigned 32-bit integer giving the IP address for this
    //+          address structure in host byte order.
    // ------------------------------------------------------------------------
    inline Uint32
    getAddressValue (void) const {
        return ntohl(m_addr.sin_addr.s_addr);
    }

    // ------------------------------------------------------------------------
    //: Set this object's IP address.  The given address must be in host byte
    //+ order.
    //
    //! PRE: The given IP address is in host byte order.
    //! POST: The given IP address is stored.
    //
    //! ARGS: port - An unsigned 32-bit integer IP address for this object in
    //+              host byte order.
    // ------------------------------------------------------------------------
    inline void
    setAddressValue (const Uint32 addr_value) {
        m_addr.sin_addr.s_addr = htonl(addr_value);
    }

    // ------------------------------------------------------------------------
    //: Copy the given array of bytes (an A record) into this structure's IP
    //+ address value.  The record must be in network byte order.  This method
    //+ is useful when working with host entries returned by gethostname(3).
    //
    //! PRE: The given array of bytes represents an A record in network byte
    //+      order.
    //! POST: The bytes are copied into this structure's IP address value.
    //
    //! ARGS: addr_value - The A record contianing an IP address in network
    //+                    byte order.
    // ------------------------------------------------------------------------
    inline void
    copyAddressValue (const char* addr_value) {
        memcpy((void*) &m_addr.sin_addr.s_addr, (void*) addr_value,
               sizeof(m_addr.sin_addr.s_addr));
    }

    // ------------------------------------------------------------------------
    //: Get the IP address associated with this structure as a human-readable
    //+ string.
    //
    //! PRE: The structure contains a valid IP address.
    //! POST: The integer IP address is converted to dotted-decmial notation
    //+       and returned as a string.
    //
    //! RETURNS: A std::string object containing this structure's IP address
    //+          in the human-readable "dotted-decimal" notation.
    // ------------------------------------------------------------------------
    std::string getAddressString(void) const;

    // ------------------------------------------------------------------------
    //: Set the IP address for this object using the given string.  The string
    //+ can be a hostname or a dotted-decimal IP address.
    //
    //! PRE: None.
    //! POST: If the address is valid, the object's IP address is updated
    //+       appropriately.
    //
    //! ARGS: addr - An address string in dotted-decimal address notation or
    //+              as a hostname.
    //
    //! RETURNS: true  - The address was valid and the set operation
    //+                  succeeded.
    //! RETURNS: false - The address could not be looked up.  An error message
    //+                  is printed to stderr explaining what went wrong.
    // ------------------------------------------------------------------------
    inline bool
    setAddress (const std::string& addr) {
        return lookupAddress(addr);
    }

    // ------------------------------------------------------------------------
    //: Get the size of this object's encapsulated address structure.
    //
    //! PRE: None.
    //! POST: The static size of m_addr is returned to the caller.
    //
    //! RETURNS: A value of type size_t giving the size of the encapsualted
    //+          address structure.
    // ------------------------------------------------------------------------
    inline size_t
    size (void) const {
        return sizeof(m_addr);
    }

    // ------------------------------------------------------------------------
    //: Get the size of this object's IP address value.
    //
    //! PRE: None.
    //! POST: The static size of the IP address is returned to the caller.
    //
    //! RETURNS: A value of type size_t giving the size of this object's IP
    //+          address.
    // ------------------------------------------------------------------------
    inline size_t
    addressSize (void) const {
        return sizeof(m_addr.sin_addr.s_addr);
    }

    // ------------------------------------------------------------------------
    //: Convert this object to a sockaddr_in struct.
    //
    //! PRE: None.
    //! POST: A copy of a sockaddr_in struct equivalent to this object is
    //+       returned to the caller.
    // ------------------------------------------------------------------------
    inline struct sockaddr_in
    toSockaddrInet (void) {
        return m_addr;
    }

    // ------------------------------------------------------------------------
    //: Overloaded assignment operator to ensure that assignments work
    //+ correctly.
    //
    //! PRE: None.
    //! POST: A copy of the given vpr::InetAddrWinSock object is made in this
    //+       object which is then returned to the caller.
    //
    //! ARGS: addr - The vpr::InetAddrWinSock object to be copied into this
    //+              object.
    //
    //! RETURNS: A reference to this object.
    // ------------------------------------------------------------------------
    inline InetAddrWinSock&
    operator= (const InetAddrWinSock& addr) {
        copy(addr);
        return *this;
    }

    // ------------------------------------------------------------------------
    //: Overloaded equality operator.
    // ------------------------------------------------------------------------
    bool
    operator== (const InetAddrWinSock& addr) {
        return ((m_addr.sin_addr.s_addr == addr.m_addr.sin_addr.s_addr) &&
                (m_addr.sin_port == addr.m_addr.sin_port) &&
                (m_addr.sin_family == addr.m_addr.sin_family));
    }

    // ------------------------------------------------------------------------
    //: Overloaded inequality operator.
    // ------------------------------------------------------------------------
    bool
    operator!= (const InetAddrWinSock& addr) {
        return ! (*this == addr);
    }

//protected:
    struct sockaddr_in m_addr;    //: The Ineternet address structure

protected:
    // ------------------------------------------------------------------------
    //: Make a copy of the given vpr::InetAddrWinSock object in this object.
    //
    //! PRE: None.
    //! POST: The memory in m_addr is overwritten with that of the given
    //+       object's m_addr structure.
    //
    //! ARGS: addr - The vpr::InetAddrWinSock object to be copied into this
    //+              object.
    // ------------------------------------------------------------------------
    inline void
    copy (const InetAddrWinSock& addr) {
        memcpy((void*) &m_addr, (void*) &addr.m_addr, sizeof(m_addr));
    }

    // ------------------------------------------------------------------------
    //: Look up the given address and store the value in m_addr.
    //
    //! PRE: None.
    //! POST: The given address string is converted into a 32-bit INET
    //+       address.  The m_addr member variable is populated accordingly.
    //
    //! RETURNS: true  - The address lookup was successful.
    //! RETURNS: false - The address could not be looked up.  An error message
    //+                  is printed to stderr explaining what went wrong.
    // ------------------------------------------------------------------------
    bool lookupAddress(const std::string& addr);
};

}; // End of vpr namespace


#endif   /* _VPR_INET_ADDR_WINSOCK_H_ */
