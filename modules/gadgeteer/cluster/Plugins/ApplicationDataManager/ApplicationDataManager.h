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

#include <vpr/Util/Singleton.h>

#include <gadget/gadgetConfig.h>
#include <gadget/Type/DeviceFactory.h>

// Must implement the Abstract Base Class in order to be a manager used on the ClusterNetwork
#include <cluster/ClusterPlugin.h>

#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/RTRC/ConfigChunkHandler.h>

#include <map>

namespace cluster
{
   class ApplicationData;
   class ApplicationDataRequest;
   class ApplicationDataServer;

class GADGET_CLASS_API ApplicationDataManager : public cluster::ClusterPlugin
{
   vprSingletonHeader( ApplicationDataManager );

public:
   ApplicationDataManager();
   virtual ~ApplicationDataManager();
   
   virtual void preDraw();
   virtual void postPostFrame();
   virtual bool isPluginReady();
   virtual std::string getManagerName()
   {
      return(std::string("ApplicationDataManager"));
   }
//   bool recognizeApplicationDataManagerConfig(jccl::ConfigChunkPtr chunk);
   
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

   bool recognizeApplicationDataManagerConfig(jccl::ConfigChunkPtr chunk);

//private:      
   /**
    * Returns the string representation of the chunk type used for the ApplicationDataManager
    */   
   static std::string getChunkType() { return std::string( "ApplicationDataManager" ); }

public:
   /**
    * Called each frame by the kernel to update all application level data(ApplicationData)
    */
   void updateAll();

   /**
    * Add a ApplicationData object to the current configuration.
    *
    * @pre This should only be called by the ApplicationData constructor
    */
   void addApplicationData(ApplicationData* new_user_data);

   /**
    * Remove the ApplicationData object from the current configuration
    * @pre This should only be called by the ApplicationData destructor
    */
   void removeApplicationData(ApplicationData* old_user_data);

   /**
    * Print a list of all ApplicationData objects currently in the configuration
    */
   void dumpApplicationData();

private:
   /**
    * Send all of the remaining requests for ApplicationData
    */
   void sendApplicationDataRequests();

public:
   /**
    * Get a pointer to the ApplicationDataServer for the given name
    */
   ApplicationDataServer* getApplicationDataServer(vpr::GUID id);

   /**
    * Get a pointer to the ApplicationData object that is being updated by a remote machine
    */
   ApplicationData* getRemoteApplicationData(vpr::GUID id);

private:
   /**
    * Add a request for a ApplicationData machine on a remote mahine
    */
   void addPendingApplicationDataRequest(ApplicationDataRequest* new_appdata_req, std::string hostname);

public:
   /**
    * Remove a ApplicationData request that has been fulfilled.
    */
   void removePendingApplicationDataRequest(const vpr::GUID& guid);


private:
   std::map<vpr::GUID, ApplicationData*>           mRemoteApplicationData;              /**< Application level ApplicationData list. */
   vpr::Mutex                                      mRemoteApplicationDataLock;          /**< Lock on ApplicationData list.*/   

   std::map<ApplicationDataRequest*, std::string>  mPendingApplicationDataRequests;     /**< ApplicationData Request list. */
   vpr::Mutex                                      mPendingApplicationDataRequestsLock; /**< Lock on ApplicationData Request list.*/   

   std::map<vpr::GUID, ApplicationDataServer*>     mApplicationDataServers;             /**< ApplicationData Server list. */
   vpr::Mutex                                      mApplicationDataServersLock;         /**< Lock ApplicationData Server list.*/   

   vpr::Uint32                                     mFrameNumber;                 /**< Keeps track of the local frame number */
};

} // end namespace

#endif
