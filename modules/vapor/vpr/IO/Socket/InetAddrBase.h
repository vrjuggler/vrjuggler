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

#ifndef _VPR_INET_ADDR_BASE_H_
#define _VPR_INET_ADDR_BASE_H_

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/IO/Socket/SocketTypes.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/ReturnStatus.h>

namespace vpr {

/**
 * Cross-platform abstraction to Internet address structures.
 */
class VPR_CLASS_API InetAddrBase {
public:
    //static const InetAddr AnyAddr;  -- NEED An AnyAddr defined
#if 0
  Comment out the conscturctors.  These need to be implemented in the final versions

    /**
     * Default constructor.  This initializes the memory for the encapsulated
     * address structure.
     *
     * @pre None.
     * @post Zero out the address and set everything to wildcard values.
     *
     * @param port An unsigned 16-bit integer port number for this address
     *              in host byte order.
     */
    InetAddr (const vpr::Uint16 port = 0)
    { vprASSERT(false && "Implement me");}

    /**
     * Copy constructor.
     *
     * @pre None.
     * @post A copy of the given vpr::InetAddr object is made in this object.
     *
     * @param addr The vpr::InetAddr object to be copied into this object.
     */
    InetAddr (const InetAddr& addr)
    {vprASSERT(false && "Implement me");}
#endif

    /**
     * Get the protocol family of this address structure.
     *
     * @pre The protocol family has been properly initialized.
     * @post The protocol family is returned as a vpr::SocketTypes::Domain
     *       value.
     *
     * @return A vpr::SocketTypes::Domain value representing this object's
     *         protocol family.
     */
    vpr::SocketTypes::Domain getFamily(void) const
    { vprASSERT(false && "Implement me");  return vpr::SocketTypes::LOCAL; }

    /**
     * Set the protocol family of this address.
     *
     * @pre None.
     * @post The given protocol family (a vpr::SocketTypes::Domain value) is
     *       mapped to the appropriate platform-specific protocol family
     *       value and stored.
     *
     * @param family The protocol family value.
     */
    void setFamily(const vpr::SocketTypes::Domain family)
    { vprASSERT(false && "Implement me");}

    /**
     * Get this address' port in host byte order.
     *
     * @pre The port has been initialized properly in network byte order.
     * @post The port associated with this address structure is returned to
     *       the caller in host byte order.
     *
     * @return An integer giving the port for this address structure in host
     *         byte order.
     */
    vpr::Uint16
    getPort (void) const
    {vprASSERT(false && "Implement me"); return 0; }

    /**
     * Set this address' port.  The given port must be in host byte order.
     *
     * @pre The given port number is in host byte order.
     * @post The given port number is stored in the address.
     *
     * @param port A port number for this address in host byte order.
     */
    inline void
    setPort (const vpr::Uint16 port)
    {vprASSERT(false && "Implement me");}

    /**
     * Get this address structure's Internet address in host byte order.
     *
     * @pre The IP address has been initialized properly in network byte
     *      order.
     * @post The IP address associated with this address structure is
     *       returned to the caller in host byte order.
     *
     * @return An unsigned 32-bit integer giving the IP address for this
     *         object in host byte order.
     */
    inline vpr::Uint32
    getAddressValue (void) const
    {vprASSERT(false && "Implement me"); return 0;}

    /**
     * Get the IP address associated with this object as a human-readable
     * string.
     *
     * @pre The structure contains a valid IP address.
     * @post The integer IP address is converted to dotted-decmial notation
     *       and returned as a string.
     *
     * @return A std::string object containing this structure's IP address
     *         in the human-readable "dotted-decimal" notation.
     */
    std::string getAddressString(void) const
    {vprASSERT(false && "Implement me"); return std::string("");}

    /**
     * Set the address for this object using the given address.  It must be
     * of the form <address>:<port> where <address> can be a hostname or a
     * dotted-decimal IP address.
     *
     * @param address A string giving the address (either hostname or IP
     *                 address).
     * @param port    The port to associate with the IP address.
     *
     * @return <code>vpr::ReturnStatus::Succeed</code> is returned if the address
     *         was valid and the set operation succeeded.<br>
     *         <code>vpr::ReturnStatus::Fail</code> is returned if the address
     *         could not be looked up.
     */
    vpr::ReturnStatus setAddress (const std::string& addr)
    {vprASSERT(false && "Implement me"); return vpr::ReturnStatus(vpr::ReturnStatus::Fail); }

    /**
     * Set the address for this object using the given address and port
     * number.  The address string can be a hostname or a dotted-decimal IP
     * address.
     *
     * @param addr an address string in IP format or hostname formant
     *
     * @return <code>vpr::ReturnStatus::Succeed</code> is returned if the address was
     *         valid and the set operation succeeded.<br>
     *         <code>vpr::ReturnStatus::Fail</code> is returned if the address
     *         could not be looked up.
     */
    vpr::ReturnStatus setAddress (const std::string& addr, const Uint16 port)
    {vprASSERT(false && "Implement me"); return vpr::ReturnStatus(vpr::ReturnStatus::Fail); }

    /**
     * Set the address for this object using the given address and port
     * number.  The address must be the actual 32-bit integer value.
     *
     * @pre None.
     * @post
     *
     * @param address A 32-bit integer IP address.
     * @param port    The port to associate with the IP address.
     */
    vpr::ReturnStatus setAddress (const vpr::Uint32 address, const vpr::Uint16 port)
    {vprASSERT(false && "Implement me"); return vpr::ReturnStatus(vpr::ReturnStatus::Fail); }

    /**
     * Overloaded assignment operator to ensure that assignments work
     * correctly.
     *
     * @pre None.
     * @post A copy of the given vpr::InetAddr object is made in this object
     *       which is then returned to the caller.
     *
     * @param addr The vpr::InetAddr object to be copied into this object.
     *
     * @return A reference to this object.
     */
    /*
    inline InetAddr&
    operator= (const InetAddr& addr)
    {vprASSERT(false && "Implement me");}

    bool operator==(const InetAddr& addr)
    */
};

}; // End of vpr namespace


#endif   /* _VPR_INET_ADDR_H_ */
