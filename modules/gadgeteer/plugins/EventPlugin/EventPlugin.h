/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _CLUSTER_EVENT_PLUGIN_H
#define _CLUSTER_EVENT_PLUGIN_H

#include <cluster/PluginConfig.h>

#include <list>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/version.hpp>

#include <vpr/vpr.h>

#if defined(__GNUC__) && __GNUC__ >= 4
#  include <tr1/unordered_map>
#elif defined(_MSC_VER) && _MSC_VER >= 1500
#  include <unordered_map>
#elif BOOST_VERSION >= 103600
#  include <boost/unordered_map.hpp>
/#elif defined(VPR_HASH_MAP_INCLUDE)
#  include VPR_HASH_MAP_INCLUDE
#else
#  include <map>
#endif

#include <vpr/Util/GUID.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <cluster/ClusterPlugin.h>
#include <gadget/NodePtr.h>


namespace cluster
{
   class ApplicationData;

class GADGET_CLUSTER_PLUGIN_CLASS_API EventPlugin
   : public cluster::ClusterPlugin
{
protected:
   EventPlugin();

public:
   typedef boost::function<void (const std::string&)> callback_t;

public:
   virtual ~EventPlugin();

   /**
    * Creates a EventPlugin instance and returns it wrapped in a
    * ClusterPluginPtr object.
    */
   static ClusterPluginPtr create();
   
   /**
    * Get the GUID associated with this plugin.
    */
   vpr::GUID getHandlerGUID()
   {
      return EventPlugin::mPluginGUID;
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
      return "EventPlugin";
   }
   virtual std::string getHandlerName()
   {
      return "EventPlugin";
   }
   
   virtual void recoverFromLostNode(gadget::NodePtr lostNode) 
   {
      boost::ignore_unused_variable_warning(lostNode);
   }

public:
   /**
    * Registers a cllback for an asynchronous event that will be invokes in
    * preDraw() as soon as all cluster nodes have registered the same event.
    */
   void waitForEvent(const std::string& eventId, callback_t& callback);

private:
   /**
    * Helper method that invokes the callback for an event and cleans up the
    * callback and waiting maps.
    */
   void invokeAndClearCallback(const std::string& eventId);

public:
   static const vpr::GUID mPluginGUID;

private:
#if defined(__GNUC__) && __GNUC__ >= 4 || defined(_MSC_VER) && _MSC_VER >= 1500
   typedef std::tr1::unordered_map<std::string, callback_t> callback_map_t;
   typedef std::tr1::unordered_map<std::string, vpr::Uint32> waiting_map_t;
#elif BOOST_VERSION >= 103600
   typedef boost::unordered_map<std::string, callback_t> callback_map_t;
   typedef boost::unordered_map<std::string, vpr::Uint32> waiting_map_t;
#elif defined(VPR_HASH_MAP_INCLUDE)
   typedef std::hash_map<std::string, callback_t> callback_map_t;
   typedef std::hash_map<std::string, vpr::Uint32> waiting_map_t;
#else
   typedef std::map<std::string, callback_t> callback_map_t;
   typedef std::map<std::string, vpr::Uint32> waiting_map_t;
#endif

   callback_map_t mWaitingCallbacks; /**< Map from event token to callback to invoke when completed. */
   waiting_map_t  mWaitingCount;     /**< Waiting count used on server. */
};

} // end namespace

#endif
