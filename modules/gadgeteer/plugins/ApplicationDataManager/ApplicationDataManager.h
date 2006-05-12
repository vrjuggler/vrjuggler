/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
#include <vector>
#include <map>

#include <vpr/vpr.h>
#include <vpr/Util/GUID.h>
#include <vpr/Sync/Mutex.h>

#include <cluster/ClusterPlugin.h>

#include <jccl/Config/ConfigElementPtr.h>


namespace cluster
{
   class ApplicationData;
   class ApplicationDataRequest;
   class ApplicationDataServer;
   class Packet;

class GADGET_CLUSTER_PLUGIN_CLASS_API ApplicationDataManager
   : public cluster::ClusterPlugin
{
public:
   ApplicationDataManager();
   virtual ~ApplicationDataManager();
   
   // ---------- ClusterPlugin Interface ----------- //   
public:   
   /**
    * Get the GUID associated with this plugin.
    */
   vpr::GUID getHandlerGUID()
   {
      return mHandlerGUID;
   }

   /**
    * Handle a incoming packet.
    */
   void handlePacket(Packet* packet, gadget::Node* node);
   
   /**
    * Called each frame by the kernel to update all application level data(ApplicationData).
    */
   virtual void preDraw();
   
   /**
    * Called each frame by the kernel to update the cluster after the InputManager has updated it's data.
    */
   virtual void postPostFrame();

   /**
    * Send all pending requests for ApplicationData to other Nodes.
    */
   virtual void sendRequests();

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
   
   virtual void recoverFromLostNode(gadget::Node* lost_node) 
   {
      boost::ignore_unused_variable_warning(lost_node);
   }

   // ---------- ConfigElementHandler Interface ----------- //
public:   
   /** Add the pending element to the configuration.
    *  @pre configCanHandle (element) == true.
    *  @return true iff element was successfully added to configuration.
    */
   bool configAdd(jccl::ConfigElementPtr element);
   
   /** Remove the pending element from the current configuration.
    *  @pre configCanHandle (element) == true.
    *  @return true iff the element (and any objects it represented)
    *          were successfully removed.
    */
   bool configRemove(jccl::ConfigElementPtr element);

   /** Checks if this handler can process element.
    *  Typically, an implementation of handler will check the element's
    *  description name/token to decide if it knows how to deal with
    *  it.
    *  @return true iff this handler can process element.
    */
   bool configCanHandle(jccl::ConfigElementPtr element);

private:      
   bool recognizeApplicationDataManagerConfig(jccl::ConfigElementPtr element);
   bool recognizeApplicationDataConfig(jccl::ConfigElementPtr chunk);

   /**
    * Returns the string representation of the element type used for the
    * ApplicationDataManager.
    */   
   static std::string getElementType();

   // ---------- ApplicationData Interface ----------- //
public:
   /**
    * Add a ApplicationData object to the current configuration.
    *
    * @pre This should only be called by the ApplicationData constructor
    */
   virtual void addSerializableObject(vpr::SerializableObject* object);

   /**
    * Remove the ApplicationData object from the current configuration
    * @pre This should only be called by the ApplicationData destructor
    */
   void removeApplicationData(ApplicationData* old_user_data);

   /**
    * Print a list of all ApplicationData objects currently in the configuration
    */
   void dumpApplicationData();
   


   // ---------- Helper Functions ----------- //
private:
   /**
    * Get a pointer to the ApplicationDataServer for the given name
    */
   ApplicationDataServer* getApplicationDataServer(const vpr::GUID& id);

   /**
    * Get a pointer to the ApplicationData object that is being updated by a remote machine
    */
   ApplicationData* getRemoteApplicationData(const vpr::GUID& id);

   /**
    * Add a request for a ApplicationData machine on a remote mahine
    */
   void addPendingApplicationDataRequest(ApplicationDataRequest* new_appdata_req, const std::string& hostname);

   /**
    * Remove a ApplicationData request that has been fulfilled.
    */
   void removePendingApplicationDataRequest(const vpr::GUID& guid);
private:
   std::map<vpr::GUID, ApplicationData*>           mRemoteApplicationData;              /**< Application level ApplicationData list. */
   vpr::Mutex                                      mRemoteApplicationDataLock;          /**< Lock on ApplicationData list.*/   
   
   std::vector<ApplicationData*>                   mNeedsConfiged;              /**< Application level ApplicationData list. */
   vpr::Mutex                                      mNeedsConfigedLock;          /**< Lock on ApplicationData list.*/   

   std::map<ApplicationDataRequest*, std::string>  mPendingApplicationDataRequests;     /**< ApplicationData Request list. */
   vpr::Mutex                                      mPendingApplicationDataRequestsLock; /**< Lock on ApplicationData Request list.*/   

   std::map<vpr::GUID, ApplicationDataServer*>     mApplicationDataServers;             /**< ApplicationData Server list. */
   vpr::Mutex                                      mApplicationDataServersLock;         /**< Lock ApplicationData Server list.*/   

   vpr::GUID                                       mHandlerGUID;
   vpr::Uint32                                     mFrameNumber;                 /**< Keeps track of the local frame number */   
};

} // end namespace

#endif
