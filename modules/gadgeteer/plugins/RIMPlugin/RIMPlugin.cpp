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
#include <gadget/AbstractNetworkManager.h>
#include <plugins/RIMPlugin/DeviceServer.h>
#include <plugins/RIMPlugin/VirtualDevice.h>

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

#include <boost/lexical_cast.hpp>
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

struct DeviceNamePred
{
   DeviceNamePred(const std::string& deviceName)
      : mDeviceName(deviceName)
   {
      /* Do nothing. */ ;
   }

   bool operator()(DeviceServerPtr ds)
   {
      return ds->getName() == mDeviceName;
   }

   const std::string& mDeviceName;
};

RIMPlugin::RIMPlugin()
   : mHandlerGUID("9c3fb301-b142-4c6f-8ca3-1570898974d0")
{;}

RIMPlugin::~RIMPlugin()
{
   mVirtualDevices.clear();
   mDeviceServers.clear();
}

void RIMPlugin::preDraw()
{
   // Do nothing we are only here to sync.
}

void RIMPlugin::postPostFrame()
{
   sendDataAndSync();
}

void RIMPlugin::sendRequests()
{
   // Do nothing
}

/** Add the pending elm to the configuration.
 *  @pre configCanHandle (elm) == true.
 *  @return true iff elm was successfully added to configuration.
 */
bool RIMPlugin::configAdd(jccl::ConfigElementPtr elm)
{
   vprASSERT(ClusterManager::instance()->recognizeRemoteDeviceConfig(elm));
   vprASSERT(ClusterManager::instance()->isClusterActive());
   std::string device_name = elm->getName();

   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
      << clrOutBOLD(clrCYAN,"[RIMPlugin] ")
      << "Adding device: " << device_name
      << std::endl << vprDEBUG_FLUSH;

   vprASSERT(cluster::ClusterManager::instance()->isClusterActive() && "RIM called in non-cluster mode.")
   bool master = cluster::ClusterManager::instance()->isMaster();
   bool result(false);

   // If we are the master, configure the device and tell all slaves to prepare
   // virtual devices.
   if (master)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
         << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
         << "Configuring device on master node: " << device_name
         << std::endl << vprDEBUG_FLUSH;

      gadget::InputManager::instance()->configureDevice(elm);
      gadget::Input* input_device = gadget::InputManager::instance()->getDevice(device_name);
      if ( input_device != NULL )
      {
         result = addDeviceServer(device_name, input_device);
         DeviceServerPtr device_server = getDeviceServer(device_name);
         vprASSERT(NULL != device_server.get() && "Must have device server.");

         std::string temp_string = input_device->getInputTypeName();
         vpr::GUID   temp_guid   = device_server->getId();
         cluster::DeviceAck* device_ack =
            new cluster::DeviceAck(mHandlerGUID, temp_guid,
                                   device_name, temp_string, true);

         gadget::AbstractNetworkManager::node_list_t nodes = cluster::ClusterManager::instance()->getNetwork()->getNodes();
         for (gadget::AbstractNetworkManager::node_list_t::iterator itr = nodes.begin(); itr != nodes.end(); itr++)
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
               << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
               << "Sending device ack [" << device_name << "] to [" << (*itr)->getName() << "]"
               << std::endl << vprDEBUG_FLUSH;

            // Create a responce ACK
            (*itr)->send(device_ack);
         }
      }
   }
   else
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
         << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
         << "Configuring device on slave node: " << device_name
         << std::endl << vprDEBUG_FLUSH;

      result = true;
   }

   return result;
}

/** Remove the pending element from the current configuration.
 *  @pre configCanHandle (element) == true.
 *  @return true iff the element (and any objects it represented)
 *          were successfully removed.
 */
bool RIMPlugin::configRemove(jccl::ConfigElementPtr elm)
{
   boost::ignore_unused_variable_warning(elm);
   return false;
}

/** Checks if this handler can process element.
 *  Typically, an implementation of handler will check the element's
 *  description name/token to decide if it knows how to deal with
 *  it.
 *  @return true iff this handler can process element.
 */
bool RIMPlugin::configCanHandle(jccl::ConfigElementPtr elm)
{
   // XXX: We may still use this to handle the configuration 
   //      of clustered RIM connections.
   return ClusterManager::instance()->recognizeRemoteDeviceConfig(elm);
}

bool RIMPlugin::isPluginReady()
{
   return true;
}

void RIMPlugin::recoverFromLostNode(gadget::Node* lost_node)
{
   removeVirtualDevicesOnHost(lost_node->getHostname());
}

/**
 * Handle a incoming packet.
 */
void RIMPlugin::handlePacket(cluster::Packet* packet, gadget::Node* node)
{
   //We are only handling data packets right now.
   if ( NULL != packet && NULL != node )
   {
      switch ( packet->getPacketType() )
      {
      case cluster::Header::RIM_DEVICE_ACK:
         {
            cluster::DeviceAck* temp_device_ack = dynamic_cast<cluster::DeviceAck*>(packet);
            vprASSERT(NULL != temp_device_ack && "Dynamic cast failed!");
            std::string device_name = temp_device_ack->getDeviceName();
            vprASSERT(temp_device_ack->getAck() && "We only have device ACKs now.");

            if ( getVirtualDevice(device_name) != NULL )
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrRED, "ERROR:")
               << "Somehow we already have a virtual device named: " << device_name << std::endl << vprDEBUG_FLUSH;
            }
            else
            {
               addVirtualDevice(temp_device_ack->getId(), device_name,
                                temp_device_ack->getDeviceBaseType(),
                                temp_device_ack->getHostname());

               // Add this virtual device to the InputManager's list of devices.
               gadget::InputManager::instance()->addRemoteDevice(getVirtualDevice(device_name), device_name);
            }
            break;
         }
      case cluster::Header::RIM_DATA_PACKET:
         {
            cluster::DataPacket* temp_data_packet = dynamic_cast<cluster::DataPacket*>(packet);
            vprASSERT(NULL != temp_data_packet && "Dynamic cast failed!");

            //vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "RIM::handlePacket()..." << std::endl <<  vprDEBUG_FLUSH;
            //temp_data_packet->printData(1);

            gadget::Input* virtual_device = getVirtualDevice(temp_data_packet->getObjectId());
            if ( virtual_device != NULL )
            {
               vpr::BufferObjectReader* temp_reader = new vpr::BufferObjectReader(temp_data_packet->getDeviceData());

               temp_reader->setAttrib("rim.timestamp.delta", node->getDelta());
               virtual_device->readObject(temp_reader);
            }
            break;
         }
      default:
         {
            throw ClusterException("RIMPlugin does not handle packet type: "
               + boost::lexical_cast<std::string>(packet->getPacketType()), VPR_LOCATION);
            break;
         }
      }
   }
}

bool RIMPlugin::addVirtualDevice(const vpr::GUID& device_id,
                                 const std::string& name,
                                 const std::string& device_base_type,
                                 const std::string& hostname)
{
   vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
   << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
   << "Creating Virtual Device: " << name << std::endl << vprDEBUG_FLUSH;

   gadget::Input* input_device = gadget::BaseTypeFactory::instance()->loadNetDevice(device_base_type);
   VirtualDevicePtr virtual_device = VirtualDevicePtr(new VirtualDevice(name, device_id, device_base_type, hostname, input_device));

   mVirtualDevices[device_id] = virtual_device;

   return true;
}

gadget::Input* RIMPlugin::getVirtualDevice(const vpr::GUID& deviceId)
{
   vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

   for ( virtual_device_map_t::iterator i = mVirtualDevices.begin();
         i != mVirtualDevices.end() ; i++ )
   {
      if ( (*i).first == deviceId )
      {
         return((*i).second->getDevice());
      }
   }
   return NULL;
}

gadget::Input* RIMPlugin::getVirtualDevice(const std::string& deviceName)
{
   vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

   for ( virtual_device_map_t::iterator i = mVirtualDevices.begin();
         i != mVirtualDevices.end() ; i++ )
   {
      if ( (*i).second->getName() == deviceName )
      {
         return((*i).second->getDevice());
      }
   }
   return NULL;
}

bool RIMPlugin::removeVirtualDevicesOnHost(const std::string& hostName)
{
   // - Get a list of all remote devices on the given host
   // - Remove them from the current configuration
   vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

   for ( virtual_device_map_t::iterator i = mVirtualDevices.begin();
         i != mVirtualDevices.end(); i++ )
   {
      if ( (*i).second->getRemoteHostname() == hostName )
      {
         removeVirtualDevice((*i).second->getName());
      }
   }

   return true;
}

void RIMPlugin::removeVirtualDevice(const vpr::GUID& device_id)
{
   vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

   for ( virtual_device_map_t::iterator i = mVirtualDevices.begin();
         i != mVirtualDevices.end() ; i++ )
   {
      if ( (*i).first == device_id )
      {
         // Remove remote device from the InputManager
         gadget::InputManager::instance()->removeDevice((*i).second->getName());
         mVirtualDevices.erase(i);
         return;
      }
   }
}

void RIMPlugin::removeVirtualDevice(const std::string& device_name)
{
   vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

   // Remove remote device from the InputManager
   gadget::InputManager::instance()->removeDevice(device_name);

   for ( virtual_device_map_t::iterator i = mVirtualDevices.begin();
         i != mVirtualDevices.end() ; i++ )
   {
      if ( (*i).second->getName() == device_name )
      {
         (*i).second->debugDump(vprDBG_CONFIG_LVL);
         mVirtualDevices.erase(i);
         return;
      }
   }
}

void RIMPlugin::debugDumpVirtualDevices(int debug_level)
{
   vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

   vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                                    std::string("-------------- Virtual Devices --------------\n"),
                                    std::string("---------------------------------------------\n"));
   for ( virtual_device_map_t::iterator j = mVirtualDevices.begin(); j != mVirtualDevices.end(); j++ )
   {
      (*j).second->debugDump(debug_level);
   }
}


// ===================== DEVICE SERVERS =============================

bool RIMPlugin::addDeviceServer(const std::string& name,
                                         gadget::Input* device)
{
   vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

   DeviceServerPtr temp_device_server =
      DeviceServerPtr(new DeviceServer(name, device, mHandlerGUID));
   mDeviceServers.push_back(temp_device_server);

   return true;
}

void RIMPlugin::addDeviceServer(DeviceServerPtr device)
{
   vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);
   mDeviceServers.push_back(device);
}



DeviceServerPtr RIMPlugin::getDeviceServer(const std::string& deviceName)
{
   vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

   DeviceNamePred pred(deviceName);
   device_server_list_t::iterator found = std::find_if(mDeviceServers.begin(), mDeviceServers.end(), pred);

   if (found != mDeviceServers.end())
   {
      return *found;
   }
   return DeviceServerPtr();
}

void RIMPlugin::removeDeviceServer(const std::string& deviceName)
{
   vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

   DeviceNamePred pred(deviceName);
   std::remove_if(mDeviceServers.begin(), mDeviceServers.end(), pred);
}

void RIMPlugin::debugDumpDeviceServers(int debug_level)
{
   vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

   vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                                    std::string("-------------- Device Servers --------------\n"),
                                    std::string("---------------------------------------------\n"));
   for ( device_server_list_t::iterator j = mDeviceServers.begin(); j != mDeviceServers.end(); j++ )
   {
      (*j)->debugDump(debug_level);
   }
}

void RIMPlugin::sendDataAndSync()
{
   //      vpr::Interval first;
   //      vpr::Interval second;

   //      std::cout << "Number Device Servers: " << mDeviceServers.size() << " Number Virtual Devices" << mVirtualDevices.size() << std::endl;

   vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

   // Update all local device servers and send their data.
   for ( unsigned int i=0; i<mDeviceServers.size(); i++ )
   {
      mDeviceServers[i]->updateLocalData();
      mDeviceServers[i]->send();
   }

   //      second.setNow();
   //      vpr::Interval diff_time4(second-first);
   //      std::cout << "Recv DeviceData Time: " << diff_time4.getBaseVal() << std::endl;
}

}  // end namespace cluster
