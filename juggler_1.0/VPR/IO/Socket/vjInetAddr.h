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

#ifndef _VJ_INET_ADDR_H_
#define _VJ_INET_ADDR_H_

#include <vjConfig.h>

#include <string.h>
#include <string>

#if defined __WIN32__ || defined WIN32 || defined _Windows || defined _WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <netinet/in.h>
#endif

#include <VPR/IO/Socket/vjSocketTypes.h>


// XXX: Extend to work with IPv6.

// ----------------------------------------------------------------------------
//: Cross-platform abstraction to Internet address structures.
// ----------------------------------------------------------------------------
//!PUBLIC_API:
class VJ_CLASS_API vjInetAddr {
public:
    // ------------------------------------------------------------------------
    //: Default constructor.  This initializes the memory for the encapsulated
    //+ address structure.
    //
    //! PRE: None.
    //! POST: The m_addr structure has its memory zeroed.
    // ------------------------------------------------------------------------
    vjInetAddr (void) {
        memset(&m_addr, 0, sizeof(m_addr));
    }

    // ------------------------------------------------------------------------
    //: Construct a vjInetAddr object from a pointer to a sockaddr struct.
    //
    //! PRE: The given pointer points to a valid sockaddr struct.
    //! POST: The memory pointed to by addr is copied into m_addr.
    //
    //! ARGS: addr - A pointer to a sockaddr struct
    // ------------------------------------------------------------------------
    vjInetAddr (struct sockaddr* addr) {
        memcpy((void*) &m_addr, (void*) addr, sizeof(m_addr));
    }

    // ------------------------------------------------------------------------
    //: Copy constructor.
    //
    //! PRE: None.
    //! POST: A copy of the given vjInetAddr object is made in this object.
    //
    //! ARGS: addr - The vjInetAddr object to be copied into this object.
    // ------------------------------------------------------------------------
    vjInetAddr (const vjInetAddr& addr) {
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
    //: Get the protocol family of this address structure.
    //
    //! PRE: The protocol family has been properly initialized.
    //! POST: The protocol family is returned as a vjSocketTypes::Domain
    //+       value.
    //
    //! RETURNS: A vjSocketTypes::Domain value representing this object's
    //+          protocol family.
    // ------------------------------------------------------------------------
    vjSocketTypes::Domain getFamily(void) const;

    // ------------------------------------------------------------------------
    //: Set the protocol family of this address structure.
    //
    //! PRE: None.
    //! POST: The given protocol family (a vjSocketTypes::Domain value) is
    //+       mapped to the appropriate platform-specific protocol family
    //+       value and stored.
    //
    //! ARGS: family - The protocol family value.
    // ------------------------------------------------------------------------
    void setFamily(const vjSocketTypes::Domain family);

    // ------------------------------------------------------------------------
    //: Get this address' port in host byte order.
    //
    //! PRE: The port has been initialized properly in network byte order.
    //! POST: The port associated with this address structure is returned to
    //+       the caller in host byte order.
    //
    //! RETURNS: An unsigned short (16-bit value) giving the port for this
    //+          address structure in host byte order.
    // ------------------------------------------------------------------------
    inline unsigned short
    getPort (void) const {
        return ntohs(m_addr.sin_port);
    }

    // ------------------------------------------------------------------------
    //: Set this address' port.  The given port must be in host byte order.
    //
    //! PRE: The given port number is in host byte order.
    //! POST: The given port number is stored in the address structure in
    //+       network byte order.
    //
    //! ARGS: port - An unsigned short (16-bit value) port number for this
    //+              address structure in host byte order.
    // ------------------------------------------------------------------------
    inline void
    setPort (const unsigned short port) {
        m_addr.sin_port = htons(port);
    }

    // ------------------------------------------------------------------------
    //: Get this address structure's Internet address in host byte order.
    //
    //! PRE: The IP address has been initialized properly in network byte
    //+      order.
    //! POST: The IP address associated with this address structure is
    //+       returned to the caller in host byte order.
    //
    //! RETURNS: An unsigned int (32-bit value) giving the IP address for this
    //+          address structure in host byte order.
    // ------------------------------------------------------------------------
    inline unsigned int
    getAddressValue (void) const {
        return ntohl(m_addr.sin_addr.s_addr);
    }

    // ------------------------------------------------------------------------
    //: Set this structure's IP address.  The given address must be in host
    //+ byte order.
    //
    //! PRE: The given IP address is in host byte order.
    //! POST: The given IP address is stored in the address structure in
    //+       network byte order.
    //
    //! ARGS: port - An unsigned int (32-bit value) IP address for this
    //+              address structure in host byte order.
    // ------------------------------------------------------------------------
    inline void
    setAddressValue (const unsigned int addr_value) {
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
    //! POST: A copy of the given vjInetAddr object is made in this object
    //+       which is then returned to the caller.
    //
    //! ARGS: addr - The vjInetAddr object to be copied into this object.
    //
    //! RETURNS: A reference to this object.
    // ------------------------------------------------------------------------
    inline vjInetAddr&
    operator= (const vjInetAddr& addr) {
        copy(addr);
        return *this;
    }

//protected:
    struct sockaddr_in m_addr;    //: The Ineternet address structure

protected:
    // ------------------------------------------------------------------------
    //: Make a copy of the given vjInetAddr object in this object.
    //
    //! PRE: None.
    //! POST: The memory in m_addr is overwritten with that of the given
    //+       object's m_addr structure.
    //
    //! ARGS: addr - The vjInetAddr object to be copied into this object.
    // ------------------------------------------------------------------------
    inline void
    copy (const vjInetAddr& addr) {
        memcpy((void*) &m_addr, (void*) &addr.m_addr, sizeof(m_addr));
    }
};


#endif	/* _VJ_INET_ADDR_H_ */
