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

#ifndef CLUSTER_APPLICATION_BARRIER_SERVER_H
#define CLUSTER_APPLICATION_BARRIER_SERVER_H

#include <cluster/PluginConfig.h>

#include <boost/noncopyable.hpp>
#include <cluster/Packets/DataPacketPtr.h>
#include <plugins/ApplicationBarrierManager/ApplicationBarrierServerPtr.h>

namespace vpr
{
   class BufferObjectWriter;
}

namespace cluster
{
class ApplicationBarrier;

class GADGET_CLUSTER_PLUGIN_API ApplicationBarrierServer
   : boost::noncopyable
{
protected:
   /**
    * Create a new ApplicationBarrierServer.
    *
    * @param guid      GUID of the given ApplicationBarrier.
    * @param user_data Pointer to the ApplicationBarrier that we are sharing.
    * @param plugin_id GUID that should be placed at the beginning of 
    *                  each data packet so that the receiver knows which 
    *                  plugin the data is coming from.
    */
   ApplicationBarrierServer(const vpr::GUID& guid, ApplicationBarrier* userData, const vpr::GUID& pluginId);

public:
   /**
    * Creates a ApplicationBarrierServer instance and returns it wrapped in a
    * ApplicationBarrierServerPtr object.
    *
    * @since 1.3.7
    */
   static ApplicationBarrierServerPtr create(const vpr::GUID& guid, ApplicationBarrier* userData,
                                          const vpr::GUID& pluginGuid);
   /**
    * Release all memory that is no longer needed.
    */
   virtual ~ApplicationBarrierServer();

   /**
    * Send wait, to each client.
    */
   void serializeAndSend();

   /**
    * Print information about this ApplicationBarrierServer to the screen.
    *
    * @param debug_level The debug level that the information will be
    *                    displayed at.
    */
   void debugDump(int debugLevel);

   /**
    * Return the GUID of the ApplicationBarrier that this server is responsible
    * for.
    */
   vpr::GUID getId();

private:
   ApplicationBarrier*  mApplicationBarrier;    /**< Structure that is being shared across the cluster. */
   DataPacketPtr                mDataPacket;    /**< Packet will be sent across the cluster. */
};	

} // end namespace cluster

#endif
