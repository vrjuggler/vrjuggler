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

#include <gadget/gadgetConfig.h>
#include <gadget/Util/Debug.h>

#include <cluster/Plugins/RemoteInputManager/RemoteInputManager.h>

// Accept Thread
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>

// Sharing Devices
#include <gadget/Type/BaseTypeFactory.h>
#include <cluster/Plugins/RemoteInputManager/VirtualDevice.h>
#include <cluster/Plugins/RemoteInputManager/DeviceServer.h>
#include <gadget/Type/DeviceFactory.h>

// ClusterNetwork
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>
#include <cluster/ClusterManager.h>

// IO Packets
#include <cluster/Packets/PacketFactory.h>
#include <cluster/Packets/ConnectionRequest.h>
#include <cluster/Packets/DeviceRequest.h>
#include <cluster/Packets/ApplicationDataRequest.h>
#include <cluster/Packets/EndBlock.h>

// Configuration
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ConfigChunkPtr.h>

#include <map>

namespace cluster
{
   vprSingletonImp( RemoteInputManager );
	
   RemoteInputManager::RemoteInputManager()
   {
      mFrameNumber = 0;
      mReconfigurationNeededOnConnection = false;
   }

	RemoteInputManager::~RemoteInputManager()
	{
      for(std::map<vpr::GUID, VirtualDevice*>::iterator j = mVirtualDevices.begin(); j != mVirtualDevices.end(); j++)
      {
         if ((*j).second != NULL)
         {
            delete (*j).second;
         }
      }  
      for(std::vector<DeviceServer*>::iterator j = mDeviceServers.begin(); j != mDeviceServers.end(); j++)
      {
         if ((*j) != NULL)
         {
            delete (*j);
         }
      }  
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

      for (std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
           i != mVirtualDevices.end() ; i++)
      {
         if ((*i).first == device_id)
         {
            return((*i).second->getDevice());
         }
      }
      return NULL;
   }

   gadget::Input* RemoteInputManager::getVirtualDevice(const std::string& device_name)
   { 
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);

      for (std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
           i != mVirtualDevices.end() ; i++)
      {
         if ((*i).second->getName() == device_name)
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
      for (std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
           i != mVirtualDevices.end() ; i++)
      {
         if ((*i).second->getRemoteHostname() == host_name)
         {
            devices_to_remove.push_back((*i).second->getName());
         }
      }

      for (std::vector<std::string>::iterator i = devices_to_remove.begin();
           i != devices_to_remove.end();i++)
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
      
      for (std::vector<cluster::DeviceServer*>::iterator i = mDeviceServers.begin();
           i != mDeviceServers.end() ; i++)
      {
         (*i)->removeClient(host_name);
      }
      return vpr::ReturnStatus::Succeed;
   }

   void RemoteInputManager::removeVirtualDevice(const vpr::GUID& device_id)
   {
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);
      
      for (std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
           i != mVirtualDevices.end() ; i++)
      {
         if ((*i).first == device_id)
         {
            delete (*i).second;
            mVirtualDevices.erase(i);
            return;
         }
      }
   }
   
   void RemoteInputManager::removeVirtualDevice(const std::string& device_name)
   { 
      vpr::Guard<vpr::Mutex> guard(mVirtualDevicesLock);
      
      for (std::map<vpr::GUID, VirtualDevice*>::iterator i = mVirtualDevices.begin();
           i != mVirtualDevices.end() ; i++)
      {
         if ((*i).second->getName() == device_name)
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
		for(std::map<vpr::GUID, VirtualDevice*>::iterator j = mVirtualDevices.begin(); j != mVirtualDevices.end(); j++)
		{
			(*j).second->debugDump(debug_level);
		}  
	}


   // ===================== DEVICE SERVERS =============================

   vpr::ReturnStatus RemoteInputManager::addDeviceServer(const std::string& name, gadget::Input* device)
   {
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);      
      
      DeviceServer* temp_device_server = new cluster::DeviceServer(name, device);
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
      
      for (std::vector<DeviceServer*>::iterator i = mDeviceServers.begin();
           i != mDeviceServers.end() ; i++)
      {
         if ((*i)->getName() == device_name)
         {
            return(*i);
         }
      }
      return NULL;
   }
   
   void RemoteInputManager::removeDeviceServer(const std::string& device_name)
   { 
      vpr::Guard<vpr::Mutex> guard(mDeviceServersLock);
      
      for (std::vector<DeviceServer*>::iterator i = mDeviceServers.begin();
           i != mDeviceServers.end() ; i++)
      {
         if ((*i)->getName() == device_name)
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
		for(std::vector<DeviceServer*>::iterator j = mDeviceServers.begin(); j != mDeviceServers.end(); j++)
		{
			(*j)->debugDump(debug_level);
		}  
	}
   
   void RemoteInputManager::preDraw()
   {;
   }
   void RemoteInputManager::postPostFrame()
   {
      updateAll();
   }

   void RemoteInputManager::updateAll()
   {
      mFrameNumber++;
                        //      vpr::Interval first;
                        //      vpr::Interval second;

                        //      std::cout << "Number Device Servers: " << mDeviceServers.size() << " Number Virtual Devices" << mVirtualDevices.size() << std::endl;

      lockDeviceServers();
      
            // Start rendering all the pipes
      for(unsigned int i=0; i<mDeviceServers.size(); i++)
         mDeviceServers[i]->go();
      
            // Start rendering all the pipes
      for(unsigned int i=0; i<mDeviceServers.size(); i++)
         mDeviceServers[i]->sync();

      unlockDeviceServers();  
            
      
      //////////////////////////////////////////////////////////////////////////////
      //                           1. Send DeviceReq                              //
      //////////////////////////////////////////////////////////////////////////////
      
      
      ClusterNetwork::instance()->lockClusterNodes();
      
      sendDeviceRequests();      
      
      ClusterNetwork::instance()->unlockClusterNodes();

                        //      second.setNow();
                        //      vpr::Interval diff_time4(second-first);
                        //      std::cout << "Recv DeviceData Time: " << diff_time4.getBaseVal() << std::endl;
   
   }



   //////////////////////////
   //    CONFIG METHODS    //
   //////////////////////////

   /** Add the pending chunk to the configuration.
    *  PRE: configCanHandle (chunk) == true.
    *  @return true iff chunk was successfully added to configuration.
    */
   bool RemoteInputManager::configAdd(jccl::ConfigChunkPtr chunk)
   {
     if (ClusterManager::instance()->recognizeRemoteDeviceConfig(chunk))
     {
        std::string device_host = chunk->getProperty<std::string>("deviceHost");
        ClusterNode* node = cluster::ClusterNetwork::instance()->getClusterNodeByName(device_host);
        std::string device_name = chunk->getName();

        vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[RemoteInputManager] ")
           << "Adding the Remote Device: " << device_name
           << " to the RIM Pending List\n" << vprDEBUG_FLUSH;

        if (node == NULL)
        {
           vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[RemoteInputManager] ")
              << "Cluster node: " << device_host
              << " does not exist! WE SHOULD NEVER REACH THIS HERE!!!!\n" << vprDEBUG_FLUSH;            
           return false;
        }
        else if (!node->isConnected())
        {
           vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[RemoteInputManager] ")
              << "Cluster node: " << device_host
              << " is not connected! WE SHOULD NEVER REACH THIS HERE!!!!\n" << vprDEBUG_FLUSH;            
           return false;
        }
        
        DeviceRequest* device_req = new DeviceRequest(device_name);
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


   /** Remove the pending chunk from the current configuration.
    *  PRE: configCanHandle (chunk) == true.
    *  @return true iff the chunk (and any objects it represented)
    *          were successfully removed.
    */
   bool RemoteInputManager::configRemove(jccl::ConfigChunkPtr chunk)
   {
      if (ClusterManager::instance()->recognizeRemoteDeviceConfig(chunk))
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL) << clrOutBOLD(clrCYAN,"[RemoteInputManager] ")
            << "Removing the Remote Device: " << chunk->getName() 
            << " from the active configuration \n" << vprDEBUG_FLUSH;

         removeVirtualDevice(chunk->getName());
         if (this->mVirtualDevices.size()== 0 && mDeviceServers.size() == 0)
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


   /** Checks if this handler can process chunk.
    *  Typically, an implementation of handler will check the chunk's
    *  description name/token to decide if it knows how to deal with
    *  it.
    *  @return true iff this handler can process chunk.
    */
   bool RemoteInputManager::configCanHandle(jccl::ConfigChunkPtr chunk)
   {
       return( ClusterManager::instance()->recognizeRemoteDeviceConfig(chunk) );

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
      std::vector<jccl::ConfigChunkPtr>::iterator active_begin = cfg_mgr->getActiveBegin();
      std::vector<jccl::ConfigChunkPtr>::iterator active_end   = cfg_mgr->getActiveEnd();      
      std::vector<jccl::ConfigChunkPtr>::iterator i;

        // Find the active device that we want to remove
      for (i = active_begin ; i != active_end ; i++)
      {
         if (/*recognizeRemoteDeviceConfig(*i) && */(*i)->getName() == device_name)
         {
            jccl::ConfigManager::PendingChunk pending;
            pending.mType = jccl::ConfigManager::PendingChunk::REMOVE;
            pending.mChunk = (*i);
            cfg_mgr->addPending(pending);
   
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
      std::vector<jccl::ConfigChunkPtr>::iterator active_begin = cfg_mgr->getActiveBegin();
      std::vector<jccl::ConfigChunkPtr>::iterator active_end   = cfg_mgr->getActiveEnd();      
      std::vector<jccl::ConfigChunkPtr>::iterator i;
      for (i = active_begin ; i != active_end ; i++)
      {
         if (/*recognizeRemoteDeviceConfig(*i) && */(*i)->getName() == device_name)
         {
            jccl::ConfigManager::PendingChunk pending_remove;
            pending_remove.mType = jccl::ConfigManager::PendingChunk::REMOVE;
            pending_remove.mChunk = (*i);
            cfg_mgr->addPending(pending_remove);

            jccl::ConfigManager::PendingChunk pending_add;
            pending_add.mType = jccl::ConfigManager::PendingChunk::ADD;
            pending_add.mChunk = (*i);
            cfg_mgr->addPending(pending_add);
            
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
      if (getReconfigurationNeededOnConnection())
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
      for ( i = begin ; i != end ; i++)
      {
         if ((*i).first->getDeviceName() == device_name)
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
      for (i = begin ; i != end ; i++)
      {
         if ((*i).second->isConnected())
         {
//            (*i).second->lockSocket();
//            (*i).first->send((*i).second->getSockStream());
            (*i).second->send((*i).first);
//            (*i).second->unlockSocket();
         }
      }
   }


}	// end namespace cluster
