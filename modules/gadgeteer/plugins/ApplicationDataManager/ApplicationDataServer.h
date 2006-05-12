/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef CLUSTER_APPLICATION_DATA_SERVER_H
#define CLUSTER_APPLICATION_DATA_SERVER_H

#include <cluster/PluginConfig.h>

namespace vpr
{
   class BufferObjectWriter;
}

namespace gadget
{
   class Node;
}

namespace cluster
{
   class ApplicationData;
   class DataPacket;

   class GADGET_CLUSTER_PLUGIN_CLASS_API ApplicationDataServer
   {
   public:
      /**
       * Create a new ApplicationDataServer.
       *
       * @param guid      GUID of the given ApplicationData.
       * @param user_data Pointer to the ApplicationData that we are sharing.
       * @param plugin_id GUID that should be placed at the beginning of 
       *                  each data packet so that the receiver knows which 
       *                  plugin the data is coming from.
       */
      ApplicationDataServer(const vpr::GUID& guid, ApplicationData* user_data, const vpr::GUID& plugin_id);

      /**
       * Release all memory that is no longer needed.
       */
      ~ApplicationDataServer();

      /**
       * Serialize the local ApplicationData and store it in a DataPacket to
       * send to each client later.
       */
      //void updateLocalData();
      
      /**
       * Send mDataPacket, which has been updated in updateLocalData, to each
       * client.
       */
      //void send();

      void serializeAndSend();

      /**
       * Add a Node to the list of clients that should receive data for this
       * ApplicationData.
       *
       * @param new_client_node Node that will be added to the list.
       */
      void addClient(gadget::Node* new_client_node);

      /**
       * Remove a Node from the list of clients that should receive data for
       * this ApplicationData.
       *
       * @param host_name Hostname of the Node that will be removed from
       *                  the list.
       */
      void removeClient(const std::string& host_name);

      /**
       * Print information about this ApplicationDataServer to the screen.
       *
       * @param debug_level The debug level that the information will be
       *                    displayed at.
       */
      void debugDump(int debug_level);

      /**
       * Return the GUID of the ApplicationData that this server is responsible
       * for.
       */
      vpr::GUID getId();
   private:
      std::vector<gadget::Node*>           mClients;            /**< Vecor of nodes that this server needs to send data to. */
      vpr::Mutex                                   mClientsLock;        /**< Lock for the list of clients. */   
      
      ApplicationData*                             mApplicationData;    /**< Structure that is being shared across the cluster. */
      DataPacket*                                  mDataPacket;         /**< Packet will be sent across the cluster. */
      vpr::BufferObjectWriter*                     mBufferObjectWriter; /**< ObjectWriter used to serialize the ApplicationData. */
      std::vector<vpr::Uint8>*                     mDeviceData;         /**< Vector that conatins the data that will be sent across the node */
   };

} // end namespace cluster

#endif
