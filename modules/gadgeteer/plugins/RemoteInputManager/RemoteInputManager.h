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

#ifndef _CLUSTER_REMOTE_INPUT_MANAGER_H
#define _CLUSTER_REMOTE_INPUT_MANAGER_H

#include <vpr/Util/Singleton.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/ReturnStatus.h>	
#include <vpr/Util/Assert.h>

#include <gadget/gadgetConfig.h>
// Must implement the Abstract Base Class in order to be a manager used on the ClusterNetwork
#include <cluster/ClusterPlugin.h>

#include <jccl/Config/ConfigChunkPtr.h>

#include <list>

namespace gadget
{
   class Input;
}
namespace cluster
{
   class VirtualDevice;
   class DeviceServer;
   class ClusterNode;
   class DeviceRequest;

class GADGET_CLASS_API RemoteInputManager  : public cluster::ClusterPlugin
{
   vprSingletonHeader( RemoteInputManager );
public:
   RemoteInputManager();
   virtual ~RemoteInputManager();

   /** Calls any action needed by this plugin before draw()
    *
    *  This function was inherited from the
    *  ClusterPlugin abstract class. 
    */
   virtual void preDraw();
   
   /** Calls any action needed by this plugin before postFrame()
    *
    *  This function was inherited from the
    *  ClusterPlugin abstract class. 
    */
   virtual void postPostFrame();
   
   /** Returns the status of RemoteInputManager
    *
    *  @return true If plugin is completly configured
    *               and has no pending tasks.
    *  
    */
   virtual bool isPluginReady();
   
   /** 
    *  Returns the name of the this plugin
    */
   virtual std::string getManagerName()
   {
      return(std::string("RemoteInputManager"));
   }
     
   /**
    * Create a new virtual device on the local system and add
    * it to the list of virtual devices.
    */
   vpr::ReturnStatus addVirtualDevice(const vpr::GUID& device_id, const std::string& name, 
                                      const std::string& device_base_type, const std::string& hostname);
   void addVirtualDevice(VirtualDevice* device);
   void removeVirtualDevice(const std::string& device_name);
   void removeVirtualDevice(const vpr::GUID& device_id);
   void debugDumpVirtualDevices(int debug_level);

   /** Locks the active list.
    *
    *  This function blocks until it can lock the std::map of active
    *  ClusterNodes.
    *
    *  The caller of this method must call unlockActive() when it
    *  is finished viewing/modifying the active list.
    */
   void lockVirtualDevices()
   { mVirtualDevicesLock.acquire(); }

   /** Unlocks the active list.
    *
    *  The method releases the lock on the active connections std::map.
    *
    *  The caller of this method must have previously locked the active
    *  list with lockActive().
    */
   void unlockVirtualDevices()
   { mVirtualDevicesLock.release(); }

   gadget::Input* getVirtualDevice(const vpr::GUID& device_id);
   
   gadget::Input* getVirtualDevice(const std::string& device_name);

   vpr::ReturnStatus removeVirtualDevicesOnHost(const std::string& host_name);

   vpr::ReturnStatus removeDeviceClientsForHost(const std::string& host_name);

   vpr::Uint16    getNumVirtualDevices()
   {
      return mVirtualDevices.size();
   }
   
   /** Get an iterator to the beginning of the active std::map.
    *  The caller of this method must have locked the active list.
    */
   std::map<vpr::GUID, cluster::VirtualDevice*>::iterator getVirtualDevicesBegin()
   {
      vprASSERT(1 == mVirtualDevicesLock.test());
      return mVirtualDevices.begin();
   }
   
   /** Get an iterator to the end of the active list.
    *  The caller of this method must have locked the active list.
    */
   std::map<vpr::GUID, cluster::VirtualDevice*>::iterator getVirtualDevicesEnd()
   {
      vprASSERT(1 == mVirtualDevicesLock.test());
      return mVirtualDevices.end();
   }

   // =================== VIRTUAL DEVICES ======================

   vpr::ReturnStatus addDeviceServer(const std::string& name, gadget::Input* device);
   void addDeviceServer(DeviceServer* device);
   void removeDeviceServer(const std::string& device_name);
   void removeDeviceServer(const vpr::Uint16& device_id);
     
   
   void debugDumpDeviceServers(int debug_level);

   /** Locks the active list.
    *
    *  This function blocks until it can lock the std::map of active
    *  ClusterNodes.
    *
    *  The caller of this method must call unlockActive() when it
    *  is finished viewing/modifying the active list.
    */
   void lockDeviceServers()
   { mDeviceServersLock.acquire(); }

   /** Unlocks the active list.
    *
    *  The method releases the lock on the active connections std::map.
    *
    *  The caller of this method must have previously locked the active
    *  list with lockActive().
    */
   void unlockDeviceServers()
   { mDeviceServersLock.release(); }
   
   DeviceServer* getDeviceServer(const std::string& device_name);
   
   /** Get an iterator to the beginning of the active std::map.
    *  The caller of this method must have locked the active list.
    */
   std::vector<cluster::DeviceServer*>::iterator getDeviceServersBegin()
   {
      vprASSERT(1 == mDeviceServersLock.test());
      return mDeviceServers.begin();
   }
   
   /** Get an iterator to the end of the active list.
    *  The caller of this method must have locked the active list.
    */
   std::vector<cluster::DeviceServer*>::iterator getDeviceServersEnd()
   {
      vprASSERT(1 == mDeviceServersLock.test());
      return mDeviceServers.end();
   }
   
   void updateAll();
   
   void setReconfigurationNeededOnConnection(bool val)
   { mReconfigurationNeededOnConnection = val; }

   bool getReconfigurationNeededOnConnection()
   { return(mReconfigurationNeededOnConnection); }

   void updateForNewConnection();

   //////////////////////////
   //    CONFIG METHODS    //
   //////////////////////////

   bool configAdd(jccl::ConfigChunkPtr chunk);
   bool configRemove(jccl::ConfigChunkPtr chunk);
   bool configCanHandle(jccl::ConfigChunkPtr chunk);
   jccl::ConfigChunkPtr getConfigChunkPointer(std::string& name);
   vpr::ReturnStatus createPendingConfigRemove(std::string device_name);
   vpr::ReturnStatus createPendingConfigRemoveAndAdd(std::string device_name);
   
   void addPendingDeviceRequest(DeviceRequest* new_device_req, ClusterNode* node);
   void removePendingDeviceRequest(std::string device_name);
   void sendDeviceRequests();
   vpr::Uint16 getNumberPendingDeviceRequests();
   
private:   
   std::map<DeviceRequest*, ClusterNode*>          mPendingDeviceRequests;     /**< UserData Request list. */
   vpr::Mutex                                      mPendingDeviceRequestsLock; /**< Lock on UserData Request list.*/   
   
   std::map<vpr::GUID, cluster::VirtualDevice*>    mVirtualDevices;     /**< List of Virtual Devices on the local Node. */
   vpr::Mutex                                      mVirtualDevicesLock; /**< Lock on Virtual Device list.*/

   std::vector<cluster::DeviceServer*>             mDeviceServers;      /**< List of Devices that should act as servers to remote ClusterNodes.*/
   vpr::Mutex                                      mDeviceServersLock;  /**< Lock on Device Server list.*/

   vpr::Uint32                                     mFrameNumber;
   bool                          mReconfigurationNeededOnConnection;    /**< We need to try to reconfigure on the next connection made. */
};

} // end namespace gadget

#endif
