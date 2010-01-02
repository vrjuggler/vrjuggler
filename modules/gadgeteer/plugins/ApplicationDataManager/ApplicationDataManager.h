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

#ifndef _CLUSTER_APPLICATION_DATA_MANAGER_H
#define _CLUSTER_APPLICATION_DATA_MANAGER_H

#include <cluster/PluginConfig.h>
#include <boost/noncopyable.hpp>
#include <vpr/Util/GUID.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <cluster/ClusterPlugin.h>
#include <plugins/ApplicationDataManager/ApplicationDataServerPtr.h>
#include <gadget/NodePtr.h>

#include <list>
#ifdef VPR_HASH_MAP_INCLUDE
#  include VPR_HASH_MAP_INCLUDE
#else
#  include <map>
#endif

namespace cluster
{
   class ApplicationData;

class GADGET_CLUSTER_PLUGIN_CLASS_API ApplicationDataManager
   : public cluster::ClusterPlugin
{
protected:
   ApplicationDataManager();
public:
   virtual ~ApplicationDataManager();

   /**
    * Creates a ApplicationDataManager instance and returns it wrapped in a
    * ClusterPluginPtr object.
    *
    * @since 1.3.7
    */
   static ClusterPluginPtr create();
   
   /**
    * Get the GUID associated with this plugin.
    */
   vpr::GUID getHandlerGUID()
   {
      return ApplicationDataManager::mPluginGUID;
   }

   /**
    * Handle a incoming packet.
    */
   void handlePacket(PacketPtr packet, gadget::NodePtr node);
   
   /**
    * Called each frame by the kernel to update all application level data(ApplicationData).
    */
   virtual void preDraw();
   
   /**
    * Called each frame by the kernel to update the cluster after the InputManager has updated it's data.
    */
   virtual void postPostFrame();

   /**
    * Is this ClusterPlugin ready for the cluster to start the application.
    */
   virtual bool isPluginReady();

   /**
    * Return the name of this ClusterPlugin.
    */
   virtual std::string getPluginName()
   {
      return(std::string("ApplicationDataManager"));
   }
   virtual std::string getHandlerName()
   {
      return(std::string("ApplicationDataManager"));
   }
   
   virtual void recoverFromLostNode(gadget::NodePtr lostNode) 
   {
      boost::ignore_unused_variable_warning(lostNode);
   }

   // ---------- ApplicationData Interface ----------- //
public:
   /**
    * Add a ApplicationData object to the current configuration.
    *
    * @pre This should only be called by the ApplicationData constructor
    */
   virtual void addSerializableObject(vpr::SerializableObject* object);

   /**
    * Print a list of all ApplicationData objects currently in the configuration
    */
   void dumpApplicationData();
   
   // ---------- Helper Functions ----------- //
private:

   /**
    * Get a pointer to the ApplicationData object that is being updated by a remote machine
    */
   ApplicationData* getApplicationData(const vpr::GUID& id)
   {
      object_map_t::iterator found = mObjects.find(id);
      if (mObjects.end() == found)
      {
         return NULL;
      }
      return (*found).second;
   }

public:
   static const vpr::GUID                          mPluginGUID;

private:
#ifdef VPR_HASH_MAP_INCLUDE
   typedef std::hash_map<vpr::GUID, ApplicationData*, vpr::GUID::hash> object_map_t;
   typedef std::hash_map<vpr::GUID, ApplicationDataServerPtr, vpr::GUID::hash>  server_map_t;
#else
   typedef std::map<vpr::GUID, ApplicationData*> object_map_t;
   typedef std::map<vpr::GUID, ApplicationDataServerPtr>  server_map_t;
#endif

   object_map_t         mObjects;       /**< Application level ApplicationData list. */
   server_map_t         mServers;       /**< ApplicationData Server list. */
};

} // end namespace

#endif
