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

#ifndef _VPR_INET_ADDR_NSPR_H_
#define _VPR_INET_ADDR_NSPR_H_

#include <vpr/vprConfig.h>

#include <vpr/IO/Socket/SocketTypes.h>
#include <vpr/IO/Socket/InetAddrBase.h>

#include <prio.h>
#include <prnetdb.h>

namespace vpr {

// ----------------------------------------------------------------------------
//: Cross-platform abstraction to Internet address structures.
// ----------------------------------------------------------------------------
class VPR_CLASS_API InetAddrNSPR : public InetAddrBase
{
public:
    static const InetAddrNSPR AnyAddr;     // constructor defaults to any addr

    // ------------------------------------------------------------------------
    //: Default constructor.  This initializes the memory for the encapsulated
    //+ address structure.
    //
    //! PRE: None.
    //! POST: Zero out the address and set everything to ANY
    //
    //! ARGS: port - An unsigned 16-bit integer port number for this address
    //+              in host byte order.
    // ------------------------------------------------------------------------
    InetAddrNSPR (const Uint16 port = 0)
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
    //: Set the address for this object using the given address.  It must be
    //+ of the form <address>:<port> where <address> can be a hostname or a
    //+ dotted-decimal IP address.
    //
    //! PRE: None.
    //! POST:
    //
    //! ARGS: address - A string giving the address and port number separated
    //+                 by a colon.  The address can be a hostname or a
    //+                 dotted-decimal IP address.
    // ------------------------------------------------------------------------
    Status setAddress(const std::string& address);

    // ------------------------------------------------------------------------
    //: Set the address for this object using the given address and port
    //+ number.  The address string can be a hostname or a dotted-decimal IP
    //+ address.
    //
    //! ARGS: address - A string giving the address (either hostname or IP
    //+                 address).
    //! ARGS: port    - The port to associate with the IP address.
    // ------------------------------------------------------------------------
    inline Status setAddress (const std::string& address, const Uint16 port)
    {
       Status retval;
       retval = lookupAddress(address);
       setFamily(SocketTypes::INET);
       setPort(port);
       return retval;
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
    inline Status setAddress (const Uint32 address, const Uint16 port) {
        setAddressValue(address);
        setPort(port);
        setFamily(SocketTypes::INET);
        return Status();
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
       return PR_ntohs(PR_NetAddrInetPort(&mAddr));
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
    { PR_NetAddrInetPort(&mAddr) = PR_htons(port); }

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
    {
       mAddr = addr.mAddr;
       return *this;
    }

    // ------------------------------------------------------------------------
    //: Overloaded equality operator.
    // ------------------------------------------------------------------------
    bool
    operator== (const InetAddrNSPR& addr) const {
        return ((mAddr.inet.family == addr.mAddr.inet.family) &&
                (mAddr.inet.port == addr.mAddr.inet.port) &&
                (mAddr.inet.ip == addr.mAddr.inet.ip));
    }

    // ------------------------------------------------------------------------
    //: Overloaded inequality operator.
    // ------------------------------------------------------------------------
    bool
    operator!= (const InetAddrNSPR& addr) const {
        return ! (*this == addr);
    }

    // --- Impl specific --- //

    // Get the info needed by other NSPR commands
    PRNetAddr* getPRNetAddr()
    {
       return &mAddr;
    }

    // Get the info needed by other NSPR commands
    const PRNetAddr* getPRNetAddr() const
    {
       return &mAddr;
    }

protected:
  // -------------------------------------------------------------------------
   //: Set this structure's IP address.  The given address must be in host
   //+ byte order.
   //
   //! PRE: The given IP address is in host byte order.
   //! POST: The given IP address is stored in the address structure in
   //+       network byte order.
   //
   //! ARGS: port - An unsigned int IP address for this address object in host
   //+              byte order.
   // -------------------------------------------------------------------------
   inline void
   setAddressValue (const Uint32 addr_value)
   {
      mAddr.inet.ip = PR_htonl(addr_value);
   }

   // -------------------------------------------------------------------------
   // Look up the address in m_name and store the address in m_remote_addr.
   // -------------------------------------------------------------------------
   Status lookupAddress(const std::string& address);

   PRNetAddr   mAddr;         // Actual address
};

}; // End of vpr namespace


#endif   /* _VPR_INET_ADDR_H_ */
