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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _VPR_SIM_NETWORK_NODE_H_
#define _VPR_SIM_NETWORK_NODE_H_

#include <vpr/vprConfig.h>

#include <string>
#include <map>
#include <vpr/vpr.h>
#include <vpr/Util/Assert.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>
#include <vpr/IO/Socket/SocketTypes.h>
#include <boost/smart_ptr.hpp>


namespace vpr
{

class SocketImplSIM;

namespace sim
{

class Message;

/** \class NetworkNode NetworkNode.h vpr/md/SIM/Network/NetworkNode.h
 *
 * This class must be default constrible, assignable, and copy constructible.
 */
class NetworkNode
{
public:
   NetworkNode()
      : mIndex(0)
      , mType(0)
      , mIpAddr(0)
   {
      /* Do nothing. */ ;
   }

   /**
    * Constructs a new network node using the given paramters.  This will
    * parse the given IP address (it must be a standard IPv4 dotted-decimal
    * string) and create the corresponding integer address.
    */
   NetworkNode(const vpr::Uint32 index, const vpr::Uint8 type,
               const std::string& ip_str);

   ~NetworkNode()
   {
      /* Do nothing. */ ;
   }

   void addSocket(vpr::SocketImplSIM* sock);

   vpr::ReturnStatus removeSocket(const vpr::SocketImplSIM* sock);

   /**
    * Checks to see if this node currently has a socket of the given type in
    * use on the named port.
    */
   bool hasSocket(const vpr::Uint32 port,
                  const vpr::SocketTypes::Type type);

   vpr::SocketImplSIM* getSocket(const vpr::Uint32 port,
                                 const vpr::SocketTypes::Type type);

   const vpr::Uint32& getIndex() const
   {
      return mIndex;
   }

   void setIndex(const vpr::Uint32 index)
   {
      mIndex = index;
   }

   const vpr::Uint8& getType() const
   {
      return mType;
   }

   void setType(const vpr::Uint8 type)
   {
      mType = type;
   }

   /**
    * Retrieves the 32-bit IPv4 address for this network node.
    */
   const vpr::Uint32& getIpAddress() const
   {
      return mIpAddr;
   }

   const std::string& getIpAddressString() const
   {
      return mIpStr;
   }

   /** Get an unused TCP port number */
   vpr::Uint32 getUnassignedTcpPortNumber();

   /** Get an unused TCP port number */
   vpr::Uint32 getUnassignedUdpPortNumber();


private:
   vpr::Uint32 mIndex;
   vpr::Uint8  mType;            /**< ???? - Int value from tiers for WAN/LAN, etc*/
   std::string mIpStr;           /**< String for the IP address */
   vpr::Uint32 mIpAddr;          /**< IP Address of the node */

   
   typedef std::map<vpr::Uint32, vpr::SocketImplSIM*>  socket_map_t;
   socket_map_t mStreamSocketMap;   /**< Map of all the TCP type sockets on this node */
   socket_map_t mDgramSocketMap;    /**< Map of all the UDP type sockets on this node */

   vpr::Mutex mStreamMapMutex;
   vpr::Mutex mDgramMapMutex;
};


typedef boost::shared_ptr<NetworkNode> NetworkNodePtr;

} // End of sim namespace

} // End of vpr namespace


#endif /* _VPR_SIM_NETWORK_NODE_H_ */
