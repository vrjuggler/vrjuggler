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

#ifndef _CLUSTER_CLUSTER_MANAGER_H
#define _CLUSTER_CLUSTER_MANAGER_H

#include <vpr/Util/Singleton.h>
#include <vpr/vpr.h>
#include <vpr/DynLoad/LibraryLoader.h>

#include <gadget/gadgetConfig.h>
#include <gadget/Type/DeviceFactory.h>

#include <jccl/Config/ConfigElementPtr.h>
#include <jccl/RTRC/ConfigElementHandler.h>

#include <list>

namespace cluster
{
   class ClusterPlugin;
   class Packet;
   class ClusterNode;

class GADGET_CLASS_API ClusterManager : public jccl::ConfigElementHandler
{      
   vprSingletonHeader( ClusterManager );
public:
   ClusterManager();
   virtual ~ClusterManager();

   // ClusterPlugin Interface
public:
   void recoverFromLostNode(ClusterNode* lost_node);
   void handlePacket(Packet* packet, ClusterNode* node);
   void addPlugin(ClusterPlugin* new_manager);
   void removePlugin(ClusterPlugin* old_manager);
   bool doesPluginExist(ClusterPlugin* old_manager);
   
   void sendRequests();
   void preDraw();
   void postPostFrame();
   void createBarrier();
   
   //Helper Functions for Plugins
private:   
   void sendEndBlocksAndSignalUpdate(int temp);
   
   //Configuration
public:
   bool recognizeRemoteDeviceConfig(jccl::ConfigElementPtr element);
   bool recognizeClusterManagerConfig(jccl::ConfigElementPtr element);
   

   /** Add the pending element to the configuration.
    *  @pre configCanHandle(element) == true.
    *  @return true iff element was successfully added to configuration.
    */
   bool configAdd(jccl::ConfigElementPtr element);
   
   /** Remove the pending element from the current configuration.
    *  @pre configCanHandle(element) == true.
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
   
   //Config Helper Functions
private:      
   /**
    * Returns the string representation of the element type used for the
    * ClusterManager.
    */   
   static std::string getElementType();
   
   //Start Barrier Stuff
public:
   ClusterPlugin* getPluginByGUID(const vpr::GUID& plugin_guid);

   //General helper functions
public:
   jccl::ConfigElementPtr getConfigElementPointer(std::string& name);

public:
   /*
   Truth table for ClusterManager

   Active   Ready
      1        0  = 0
      1        1  = 1
      0        0  = 1
      0        1  = 1

      (NOT(Active AND (NOT READY)))   
   */

   bool isClusterActive()
   {
      vpr::Guard<vpr::Mutex> guard(mClusterActiveLock);
      return mClusterActive;
   }

   bool isClusterReady();

   bool pluginsReady();

   void setClusterReady(const bool ready)
   {
      vpr::Guard<vpr::Mutex> guard(mClusterReadyLock);
      std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX DONE - Set XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
      mClusterReady = ready;
   }

public:
   friend GADGET_API(std::ostream&) operator<<(std::ostream& out,
                                               ClusterManager& mgr);
   std::vector<std::string> getClusterNodes()
   {
      vpr::Guard<vpr::Mutex> guard(mClusterNodesLock);
      return mClusterNodes;
   }

private:
   std::list<ClusterPlugin*>     mPlugins;            /**< List of Plugins.*/
   vpr::Mutex                    mPluginsLock;        /**< Lock on plugins list.*/
   std::string                   mBarrierMachineName; /**< Name of the barrier machine.*/
   std::map<vpr::GUID, ClusterPlugin*> mPluginMap;
   vpr::LibraryLoader            mPluginLoader;

   vpr::Mutex                    mClusterNodesLock;
   std::vector<std::string>      mClusterNodes;
   
   vpr::Mutex                    mClusterActiveLock;  /**< Lock on ClusterActive bool.*/
   bool                          mClusterActive;
   
   vpr::Mutex                    mClusterReadyLock;  /**< Lock on ClusterActive bool.*/
   bool                          mClusterReady;
};

} // end namespace

#endif
