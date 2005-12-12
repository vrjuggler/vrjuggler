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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <string>
#include <vector>
#include <prio.h>
#include <prnetdb.h>

#include <vpr/IO/Socket/SocketTypes.h>
#include <vpr/IO/Socket/InetAddrBase.h>
#include <vpr/Util/IllegalArgumentException.h>


namespace vpr
{

// Forward declare this.  It is defined in vpr/IO/Socket/InetAddr.h which may
// not be included by this file.
struct InetAddrHash;

/** \class InetAddrNSPR InetAddrNSPR.h vpr/IO/Socket/InetAddr.h
 *
 * Cross-platform abstraction to Internet address structures.  This is
 * typedef'd to vpr::InetAddr.
 */
class VPR_CLASS_API InetAddrNSPR : public InetAddrBase
{
public:
   static const InetAddrNSPR AnyAddr;     // constructor defaults to any addr

   typedef vpr::InetAddrHash hash;

   /**
    * Default constructor.
    *
    * @post Zero out the address and set it to \c PR_INADDR_ANY.
    */
   InetAddrNSPR()
   {
      memset(&mAddr, 0, sizeof(mAddr));
      setFamily(SocketTypes::INET);
      setAddressValue(PR_INADDR_ANY);
      setPort(0);
   }

   /**
    * Copy constructor.
    *
    * @post A copy of the given vpr::InetAddr object is made in this object.
    *
    * @param addr The vpr::InetAddr object to be copied into this object.
    */
   InetAddrNSPR(const InetAddrNSPR& addr)
   {
      mAddr = addr.mAddr;
   }

   /**
    * Returns the local host's default address via the given object reference.
    *
    * @param hostAddr Storage for the returned address object.
    *
    * @throw UnknownHostException If no IP address for the host could be found.
    */
   static void getLocalHost(vpr::InetAddrNSPR& hostAddr)
      throw (UnknownHostException);

   /**
    * Retrieves all the IPv4 addresses associated with the local machine,
    * including the loopback address (127.0.0.1) if so indicated.
    *
    * @post \p hostAddrs contains vpr::InetAddr objetcs holding all the local
    *       IPv4 addresses for the local machine.
    *
    * @param hostAddrs    Storage for the discovered local IPv4 addresses.
    *                     The vector is cleared before the addresses are
    *                     added, so any objects currently in the vector are
    *                     lost.
    * @param withLoopback A flag indicating whether to include the loopback
    *                     address (127.0.0.1) in \p hostAddrs. This parameter
    *                     is optional and defaults to false.
    *
    * @note This method currently supports only IPv4.
    *
    * @throw vpr::Exception is thrown if a fatal error occurs that prevents
    *        discovery of the local machine's addresses.
    *
    * @since 1.1.12
    */
   static void getAllLocalAddrs(std::vector<vpr::InetAddrNSPR>& hostAddrs,
                                const bool withLoopback = false);

   /**
    * Sets the address for this object using the given address.  It must be
    * of the form \c address:port where \c address can be a hostname or a
    * dotted-decimal IP address.
    *
    * @param address A string giving the address and port number separated by
    *                a colon.  The address can be a hostname or a
    *                dotted-decimal IP address.
    *
    * @throw UnknownHostException If no IP address for the host could be found.
    */
   void setAddress(const std::string& address) throw (UnknownHostException);

   /**
    * Sets the address for this object using the given address and port
    * number.  The address string can be a hostname or a dotted-decimal IP
    * address.
    *
    * @param address A string giving the address (either hostname or IP
    *                address).
    * @param port    The port to associate with the IP address.
    *
    * @throw UnknownHostException If no IP address for the host could be
    *                             found.
    */
   void setAddress(const std::string& address, const vpr::Uint16 port)
      throw (UnknownHostException)
   {
      lookupAddress(address);
      setFamily(SocketTypes::INET);
      setPort(port);
   }

   /**
    * Constructs an address object using the given address and port number.
    * The address must be the actual 32-bit integer value.
    *
    * @param address A 32-bit integer IP address.
    * @param port    The port to associate with the IP address.
    */
   void setAddress(const vpr::Uint32 address, const vpr::Uint16 port)
   {
      setAddressValue(address);
      setPort(port);
      setFamily(SocketTypes::INET);
   }

   /**
    * Gets the protocol family of this address structure.
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
    * Sets the protocol family of this address structure.
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
   void setFamily(const vpr::SocketTypes::Domain family)
      throw (IllegalArgumentException);

   /**
    * Get this address' port in host byte order.
    *
    * @pre The port has been initialized properly in network byte order.
    * @post The port associated with this address structure is returned to
    *       the caller in host byte order.
    *
    * @return A Uint16 giving the port for this address structure in host byte
    *         order.
    */
   vpr::Uint16 getPort() const
   {
      return PR_ntohs(PR_NetAddrInetPort(&mAddr));
   }

   /**
    * Sets this address' port.  The given port must be in host byte order.
    *
    * @pre The given port number is in host byte order.
    * @post The given port number is stored in the address structure in
    *       network byte order.
    *
    * @param port A Uint16 port number for this address structure in host byte
    *             order.
    */
   void setPort(const vpr::Uint16 port)
   {
      PR_NetAddrInetPort(&mAddr) = PR_htons(port);
   }

   /**
    * Gets this address structure's Internet address in host byte order.
    *
    * @pre The IP address has been initialized properly in network byte
    *      order.
    * @post The IP address associated with this address structure is
    *       returned to the caller in host byte order.
    *
    * @return An unsigned int (32-bit value) giving the IP address for this
    *         address structure in host byte order.
    */
   vpr::Uint32 getAddressValue() const
   {
      return PR_ntohl(mAddr.inet.ip);
   }

   /**
    * Gets the IP address associated with this structure as a human-readable
    * string.
    *
    * @pre The structure contains a valid IP address.
    * @post The integer IP address is converted to dotted-decmial notation
    *       and returned as a string.
    *
    * @return A std::string object containing this structure's IP address in
    *         the human-readable "dotted-decimal" notation.
    */
   std::string getAddressString() const;

   /**
    * Returns the fully qualified hostname for this address.
    */
   void getHostname(std::string& hostname) const throw (UnknownHostException);

   /**
    * Returns the fully qualified primary hostname for this address and all
    * known aliases.
    */
   std::vector<std::string> getHostnames() const throw (UnknownHostException);

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
   InetAddrNSPR& operator= (const InetAddrNSPR& addr)
   {
      mAddr = addr.mAddr;
      return *this;
   }

   /**
    * Overloaded equality operator.
    */
   bool operator== (const InetAddrNSPR& addr) const
   {
      return((mAddr.inet.family == addr.mAddr.inet.family) &&
             (mAddr.inet.port == addr.mAddr.inet.port) &&
             (mAddr.inet.ip == addr.mAddr.inet.ip));
   }

   /**
    * Overloaded inequality operator.
    */
   bool operator!= (const InetAddrNSPR& addr) const
   {
      return ! (*this == addr);
   }

   // --- Impl specific --- //

   /** Gets the info needed by other NSPR commands. */
   PRNetAddr* getPRNetAddr()
   {
      return &mAddr;
   }

   /** Gets the info needed by other NSPR commands. */
   const PRNetAddr* getPRNetAddr() const
   {
      return &mAddr;
   }

protected:
   /**
    * Sets this structure's IP address.  The given address must be in host
    * byte order.
    *
    * @pre The given IP address is in host byte order.
    * @post The given IP address is stored in the address structure in
    *       network byte order.
    *
    * @param addrValue An unsigned int IP address for this address object in
    *                  host byte order.
    */
   void setAddressValue(const vpr::Uint32 addrValue)
   {
      mAddr.inet.ip = PR_htonl(addrValue);
   }

   /**
    * Looks up the address in mName and store the address in mAddr.
    *
    * @post The given address string is converted into a 32-bit INET
    *       address.  The mAddr member variable is populated accordingly.
    *
    * @throw UnknownHostException If address lookup failed.
    */
   void lookupAddress(const std::string& address) throw (UnknownHostException);

   PRNetAddr   mAddr;         /**< Actual address */
};

} // End of vpr namespace


#endif   /* _VPR_INET_ADDR_H_ */
