/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _GADGET_PACKET_HANDLER_H_
#define _GADGET_PACKET_HANDLER_H_

#include <gadget/gadgetConfig.h>

#include <string> 
#include <boost/concept_check.hpp>
#include <vpr/Util/GUID.h>

namespace vpr
{
   class SerializableObject;
}

namespace cluster
{
   class Packet;
}

namespace gadget
{

class Node;

/** \class PacketHandler PacketHandler.h gadget/PacketHandler.h
 *
 * Packet handler interface used for network communication.
 */
class GADGET_CLASS_API PacketHandler
{
public:
   PacketHandler()
   {;}
   virtual ~PacketHandler()
   {;}

   /**
    * Get the GUID associated with this handler.
    */
   virtual vpr::GUID getHandlerGUID() = 0;
   
   virtual std::string getHandlerName() = 0;

   /**
    * Handle a incoming packet.
    */
   virtual void handlePacket(cluster::Packet* packet, Node* node) = 0;
   
   /**
    * Virtual function used to inform all handlers that the network
    * has lost its connection to the given Node.
    */
   virtual void recoverFromLostNode(Node* lostNode) = 0;
};

} // end namespace gadget

#endif /*_GADGET_PACKET_HANDLER_H_*/
