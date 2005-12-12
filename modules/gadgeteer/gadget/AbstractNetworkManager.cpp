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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>

#include <boost/concept_check.hpp>

#include <cluster/ClusterManager.h>
#include <cluster/Packets/Header.h>
#include <cluster/Packets/Packet.h>
#include <cluster/Packets/PacketFactory.h>

#include <gadget/Node.h>
#include <gadget/AbstractNetworkManager.h>
#include <gadget/PacketHandler.h>
#include <gadget/Util/Debug.h>

#include <jccl/Config/ConfigElement.h>
#include <jccl/RTRC/ConfigManager.h>

#include <vpr/IO/Socket/InetAddr.h>
#include <vpr/Util/ReturnStatus.h>

#include <iomanip>

namespace gadget
{
   AbstractNetworkManager::AbstractNetworkManager() 
      : mNodes(0), mHandlerMap()
   {;}

   AbstractNetworkManager::~AbstractNetworkManager()
   {
      shutdown();
   }

   /**
    * Determine if the given hostname matches the local machine's hostname.
    */
   bool AbstractNetworkManager::isLocalHost(const std::string& test_host_name)
   {
      // Resolve the address of the hostname given to test.
      vpr::InetAddr test;
      test.setAddress(test_host_name, 0);

      // Get local address.
      return hostCompare(vpr::InetAddr::getLocalHost(), test);
   }

   bool AbstractNetworkManager::hostCompare(const vpr::InetAddr& first, const vpr::InetAddr& second)
   {
      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
         << "+================= Local addresses ==================+"
         << std::endl << vprDEBUG_FLUSH;
      std::vector<std::string> first_names = first.getHostnames();
      for (std::vector<std::string>::iterator itr = first_names.begin() ; itr != first_names.end() ; ++itr)
      {
         vpr::InetAddr temp;
         temp.setAddress( (*itr), 0 );
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
            << "| Name: " << std::setw(25) << (*itr) << " | "
            << std::setw(16) << temp.getAddressString() << " | "
            << std::endl << vprDEBUG_FLUSH;
      }

      vprDEBUG(gadgetDBG_NET_MGR,vprDBG_HVERB_LVL)
         << "+======= Resolved addresses for given hostname ======+"
         << std::endl << vprDEBUG_FLUSH;
      std::vector<std::string> second_names = second.getHostnames();
      for (std::vector<std::string>::iterator itr = second_names.begin() ; itr != second_names.end() ; ++itr)
      {
         vpr::InetAddr temp;
         temp.setAddress( (*itr), 0 );
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
            << "| Name: " << std::setw(25) << (*itr) << " | "
            << std::setw(16) << temp.getAddressString() << " | "
            << std::endl << vprDEBUG_FLUSH;
      }

      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
         << "+====================================================+"
         << std::endl << vprDEBUG_FLUSH;

      bool result = false;
      if (first.getAddressString() == second.getAddressString())
      {
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
            << "| We have a match.                                   |"
            << std::endl << vprDEBUG_FLUSH;
         result = true;
      }
      else
      {
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
            << "| NO match.                                          |"
            << std::endl << vprDEBUG_FLUSH;
         result = false;
      }
      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
         << "+====================================================+"
         << std::endl << vprDEBUG_FLUSH;

      return result;
   }


   void AbstractNetworkManager::handlePacket(cluster::Packet* packet, Node* node)
   {
      // If the ClusterManager should handle this packet, then do so.
      if (packet->getPacketType() == cluster::Header::RIM_END_BLOCK)
      {
         // -Set New State
         if (node == NULL)
         {
            return;
         }

         node->setUpdated( true );
         return;
      }
      else if (packet->getPacketType() == cluster::Header::RIM_CONNECTION_REQ ||
               packet->getPacketType() == cluster::Header::RIM_CONNECTION_ACK)
      {
         //handleLocalPacket(packet, node);
         
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CRITICAL_LVL )
            << clrOutBOLD( clrRED, "[ERROR] " )
            << "RIM_CONNECTION_REQ & RIM_CONNECTION_ACK data packet types are depreciated."
            << std::endl << vprDEBUG_FLUSH;
         return;
      }

      vpr::GUID handler_guid = packet->getPluginId();

      PacketHandler* temp_handler = getHandlerByGUID( handler_guid );

      if (NULL != temp_handler)
      {
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
            << clrOutBOLD(clrMAGENTA,"[AbstractNetworkManager]")
            << " Handler \"" << temp_handler->getHandlerName() << "\" will handle this packet."
            << std::endl << vprDEBUG_FLUSH;


         temp_handler->handlePacket( packet, node );
      }
      else
      {
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
            << clrOutBOLD( clrMAGENTA, "[AbstractNetworkManager]" )
            << " Handler " << handler_guid.toString() << " does not exist to handle this packet."
            << std::endl << vprDEBUG_FLUSH;
      }
   }

   vpr::ReturnStatus AbstractNetworkManager::addNode(const std::string& name, const std::string& host_name,
                                                    const vpr::Uint16& port, vpr::SocketStream* socket_stream)
   {
      // -Create a new Node using the given information
      // -Add the new node to the AbstractNetworkManager
      
      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
         << clrOutBOLD(clrMAGENTA,"[AbstractNetworkManager]")
         << " Adding node: " << name
         << std::endl << vprDEBUG_FLUSH;

      Node* temp_node = new Node(name, host_name, port, socket_stream, this);
      mNodes.push_back( temp_node );
      
      return vpr::ReturnStatus::Succeed;
   }

   void AbstractNetworkManager::addNode(Node* node)
   {
      // -Add the given node to the AbstractNetworkManager
      
      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
         << clrOutBOLD( clrMAGENTA, "[AbstractNetworkManager]" )
         << " Adding node: " << node->getName()
         << std::endl << vprDEBUG_FLUSH;

      mNodes.push_back( node );
   }
   
   Node* AbstractNetworkManager::getNodeByHostname(const std::string& host_name)
   {
      vpr::DebugOutputGuard dbg_output( gadgetDBG_NET_MGR, vprDBG_VERB_LVL,
         std::string("-------- getNodeByHostname() --------\n"),
         std::string("--------------------------------------------\n"));

      vpr::InetAddr searching_for_node;
      searching_for_node.setAddress( host_name, 0 );

      vprDEBUG( gadgetDBG_NET_MGR, vprDBG_VERB_LVL )
         << clrOutBOLD( clrMAGENTA, "[AbstractNetworkManager]" )
         << " Looking for Node with hostname: " << host_name
         << std::endl << vprDEBUG_FLUSH;
 
         
      // -Find Node with given hostname and return a pointer to it.
      // -If we do not find one, return NULL
      for (std::vector<Node*>::iterator i = mNodes.begin();
           i != mNodes.end() ; i++)
      {
         vpr::InetAddr testing_node;
         testing_node.setAddress( (*i)->getHostname(), 0 );
         
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_VERB_LVL )
            << clrOutBOLD( clrMAGENTA, "[AbstractNetworkManager]" )
            << " Searcing for: " << searching_for_node.getAddressString()
            << " Testing: " << testing_node.getAddressString()
            << std::endl << vprDEBUG_FLUSH;

         if (searching_for_node.getAddressString() == testing_node.getAddressString())
         {
            return(*i);
         }
      }

      return(NULL);
   }

   Node* AbstractNetworkManager::getNodeByName(const std::string& node_name)
   {
      // -Find Node with given name and return a pointer to it.
      // -If we do not find one, return NULL
      for (std::vector<Node*>::iterator i = mNodes.begin();
           i != mNodes.end() ; i++)
      {
         if ((*i)->getName() == node_name)
         {
            return *i;
         }
      }

      return NULL;
   }

   vpr::Uint16 AbstractNetworkManager::getNumPendingNodes()
   {
      int num_pending = 0;
      
      for (std::vector<Node*>::iterator i = mNodes.begin();
           i != mNodes.end() ; i++)
      {
         if (Node::PENDING == (*i)->getStatus())
         {
            ++num_pending;
         }
      }

      return num_pending;
   }

   bool AbstractNetworkManager::attemptPendingNodes()
   {
      bool ret_val = false;
      
      for (std::vector<Node*>::iterator i = mNodes.begin();
           i != mNodes.end() ; i++)
      {
         if ( Node::PENDING == (*i)->getStatus() )
         {
            if ( attemptConnect(*i).success() )
            {
               // If any of the nodes were successful connecting
               // then we should return true
               ret_val = true;
            }
         }
      }

      return ret_val;
   }

   void AbstractNetworkManager::shutdown()
   {
      for (std::vector<Node*>::iterator j = mNodes.begin(); j != mNodes.end(); j++)
      {
          (*j)->shutdown();
      }
   }


   void AbstractNetworkManager::debugDumpNodes(int debug_level)
   {
      vpr::DebugOutputGuard dbg_output(gadgetDBG_NET_MGR,debug_level,
         std::string("-------------- Cluster Network --------------\n"),
         std::string("---------------------------------------------\n"));
      for(std::vector<Node*>::iterator j = mNodes.begin(); j != mNodes.end(); j++)
      {
         (*j)->debugDump( debug_level );
      }
   }
   
   bool AbstractNetworkManager::recognizeClusterMachineConfig(jccl::ConfigElementPtr element)
   {
      return (element->getID() == getClusterNodeElementType());
   }

   bool AbstractNetworkManager::configCanHandle(jccl::ConfigElementPtr element)
   {
       return recognizeClusterMachineConfig(element);
   }

   bool AbstractNetworkManager::configAdd(jccl::ConfigElementPtr element)
   {
      if (recognizeClusterMachineConfig(element))
      {
         // -If local machine element
         //   -Add machine specific ConfigElements to the pending list.
         //   -Start Listening thread
         // -Else
         //   -Add Node to AbstractNetworkManager

         if (isLocalHost( element->getProperty<std::string>( "host_name" ) ))
         {
            // NOTE: Add all machine dependent ConfigElementPtr's here
            vprASSERT( element->getNum("display_system") == 1 
               && "A Cluster System element must have exactly 1 display_system element" );

            std::vector<jccl::ConfigElementPtr> cluster_node_elements =
               element->getChildElements();

            for (std::vector<jccl::ConfigElementPtr>::iterator i = cluster_node_elements.begin();
                 i != cluster_node_elements.end();
                 ++i)
            {
               jccl::ConfigManager::instance()->addConfigElement(*i, jccl::ConfigManager::PendingElement::ADD);

               vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL ) << clrSetBOLD(clrCYAN)
                  << clrOutBOLD( clrMAGENTA,"[AbstractNetworkManager]" )
                  << " Adding Machine specific ConfigElement: "
                  << (*i)->getName() << clrRESET << std::endl << vprDEBUG_FLUSH;
            }

            const int listen_port = element->getProperty<int>( "listen_port" );
            startListening( listen_port, false );
         }
         else
         {
            vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
               << clrOutBOLD( clrMAGENTA, "[AbstractNetworkManager]" )
               << " Adding Node: " << element->getName()
               << " to the Cluster Network\n" << vprDEBUG_FLUSH;

            std::string    name        = element->getName();
            std::string    host_name   = element->getProperty<std::string>( "host_name" );
            vpr::Uint16    listen_port = element->getProperty<int>( "listen_port" );

            addNode(name, host_name, listen_port);
         }
         return true;
      }
      return false;
   }

   bool AbstractNetworkManager::configRemove(jccl::ConfigElementPtr element)
   {
      if (recognizeClusterMachineConfig( element ))
      {
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
            << clrOutBOLD( clrMAGENTA, "[AbstractNetworkManager]" )
            << " Removing the Node: " << element->getName()
            << " from the Cluster Network\n" << vprDEBUG_FLUSH;
         return true;
      }
      else
      {
         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
            << clrOutBOLD( clrMAGENTA, "[AbstractNetworkManager]" )
            << " ERROR, Something is seriously wrong, we should never get here\n"
            << vprDEBUG_FLUSH;
         return false;
      }
   }

   std::string AbstractNetworkManager::getClusterNodeElementType()
   {
      return "cluster_node";
   }

   void AbstractNetworkManager::updateNewConnections()
   {
      // This fuction is needed in order to gurantee that all cluster
      // nodes become active at the same time, the beginning of the frame
      //
      // -Loop over all Nodes checking for new connections
      // -If Node is a new connection
      //   - Set it to connected

      bool new_connection = false;
      
      for (std::vector<Node*>::iterator i = mNodes.begin() ;
            i != mNodes.end() ; i++)
      {
         if ((*i)->getStatus() == Node::NEWCONNECTION)
         {
            vprDEBUG( gadgetDBG_NET_MGR, vprDBG_STATE_LVL )
               << clrOutBOLD( clrMAGENTA, "[AbstractNetworkManager]" )
               << " Node: " << (*i)->getName()
               << " is now CONNECTED."
               << std::endl << vprDEBUG_FLUSH;

            new_connection = true;
            (*i)->start();
            (*i)->setStatus( Node::CONNECTED );
         }
      }

      // If we have pending nodes, attempt to connect to them.
      if (getNumPendingNodes() > 0)
      {
         attemptPendingNodes();
      }
      
      if (new_connection)
      {
         // - If the pending list is stale
         //   - Refresh the list
         if (jccl::ConfigManager::instance()->isPendingStale())
         {
            vprDEBUG( gadgetDBG_NET_MGR, vprDBG_STATE_LVL )
               << clrOutBOLD( clrMAGENTA, "[AbstractNetworkManager]" )
               << " New connections were made, so refresh the pending list."
               << std::endl << vprDEBUG_FLUSH;
            jccl::ConfigManager::instance()->refreshPendingList();
         }

         debugDumpNodes(0);
      }
   }
   
   PacketHandler* AbstractNetworkManager::getHandlerByGUID(const vpr::GUID& handler_guid)
   {
      std::map<vpr::GUID, PacketHandler*>::const_iterator i = mHandlerMap.find( handler_guid );
      if (i != mHandlerMap.end())
      {
         return ((*i).second);
      }
      return NULL;
   }
   
   /**
    * Adds a new plugin to the ClusterManager.
    */
   void AbstractNetworkManager::addHandler(PacketHandler* new_handler)
   {
         std::pair<vpr::GUID, PacketHandler*> p 
            = std::make_pair( new_handler->getHandlerGUID(), new_handler );
         mHandlerMap.insert( p );

         vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
            << clrOutBOLD( clrMAGENTA, "[Reactor] " )
            << "Adding Handler: " << new_handler->getHandlerName() << std::endl << vprDEBUG_FLUSH;
   }
} // end namespace gadget
