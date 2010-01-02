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

#include <gadget/Util/Debug.h>
#include <cluster/ClusterDepChecker.h>
#include <cluster/ClusterManager.h>
#include <gadget/Node.h>
#include <gadget/Type/DeviceFactory.h>
#include <cluster/ClusterNetwork.h>
#include <jccl/RTRC/ConfigManager.h>

#include <boost/concept_check.hpp>

namespace cluster
{

bool ClusterDepChecker::depSatisfied(jccl::ConfigElementPtr element)
{
   if (element->getID() == ClusterNetwork::getClusterNodeElementType())
   {
      // Machine Specific elements should have no dependencies since we are
      // simply inserting the child elements into the pending list. This is
      // to fix errors like the embedded keyboard window in a DisplayWindow
      // would always create a dependancy loop.
      debugOutDependencies( element, vprDBG_WARNING_LVL );
      return true;
   }
   /*
   else if (cluster::ClusterManager::instance()->recognizeRemoteDeviceConfig(element))
   {
      // Remote devices should have no dependencies since we are not actually
      // configuring anything, we are only creating a data structure that we
      // can determine without any other elements.
      // Virtual devices should not have any dependencies.

      // RemoteDeviceConfig has only two dependencies
      //   - deivceHost exists in Active List
      //   - Node exists and is connected
      //   - Remote Device needs to be configured

      bool pass = true;

      jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();

      // device_host exists in active configuration
      std::string device_host = element->getProperty<std::string>( "device_host" );
      gadget::NodePtr node = cluster::ClusterManager::instance()->getNetwork()->getNodeByName( device_host );

      if (!cfg_mgr->isElementInActiveList(device_host) || NULL == node)
      {
         pass = false;
      }
      else if (gadget::Node::DISCONNECTED == node->getStatus())
      {
         pass = false;
         
         node->setStatus( gadget::Node::PENDING );
      }
      else if (gadget::Node::PENDING == node->getStatus() ||
               gadget::Node::NEWCONNECTION == node->getStatus())
      {
         // Wait until we are fully connected.
         pass = false;
      }

      return pass;
   }
   */
   else
   {
      vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL ) << "ERROR, Something is seriously wrong, we should never get here\n"
         << vprDEBUG_FLUSH;
      return true;
   }
}


// We need to handle Machine Specific elements because in reality they have no
// dependencies.
bool ClusterDepChecker::canHandle(jccl::ConfigElementPtr element)
{
   return (element->getID() == ClusterNetwork::getClusterNodeElementType()); // ||
           //cluster::ClusterManager::instance()->recognizeRemoteDeviceConfig( element ));
}

void ClusterDepChecker::debugOutDependencies(jccl::ConfigElementPtr element,
                                             const int dbg_lvl)
{
   vprDEBUG_NEXT_BEGIN( vprDBG_ALL, dbg_lvl )
      << "---- Dependencies for " << element->getName()
      << " (type: " << element->getID() << ") ----\n" << vprDEBUG_FLUSH;

   if (element->getID() == ClusterNetwork::getClusterNodeElementType())
   {
      // Machine Specific element should have no dependencies since we are
      // simply inserting the child elements into the pending list.
      vprDEBUG_NEXT( vprDBG_ALL, dbg_lvl ) << "0: "
          << "Host node's ConfigElement"
          << " ==> " << vprDEBUG_FLUSH;
      
      vprDEBUG_CONT( vprDBG_ALL, dbg_lvl ) << "none.\n"
         << vprDEBUG_FLUSH;
   }
   /*
   else if (cluster::ClusterManager::instance()->recognizeRemoteDeviceConfig( element ))
   {
      std::string device_host = element->getProperty<std::string>( "device_host");
      gadget::NodePtr node = cluster::ClusterManager::instance()->getNetwork()->getNodeByName( device_host );

      vprDEBUG_NEXT( vprDBG_ALL, dbg_lvl ) << "1: "
          << "Device node's ConfigElement"
          << " ==> " << vprDEBUG_FLUSH;

      jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();

      if (!cfg_mgr->isElementInActiveList( device_host ) || NULL == node)
      {
         vprDEBUG_CONT( vprDBG_ALL, dbg_lvl ) << "not in active list.\n"
            << vprDEBUG_FLUSH;
      }
      else
      {
         vprDEBUG_CONT( vprDBG_ALL, dbg_lvl ) << "in active list.\n"
            << vprDEBUG_FLUSH;
      }

      vprDEBUG_NEXT( vprDBG_ALL, dbg_lvl ) << "2: "
          << "Device host connection status"
          << " ==> " << vprDEBUG_FLUSH;
      
      if (gadget::Node::DISCONNECTED == node->getStatus())
      {
         vprDEBUG_CONT( vprDBG_ALL, dbg_lvl ) << "disconnected.\n"
            << vprDEBUG_FLUSH;
      }
      else if (gadget::Node::PENDING == node->getStatus())
      {
         vprDEBUG_CONT( vprDBG_ALL, dbg_lvl ) << "pending.\n"
            << vprDEBUG_FLUSH;
      }
      else if (gadget::Node::NEWCONNECTION == node->getStatus())
      {
         vprDEBUG_CONT( vprDBG_ALL, dbg_lvl ) << "new connection.\n"
            << vprDEBUG_FLUSH;
      }
   }
   */
   vprDEBUG_CONT_END( vprDBG_ALL, dbg_lvl ) << std::endl << vprDEBUG_FLUSH;
}

}  // end namespace cluster
