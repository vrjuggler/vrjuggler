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

#ifndef _VPR_SIM_NETWORK_NODE_H_
#define _VPR_SIM_NETWORK_NODE_H_

#include <vpr/vprConfig.h>

#include <string>
#include <map>
#include <vpr/vpr.h>
#include <vpr/Util/Assert.h>
#include <vpr/IO/Socket/SocketTypes.h>


namespace vpr
{

class SocketImplSIM;

namespace sim
{

class Message;

/**
 * This class must be default constrible, assignable, and copy constructible.
 */
class NetworkNode
{
public:
   NetworkNode (void)
      : mIndex(0), mType(0), mIpAddr(0)
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

   ~NetworkNode (void)
   {
      /* Do nothing. */ ;
   }

   void addSocket(vpr::SocketImplSIM* sock);

   /**
    * Checks to see if this node currently has a socket of the given type in
    * use on the named port.
    */
   bool hasSocket(const vpr::Uint32 port,
                  const vpr::SocketTypes::Type type) const;

   vpr::SocketImplSIM* getSocket(const vpr::Uint32 port,
                                 const vpr::SocketTypes::Type type) const;

   const vpr::Uint32& getIndex (void) const
   {
      return mIndex;
   }

   void setIndex (const vpr::Uint32 index)
   {
      mIndex = index;
   }

   const vpr::Uint8& getType (void) const
   {
      return mType;
   }

   void setType (const vpr::Uint8 type)
   {
      mType = type;
   }

   /**
    * Retrieves the 32-bit IPv4 address for this network node.
    */
   const vpr::Uint32& getIpAddress (void) const
   {
      return mIpAddr;
   }

   const std::string& getIpAddressString (void) const
   {
      return mIpStr;
   }

   /**
    * Passes a copy of the given message on to the socket using this node.
    */
   void deliverMessage(vpr::sim::MessagePtr msg);

private:
   vpr::Uint32 mIndex;
   vpr::Uint8  mType;
   std::string mIpStr;
   vpr::Uint32 mIpAddr;

   std::map<vpr::Uint32, vpr::SocketImplSIM*> mStreamSocketMap;
   std::map<vpr::Uint32, vpr::SocketImplSIM*> mDgramSocketMap;
};

} // End of sim namespace

} // End of vpr namespace


#endif /* _VPR_SIM_NETWORK_NODE_H_ */
