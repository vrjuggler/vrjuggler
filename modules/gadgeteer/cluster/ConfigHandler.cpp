/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#include <gadget/gadgetConfig.h>
#include <boost/concept_check.hpp>
#include <jccl/Config/Configuration.h>
#include <jccl/RTRC/ConfigManager.h>
#include <cluster/ConfigHandler.h>
#include <cluster/Packets/ConfigPacket.h>

namespace cluster
{

ConfigHandler::ConfigHandler()
{;}

ConfigHandler::~ConfigHandler()
{;}

ConfigHandlerPtr ConfigHandler::create()
{
   return ConfigHandlerPtr(new ConfigHandler());
}

void ConfigHandler::recoverFromLostNode( gadget::NodePtr node )
{
   boost::ignore_unused_variable_warning(node);

   /*
   vpr::Guard<vpr::Mutex> guard( mPluginsLock );

   for ( plugin_list_t::iterator itr = mPlugins.begin(); itr != mPlugins.end(); itr++ )
   {
      (*itr)->recoverFromLostNode( node );
   }
   */
}

void ConfigHandler::handlePacket(cluster::PacketPtr packet, gadget::NodePtr node)
{
   boost::ignore_unused_variable_warning(node);

   vprASSERT(Header::CONFIG_PACKET == packet->getPacketType() && "Not a config packet.");
   cluster::ConfigPacketPtr cfg_pkt = boost::dynamic_pointer_cast<cluster::ConfigPacket>(packet);
   vprASSERT(NULL != cfg_pkt.get() && "Failed to cast ConfigPacket.");

   jccl::Configuration incoming_config;
   //Loading from an istream
   std::istringstream config_input(cfg_pkt->getConfig());
   config_input >> incoming_config;

   vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
      << clrOutBOLD( clrCYAN, "[ConfigHandler] " )
      << "Got configuration packet."
      << std::endl << vprDEBUG_FLUSH;

   if (jccl::ConfigManager::PendingElement::ADD == cfg_pkt->getType())
   {
      jccl::ConfigManager::instance()->addConfigurationAdditions(&incoming_config);
   }
   else
   {
      jccl::ConfigManager::instance()->addConfigurationRemovals(&incoming_config);
   }
}

} // End of cluster namespace
