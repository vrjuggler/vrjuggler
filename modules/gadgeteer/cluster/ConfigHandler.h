/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_CONFIG_HANDLER_H_
#define _GADGET_CONFIG_HANDLER_H_

#include <gadget/gadgetConfig.h>
#include <gadget/PacketHandler.h>
#include <gadget/Node.h>
#include <cluster/ConfigHandlerPtr.h>

namespace cluster
{

/** \class ConfigHandler ConfigHandler.h gadget/ConfigHandler.h
 *
 * Packet handler interface used for network communication.
 */
class GADGET_API ConfigHandler
   : public gadget::PacketHandler
{
protected:
   ConfigHandler();

public:
   virtual ~ConfigHandler();

   /**
    * Creates a ConfigHandler instance and returns it wrapped in a
    * ConfigHandlerPtr object.
    *
    * @since 1.3.7
    */
   static ConfigHandlerPtr create();

   /**
    * Get the GUID associated with this handler.
    */
   virtual vpr::GUID getHandlerGUID()
   {
      return vpr::GUID("f3ea94e2-82fc-43f6-a57f-474d3fd1d6eb");
   }
   
   virtual std::string getHandlerName()
   {
      return std::string("Config Handler");
   }

   /**
    * Handle a incoming packet.
    */
   virtual void handlePacket(cluster::PacketPtr packet, gadget::NodePtr node);
   
   /**
    * Virtual function used to inform all handlers that the network
    * has lost its connection to the given Node.
    */
   virtual void recoverFromLostNode(gadget::NodePtr lostNode);
};

} // end namespace gadget

#endif /*_GADGET_CONFIG_HANDLER_H_*/
