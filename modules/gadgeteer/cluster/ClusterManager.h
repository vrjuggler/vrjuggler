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

#include <boost/signals.hpp>

#include <vpr/Util/GUID.h>
#include <vpr/Util/Singleton.h>
#include <vpr/DynLoad/Library.h>

#include <jccl/Config/ConfigElementPtr.h>
#include <jccl/Config/Configuration.h>
#include <jccl/RTRC/ConfigElementHandler.h>

#include <cluster/ClusterDepChecker.h>
#include <cluster/ClusterNetwork.h>
#include <cluster/ClusterPluginPtr.h>
#include <cluster/Packets/PacketPtr.h>
#include <cluster/ConfigHandlerPtr.h>
#include <gadget/Util/Debug.h>

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

/** \class ClusterManager ClusterManager.h cluster/ClusterManager.h
 *
 * Manages the synchronization of all ClusterPlugins.
 */
class GADGET_CLASS_API ClusterManager : public jccl::ConfigElementHandler
{
   vprSingletonHeader( ClusterManager );
protected:
   /** Constructor is hidden, so no direct instantiation is allowed. */
   ClusterManager();

   virtual ~ClusterManager();

   /** Constructor is hidden, so no copying is allowed. */
   ClusterManager( const ClusterManager& cm )
      : jccl::ConfigElementHandler( cm )
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
   void addPlugin( ClusterPluginPtr plugin );

   /**
    * Remove an existing ClusterPlugin.
    */
   void removePlugin( ClusterPluginPtr plugin );

   /**
    * Return the ClusterPlugin with the given GUID.
    */
   ClusterPluginPtr getPluginByGUID( const vpr::GUID& plugin_guid );
private:
   /**
    * Return true if the specified ClusterPlugin exists.
    */
   bool doesPluginExist( ClusterPluginPtr old_plugin );

   /**
    * Returns the string representation of the element type used for the
    * ClusterManager.
    */
   static std::string getElementType();
public:
   /**
    * Send end block to all other connected nodes and
    * signal each connected node to sync.
    */
   void update( const int temp = 0);

   /**
    * Create a software barrier by having all slave nodes send an
    * end block to the master. Once the master receives an end block
    * from all nodes, it sends a go message back to all nodes.
    */
   void barrier();

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
   void swapBarrier();

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
   void configCluster( jccl::ConfigElementPtr element );

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
      return mClusterActive;
   }

   bool isMaster()
   {
      return mIsMaster;
   }

   /**
    * Called when a window is opened on the local machine.
    */
   void windowOpened()
   {
      mWindowOpened = true;
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
    * Output the current status of the cluster.
    */
   friend GADGET_API( std::ostream& ) operator<<( std::ostream& out,
                                                  ClusterManager& mgr );

   /**
    * Get a list of hostnames for all ClusterNodes.
    */
   std::vector<std::string> getNodes()
   {
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

private:
   ClusterDepChecker            mDepChecker;

   typedef std::list<ClusterPluginPtr> plugin_list_t;
#ifdef VPR_HASH_MAP_INCLUDE
   typedef std::hash_map<vpr::GUID, ClusterPluginPtr, vpr::GUID::hash> plugin_map_t;
#else
   typedef std::map<vpr::GUID, ClusterPluginPtr> plugin_map_t;
#endif

   plugin_list_t                mPlugins;            /**< List of Plugins.*/
   std::string                  mBarrierMachineName; /**< Name of the barrier machine.*/
   plugin_map_t                 mPluginMap;   /**< Map of ClusterPlugins. */
   std::vector<vpr::LibraryPtr> mLoadedPlugins;

   std::vector<std::string>     mNodes;              /**< Hostnames of the nodes in the cluster. */

   bool                         mClusterActive;      /**< Flag informing us if this app is running on a cluster. */

   bool                         mClusterStarted;     /**< If the cluster has already started. */
   bool                         mWindowOpened;       /**< If a window has been opened on the local machine. */

   bool                         mIsMaster;
   bool                         mIsSlave;

   bool                         mSoftwareSwapLock;

   //@{
   /** @name Cluster configuration elements. */
   jccl::ConfigElementPtr       mClusterElement;
   std::map<std::string, jccl::ConfigElementPtr>        mClusterNodeElements;
   jccl::Configuration          mSystemConfiguration;
   //@}

   ClusterNetwork*              mClusterNetwork;        /**< The network representation of the cluster. */
   ConfigHandlerPtr             mConfigHandler;         /**< Delegate that handles all configuration packets. */

   vpr::Uint64                  mPreDrawCallCount;       /**< # calls to preDraw() */
   vpr::Uint64                  mPostPostFrameCallCount; /**< # calls to postPostFrame() */
   boost::signals::connection   mConfigChangeConn;
};

} // end namespace

#endif
