/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <cluster/PluginConfig.h>
#include <gadget/Util/Debug.h>

#include <plugins/RemoteInputManager/RemoteInputManager.h>

// Accept Thread
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>

// Sharing Devices
#include <gadget/Type/BaseTypeFactory.h>
#include <plugins/RemoteInputManager/VirtualDevice.h>
#include <plugins/RemoteInputManager/DeviceServer.h>
#include <gadget/Type/DeviceFactory.h>
#include <gadget/InputManager.h>

// ClusterNetwork
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>
#include <cluster/ClusterManager.h>

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
   GADGET_CLUSTER_PLUGIN_EXPORT(void) initPlugin(cluster::ClusterManager* mgr)
   {
      mgr->addPlugin(new cluster::RemoteInputManager());
   }
}

namespace cluster
{
   RemoteInputManager::RemoteInputManager() 
   : mPluginGUID("a35b834d-e7e8-4f34-830b-755c282845b5")
   {
      mFrameNumber = 0;
      mReconfigurationNeededOnConnection = false;
   }

   RemoteInputManager::~RemoteInputManager()
   {
      for ( std::map<vpr::GUID, VirtualDevice*>::iterator j = mVirtualDevices.begin(); j != mVirtualDevices.end(); j++ )
      {
         if ( (*j).second != NULL )
         {
            delete (*j).second;
         }
      }  
      for ( std::vector<DeviceServer*>::iterator j = mDeviceServers.begin(); j != mDeviceServers.end(); j++ )
      {
         if ( (*j) != NULL )
         {
            delete (*j);
         }
      }  
   }

   void RemoteInputManager::recoverFromLostNode(ClusterNode* lost_node)
   {
      removeVirtualDevicesOnHost(lost_node->getHostname());
      removeDeviceClientsForHost(lost_node->getHostname());
      
      // Since we have lost a connection we need to set a flag so 
      // that when we gain a new connection we will reconfigure.
      setReconfigurationNeededOnConnection(true);
   }

   /**
    * Handle a incoming packet.
    */
   void RemoteInputManager::handlePacket(Packet* packet, ClusterNode* node)
   {
      //We are only handling data packets right now.
      if ( NULL != packet && NULL != node )
      {
         switch ( packet->getPacketType() )
         {
         case cluster::Header::RIM_DEVICE_REQ:
            {
               DeviceRequest* temp_device_request = dynamic_cast<DeviceRequest*>(packet);
               vprASSERT(NULL != temp_device_request && "Dynamic cast failed!");
               std::string device_name = temp_device_request->getDeviceName();

               if ( jccl::ConfigManager::instance()->isPendingStale() )
               {
                  gadget::Input* temp_input_device = gadget::InputManager::instance()->getDevice(device_name);
                  if ( temp_input_device != NULL )
                  {
                     DeviceServer* temp_device_server = getDeviceServer(device_name);
                     if ( NULL == temp_device_server )
                     {
                        addDeviceServer(device_name, temp_input_device);
                        temp_device_server = getDeviceServer(device_name);
                     }

                     temp_device_server->addClient(node);

                     // Create a responce ACK
                     std::string temp_string = temp_input_device->getBaseType();
                     vpr::GUID   temp_guid   = temp_device_server->getId();
                     DeviceAck* temp_ack = new DeviceAck(getPluginGUID(), temp_guid, device_name, temp_string, true);
                     node->send(temp_ack);
                  }
                  else
                  {
                     std::string temp_string = "";
                     vpr::GUID empty_id;
                     DeviceAck* temp_ack = new DeviceAck(getPluginGUID(), empty_id, device_name, temp_string/*BaseType*/, false);
                     node->send(temp_ack);
                  }
               }
               else
               {
                  vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) 
                  << clrOutBOLD(clrRED,"Pending List is not stale(Config Manager is still configuring the local system) ")
                  << clrOutBOLD(clrRED,"So we can not process this device request right now.") << std::endl << vprDEBUG_FLUSH;            

                  std::string temp_string = "";
                  vpr::GUID empty_id;
                  DeviceAck* temp_ack = new DeviceAck(getPluginGUID(), empty_id, device_name, temp_string/*BaseType*/, false);
                  node->send(temp_ack);
               }
               break;
            }
         case cluster::Header::RIM_DEVICE_ACK:
            {
               // -If ACK
               //   -Create VirtualDevice
               // -If Nack
               //   -Do nothing(let the config manager worry about re-trying)

               DeviceAck* temp_device_ack = dynamic_cast<DeviceAck*>(packet);
               vprASSERT(NULL != temp_device_ack && "Dynamic cast failed!");
               std::string device_name = temp_device_ack->getDeviceName();

               if ( temp_device_ack->getAck() )
               {
                  removePendingDeviceRequest(device_name);

                  if ( getVirtualDevice(device_name) != NULL )
                  {
                     vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrRED, "ERROR:") 
                     << "Somehow we already have a virtual device named: " << device_name << std::endl << vprDEBUG_FLUSH;
                  }
                  else
                  {
                     addVirtualDevice(temp_device_ack->getId(), device_name, temp_device_ack->getDeviceBaseType(), temp_device_ack->getHostname());
                     
                     // Add this virtual device to the InputManager's list of devices.
                     gadget::InputManager::instance()->addRemoteDevice(getVirtualDevice(device_name), device_name);
                  }
               }
               else
               {  //XXX: FIX
                  // Do Nothing Since we will just re-try later
                  //createPendingConfigRemoveAndAdd(mDeviceName);
                  //jccl::ConfigManager::instance()->delayStalePendingList();
               }
               break;
            }
         case cluster::Header::RIM_DATA_PACKET:
            {
               DataPacket* temp_data_packet = dynamic_cast<DataPacket*>(packet);
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
               std::cout << "RIM DOES NOT HANDLE THIS PACKET TYPE" << packet->getPacketType() << std::endl;
               break;
            }
         } // End switch
      } // End if
   }


   vpr::ReturnStatus RemoteInputManager::addVirtualDevice(const vpr::GUID& device_id, const std::string& name, 
                                                          const std::string& device_base_type, const std::string& hostname)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      << clrOutBOLD(clrMAGENTA, "[RemoteInputManager]")
      << "Creating Virtual Device: " << name << std::endl << vprDEBUG_FLUSH;

      gadget::Input* temp_input_device = gadget::BaseTypeFactory::instance()->loadNetDevice(device_base_type);
      VirtualDevice* temp_virtual_device = new cluster::VirtualDevice(name, device_id, device_base_type, hostname, temp_input_device);

      mVirtualDevices[device_id] = temp_virtual_device;

      return(vpr::ReturnStatus::Succeed);
   }

   void RemoteInputManager::addVirtualDevice(VirtualDevice* device)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      mVirtualDevices[device->getId()] = device;
   }

   gadget::Input* RemoteInputManager::getVirtualDevice(const vpr::GUID& device_id)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      for ( std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
          i != mVirtualDevices.end() ; i++ )
      {
         if ( (*i).first == device_id )
         {
            return((*i).second->getDevice());
         }
      }
      return NULL;
   }

   gadget::Input* RemoteInputManager::getVirtualDevice(const std::string& device_name)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      for ( std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
          i != mVirtualDevices.end() ; i++ )
      {
         if ( (*i).second->getName() == device_name )
         {
            return((*i).second->getDevice());
         }
      }
      return NULL;
   }

   vpr::ReturnStatus RemoteInputManager::removeVirtualDevicesOnHost(const std::string& host_name)
   {
      // - Get a list of all remote devices on the given host
      // - Remove them from the current configuration
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      std::vector<std::string> devices_to_remove;
      for ( std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
          i != mVirtualDevices.end() ; i++ )
      {
         if ( (*i).second->getRemoteHostname() == host_name )
         {
            devices_to_remove.push_back((*i).second->getName());
         }
      }

      for ( std::vector<std::string>::iterator i = devices_to_remove.begin();
          i != devices_to_remove.end();i++ )
      {
         // We could just remove it here, but for the sake of testing RTRC
         // we will create a pending remove
         // removeVirtualDevice(*i);
         createPendingConfigRemoveAndAdd(*i);
      }
      return vpr::ReturnStatus::Succeed;
   }

   vpr::ReturnStatus RemoteInputManager::removeDeviceClientsForHost(const std::string& host_name)
   {
      // - Loop through all Device Servers and remove any device clients that may exist for the given host
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      << clrOutBOLD(clrMAGENTA,"[RemoteInputManager]")
      << " Removing client, " << host_name << " from all Device Servers.\n" << vprDEBUG_FLUSH;

      for ( std::vector<cluster::DeviceServer*>::iterator i = mDeviceServers.begin();
          i != mDeviceServers.end() ; i++ )
      {
         (*i)->removeClient(host_name);
      }
      return vpr::ReturnStatus::Succeed;
   }

   void RemoteInputManager::removeVirtualDevice(const vpr::GUID& device_id)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      for ( std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
          i != mVirtualDevices.end() ; i++ )
      {
         if ( (*i).first == device_id )
         {
            // Remove remote device from the InputManager
            gadget::InputManager::instance()->removeDevice((*i).second->getName());
            delete (*i).second;
            mVirtualDevices.erase(i);
            return;
         }
      }
   }

   void RemoteInputManager::removeVirtualDevice(const std::string& device_name)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      // Remove remote device from the InputManager
      gadget::InputManager::instance()->removeDevice(device_name);

      for ( std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
          i != mVirtualDevices.end() ; i++ )
      {
         if ( (*i).second->getName() == device_name )
         {
            (*i).second->debugDump(vprDBG_CONFIG_LVL);
            delete (*i).second;
            mVirtualDevices.erase(i);
            return;
         }
      }
   }

   void RemoteInputManager::debugDumpVirtualDevices(int debug_level)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                                       std::string("-------------- Virtual Devices --------------\n"),
                                       std::string("---------------------------------------------\n"));
      for ( std::map<vpr::GUID, VirtualDevice*>::iterator j = mVirtualDevices.begin(); j != mVirtualDevices.end(); j++ )
      {
         (*j).second->debugDump(debug_level);
      }  
   }


   // ===================== DEVICE SERVERS =============================

   vpr::ReturnStatus RemoteInputManager::addDeviceServer(const std::string& name, gadget::Input* device)
   {
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);      

      DeviceServer* temp_device_server = new cluster::DeviceServer(name, device, mPluginGUID);
      mDeviceServers.push_back(temp_device_server);
      // Set this manager active
      setActive(true);

      return(vpr::ReturnStatus::Succeed);
   }

   void RemoteInputManager::addDeviceServer(DeviceServer* device)
   {
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);
      mDeviceServers.push_back(device);

      // Set this manager active
      setActive(true);
   }

   DeviceServer* RemoteInputManager::getDeviceServer(const std::string& device_name)
   {
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

      for ( std::vector<DeviceServer*>::iterator i = mDeviceServers.begin();
          i != mDeviceServers.end() ; i++ )
      {
         if ( (*i)->getName() == device_name )
         {
            return(*i);
         }
      }
      return NULL;
   }

   void RemoteInputManager::removeDeviceServer(const std::string& device_name)
   {
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

      for ( std::vector<DeviceServer*>::iterator i = mDeviceServers.begin();
          i != mDeviceServers.end() ; i++ )
      {
         if ( (*i)->getName() == device_name )
         {
            delete (*i);
            mDeviceServers.erase(i);
            return;
         }
      }
   }

   void RemoteInputManager::debugDumpDeviceServers(int debug_level)
   {
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);

      vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                                       std::string("-------------- Device Servers --------------\n"),
                                       std::string("---------------------------------------------\n"));
      for ( std::vector<DeviceServer*>::iterator j = mDeviceServers.begin(); j != mDeviceServers.end(); j++ )
      {
         (*j)->debugDump(debug_level);
      }  
   }

   void RemoteInputManager::preDraw()
   {
      ;
   }

   void RemoteInputManager::postPostFrame()
   {
      mFrameNumber++;
      //      vpr::Interval first;
      //      vpr::Interval second;

      //      std::cout << "Number Device Servers: " << mDeviceServers.size() << " Number Virtual Devices" << mVirtualDevices.size() << std::endl;

      lockDeviceServers();

      // Start rendering all the pipes
      for ( unsigned int i=0; i<mDeviceServers.size(); i++ )
         mDeviceServers[i]->go();

      // Start rendering all the pipes
      for ( unsigned int i=0; i<mDeviceServers.size(); i++ )
         mDeviceServers[i]->sync();

      unlockDeviceServers();  

      //      second.setNow();
      //      vpr::Interval diff_time4(second-first);
      //      std::cout << "Recv DeviceData Time: " << diff_time4.getBaseVal() << std::endl;

   }

   void RemoteInputManager::sendRequests()
   {
      //////////////////////////////////////////////////////////////////////////////
      //                           1. Send DeviceReq                              //
      //////////////////////////////////////////////////////////////////////////////

      ClusterNetwork::instance()->lockClusterNodes();

      sendDeviceRequests();      

      ClusterNetwork::instance()->unlockClusterNodes();

   }



   //////////////////////////
   //    CONFIG METHODS    //
   //////////////////////////

   /** Add the pending element to the configuration.
    *  @pre configCanHandle (element) == true.
    *  @return true iff element was successfully added to configuration.
    */
   bool RemoteInputManager::configAdd(jccl::ConfigElementPtr element)
   {
      if ( ClusterManager::instance()->recognizeRemoteDeviceConfig(element) )
      {
         std::string device_host = element->getProperty<std::string>("device_host");
         ClusterNode* node = cluster::ClusterNetwork::instance()->getClusterNodeByName(device_host);
         std::string device_name = element->getName();

         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[RemoteInputManager] ")
         << "Adding the Remote Device: " << device_name
         << " to the RIM Pending List\n" << vprDEBUG_FLUSH;

         if ( node == NULL )
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[RemoteInputManager] ")
            << "Cluster node: " << device_host
            << " does not exist! WE SHOULD NEVER REACH THIS HERE!!!!\n" << vprDEBUG_FLUSH;            
            return false;
         }
         else if ( !node->isConnected() )
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[RemoteInputManager] ")
            << "Cluster node: " << device_host
            << " is not connected! WE SHOULD NEVER REACH THIS HERE!!!!\n" << vprDEBUG_FLUSH;            
            return false;
         }

         DeviceRequest* device_req = new DeviceRequest(getPluginGUID(), device_name);
         //device_req->send(node->getSockStream());
         //delete device_req;
         //node->addDeviceRequest(device_req);
         addPendingDeviceRequest(device_req, node);
         setActive(true);
         return(true);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) 
         << clrOutBOLD(clrRED,"[RemoteInputManager::ConfigAdd] ERROR, Something is seriously wrong, we should never get here\n")
         << vprDEBUG_FLUSH;
         return(false);
      }
   }


   /** Remove the pending element from the current configuration.
    *  @pre configCanHandle (element) == true.
    *  @return true iff the element (and any objects it represented)
    *          were successfully removed.
    */
   bool RemoteInputManager::configRemove(jccl::ConfigElementPtr element)
   {
      if ( ClusterManager::instance()->recognizeRemoteDeviceConfig(element) )
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[RemoteInputManager] ")
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
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << "[RemoteInputManager::configRemove] ERROR, Something is seriously wrong, we should never get here\n"
         << vprDEBUG_FLUSH;
         return(false);
      }
   }


   /** Checks if this handler can process element.
    *  Typically, an implementation of handler will check the element's
    *  description name/token to decide if it knows how to deal with
    *  it.
    *  @return true iff this handler can process element.
    */
   bool RemoteInputManager::configCanHandle(jccl::ConfigElementPtr element)
   {
      return ClusterManager::instance()->recognizeRemoteDeviceConfig(element);
   }

   vpr::Uint16 RemoteInputManager::getNumberPendingDeviceRequests()
   {
      vpr::Guard<vpr::Mutex> guard(mPendingDeviceRequestsLock);
      return(mPendingDeviceRequests.size());
   }


   bool RemoteInputManager::isPluginReady()
   {
      vpr::Guard<vpr::Mutex> guard(mPendingDeviceRequestsLock);
      return(0 == mPendingDeviceRequests.size());
   }

   vpr::ReturnStatus RemoteInputManager::createPendingConfigRemove(std::string device_name)
   {
      jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();

      cfg_mgr->lockActive();
      std::vector<jccl::ConfigElementPtr>::iterator active_begin = cfg_mgr->getActiveBegin();
      std::vector<jccl::ConfigElementPtr>::iterator active_end   = cfg_mgr->getActiveEnd();      
      std::vector<jccl::ConfigElementPtr>::iterator i;

      // Find the active device that we want to remove
      for ( i = active_begin ; i != active_end ; i++ )
      {
         if ( /*recognizeRemoteDeviceConfig(*i) && */(*i)->getName() == device_name )
         {
            jccl::ConfigManager::PendingElement pending;
            pending.mType = jccl::ConfigManager::PendingElement::REMOVE;
            pending.mElement = (*i);

            cfg_mgr->lockPending();
            cfg_mgr->addPending(pending);
            cfg_mgr->unlockPending();

            cfg_mgr->unlockActive();
            cfg_mgr->removeActive(device_name);
            cfg_mgr->lockActive();
         }
      }
      cfg_mgr->unlockActive();
      return vpr::ReturnStatus::Succeed;
   }

   vpr::ReturnStatus RemoteInputManager::createPendingConfigRemoveAndAdd(std::string device_name)
   {
      jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();

      cfg_mgr->lockActive();
      std::vector<jccl::ConfigElementPtr>::iterator active_begin = cfg_mgr->getActiveBegin();
      std::vector<jccl::ConfigElementPtr>::iterator active_end   = cfg_mgr->getActiveEnd();      
      std::vector<jccl::ConfigElementPtr>::iterator i;
      for ( i = active_begin ; i != active_end ; i++ )
      {
         if ( /*recognizeRemoteDeviceConfig(*i) && */(*i)->getName() == device_name )
         {
            jccl::ConfigManager::PendingElement pending_remove;
            pending_remove.mType = jccl::ConfigManager::PendingElement::REMOVE;
            pending_remove.mElement = (*i);

            cfg_mgr->lockPending();
            cfg_mgr->addPending(pending_remove);
            cfg_mgr->unlockPending();

            jccl::ConfigManager::PendingElement pending_add;
            pending_add.mType = jccl::ConfigManager::PendingElement::ADD;
            pending_add.mElement = (*i);

            cfg_mgr->lockPending();
            cfg_mgr->addPending(pending_add);
            cfg_mgr->unlockPending();

            cfg_mgr->unlockActive();
            cfg_mgr->removeActive(device_name);
            cfg_mgr->lockActive();
         }
      }
      cfg_mgr->unlockActive();
      return vpr::ReturnStatus::Succeed;
   }

   void RemoteInputManager::updateForNewConnection()
   {
      if ( getReconfigurationNeededOnConnection() )
      {
         jccl::ConfigManager::instance()->refreshPendingList();
         setReconfigurationNeededOnConnection(false);         
      }
   }
   void RemoteInputManager::addPendingDeviceRequest(DeviceRequest* new_device_req, ClusterNode* node)
   {
      vpr::Guard<vpr::Mutex> guard(mPendingDeviceRequestsLock);
      mPendingDeviceRequests[new_device_req] = node;
   }

   void RemoteInputManager::removePendingDeviceRequest(std::string device_name)
   {
      vpr::Guard<vpr::Mutex> guard(mPendingDeviceRequestsLock);
      std::map<DeviceRequest*, ClusterNode*>::iterator begin = mPendingDeviceRequests.begin();
      std::map<DeviceRequest*, ClusterNode*>::iterator end = mPendingDeviceRequests.end();
      std::map<DeviceRequest*, ClusterNode*>::iterator i;
      for ( i = begin ; i != end ; i++ )
      {
         if ( (*i).first->getDeviceName() == device_name )
         {
            mPendingDeviceRequests.erase(i);
            return;
         }
      }
   }
   void RemoteInputManager::sendDeviceRequests()
   {
      vpr::Guard<vpr::Mutex> guard(mPendingDeviceRequestsLock);

      std::map<DeviceRequest*, ClusterNode*>::iterator begin = mPendingDeviceRequests.begin();
      std::map<DeviceRequest*, ClusterNode*>::iterator end   = mPendingDeviceRequests.end();
      std::map<DeviceRequest*, ClusterNode*>::iterator i;
      for ( i = begin ; i != end ; i++ )
      {
         if ( (*i).second->isConnected() )
         {
            //            (*i).second->lockSocket();
            //            (*i).first->send((*i).second->getSockStream());
            (*i).second->send((*i).first);
            //            (*i).second->unlockSocket();
         }
      }
   }


}  // end namespace cluster
