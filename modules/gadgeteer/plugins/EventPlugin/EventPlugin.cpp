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

#include <boost/foreach.hpp>

#include <cluster/PluginConfig.h>

#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork.h>
#include <gadget/Node.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <plugins/EventPlugin/EventPlugin.h>
#include <plugins/EventPlugin/EventPacket.h>

extern "C"
{
   GADGET_CLUSTER_PLUGIN_EXPORT(vpr::Uint32) getGadgeteerVersion()
   {
      return __GADGET_version;
   }

   GADGET_CLUSTER_PLUGIN_EXPORT(void) initPlugin(cluster::ClusterManager* mgr)
   {
      mgr->addPlugin(cluster::EventPlugin::create());
   }
}

namespace cluster
{

const vpr::GUID EventPlugin::mPluginGUID("a882e49d-7686-4325-b2d0-be0f441ad858");

EventPlugin::EventPlugin()
{
   // This plugin is now being used(active)
   setActive(true);
}

ClusterPluginPtr EventPlugin::create()
{
   return ClusterPluginPtr(new EventPlugin());
}

EventPlugin::~EventPlugin()
{;}

bool EventPlugin::isPluginReady()
{
   return true;
}

void EventPlugin::handlePacket(PacketPtr packet, gadget::NodePtr node)
{
   if ( NULL != packet.get() && NULL != node.get() )
   {
      switch ( packet->getPacketType() )
      {
      case cluster::EventPacket::EVENT_PACKET:
      {
         EventPacketPtr event_packet = boost::dynamic_pointer_cast<EventPacket>(packet);
         vprASSERT(NULL != event_packet.get() && "Dynamic cast failed!");
         
         const std::string event_id(event_packet->getName());

         // - If server, increment token count.
         if (ClusterManager::instance()->isMaster())
         {
            mWaitingCount[event_id] += 1;
         }
         else
         {
            // If client, invoke callback and remove from mWaitingCallbacks.
            // NOTE: This packet will only be received by clients during preDraw.
            invokeAndClearCallback(event_id);
         }
         break;
      }
      default:
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL)
            << clrOutBOLD(clrCYAN,"[EventPlugin] ")
            << "Don't know how to handle a packet of type: "
            << packet->getPacketType() << std::endl << vprDEBUG_FLUSH;
         break;
      }
   }
}

void EventPlugin::postPostFrame()
{;}

void EventPlugin::preDraw()
{
   // If we are the master, check to see if any events have been finished.
   if (ClusterManager::instance()->isMaster())
   {
      ClusterNetwork* cluster_network(cluster::ClusterManager::instance()->getNetwork());
      const size_t num_nodes(cluster_network->getNumNodes());

      // Iterate over all pending events checking to see if they have been completed.
      BOOST_FOREACH(const waiting_map_t::value_type& event_count, mWaitingCount)
      {
         // If the event has been triggered on all nodes, including the master.
         // Send out a sync event.
         if (event_count.second >= num_nodes)
         {
            // Inform all clients that the event has been finished.
            const std::string& event_id(event_count.first);
            EventPacketPtr event_packet(EventPacket::create(mPluginGUID, event_id));
            cluster_network->sendToAll(event_packet);

            // Invoke the callback on the master node.
            invokeAndClearCallback(event_id);
         }
      }
   }
}

void EventPlugin::waitForEvent(const std::string& eventId, callback_t& callback)
{
   const callback_map_t::iterator found(mWaitingCallbacks.find(eventId));
   vprASSERT(mWaitingCallbacks.end() == found && "We shouldn't be registering twice.");

   //   - If server, add to mWaitingCallbacks, increment token count.
   if (ClusterManager::instance()->isMaster())
   {
      mWaitingCallbacks[eventId] = callback;
      mWaitingCount[eventId] += 1;
   }
   else
   {
      // Store the callback that will be invoked when the event is completed.
      mWaitingCallbacks[eventId] = callback;

      // Send a event packet to the master to inform it that we are waiting.
      ClusterNetwork* cluster_network(cluster::ClusterManager::instance()->getNetwork());
      gadget::NodePtr master_node(cluster_network->getNodeByName("master"));
      EventPacketPtr event_packet(EventPacket::create(mPluginGUID, eventId));
      master_node->send(event_packet);
   }
}

void EventPlugin::invokeAndClearCallback(const std::string& eventId)
{
   // Find the callback for the given event.
   const callback_map_t::iterator found(mWaitingCallbacks.find(eventId));
   vprASSERT(mWaitingCallbacks.end() != found);

   // Invoke the callback with the ID of the async event.
   const callback_t& cb = (*found).second;
   cb(eventId);

   // Remove the event from both maps.
   mWaitingCallbacks.erase(found);
   mWaitingCount.erase(eventId);
}

} // End of gadget namespace
