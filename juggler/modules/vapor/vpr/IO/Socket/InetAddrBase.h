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

#ifndef _VPR_INET_ADDR_BASE_H_
#define _VPR_INET_ADDR_BASE_H_

#include <vprConfig.h>

#include <IO/Socket/SocketTypes.h>
#include <Utils/Assert.h>

namespace vpr {

// ----------------------------------------------------------------------------
//: Cross-platform abstraction to Internet address structures.
// ----------------------------------------------------------------------------
class InetAddrBase {
public:
    //static const InetAddr AnyAddr;  -- NEED An AnyAddr defined
/*
  Comment out the conscturctors.  These need to be implemented in the final versions

    // ------------------------------------------------------------------------
    //: Default constructor.  This initializes the memory for the encapsulated
    //+ address structure.
    //
    //! PRE: None.
    //! POST: Zero out the address and set everything to wildcard values.
    // ------------------------------------------------------------------------
    InetAddr (void)
    { vprASSERT(false && "Implement me");}

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
    InetAddr (const std::string& address)
    {vprASSERT(false && "Implement me");}

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
    InetAddr (const std::string& address, const Uint16 port)
    {vprASSERT(false && "Implement me");}

    // ------------------------------------------------------------------------
    //: Construct an address that is associated with the given port number.
    //+ The address will be set to a wildcard.
    // ------------------------------------------------------------------------
    InetAddr (const Uint16 port)
    {vprASSERT(false && "Implement me");}

    // ------------------------------------------------------------------------
    //: Copy constructor.
    //
    //! PRE: None.
    //! POST: A copy of the given vpr::InetAddr object is made in this object.
    //
    //! ARGS: addr - The vpr::InetAddr object to be copied into this object.
    // ------------------------------------------------------------------------
    InetAddr (const InetAddr& addr)
    {vprASSERT(false && "Implement me");}
*/

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
    SocketTypes::Domain getFamily(void) const
    { vprASSERT(false && "Implement me");  return SocketTypes::LOCAL; }

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
    void setFamily(const SocketTypes::Domain family)
    { vprASSERT(false && "Implement me");}

    // ------------------------------------------------------------------------
    //: Get this address' port in host byte order.
    //
    //! PRE: The port has been initialized properly in network byte order.
    //! POST: The port associated with this address structure is returned to
    //+       the caller in host byte order.
    //
    //! RETURNS: An integer giving the port for this address structure in host
    //+          byte order.
    // ------------------------------------------------------------------------
    Uint16
    getPort (void) const
    {vprASSERT(false && "Implement me"); return 0; }

    // ------------------------------------------------------------------------
    //: Set this address' port.  The given port must be in host byte order.
    //
    //! PRE: The given port number is in host byte order.
    //! POST: The given port number is stored in the address.
    //
    //! ARGS: port - A port number for this address in host byte order.
    // ------------------------------------------------------------------------
    inline void
    setPort (const Uint16 port)
    {vprASSERT(false && "Implement me");}

    // ------------------------------------------------------------------------
    //: Get this address structure's Internet address in host byte order.
    //
    //! PRE: The IP address has been initialized properly in network byte
    //+      order.
    //! POST: The IP address associated with this address structure is
    //+       returned to the caller in host byte order.
    //
    //! RETURNS: An unsigned 32-bit integer giving the IP address for this
    //+          object in host byte order.
    // ------------------------------------------------------------------------
    inline Uint32
    getAddressValue (void) const
    {vprASSERT(false && "Implement me"); return 0;}

    // ------------------------------------------------------------------------
    //: Set this structure's IP address.  The given address must be in host
    //+ byte order.
    //
    //! PRE: The given IP address is in host byte order.
    //! POST: The given IP address is stored.
    //
    //! ARGS: port - An unsigned 32-bit integer IP address for this object in
    //+              host byte order.
    // ------------------------------------------------------------------------
    inline void
    setAddressValue (const Uint32 addr_value)
    {vprASSERT(false && "Implement me");}

    // ------------------------------------------------------------------------
    //: Get the IP address associated with this object as a human-readable
    //+ string.
    //
    //! PRE: The structure contains a valid IP address.
    //! POST: The integer IP address is converted to dotted-decmial notation
    //+       and returned as a string.
    //
    //! RETURNS: A std::string object containing this structure's IP address
    //+          in the human-readable "dotted-decimal" notation.
    // ------------------------------------------------------------------------
    std::string getAddressString(void) const
    {vprASSERT(false && "Implement me"); return std::string("");}

    // ------------------------------------------------------------------------
    //: Set the IP address for this object using the given string.  The string
    //+ can be a hostname or a dotted-decimal IP address.
    //
    //! ARGS: addr - an address string in IP format or hostname formant
    //
    //! RETURNS: true  - The address was valid and the set operation
    //+                  succeeded.
    //! RETURNS: false - The address could not be looked up.  An error message
    //+                  is printed to stderr explaining what went wrong.
    // ------------------------------------------------------------------------
    bool setAddress (const std::string& addr)
    {vprASSERT(false && "Implement me"); return false; }

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
    /*
    inline InetAddr&
    operator= (const InetAddr& addr)
    {vprASSERT(false && "Implement me");}

    bool operator==(const InetAddr& addr)
    */
};

}; // End of vpr namespace


#endif   /* _VPR_INET_ADDR_H_ */
