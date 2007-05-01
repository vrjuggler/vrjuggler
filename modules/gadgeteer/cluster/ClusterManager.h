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

#ifndef _CLUSTER_CLUSTER_MANAGER_H
#define _CLUSTER_CLUSTER_MANAGER_H

#include <gadget/gadgetConfig.h>

#include <list>
#include <map>
#include <boost/signals.hpp>

#include <vpr/Util/GUID.h>
#include <vpr/Util/Singleton.h>
#include <vpr/DynLoad/Library.h>

#include <jccl/Config/ConfigElementPtr.h>
#include <jccl/Config/Configuration.h>
#include <jccl/RTRC/ConfigElementHandler.h>

#include <cluster/ClusterDepChecker.h>
#include <cluster/ClusterNetwork.h>
#include <gadget/PacketHandler.h>


namespace gadget
{
   class Node;
}

class Packet;

namespace cluster
{
class ClusterPlugin;

/** \class ClusterManager ClusterManager.h cluster/ClusterManager.h
 *
 * Manages the synchronization of all ClusterPlugins.
 */
class GADGET_CLASS_API ClusterManager : public jccl::ConfigElementHandler, public gadget::PacketHandler
{
   vprSingletonHeader( ClusterManager );
protected:
   /** Constructor is hidden, so no direct instantiation is allowed. */
   ClusterManager();

   virtual ~ClusterManager();

   /** Constructor is hidden, so no copying is allowed. */
   ClusterManager( const ClusterManager& cm )
      : jccl::ConfigElementHandler( cm )
      , gadget::PacketHandler( cm )
   {
      /* Do nothing. */;
   }

   void operator=(const ClusterManager&)
   {
      /* Do nothing. */ ;
   }

public:
   /**
    * Connect to the config manager.
    *
    * @since 1.3.3
    */
   void connectToConfigManager();

   /**
    * @since 1.3.3
    */
   void disconnectFromConfigManager();

   /**
    * @since 1.3.3
    */
   void init(bool clusterMaster, bool clusterSlave);

   /**
    * @since 1.3.3
    */
   void start();

public:
   /**
    * Add a new ClusterPlugin.
    */
   void addPlugin( ClusterPlugin* new_manager );

   /**
    * Remove an existing ClusterPlugin.
    */
   void removePlugin( ClusterPlugin* old_manager );

   /**
    * Return the ClusterPlugin with the given GUID.
    */
   ClusterPlugin* getPluginByGUID( const vpr::GUID& plugin_guid );
private:
   /**
    * Return true if the specified ClusterPlugin exists.
    */
   bool doesPluginExist( ClusterPlugin* old_plugin );

   /**
    * Send end block to all other connected nodes and
    * signal each connected node to sync.
    */
   void sendEndBlocksAndSignalUpdate( const int temp );

   /**
    * Returns the string representation of the element type used for the
    * ClusterManager.
    */
   static std::string getElementType();
public:
   /**
    * Send each ClusterPlugin's requests.
    */
   void sendRequests();

   /**
    * Synchronize plugins directly before the kernel calls
    * the draw() method.
    */
   void preDraw();

   /**
    * Synchronize plugins directly after the kernel calls
    * the postFrame() method.
    */
   void postPostFrame();

   /**
    * Cycle through ClusterPlugins until one of them can
    * achieve swaplock.
    */
   void createBarrier();

   /**
    * Cause the cluster to recover when a connection to
    * a ClusterNode is lost.
    */
   virtual void recoverFromLostNode( gadget::Node* lost_node );

   /**
    * Return the representation of the network which
    * this cluster is running on.
    */
   ClusterNetwork* getNetwork()
   {
      return mClusterNetwork;
   }

   /**
    * Return true if ConfigElement is a remote device.
    */
   bool recognizeRemoteDeviceConfig( jccl::ConfigElementPtr element );

   /**
    * Return true if Configelement is a ClusterManager element.
    */
   bool recognizeClusterManagerConfig( jccl::ConfigElementPtr element );

   /**
    * Configure the cluster given the current config element.
    */
   bool configCluster( jccl::ConfigElementPtr element );

   /**
    * Configure the given ConfigElement.
    *
    * @return true iff element was successfully configured.
    */
   bool configAdd( jccl::ConfigElementPtr element );

   /**
    * Shutdown the cluster using given ConfigElement.
    *
    * @return true iff the element (and any objects it represented)
    *          were successfully removed.
    */
   bool configRemove( jccl::ConfigElementPtr element );

   /**
    * Checks if this handler can process the given element.
    *
    * @return true iff this handler can process element.
    */
   bool configCanHandle( jccl::ConfigElementPtr element );

   /**
    * Check incoming configuration for cluster config elements. If we
    * we have a cluster_manager element switch into cluster mode, and
    * remove the element. If we have a cluster_node element, store it
    * it for later use and remove it.
    */
   void configurationChanged(jccl::Configuration* cfg, vpr::Uint16 type);
   void mergeConfigurations(jccl::Configuration* dst, jccl::Configuration* src, vpr::Uint16 type);

   /**
    * Get a pointer to the ConfigElement with the given name.
    */
   jccl::ConfigElementPtr getConfigElementPointer( const std::string& name );

   /*
   Truth table for ClusterManager

   Active   Ready
      1        0  = 0
      1        1  = 1
      0        0  = 1
      0        1  = 1

      (NOT(Active AND (NOT READY)))
      (NOT(Active) OR (Active AND Ready))
   */

   /**
    * Return true if we are running on a cluster.
    */
   bool isClusterActive()
   {
      vpr::Guard<vpr::Mutex> guard( mClusterActiveLock );
      return mClusterActive;
   }

   bool isMaster()
   {
      return mIsMaster;
   }

   /**
    * Return true if all dependancies have been satisfied.
    */
   bool isClusterReady();

   /**
    * Return true if all plugins have their dependancies satisfied.
    */
   bool pluginsReady();

   /**
    * Change the ready state of the ClusterManager.
    */
   void setClusterReady( const bool ready )
   {
      vpr::Guard<vpr::Mutex> guard( mClusterReadyLock );

      vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
         << clrOutBOLD( clrCYAN, "[ClusterManager]" )
         << " Cluster is ready." << std::endl << vprDEBUG_FLUSH;

      mClusterReady = ready;
   }

   /**
    * Output the current status of the cluster.
    */
   friend GADGET_API( std::ostream& ) operator<<( std::ostream& out,
                                                  ClusterManager& mgr );

   /**
    * Get a list of hostnames for all ClusterNodes.
    */
   std::vector<std::string> getNodes()
   {
      vpr::Guard<vpr::Mutex> guard( mNodesLock );
      return mNodes;
   }

   /**
    * Return the number of times that preDraw() has been called.
    */
   vpr::Uint64 preDrawCallCount()
   {
      return mPreDrawCallCount;
   }

   /**
    * Return the number of times that postPostFrame() has been called.
    */
   vpr::Uint64 postPostFrameCallCount()
   {
      return mPostPostFrameCallCount;
   }

public:
   /**
    * Get the GUID associated with this handler.
    */
   virtual vpr::GUID getHandlerGUID()
   {
      return vpr::GUID("f3ea94e2-82fc-43f6-a57f-474d3fd1d6eb");
   }
   
   virtual std::string getHandlerName()
   {
      return "ClusterManager";
   }

   /**
    * Handle a incoming packet.
    */
   virtual void handlePacket(cluster::Packet* packet, gadget::Node* node);
   
private:
   ClusterDepChecker            mDepChecker;

   std::list<ClusterPlugin*>    mPlugins;            /**< List of Plugins.*/
   vpr::Mutex                   mPluginsLock;        /**< Lock on plugins list.*/
   std::string                  mBarrierMachineName; /**< Name of the barrier machine.*/
   std::map<vpr::GUID, ClusterPlugin*> mPluginMap;   /**< Map of ClusterPlugins. */
   std::vector<vpr::LibraryPtr> mLoadedPlugins;

   vpr::Mutex                   mNodesLock;          /**< Lock on hostname list. */
   std::vector<std::string>     mNodes;              /**< Hostnames of the nodes in the cluster. */

   vpr::Mutex                   mClusterActiveLock;  /**< Lock on ClusterActive bool.*/
   bool                         mClusterActive;      /**< Flag informing us if this app is running on a cluster. */

   vpr::Mutex                   mClusterReadyLock;   /**< Lock on ClusterReady bool.*/
   bool                         mClusterReady;       /**< Flag set true when all dependancies are satisfied. */

   bool                         mClusterStarted;     /**< If the cluster has already started. */

   bool                         mIsMaster;
   bool                         mIsSlave;

   //@{
   /** @name Cluster configuration elements. */
   jccl::ConfigElementPtr       mClusterElement;
   std::map<std::string, jccl::ConfigElementPtr>        mClusterNodeElements;
   jccl::Configuration          mSystemConfiguration;
   //@}

   ClusterNetwork*              mClusterNetwork;     /**< The network representation of the cluster. */

   vpr::Uint64                  mPreDrawCallCount;       /**< # calls to preDraw() */
   vpr::Uint64                  mPostPostFrameCallCount; /**< # calls to postPostFrame() */
   boost::signals::connection   mConfigChangeConn;
};

} // end namespace

#endif
