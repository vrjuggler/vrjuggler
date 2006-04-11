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

#ifndef _CLUSTER_APPLICATION_DATA_MANAGER_H
#define _CLUSTER_APPLICATION_DATA_MANAGER_H

#include <cluster/Plugins/PluginConfig.h>
#include <map>

#include <vpr/Util/Singleton.h>

// Must implement the Abstract Base Class in order to be a manager used on the ClusterNetwork
#include <cluster/ClusterPlugin.h>

#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ConfigChunkPtr.h>


extern "C" GADGET_CLUSTER_PLUGIN_API(cluster::ClusterPlugin*) initPlugin();

namespace cluster
{
   class ApplicationData;
   class ApplicationDataRequest;
   class ApplicationDataServer;
   class Packet;

class GADGET_CLUSTER_PLUGIN_CLASS_API ApplicationDataManager
   : public cluster::ClusterPlugin
{
   vprSingletonHeader( ApplicationDataManager );

public:
   ApplicationDataManager();
   virtual ~ApplicationDataManager();
   
   // ---------- ClusterPlugin Interface ----------- //   
public:   
   /**
    * Get the GUID associated with this plugin.
    */
   vpr::GUID getPluginGUID()
   {
      return mPluginGUID;
   }

   /**
    * Handle a incoming packet.
    */
   void handlePacket(Packet* packet, ClusterNode* node);
   
   /**
    * Called each frame by the kernel to update all application level data(ApplicationData).
    */
   virtual void preDraw();
   
   /**
    * Called each frame by the kernel to update the cluster after the InputManager has updated it's data.
    */
   virtual void postPostFrame();

   /**
    * Send all pending requests for ApplicationData to other ClusterNodes.
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
   


   // ---------- ConfigChunkHandler Interface ----------- //
public:   
   /** Add the pending chunk to the configuration.
    *  PRE: configCanHandle (chunk) == true.
    *  @return true iff chunk was successfully added to configuration.
    */
   bool configAdd(jccl::ConfigChunkPtr chunk);
   
   /** Remove the pending chunk from the current configuration.
    *  PRE: configCanHandle (chunk) == true.
    *  @return true iff the chunk (and any objects it represented)
    *          were successfully removed.
    */
   bool configRemove(jccl::ConfigChunkPtr chunk);

   /** Checks if this handler can process chunk.
    *  Typically, an implementation of handler will check the chunk's
    *  description name/token to decide if it knows how to deal with
    *  it.
    *  @return true iff this handler can process chunk.
    */
   bool configCanHandle(jccl::ConfigChunkPtr chunk);

private:      
   bool recognizeApplicationDataManagerConfig(jccl::ConfigChunkPtr chunk);
   bool recognizeApplicationDataConfig(jccl::ConfigChunkPtr chunk);
   /**
    * Returns the string representation of the chunk type used for the ApplicationDataManager
    */   
   static std::string getChunkType() { return std::string( "ApplicationDataManager" ); }
   


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

   vpr::GUID                                       mPluginGUID;
   vpr::Uint32                                     mFrameNumber;                 /**< Keeps track of the local frame number */   
};

} // end namespace

#endif
