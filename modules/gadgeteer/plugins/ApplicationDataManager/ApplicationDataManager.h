/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
#include <cluster/IdGenerator.h>

#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/RTRC/ConfigChunkHandler.h>

#include <map>

namespace cluster
{
   class SerializableData;
   class UserDataRequest;
   class UserDataServer;

class GADGET_CLASS_API ApplicationDataManager : public cluster::ClusterPlugin
{
   vprSingletonHeader( ApplicationDataManager );

public:
   ApplicationDataManager();
   virtual ~ApplicationDataManager();
   
   virtual void preDraw();
   virtual void postPostFrame();
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
    * Called each frame by the kernel to update all application level data(UserData)
    */
   void updateAll();

   /**
    * Add a UserData object to the current configuration.
    *
    * @pre This should only be called by the UserData constructor
    */
   void addUserData(SerializableData* new_user_data);

   /**
    * Remove the UserData object from the current configuration
    * @pre This should only be called by the UserData destructor
    */
   void removeUserData(SerializableData* old_user_data);

   /**
    * Print a list of all UserData objects currently in the configuration
    */
   void dumpUserData();

private:
   /**
    * Send all of the remaining requests for UserData
    */
   void sendUserDataRequests();

public:
   /**
    * Get a pointer to the UserDataServer for the given name
    */
   UserDataServer* getUserDataServer(std::string name);

   /**
    * Get a pointer to the UserData object that is being updated by a remote machine
    */
   SerializableData* getRemoteUserData(std::string name);

   /**
    * Get a pointer to the UserData object that is being updated by a remote machine
    */
   SerializableData* getRemoteUserData(vpr::Uint16 id);

private:
   /**
    * Add a request for a UserData machine on a remote mahine
    */
   void addPendingUserDataRequest(UserDataRequest* new_userdata_req, std::string hostname);

public:
   /**
    * Remove a UserData request that has been fulfilled.
    */
   void removePendingUserDataRequest(std::string userdata_name);


private:
   std::map<vpr::Uint16,SerializableData*>         mRemoteUserData;              /**< Application level UserData list. */
   vpr::Mutex                                      mRemoteUserDataLock;          /**< Lock on UserData list.*/   

   std::map<UserDataRequest*, std::string>         mPendingUserDataRequests;     /**< UserData Request list. */
   vpr::Mutex                                      mPendingUserDataRequestsLock; /**< Lock on UserData Request list.*/   

   std::map<std::string, UserDataServer*>          mUserDataServers;             /**< UserData Server list. */
   vpr::Mutex                                      mUserDataServersLock;         /**< Lock UserData Server list.*/   

   vpr::Uint32                                     mFrameNumber;                 /**< Keeps track of the local frame number */
   
   IdGenerator<vpr::Uint16>                        mUserDataIdGen;               /**< Keeps track of used/free virtual ids. */
};

} // end namespace

#endif
