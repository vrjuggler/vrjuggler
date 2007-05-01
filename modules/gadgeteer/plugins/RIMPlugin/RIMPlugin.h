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

#ifndef _CLUSTER_RIM_PLUGIN_H
#define _CLUSTER_RIM_PLUGIN_H

#include <cluster/PluginConfig.h>

#include <vpr/Util/Assert.h>

#include <cluster/ClusterPlugin.h>

#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Type/InputPtr.h>
#include <plugins/RIMPlugin/DeviceServerPtr.h>
#include <plugins/RIMPlugin/VirtualDevicePtr.h>

#include <list>
#ifdef VPR_HASH_MAP_INCLUDE
#  include VPR_HASH_MAP_INCLUDE
#else
#  include <map>
#endif

namespace gadget
{
   class Node;
}

namespace cluster
{
   class Packet;

class GADGET_CLUSTER_PLUGIN_CLASS_API RIMPlugin : public cluster::ClusterPlugin
{
public:
   RIMPlugin();
   virtual ~RIMPlugin();

   vpr::GUID getHandlerGUID()
   {
      return mHandlerGUID;
   }
   
   /**
    * This function is called when the network losses
    * a connection to a Node. The RIMPlugin needs to
    * handle this by removing all VirtualDevices on 
    * the lost node. And removing the node as a client
    * from all DeviceServers.
    */
   virtual void recoverFromLostNode(gadget::NodePtr lostNode);
   
   /**
    * Handle a incoming packet.
    */
   void handlePacket(Packet* packet, gadget::NodePtr node);

   /** Calls any action needed by this plugin before draw()
    *
    *  This function was inherited from the
    *  ClusterPlugin abstract class. 
    */
   virtual void preDraw();

   virtual void sendRequests();
   
   /** Calls any action needed by this plugin before postFrame()
    *
    *  This function was inherited from the
    *  ClusterPlugin abstract class. 
    */
   virtual void postPostFrame();
   
   /** Returns the status of RIMPlugin
    *
    *  @return true If plugin is completly configured
    *               and has no pending tasks.
    *  
    */
   virtual bool isPluginReady();
   
   /** 
    *  Returns the name of the this plugin
    */
   virtual std::string getPluginName()
   {
      return(std::string("RIMPlugin"));
   }
   
   virtual std::string getHandlerName()
   {
      return(std::string("RIMPlugin"));
   }

public:

   /** @name Virtual device management. */
   //@{
   gadget::InputPtr getVirtualDevice(const vpr::GUID& device_id);
   gadget::InputPtr getVirtualDevice(const std::string& device_name);
   //@}

   /** @name Configuration methods */
   //@{
   bool configAdd(jccl::ConfigElementPtr elm);
   bool configRemove(jccl::ConfigElementPtr elm);
   bool configCanHandle(jccl::ConfigElementPtr elm);
   //@}

   /** @name Debug methods */
   //@{
   void debugDumpDeviceServers(int debug_level);
   void debugDumpVirtualDevices(int debug_level);
   //@}

private:
   /** @name VirtualDevice methods */
   //@{
   bool addVirtualDevice(const vpr::GUID& device_id, const std::string& name,
                         const std::string& device_base_type,
                         const std::string& hostname);

   void removeVirtualDevice(const std::string& device_name);
   void removeVirtualDevice(const vpr::GUID& device_id);
   bool removeVirtualDevicesOnHost(const std::string& hostName);
   //@}

   /** @name DeviceServer methods */
   //@{
   bool addDeviceServer(const std::string& name, gadget::InputPtr device);
   void removeDeviceServer(const std::string& device_name);
   void removeDeviceServer(const vpr::Uint16& device_id);
   DeviceServerPtr getDeviceServer(const std::string& device_name);
   //@}

protected:
   vpr::GUID                    mHandlerGUID;

#ifdef VPR_HASH_MAP_INCLUDE
   typedef std::hash_map<vpr::GUID, VirtualDevicePtr, vpr::GUID::hash> virtual_device_map_t;
#else
   typedef std::map<vpr::GUID, VirtualDevicePtr> virtual_device_map_t;
#endif

   virtual_device_map_t         mVirtualDevices;     /**< List of Virtual Devices on the local Node. */

   typedef std::vector<DeviceServerPtr> device_server_list_t;
   device_server_list_t         mDeviceServers;      /**< List of Devices that should act as servers to remote Nodes.*/
};

} // end namespace gadget

#endif /* _CLUSTER_RIM_PLUGIN_H */
