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

#include <cluster/PluginConfig.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <plugins/RIMPlugin/RIMPlugin.h>

// Sharing Devices
#include <gadget/Type/BaseTypeFactory.h>
#include <gadget/Type/DeviceFactory.h>
#include <gadget/InputManager.h>
#include <gadget/RemoteInputManager.h>

// ClusterNetwork
#include <cluster/ClusterNetwork.h>
#include <gadget/Node.h>
#include <cluster/ClusterManager.h>

// IO Packets
#include <cluster/Packets/PacketFactory.h>
#include <cluster/Packets/ConnectionRequest.h>
#include <cluster/Packets/DeviceRequest.h>
#include <cluster/Packets/DeviceAck.h>
#include <cluster/Packets/ApplicationDataRequest.h>
#include <cluster/Packets/EndBlock.h>
#include <cluster/Packets/DataPacket.h>

// Configuration
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/Config/ConfigElement.h>

#include <map>

extern "C"
{
   GADGET_CLUSTER_PLUGIN_EXPORT(vpr::Uint32) getGadgeteerVersion()
   {
      return __GADGET_version;
   }

   GADGET_CLUSTER_PLUGIN_EXPORT(void) initPlugin(cluster::ClusterManager* mgr)
   {
      mgr->addPlugin(new cluster::RIMPlugin());
   }
}

namespace cluster
{
   RIMPlugin::RIMPlugin()
      : mHandlerGUID("9c3fb301-b142-4c6f-8ca3-1570898974d0")
      , mRIM(mHandlerGUID)
   {;}

   RIMPlugin::~RIMPlugin()
   {;}

   void RIMPlugin::recoverFromLostNode(gadget::Node* lost_node)
   {
      boost::ignore_unused_variable_warning(lost_node);
      // Since we have lost a connection we need to set a flag so
      // that when we gain a new connection we will reconfigure.
      //setReconfigurationNeededOnConnection(true);
   }
   
   /**
    * Handle a incoming packet.
    */
   void RIMPlugin::handlePacket(Packet* packet, gadget::Node* node)
   {
      mRIM.handlePacket(packet, node);
      // We do not handle packets, we are here only to sync.
   }

   void RIMPlugin::preDraw()
   {
      // Do nothing we are only here to sync.
   }

   void RIMPlugin::postPostFrame()
   {
      mRIM.sendDataAndSync();
   }

   void RIMPlugin::sendRequests()
   {
      mRIM.sendDeviceRequests();
   }

   //////////////////////////
   //    CONFIG METHODS    //
   //////////////////////////

   /** Add the pending element to the configuration.
    *  @pre configCanHandle (element) == true.
    *  @return true iff element was successfully added to configuration.
    */
   bool RIMPlugin::configAdd(jccl::ConfigElementPtr element)
   {
      // XXX: We may still use this to handle the configuration 
      //      of clustered RIM connections.
      if ( ClusterManager::instance()->recognizeRemoteDeviceConfig(element) )
      {
         std::string device_host = element->getProperty<std::string>("device_host");
         gadget::Node* node = ClusterManager::instance()->getNetwork()->getNodeByName(device_host);
         std::string device_name = element->getName();

         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
            << clrOutBOLD(clrCYAN,"[RIMPlugin] ")
            << "Adding the Remote Device: " << device_name
            << " to the RIM Pending List"
            << std::endl << vprDEBUG_FLUSH;

         if ( node == NULL )
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
               << clrOutBOLD(clrCYAN,"[RIMPlugin] ")
               << clrOutBOLD(clrRED, "WARNING:") << " Cluster node: " << device_host
               << " does not exist, there must be an error in the ClusterDepChecker."
               << std::endl << vprDEBUG_FLUSH;
            return false;
         }
         else if ( !node->isConnected() )
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
               << clrOutBOLD(clrCYAN,"[RIMPlugin] ")
               << clrOutBOLD(clrRED, "WARNING:") << " Cluster node: " << device_host
               << " is not connected, there must be an error in the ClusterDepChecker."
               << std::endl << vprDEBUG_FLUSH;
            return false;
         }

         DeviceRequest* device_req = new DeviceRequest(getHandlerGUID(), device_name);
         mRIM.addPendingDeviceRequest(device_req, node);
         setActive(true);
         return(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrCYAN,"[RIMPlugin] ")
            << clrOutBOLD(clrRED, "ERROR: ")
            << "recognizeRemoteDeviceConfig is broken."
            << std::endl << vprDEBUG_FLUSH;
         return(false);
      }
   }


   /** Remove the pending element from the current configuration.
    *  @pre configCanHandle (element) == true.
    *  @return true iff the element (and any objects it represented)
    *          were successfully removed.
    */
   bool RIMPlugin::configRemove(jccl::ConfigElementPtr element)
   {
      boost::ignore_unused_variable_warning(element);
      return false;

      // XXX: We may still use this to handle the configuration 
      //      of clustered RIM connections.
      /*
      if ( ClusterManager::instance()->recognizeRemoteDeviceConfig(element) )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[RIMPlugin] ")
         << "Removing the Remote Device: " << element->getName()
         << " from the active configuration \n" << vprDEBUG_FLUSH;

         removeVirtualDevice(element->getName());
         if ( this->mVirtualDevices.size()== 0 && mDeviceServers.size() == 0 )
         {
            setActive(false);
         }
         return(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
            << "[RIMPlugin::configRemove] ERROR, Something is seriously wrong, we should never get here\n"
            << vprDEBUG_FLUSH;
         return(false);
      }
      */
   }


   /** Checks if this handler can process element.
    *  Typically, an implementation of handler will check the element's
    *  description name/token to decide if it knows how to deal with
    *  it.
    *  @return true iff this handler can process element.
    */
   bool RIMPlugin::configCanHandle(jccl::ConfigElementPtr element)
   {
      // XXX: We may still use this to handle the configuration 
      //      of clustered RIM connections.
      return ClusterManager::instance()->recognizeRemoteDeviceConfig(element);
   }

   bool RIMPlugin::isPluginReady()
   {
      //XXX: Fix this to check for any pending devices. We might want to keep a local
      //     copy of this also so that we don't stall when we are waiting for pending
      //     RIM requests that are not in the cluster configuration.

      //vpr::Guard<vpr::Mutex> guard(mPendingDeviceRequestsLock);
      //return(0 == mPendingDeviceRequests.size());
      return true;
   }
}  // end namespace cluster
