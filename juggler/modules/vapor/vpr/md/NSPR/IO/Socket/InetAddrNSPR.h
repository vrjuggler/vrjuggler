
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

#ifndef _VPR_INET_ADDR_NSPR_H_
#define _VPR_INET_ADDR_NSPR_H_

#include <IO/Socket/SocketTypes.h>
#include <Utils/Assert.h>

#include <prio.h>

namespace vpr {

// ----------------------------------------------------------------------------
//: Cross-platform abstraction to Internet address structures.
// ----------------------------------------------------------------------------
class InetAddrNSPR : public InetAddrBase
{
public:
    static const InetAddr AnyAddr;     // constructor defaults to any addr

    // ------------------------------------------------------------------------
    //: Default constructor.  This initializes the memory for the encapsulated
    //+ address structure.
    //
    //! PRE: None.
    //! POST: Zero out the address and set everything to ANY
    // ------------------------------------------------------------------------
    InetAddrNSPR (void)
    {
       memset(&mAddr, 0, sizeof(mAddr));
       setFamily(SocketTypes::INET);
       setAddressValue(PR_INADDR_ANY);
       setPort(PR_INADDR_ANY);
    }

    // ------------------------------------------------------------------------
    // Set address to a string like "hostname.domain.com:<port_num>"
    // ------------------------------------------------------------------------
    InetAddrNSPR (const std::string& address)
    {
       memset(&mAddr, 0, sizeof(mAddr));
       setFamily(SocketTypes::INET);
       setAddress(address);
       setPort(PR_INADDR_ANY);
    }

    // ------------------------------------------------------------------------
    // Initialize to address and port
    // ------------------------------------------------------------------------
    InetAddrNSPR (const std::string& address, const Uint16 port)
    {
       memset(&mAddr, 0, sizeof(mAddr));
       setFamily(SocketTypes::INET);
       setAddress(address);
       setPort(port);
    }

    // ------------------------------------------------------------------------
    // Initialize to an ANY address and a given port
    // ------------------------------------------------------------------------
    InetAddrNSPR (const Uint16 port)
    {
       memset(&mAddr, 0, sizeof(mAddr));
       setFamily(SocketTypes::INET);
       setAddressValue(PR_INADDR_ANY);
       setPort(port);
    }

    // ------------------------------------------------------------------------
    //: Copy constructor.
    //
    //! PRE: None.
    //! POST: A copy of the given vpr::InetAddr object is made in this object.
    //
    //! ARGS: addr - The vpr::InetAddr object to be copied into this object.
    // ------------------------------------------------------------------------
    InetAddrNSPR (const InetAddrNSPR& addr)
    {
       mAddr = addr.mAddr;
    }

    // ------------------------------------------------------------------------
    //: Get the protocol family of this address structure.
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
    //: Set the protocol family of this address structure.
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
    //! POST: The port associated with this address structure is returned to
    //+       the caller in host byte order.
    //
    //! RETURNS: A Uint16 giving the port for this address structure in host byte order.
    // ------------------------------------------------------------------------
    Uint16
    getPort (void) const
    {
       return PR_ntohs(PR_NetAddrInetPort(mAddr.inet.port));
    }

    // ------------------------------------------------------------------------
    //: Set this address' port.  The given port must be in host byte order.
    //
    //! PRE: The given port number is in host byte order.
    //! POST: The given port number is stored in the address structure in
    //+       network byte order.
    //
    //! ARGS: port - A Uint16 port number for this
    //+              address structure in host byte order.
    // ------------------------------------------------------------------------
    inline void
    setPort (const Uint16 port)
    { PR_NetAddrInetPort(mAddr) = PR_htons(port); }

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
    inline Uint32
    getAddressValue (void) const
    {
       return PR_ntohl(mAddr.inet.ip);
    }

    // ------------------------------------------------------------------------
    //: Set this structure's IP address.  The given address must be in host
    //+ byte order.
    //
    //! PRE: The given IP address is in host byte order.
    //! POST: The given IP address is stored in the address structure in
    //+       network byte order.
    //
    //! ARGS: port - An unsigned int IP address for this
    //+              address structure in host byte order.
    // ------------------------------------------------------------------------
    inline void
    setAddressValue (const Uint32 addr_value)
    {
       mAddr.inet.ip = PR_htonl(addr_value);
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
    //
    // ARGS: addr - an address string in IP format or hostname formant
    //
    // Returns:
    //     true  - The address lookup was successful.
    //     false - The address could not be looked up.  An error message is
    //             printed to stderr explaining what went wrong.
    // ------------------------------------------------------------------------
    bool setAddress (const std::string& addr);

    // ------------------------------------------------------------------------
    //: Overloaded assignment operator to ensure that assignments work
    //+ correctly.
    //
    //! PRE: None.
    //! POST: A copy of the given vpr::InetAddr object is made in this object
    //+       which is then returned to the caller.
    //
    //! ARGS: addr - The vpr::InetAddr object to be copied into this object.
    //
    //! RETURNS: A reference to this object.
    // ------------------------------------------------------------------------
    InetAddrNSPR&
    operator= (const InetAddrNSPR& addr)
    { mAddr = addr.mAddr;}

    // --- Impl specific --- //

    // Get the info needed by other NSPR commands
    PRNetAddr* getPRNetAddr()
    {
       return &mAddr;
    }

protected:
   PRNetAddr   mAddr;         // Actual address
};

}; // End of vpr namespace


#endif   /* _VPR_INET_ADDR_H_ */
