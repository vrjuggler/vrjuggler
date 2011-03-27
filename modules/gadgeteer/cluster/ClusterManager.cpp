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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>

#include <iomanip>
#include <sstream>
#include <boost/version.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
#  include <boost/filesystem/operations.hpp>
#else
#  include <boost/filesystem/exception.hpp>
#endif

#include <vpr/vprTypes.h>
#include <vpr/System.h>
#include <vpr/DynLoad/LibraryLoader.h>
#include <vpr/Util/FileUtils.h>

#include <jccl/Config/ConfigDefinitionPtr.h>
#include <jccl/Config/ConfigDefinition.h>
#include <jccl/Config/ConfigElement.h>
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/RTRC/DependencyManager.h>

#include <gadget/gadgetParam.h>
#include <gadget/Node.h>
#include <gadget/Util/Debug.h>
#include <gadget/Util/PathHelpers.h>
#include <gadget/Util/PluginVersionException.h>

#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork.h>
#include <cluster/ClusterPlugin.h>
#include <cluster/ConfigHandler.h>
#include <cluster/Packets/ConfigPacket.h>
#include <cluster/Packets/Packet.h>

namespace fs = boost::filesystem;

namespace cluster
{
vprSingletonImp( ClusterManager );

/**
 * This struct implements a callable object (a functor, basically).  An
 * instance can be passed in where a boost::function1<bool, void*> is
 * expected.  In cluster::ClusterManager::configAdd(), instances are used
 * to handle version checking of cluster plug-ins via vpr::LibraryLoader.
 */
struct VersionCheckCallable
{
   VersionCheckCallable()
   {
   }

   /**
    * This will be invoked as a callback by methods of vpr::LibraryLoader.
    *
    * @param func A function pointer for the entry point in a dynamically
    *             loaded cluster plug-in.  This must be cast to the correct
    *             signature before being invoked.
    */
   bool operator()(void* func)
   {
      vpr::Uint32 (*version_func)();
      version_func = (vpr::Uint32 (*)()) func;

      // Call the entry point function, which, in this case, returns the
      // version of Gadgeteer against which the plug-in was compiled.
      const vpr::Uint32 plugin_gadget_ver = (*version_func)();

      if ( plugin_gadget_ver != mGadgetVersion )
      {
         std::ostringstream msg_stream;
         msg_stream << "Gadgeteer version mismatch!\n"
                    << "Cluster plug-in was compiled against Gadgeteer "
                    << "version " << plugin_gadget_ver << ",\n"
                    << "but this is Gadgeteer version " << mGadgetVersion
                    << std::endl;
         throw gadget::PluginVersionException(msg_stream.str(),
                                              VPR_LOCATION);
      }

      return true;
   }

   static const vpr::Uint32 mGadgetVersion;
};

const vpr::Uint32 VersionCheckCallable::mGadgetVersion(__GADGET_version);

/**
 * This struct implements a callable object (a functor, basically).  An
 * instance can be passed in where a boost::function1<bool, void*> is
 * expected.  In cluster::ClusterManager::configAdd(), instances are used
 * to handle dynamic loading of cluster plugins via vpr::LibraryLoader.
 */
struct PluginInitCallable
{
   PluginInitCallable(cluster::ClusterManager* clusterMgr)
      : mgr(clusterMgr)
   {
   }

   /**
    * This will be invoked as a callback by methods of vpr::LibraryLoader.
    *
    * @param func A function pointer for the entry point in a dynamically
    *             loaded cluster plug-in.  This must be cast to the correct
    *             signature before being invoked.
    */
   bool operator()( void* func )
   {
      void (*init_func)(ClusterManager*);

      // Cast the entry point function to the correct signature so that we
      // can call it.  All dynamically loaded plug-ins must have an entry
      // point function that takes a pointer to a cluster::ClusterManager
      // instance and returns nothing.
      init_func = (void (*)(ClusterManager*)) func;

      // Call the entry point function.
      (*init_func)( mgr );

      return true;
   }

   cluster::ClusterManager* mgr;
};

ClusterManager::ClusterManager()
   : mClusterActive( false )
   , mClusterStarted( false )
   , mWindowOpened( false )
   , mIsMaster(false)
   , mSoftwareSwapLock(false)
   , mListenPort(DEFAULT_SLAVE_PORT)
   , mClusterNetwork(NULL)
   , mPreDrawCallCount(0)
   , mPostPostFrameCallCount(0)
{
   mConfigHandler = ConfigHandler::create();
   mClusterNetwork = new ClusterNetwork();
   mClusterNetwork->addHandler(mConfigHandler);
}

ClusterManager::~ClusterManager()
{
   mPlugins.clear();

   disconnectFromConfigManager();
   if (NULL != mClusterNetwork)
   {
      delete mClusterNetwork;
      mClusterNetwork = NULL;
   }
}

void ClusterManager::connectToConfigManager()
{
   jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();

   // Add the new cluster network as an element handler.
   // XXX: This may not be needed after restucture.
   cfg_mgr->addConfigElementHandler( mClusterNetwork );
   jccl::DependencyManager::instance()->registerChecker(&mDepChecker);

   if (!mConfigChangeConn.connected())
   {
      // Connect to configuration change signal.
      mConfigChangeConn = cfg_mgr->addConfigurationCallback(
         boost::bind(&ClusterManager::configurationChanged, this, _1, _2)
      );
   }
}

void ClusterManager::disconnectFromConfigManager()
{
   // XXX: This may not be needed after restucture.
   if (NULL != mClusterNetwork)
   {
      jccl::ConfigManager::instance()->removeConfigElementHandler( mClusterNetwork );
   }
   jccl::DependencyManager::instance()->unregisterChecker(&mDepChecker);
   mConfigChangeConn.disconnect();
}

void ClusterManager::init(bool clusterMaster, bool clusterSlave)
{
   mIsMaster = clusterMaster;

   mClusterActive = (clusterMaster || clusterSlave);
}

void ClusterManager::start()
{
   vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_STATUS_LVL )
      << clrOutBOLD( clrCYAN, "[ClusterManager] " )
      << "Cluster mode: " << (mClusterActive ? "True":"False")
      << std::endl << vprDEBUG_FLUSH;

   mClusterStarted = true;

   // Connect the entire cluster.
   if (mIsMaster)
   {
      if (NULL == mClusterElement.get())
      {
         throw ClusterException("Master must have a ClusterManager config element.", VPR_LOCATION);
      }
      configCluster(mClusterElement);
      mClusterNetwork->connectToSlaves();


      // Send initial configuration to each node.
      ClusterNetwork::node_list_t& nodes = mClusterNetwork->getNodes();
      for (ClusterNetwork::node_list_t::iterator itr = nodes.begin(); itr != nodes.end(); itr++)
      {
         jccl::Configuration node_cfg(mSystemConfiguration);

         // Compile a string of the configuration.
         std::ostringstream node_output;
         node_output << node_cfg;

         vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
            << clrOutBOLD( clrCYAN, "[ClusterManager] " )
            << "Send configuration to nodes: " << (*itr)->getName()
            << std::endl << vprDEBUG_FLUSH;

         ConfigPacketPtr cfg_pkt = ConfigPacket::create(node_output.str(), jccl::ConfigManager::PendingElement::ADD);
         (*itr)->send(cfg_pkt);
      }
      barrier();
   }
   else
   {
      // Start listening on known port for connections.
      mClusterNetwork->waitForConnection(mListenPort);
      barrier();
   }
}

bool ClusterManager::isClusterReady()
{
   return( mWindowOpened && pluginsReady() );
}

bool ClusterManager::pluginsReady()
{
   for ( plugin_list_t::iterator itr = mPlugins.begin(); itr != mPlugins.end(); itr++ )
   {
      if ( !(*itr)->isPluginReady() )
      {
         return false;
      }
   }

   return true;
}

/**
 * Adds a new plugin to the ClusterManager.
 */
void ClusterManager::addPlugin(ClusterPluginPtr newPlugin)
{
   plugin_list_t::const_iterator found
      = std::find(mPlugins.begin(), mPlugins.end(), newPlugin);

   if ( mPlugins.end() == found )
   {
      mPlugins.push_back( newPlugin );

      // Add each plugin as a packet handler.
      mClusterNetwork->addHandler( newPlugin );

      // We can still unregister it when removed below though
      vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
         << clrOutBOLD( clrCYAN, "[ClusterManager] " )
         << "Adding Plugin: " << newPlugin->getPluginName()
         << std::endl << vprDEBUG_FLUSH;
   }
   else
   {
      // We can still unregister it when removed below though
      vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
         << clrOutBOLD( clrCYAN, "[ClusterManager] " )
         << clrOutBOLD( clrRED, "WARNING: " )
         << "Tried to add plugin more than once: " << newPlugin->getPluginName()
         << std::endl << vprDEBUG_FLUSH;
   }
}

struct PluginGuidPred
{
   PluginGuidPred(const vpr::GUID& pluginGuid)
      : mGuid(pluginGuid)
   {
      /* Do nothing. */ ;
   }

   bool operator()(ClusterPluginPtr plugin)
   {
      vprASSERT(NULL != plugin.get() && "Can't have a NULL plugin.");
      return plugin->getHandlerGUID() == mGuid;
   }

   const vpr::GUID& mGuid;
};

ClusterPluginPtr ClusterManager::getPluginByGUID( const vpr::GUID& pluginGuid )
{
   PluginGuidPred pred(pluginGuid);
   plugin_list_t::const_iterator found = std::find_if(mPlugins.begin(), mPlugins.end(), pred);

   if( found != mPlugins.end() )
   {
      return *found;
   }

   return ClusterPluginPtr();
}

/**
 * Removes a plugin from the ClusterManager
 */
void ClusterManager::removePlugin( ClusterPluginPtr oldPlugin )
{
   plugin_list_t::iterator found
      = std::find(mPlugins.begin(), mPlugins.end(), oldPlugin);

   if (mPlugins.end() != found)
   {
      vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
         << clrOutBOLD( clrCYAN, "[ClusterManager] " )
         << "Removing Plugin: " << oldPlugin->getPluginName()
         << std::endl << vprDEBUG_FLUSH;
      mPlugins.erase(found);
   }
}

void ClusterManager::preDraw()
{
   // Idea is to not create frame lock if we do not need to
   bool updateNeeded = false;

   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
      << clrOutBOLD( clrCYAN,"[ClusterManager]" )
      << " preDraw" << std::endl << vprDEBUG_FLUSH;
   getNetwork()->corkNetwork();
   for ( plugin_list_t::iterator itr = mPlugins.begin(); itr != mPlugins.end(); itr++ )
   {
      (*itr)->preDraw();
      updateNeeded = true;
   }

   if ( updateNeeded )
   {
      mPreDrawCallCount++;
      update(2);
   }
}

void ClusterManager::postPostFrame()
{
   // -If not running
   //   -If all plugins ready
   //     - isClusterReady

   // Idea is to not create frame lock if we do not need to
   bool updateNeeded = false;

   vprDEBUG( gadgetDBG_NET_MGR, vprDBG_HVERB_LVL )
      << clrOutBOLD( clrCYAN,"[ClusterManager]" )
      << " postPostFrame" << std::endl << vprDEBUG_FLUSH;
   getNetwork()->corkNetwork();
   for ( plugin_list_t::iterator itr = mPlugins.begin(); itr != mPlugins.end(); itr++ )
   {
      (*itr)->postPostFrame();
      updateNeeded = true;
   }

   if ( updateNeeded )
   {
      mPostPostFrameCallCount++;
      update(3);
   }
}

bool ClusterManager::recognizeRemoteDeviceConfig( jccl::ConfigElementPtr element )
{
   return (mClusterActive && element->getConfigDefinition()->isParent("input_device"));
}

bool ClusterManager::recognizeClusterManagerConfig( jccl::ConfigElementPtr element )
{
   return( element->getID() == ClusterManager::getElementType() );
}

/** Adds the pending element to the configuration.
 *  @pre configCanHandle(element) == true.
 *  @return true iff element was successfully added to configuration.
 */
void ClusterManager::configCluster( jccl::ConfigElementPtr element )
{
   vpr::DebugOutputGuard dbg_output( gadgetDBG_RIM, vprDBG_STATE_LVL,
                           std::string( "Cluster Manager: Adding config element.\n" ),
                           std::string( "...done adding element.\n" ) );

   vprASSERT(configCanHandle(element));
   vprASSERT(recognizeClusterManagerConfig(element));

   {
      vprDEBUG( gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL)
         << clrOutBOLD(clrCYAN,"[ClusterManager] ")
         << "Configure the Cluster: " << element->getName()
         << std::endl << vprDEBUG_FLUSH;

      // Get a list of cluster nodes to use for this cluster.
      int num_nodes = element->getNum( std::string( "cluster_node" ) );

      vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_STATUS_LVL )
         << clrOutBOLD( clrCYAN, "[ClusterManager] " )
         << "configAdd() Number of nodes: " << num_nodes
         << std::endl << vprDEBUG_FLUSH;

      for ( int i = 0 ; i < num_nodes ; ++i )
      {
         std::string new_node =
            element->getProperty<std::string>( "cluster_node" , i );
         vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_STATUS_LVL )
            << clrOutBOLD( clrCYAN, "[ClusterManager] " )
            << "configAdd() New Node Name: " << new_node
            << std::endl << vprDEBUG_FLUSH;

         if (1 != mClusterNodeElements.count(new_node))
         {
            throw ClusterException("Can't find configuration for node: " + new_node, VPR_LOCATION);
         }

         // ClusterManager element is loaded after all node elements
         // because it is dependent on them.
         jccl::ConfigElementPtr new_node_element = mClusterNodeElements[new_node];

         std::string new_node_hostname =
            new_node_element->getProperty<std::string>( "host_name" );

         vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_STATUS_LVL )
            << clrOutBOLD( clrCYAN, "[ClusterManager] " )
            << "configAdd() New Node Hostname: " << new_node_hostname
            << std::endl << vprDEBUG_FLUSH;

         if ( !cluster::ClusterNetwork::isLocalHost( new_node_hostname ) )
         {
            vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_STATUS_LVL )
               << clrOutBOLD( clrCYAN, "[ClusterManager] " )
               << "configAdd() Added Node since it is non-local"
               << std::endl << vprDEBUG_FLUSH;

            vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL )
               << clrOutBOLD( clrCYAN, "[ClusterManager] " )
               << " Adding Node: " << element->getName()
               << " to the Cluster Network\n" << vprDEBUG_FLUSH;

            std::string name        = new_node_element->getName();
            std::string host_name   = new_node_element->getProperty<std::string>( "host_name" );
            vpr::Uint16 listen_port = new_node_element->getProperty<int>( "listen_port" );
            if (0 == listen_port)
            {
               listen_port = DEFAULT_SLAVE_PORT;
            }

            mClusterNetwork->addNode(name, host_name, listen_port);
         }
         else
         {
            vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_STATUS_LVL )
               << clrOutBOLD( clrCYAN, "[ClusterManager] " )
               << "configAdd() Local configuration."
               << std::endl << vprDEBUG_FLUSH;
         }
      }
   }
}

bool ClusterManager::configAdd( jccl::ConfigElementPtr element )
{
   bool ret_val = false;      // Flag to return success
   {
      // Load the plugins.

      vpr::DebugOutputGuard dbg_output( gadgetDBG_RIM, vprDBG_STATE_LVL,
                                        std::string( "Handling cluster_manager element:\n" ),
                                        std::string( "-- end state -- \n" ) );

      // Keep this up to date with the version of the element definition we're
      // expecting to handle.
      const unsigned int cur_version(3);

      // If the element version is less than cur_version, we will not try to
      // proceed.  Instead, we'll print an error message and return false so
      // that the Config Manager knows this element wasn't consumed.
      if ( element->getVersion() < cur_version )
      {
         vprDEBUG( gadgetDBG_RIM, vprDBG_CRITICAL_LVL )
            << clrOutBOLD( clrRED, "ERROR" )
            << " [gadget::ClusterManager::configAdd()]: Element named '"
            << element->getName() << "'" << std::endl << vprDEBUG_FLUSH;
         vprDEBUG_NEXT( gadgetDBG_RIM, vprDBG_CRITICAL_LVL )
            << "is version " << element->getVersion()
            << ", but we require at least version " << cur_version
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG_NEXT( gadgetDBG_RIM, vprDBG_CRITICAL_LVL )
            << "Ignoring this element and moving on." << std::endl
            << vprDEBUG_FLUSH;

         ret_val = false;
      }
      // We got the right version of the config element and can proceed.
      else
      {
         const std::string plugin_path_prop_name( "plugin_path" );
         const int path_count( element->getNum( plugin_path_prop_name ) );
         std::vector<fs::path> search_path( path_count );

         for ( unsigned int i = 0; i < search_path.size(); ++i )
         {
            std::string temp_str =
               vpr::replaceEnvVars( element->getProperty<std::string>( plugin_path_prop_name, i ) );

            try
            {
               search_path[i] =
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
                  fs::path(temp_str);
#else
                  fs::path(temp_str, fs::native);
#endif
            }
            catch( fs::filesystem_error& fsEx )
            {
               vprDEBUG( vprDBG_ERROR, vprDBG_CRITICAL_LVL )
                  << clrOutNORM( clrRED, "ERROR:" )
                  << "[cluster::ClusterManager::configAdd()] File system "
                  << "exception caught while converting"
                  << std::endl << vprDEBUG_FLUSH;
               vprDEBUG_NEXT( vprDBG_ERROR, vprDBG_CRITICAL_LVL )
                  << "'" << temp_str << "'"
                  << std::endl << vprDEBUG_FLUSH;
               vprDEBUG_NEXT( vprDBG_ERROR, vprDBG_CRITICAL_LVL )
                  << "to a Boost.Filesystem path."
                  << std::endl << vprDEBUG_FLUSH;
               vprDEBUG_NEXT( vprDBG_ERROR, vprDBG_CRITICAL_LVL )
                  << fsEx.what() << std::endl << vprDEBUG_FLUSH;
            }
         }

         // Append a default plug-in search path to search_path.
         const fs::path default_search_dir =
            gadget::getDefaultPluginRoot() / std::string("plugins");

         vprDEBUG(gadgetDBG_RIM, vprDBG_VERB_LVL)
            << "[cluster::ClusterManager::configAdd()] Appending "
            << "default search path '"
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
            << default_search_dir.string()
#else
            << default_search_dir.native_directory_string()
#endif
            << "'\n" << vprDEBUG_FLUSH;

#if defined(GADGET_DEBUG)
         // For a debug build, search in the debug subdirectory of
         // default_search_dir before looking in default_search_dir.
         search_path.push_back(default_search_dir / std::string("debug"));
#endif

         search_path.push_back( default_search_dir );

         // --- Load cluster plugin dsos -- //
         // - Load individual plugins
         const std::string plugin_prop_name( "plugin" );
         const std::string get_version_func( "getGadgeteerVersion" );
         const std::string plugin_init_func( "initPlugin" );

         int plugin_count = element->getNum( plugin_prop_name );
         std::string plugin_dso_name;

         for ( int i = 0; i < plugin_count; ++i )
         {
            plugin_dso_name =
               element->getProperty<std::string>(plugin_prop_name, i);

            if ( !plugin_dso_name.empty() )
            {
               vprDEBUG( gadgetDBG_RIM, vprDBG_STATE_LVL )
                  << "[cluster::ClusterManager::configAdd()] Loading "
                  << "plugin DSO '" << plugin_dso_name << "'"
                  << std::endl << vprDEBUG_FLUSH;

               vpr::LibraryPtr dso =
                  vpr::LibraryLoader::findDSO(plugin_dso_name, search_path);

               if ( dso.get() != NULL )
               {
                  try
                  {
                     VersionCheckCallable version_functor;
                     vpr::LibraryLoader::callEntryPoint(dso,
                                                        get_version_func,
                                                        version_functor);

                     PluginInitCallable init_functor(this);
                     vpr::LibraryLoader::callEntryPoint(dso,
                                                        plugin_init_func,
                                                        init_functor);

                     mLoadedPlugins.push_back(dso);
                  }
                  catch (gadget::PluginVersionException& ex)
                  {
                     vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                        << clrOutBOLD(clrRED, "ERROR")
                        << ": Version mismatch while loading cluster "
                        << "plug-in DSO '" << plugin_dso_name << "'\n"
                        << vprDEBUG_FLUSH;
                     vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                        << "This plug-in will not be usable.\n"
                        << vprDEBUG_FLUSH;
                     vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                        << ex.getExtendedDescription() << std::endl
                        << vprDEBUG_FLUSH;
                  }
                  catch (vpr::Exception& ex)
                  {
                     vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                        << clrOutBOLD(clrRED, "ERROR")
                        << ": Failed to load cluster plug-in DSO '"
                        << plugin_dso_name << "'\n" << vprDEBUG_FLUSH;
                     vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                        << "This plug-in will not be usable.\n"
                        << vprDEBUG_FLUSH;
                     vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                        << ex.what() << std::endl << vprDEBUG_FLUSH;
                  }
               }
               else
               {
                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                     << clrOutBOLD(clrRED, "ERROR")
                     << ": Failed to find cluster plug-in DSO '"
                     << plugin_dso_name << "'\n" << vprDEBUG_FLUSH;
               }
            }
         }

         // Find out if we should we software swap lock.
         const std::string swap_lock_prop_name( "use_software_swap_lock" );
         mSoftwareSwapLock = element->getProperty<bool>( swap_lock_prop_name);

         ret_val = true;
      }

      // Dump the status
      {
         vpr::DebugOutputGuard dbg_output( gadgetDBG_RIM, vprDBG_CONFIG_LVL,
                                 std::string( "New Cluster Manager state:\n" ),
                                 std::string( "-- end state -- \n" ) );
         vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
            << (*this) << std::endl << vprDEBUG_FLUSH;
      }
   }

   return ret_val;
}

 /** Remove the pending element from the current configuration.
  *  @pre configCanHandle(element) == true.
  *  @return true iff the element (and any objects it represented)
  *          were successfully removed.
  */
bool ClusterManager::configRemove( jccl::ConfigElementPtr element )
{
   if ( recognizeClusterManagerConfig( element ) )
   {
      vprDEBUG( gadgetDBG_RIM,vprDBG_CONFIG_LVL )
         << "[ClusterManager] Shutdown the Cluster: " << element->getName()
         << std::endl << vprDEBUG_FLUSH;
      return( true );
   }
   else
   {
      vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
         << "[ClusterManager::configRemove()] ERROR: Something is "
         << "seriously wrong, we should never get here." << std::endl
         << vprDEBUG_FLUSH;
      return( false );
   }
}

/** Checks if this handler can process element.
 *  Typically, an implementation of handler will check the element's
 *  description name/token to decide if it knows how to deal with
 *  it.
 *  @return true iff this handler can process element.
 */
bool ClusterManager::configCanHandle( jccl::ConfigElementPtr element )
{
   return(recognizeClusterManagerConfig( element ));
}

std::string ClusterManager::getElementType()
{
   return "cluster_manager";
}

void ClusterManager::mergeConfigurations(jccl::Configuration* dst, jccl::Configuration* src, vpr::Uint16 type)
{
   std::vector<jccl::ConfigElementPtr>& src_elms = src->vec();

   for (std::vector<jccl::ConfigElementPtr>::iterator itr = src_elms.begin();
        itr != src_elms.end(); itr++)
   {
      if (jccl::ConfigManager::PendingElement::ADD == type)
      {
         dst->add(*itr);
      }
      else
      {
         dst->remove((*itr)->getName());
      }
   }
}

struct ElementTypePred
{
   ElementTypePred(const std::string& type)
      : mType(type)
   {
      /* Do nothing. */ ;
   }

   bool operator()(jccl::ConfigElementPtr elm)
   {
      return elm->getID() == mType;
   }

   const std::string mType;
};

void ClusterManager::configurationChanged(jccl::Configuration* cfg, vpr::Uint16 type)
{
   vprASSERT(!(mIsMaster && mClusterStarted) && "Configuration can not change after cluster is started.");

   const std::string cluster_type("cluster_manager");
   const std::string cluster_node_type("cluster_node");

   std::vector<jccl::ConfigElementPtr> cluster_elements;
   cfg->getByType(cluster_type, cluster_elements);

   if (cluster_elements.size() > 0)
   {
      if (jccl::ConfigManager::PendingElement::REMOVE == type)
      {
         throw ClusterException("Can't remove a cluster_manager element.", VPR_LOCATION);
      }
      if (NULL != mClusterElement.get() || cluster_elements.size() > 1)
      {
         throw cluster::ClusterException("Can't have more than one cluster configurations.", VPR_LOCATION);
      }

      mClusterElement = cluster_elements[0];
      //cfg->remove(mClusterElement->getName());
   }

   std::vector<jccl::ConfigElementPtr> cluster_node_elms;
   cfg->getByType(cluster_node_type, cluster_node_elms);

   for (std::vector<jccl::ConfigElementPtr>::iterator itr = cluster_node_elms.begin();
        itr != cluster_node_elms.end(); itr++)
   {
      jccl::ConfigElementPtr node_elm = *itr;
      std::string node_name = node_elm->getName();
      if (mClusterNodeElements.count(node_name) > 0)
      {
         throw ClusterException("Already have a configuration element for cluster node: " + node_name);
      }

      vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
         << clrOutBOLD( clrCYAN, "[ClusterManager] " )
         << "Adding config for node: " << node_name
         << std::endl << vprDEBUG_FLUSH;

      mClusterNodeElements[node_name] = node_elm;

      // XXX: Leave the ClusterManager configuration around to load the correct plugins.
      //cfg->remove(node_name);

      std::string new_node_hostname =
         node_elm->getProperty<std::string>( "host_name" );

      if (cluster::ClusterNetwork::isLocalHost( new_node_hostname ))
      {
         mLocalNodeElement = node_elm;

         // NOTE: Add all machine dependent ConfigElementPtr's here
         vprASSERT( node_elm->getNum("display_system") == 1 
            && "A Cluster System element must have exactly 1 display_system element" );

         // Get a list of all embedded elements.
         typedef std::vector<jccl::ConfigElementPtr> elm_list_t;
         elm_list_t child_elms = node_elm->getChildElements();

         // If there are no display windows, then don't wait for the display to open.
         ElementTypePred pred("display_window");
         elm_list_t::const_iterator found = std::find_if(child_elms.begin(), child_elms.end(), pred);
         if (child_elms.end() == found)
         { mWindowOpened = true; }

         for (elm_list_t::iterator i = child_elms.begin(); i != child_elms.end(); ++i)
         {
            jccl::ConfigManager::instance()->addConfigElement(*i, jccl::ConfigManager::PendingElement::ADD);

            vprDEBUG( gadgetDBG_NET_MGR, vprDBG_CONFIG_LVL ) << clrSetBOLD(clrCYAN)
               << clrOutBOLD( clrMAGENTA,"[NetworkManager]" )
               << " Adding Machine specific ConfigElement: "
               << (*i)->getName() << clrRESET << std::endl << vprDEBUG_FLUSH;
         }
      }
   }

   vprDEBUG( gadgetDBG_RIM, vprDBG_CONFIG_LVL )
      << clrOutBOLD( clrCYAN, "[ClusterManager] " )
      << "Copying configuration from: " << cfg->getFileName()
      << std::endl << vprDEBUG_FLUSH;

   mergeConfigurations(&mSystemConfiguration, cfg, type);
}

/**
 * Dump the current Status of the InputManager, listing all
 * the devices, proxies and internal settings
 */
std::ostream& operator<<( std::ostream& out, ClusterManager& mgr )
{
   out << std::endl << "========== ClusterManager Status =========="
       << std::endl;
   out << "preDraw() call count:       " << mgr.mPreDrawCallCount << std::endl;
   out << "postPostFrame() call count: " << mgr.mPostPostFrameCallCount << std::endl;
   out << "Plugins:" << std::endl;

   // Dump Plugins
   for ( ClusterManager::plugin_list_t::iterator itr = mgr.mPlugins.begin();
         itr != mgr.mPlugins.end(); itr++ )
   {
      if ( (*itr).get() != NULL )
      {
         out << "  name:" << std::setw(30) << std::setfill(' ') << (*itr)->getPluginName()
             << "  guid:" << std::setw(30) << std::setfill(' ') << (*itr)->getHandlerGUID()
             << "  active:" << std::setw(7) << std::setfill(' ')
             << ( (*itr)->isActive() ? "true" : "false" ) << std::endl;
      }
   }

   out << std::endl;

   return out;
}

} // End of cluster namespace
