/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
#include <map>

namespace cluster
{

const int DEFAULT_SLAVE_PORT = 12599;

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
    * Disconnect from the ConfigManager cleanly.
    *
    * @since 1.3.3
    */
   void disconnectFromConfigManager();

   /**
    * Initialize the ClusterManager so that it knows if it is active.
    *
    * @param clusterMaster True if the local node is the cluster master.
    * @param clusterSlave True if the local node is a slave.
    *
    * @since 1.3.3
    */
   void init(bool clusterMaster, bool clusterSlave);

   /**
    * Start the cluster by making initial connections.
    *
    * @note The master node connects to each slave node on a
    *       specified port.
    *
    * @since 1.3.3
    */
   void start();

   /**
    * Set the port that we should listen on if we are a slave.
    * @since 1.3.7
    */
   void setListenPort(const vpr::Uint16 listenPort)
   {
      mListenPort = listenPort;
   }

public:
   //@{
   /** @name Plugin management. */

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
   //@}

public:
   //@{
   /** @name ConfigElementHandler Interface. */

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
   //@}

   //@{
   /** @name Configuration helper methods. */

   /**
    * Return true if ConfigElement is a remote device.
    */
   bool recognizeRemoteDeviceConfig( jccl::ConfigElementPtr element );

   /**
    * Store incoming configuration to be sent to cluster slaves.
    */
   void configurationChanged(jccl::Configuration* cfg, vpr::Uint16 type);

private:
   /**
    * Return true if Configelement is a ClusterManager element.
    */
   bool recognizeClusterManagerConfig( jccl::ConfigElementPtr element );

   /**
    * Returns the string representation of the element type used for the
    * ClusterManager.
    */
   static std::string getElementType();

   /**
    * Configure the cluster given the current config element.
    */
   void configCluster( jccl::ConfigElementPtr element );

   /**
    * Merge the source configuration into the destination depending
    * on the change type.
    *
    * @param dst Destination configuration to merge into.
    * @param src Source configuration to copy changes out of.
    * @param type Type of change in source configuration. (ADD/REMOVE)
    */
   void mergeConfigurations(jccl::Configuration* dst, jccl::Configuration* src, vpr::Uint16 type);
   //@}

public:
   //@{
   /** @name Cluster methods. */

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
    * Send end block to all other connected nodes and
    * signal each connected node to sync.
    */
   void update( const int temp = 0)
   {
      mClusterNetwork->update(temp);
   }

   /**
    * Create a software barrier by having all slave nodes send an
    * end block to the master. Once the master receives an end block
    * from all nodes, it sends a go message back to all nodes.
    */
   void barrier()
   {
      mClusterNetwork->barrier(mIsMaster);
   }

   /**
    * Cycle through ClusterPlugins until one of them can
    * achieve swaplock.
    */
   void swapBarrier()
   {
      if (mSoftwareSwapLock)
      {
         barrier();
      }
   }

   /**
    * Return the representation of the network which
    * this cluster is running on.
    */
   ClusterNetwork* getNetwork()
   {
      return mClusterNetwork;
   }

   /**
    * Return true if we are running on a cluster.
    */
   bool isClusterActive()
   {
      return mClusterActive;
   }

   /**
    * Return true if we are the cluster master.
    */
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
   //@}

   /**
    * Output the current status of the cluster.
    */
   friend GADGET_API( std::ostream& ) operator<<( std::ostream& out,
                                                  ClusterManager& mgr );

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

   plugin_list_t                mPlugins;               /**< List of Plugins.*/
   std::string                  mBarrierMachineName;    /**< Name of the barrier machine.*/
   std::vector<vpr::LibraryPtr> mLoadedPlugins;         /**< List of so/dlls for plugins. */

   bool                         mClusterActive;         /**< Flag informing us if this app is running on a cluster. */
   bool                         mClusterStarted;        /**< If the cluster has already started. */
   bool                         mWindowOpened;          /**< If a window has been opened on the local machine. */
   bool                         mIsMaster;              /**< True if we are the cluster master. */
   bool                         mSoftwareSwapLock;      /**< If we should swap lock the cluster in software. */

   //@{
   /** @name Cluster configuration elements. */
   jccl::ConfigElementPtr       mClusterElement;
   std::map<std::string, jccl::ConfigElementPtr>        mClusterNodeElements;
   jccl::ConfigElementPtr       mLocalNodeElement;
   jccl::Configuration          mSystemConfiguration;
   //@}

   vpr::Uint16                  mListenPort;            /**< Port that we should listen on if we are a slave. */
   ClusterNetwork*              mClusterNetwork;        /**< The network representation of the cluster. */
   ConfigHandlerPtr             mConfigHandler;         /**< Delegate that handles all configuration packets. */

   vpr::Uint64                  mPreDrawCallCount;       /**< # calls to preDraw() */
   vpr::Uint64                  mPostPostFrameCallCount; /**< # calls to postPostFrame() */
   boost::signals::connection   mConfigChangeConn;
};

} // end of cluster namespace

#endif /*_CLUSTER_CLUSTER_MANAGER_H*/
