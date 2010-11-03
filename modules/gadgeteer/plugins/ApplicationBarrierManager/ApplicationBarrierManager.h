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

#ifndef _CLUSTER_APPLICATION_BARRIER_MANAGER_H
#define _CLUSTER_APPLICATION_BARRIER_MANAGER_H

#include <cluster/PluginConfig.h>

#include <list>
#include <boost/noncopyable.hpp>
#include <boost/version.hpp>

#include <vpr/vpr.h>

#if defined(__GNUC__) && __GNUC__ >= 4
#  include <tr1/unordered_map>
#elif defined(_MSC_VER) && _MSC_VER >= 1500
#  include <unordered_map>
#elif BOOST_VERSION >= 103600
#  include <boost/unordered_map.hpp>
#elif defined(VPR_HASH_MAP_INCLUDE)
#  include VPR_HASH_MAP_INCLUDE
#else
#  include <map>
#endif

#include <vpr/Util/GUID.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <cluster/ClusterPlugin.h>
#include <plugins/ApplicationBarrierManager/ApplicationBarrierServerPtr.h>
#include <gadget/NodePtr.h>


namespace cluster
{
   class ApplicationBarrier;

class GADGET_CLUSTER_PLUGIN_CLASS_API ApplicationBarrierManager
   : public cluster::ClusterPlugin
{
protected:
   ApplicationBarrierManager();
public:
   virtual ~ApplicationBarrierManager();

   /**
    * Creates a ApplicationBarrierManager instance and returns it wrapped in a
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
      return ApplicationBarrierManager::mPluginGUID;
   }

   /**
    * Handle a incoming packet.
    */
   void handlePacket(PacketPtr packet, gadget::NodePtr node);
   
   /**
    * Called each frame by the kernel to update all application level data(ApplicationBarrier).
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
      return(std::string("ApplicationBarrierManager"));
   }
   virtual std::string getHandlerName()
   {
      return(std::string("ApplicationBarrierManager"));
   }
   
   virtual void recoverFromLostNode(gadget::NodePtr lostNode) 
   {
      boost::ignore_unused_variable_warning(lostNode);
   }

   // ---------- ApplicationBarrier Interface ----------- //
public:
   /**
    * Add a ApplicationBarrier object to the current configuration.
    *
    * @pre This should only be called by the ApplicationBarrier constructor
    */
   virtual void addSerializableObject(vpr::SerializableObject* object);

   virtual void sendWait(const vpr::GUID& id);

   /**
    * Print a list of all ApplicationBarrier objects currently in the configuration
    */
   void dumpApplicationBarrier();
   
   // ---------- Helper Functions ----------- //
private:

   /**
    * Get a pointer to the ApplicationBarrier object that is being updated by a remote machine
    */
   ApplicationBarrier* getApplicationBarrier(const vpr::GUID& id)
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
#if defined(__GNUC__) && __GNUC__ >= 4 || defined(_MSC_VER) && _MSC_VER >= 1500
   typedef std::tr1::unordered_map<vpr::GUID
                                , ApplicationBarrier*
                                , vpr::GUID::hash> object_map_t;
   typedef std::tr1::unordered_map<vpr::GUID
                                , ApplicationBarrierServerPtr
                                , vpr::GUID::hash> server_map_t;
#elif BOOST_VERSION >= 103600
   typedef boost::unordered_map<vpr::GUID
                              , ApplicationBarrier*
                              , vpr::GUID::hash> object_map_t;
   typedef boost::unordered_map<vpr::GUID
                              , ApplicationBarrierServerPtr
                              , vpr::GUID::hash> server_map_t;
#elif defined(VPR_HASH_MAP_INCLUDE)
   typedef std::hash_map<vpr::GUID, ApplicationBarrier*, vpr::GUID::hash> object_map_t;
   typedef std::hash_map<vpr::GUID, ApplicationBarrierServerPtr, vpr::GUID::hash>  server_map_t;
#else
   typedef std::map<vpr::GUID, ApplicationBarrier*> object_map_t;
   typedef std::map<vpr::GUID, ApplicationBarrierServerPtr>  server_map_t;
#endif

   object_map_t         mObjects;       /**< Application level ApplicationBarrier list. */
   server_map_t         mServers;       /**< ApplicationBarrier Server list. */
};

} // end namespace

#endif
