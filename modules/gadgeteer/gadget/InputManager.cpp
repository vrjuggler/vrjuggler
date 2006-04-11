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

#include <gadget/gadgetConfig.h>

#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>

#include <vpr/DynLoad/LibraryFinder.h>
#include <vpr/Util/FileUtils.h>

#include <gadget/InputManager.h> // my header...
#include <gadget/Type/Proxy.h>
#include <gadget/Type/DeviceFactory.h>
#include <gadget/ProxyFactory.h>
#include <gadget/Type/DeviceInterface.h>

#include <gadget/Util/Debug.h>

#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/RTRC/ConfigManager.h>

#include <cluster/ClusterManager.h>
#include <gadget/Type/BaseTypeFactory.h>

#include <gadget/InputLogger.h>

namespace gadget
{

   vprSingletonImp( InputManager );    // Implementation of singleton

// Local helpers
bool recognizeProxyAlias( jccl::ConfigChunkPtr chunk );

/**********************************************************
  InputManager::InputManager()

  InputManager Constructor

*********************************************** ahimberg */
InputManager::InputManager()
{
   
}


/**********************************************************
  InputManager::~InputManager()

  InputManager Destructor

*********************************************** ahimberg */
InputManager::~InputManager()
{
   for (tDevTableType::iterator a = mDevTable.begin(); a != mDevTable.end(); a++)    // Stop all devices
      if ((*a).second != NULL)
      {
         (*a).second->stopSampling();
         delete (*a).second;
      }

   // Delete all the proxies
   for(std::map<std::string, Proxy*>::iterator j = mProxyTable.begin(); j != mProxyTable.end(); j++)
   {
      delete (*j).second;
   }
}

/** Adds the given config chunk to the input system. */
bool InputManager::configAdd(jccl::ConfigChunkPtr chunk)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                              std::string("Input Manager: Adding pending config chunk.\n"),
                              std::string("...done adding chunk.\n"));

   vprASSERT(configCanHandle(chunk));

   bool ret_val = false;      // Flag to return success
   
   if (cluster::ClusterManager::instance()->recognizeRemoteDeviceConfig(chunk))
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "InputManager can not handle remote devices, we must use Remote Input Manager."
         << vprDEBUG_FLUSH;
      ret_val = false;
   }
   else if(DeviceFactory::instance()->recognizeDevice(chunk))
      ret_val = configureDevice(chunk);
   else if(ProxyFactory::instance()->recognizeProxy(chunk))
      ret_val = configureProxy(chunk);
   else if(recognizeProxyAlias(chunk))
      ret_val = configureProxyAlias(chunk);
   else if(chunk->getDescToken() == std::string("displaySystem"))
   {
      // XXX: Put signal here to tell draw manager to lookup new stuff
      mDisplaySystemChunk = chunk;     // Keep track of the display system chunk
      ret_val = true;
   }
   else if(chunk->getDescToken() == std::string("gadget_logger"))
   {
      ret_val = configureInputLogger(chunk);
   }
   else if(chunk->getDescToken() == std::string("InputManager"))
   {
      vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                                       std::string("Handling InputManager chunk:\n"),
                                       std::string("-- end state -- \n"));

      // --- Load device driver dsos -- //
      // - Load individual drivers
      const std::string driver_prop_name("driver");
      int driver_count = chunk->getNum(driver_prop_name);
      std::string driver_dso;

      for ( int i = 0; i < driver_count; ++i )
      {
         driver_dso =
            vpr::replaceEnvVars(chunk->getProperty<std::string>(driver_prop_name, i));
         if(!driver_dso.empty())
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
               << "InputMgr::config: Loading driver DSO '" << driver_dso << "'\n" << vprDEBUG_FLUSH;

            // If any part of the driver loading fails, the object driver_library
            // will go out of scope at the end of this iteration, thereby freeing
            // the allocated memory.
            vpr::LibraryPtr driver_library =
               vpr::LibraryPtr(new vpr::Library(driver_dso));
            this->loadDriverDSO(driver_library);
         }
      }

      // - Load driver directory
      const std::string dir_prop_name("driverDirectory");
      const std::string dso_ext_name("dsoExtName");
      int dir_count = chunk->getNum(dir_prop_name);
      std::string driver_dir;

      const std::string driver_ext = chunk->getProperty<std::string>(dso_ext_name);

      for ( int i = 0; i < dir_count; ++i )
      {
         driver_dir = vpr::replaceEnvVars(chunk->getProperty<std::string>(dir_prop_name, i));

         // The vpr::LibraryFinder will throw an exception if driver_dir is
         // (somehow) an invalid path.
         try
         {
            if (boost::filesystem::exists(driver_dir))
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                  << "Searching for driver DSOs in '" << driver_dir << "'\n"
                  << vprDEBUG_FLUSH;

               vpr::LibraryFinder finder(driver_dir, driver_ext);
               vpr::LibraryFinder::LibraryList libs = finder.getLibraries();

               for ( vpr::LibraryFinder::LibraryList::iterator lib = libs.begin();
                     lib != libs.end();
                     ++lib )
               {
                  this->loadDriverDSO(*lib);
               }
            }
            else
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                  << clrOutBOLD(clrYELLOW, "WARNING:")
                  << " Invalid directory for driver dso's: " << driver_dir
                  << std::endl << vprDEBUG_FLUSH;
            }
         }
         catch (boost::filesystem::filesystem_error& fsEx)
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << clrOutNORM(clrRED, "ERROR:")
               << " File system exception caught: " << fsEx.what()
               << std::endl << vprDEBUG_FLUSH;
         }
      }

      ret_val = true;
   }

   //DumpStatus();                      // Dump the status
   {
      vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                              std::string("New Input Manager state:\n"),
                              std::string("-- end state -- \n"));
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_VERB_LVL) << (*this) << vprDEBUG_FLUSH;
   }

   if(ret_val)
   {
      updateAllData();                             // Update all the input data
      BaseDeviceInterface::refreshAllDevices();      // Refresh all the device interface handles
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
         << "Updated all devices" << std::endl << vprDEBUG_FLUSH;
   }

   return ret_val;         // Return the success flag if we added at all
}


/**
 * Removes the chunk from the current configuration.
 */
bool InputManager::configRemove(jccl::ConfigChunkPtr chunk)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                              std::string("InputManager: Removing config...\n"),
                              std::string("done removing config.\n"));
   vprASSERT(configCanHandle(chunk));

   bool ret_val = false;      // Flag to return success

   // NEED TO FIX!!!!
   if (cluster::ClusterManager::instance()->recognizeRemoteDeviceConfig(chunk))
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "InputManager can not handle remote devices, we must use Remote Input Manager."
         << vprDEBUG_FLUSH;
      ret_val = false;
   }
   else if(DeviceFactory::instance()->recognizeDevice(chunk))
      ret_val = removeDevice(chunk);
   else if(recognizeProxyAlias(chunk))
      ret_val = removeProxyAlias(chunk);
   else if(ProxyFactory::instance()->recognizeProxy(chunk))
      ret_val = removeProxy(chunk);
   else if(chunk->getDescToken() == std::string("displaySystem"))
   {
      mDisplaySystemChunk.reset();     // Keep track of the display system chunk
      ret_val = true;                  // We successfully configured.
                                       // This tell processPending to remove it to the active config
   }
   else
      ret_val = false;

   if(ret_val)
   {
      updateAllData();                             // Update all the input data
      BaseDeviceInterface::refreshAllDevices();      // Refresh all the device interface handles
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_VERB_LVL)
         << "InputManager::configRemove(): Updated all data" << std::endl
         << vprDEBUG_FLUSH;
   }

   return ret_val;         // Return the success flag if we added at all
}


// Return true if:
//  It is recognized device, proxy, or alias.
bool InputManager::configCanHandle(jccl::ConfigChunkPtr chunk)
{           // NEED TO FIX!!!!
   return ( (DeviceFactory::instance()->recognizeDevice(chunk) && 
             !cluster::ClusterManager::instance()->recognizeRemoteDeviceConfig(chunk)) ||
            ProxyFactory::instance()->recognizeProxy(chunk) ||
            recognizeProxyAlias(chunk) ||
            (chunk->getDescToken() == std::string("displaySystem")) ||
            (chunk->getDescToken() == std::string("InputManager")) ||
            (chunk->getDescToken() == std::string("gadget_logger"))
          );
}

jccl::ConfigChunkPtr InputManager::getDisplaySystemChunk()
{
   if ( mDisplaySystemChunk.get() == NULL )
   {
      jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();

      cfg_mgr->lockActive();
      {
         std::vector<jccl::ConfigChunkPtr>::iterator i;
         for(i=cfg_mgr->getActiveBegin(); i != cfg_mgr->getActiveEnd();++i)
         {
            if((*i)->getDescToken() == std::string("displaySystem"))
            {
               mDisplaySystemChunk = *i;
               break;         // This guarantees that we get the first displaySystem chunk.
            }
         }
      }
      cfg_mgr->unlockActive();
      //vprASSERT(mDisplaySystemChunk.get() != NULL && "No Display Manager chunk found!");
   }
   return mDisplaySystemChunk;
}

// Check if the device factory or proxy factory can handle the chunk
bool InputManager::configureDevice(jccl::ConfigChunkPtr chunk)
{
   bool ret_val;
   std::string dev_name = chunk->getFullName();

   vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                                 std::string("InputManager::configureDevice: dev[") + dev_name + std::string("]\n"),
                                 std::string("done configuring device\n"));

   Input* new_device;
   new_device = DeviceFactory::instance()->loadDevice(chunk);

   if ((new_device != NULL) && (new_device->startSampling()))
   {
      addDevice(new_device);
      ret_val = true;
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
         << "   Successfully added dev: " << dev_name.c_str() << std::endl
         << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:")
                    << "new dev " << clrSetBOLD(clrCYAN) << dev_name.c_str() << clrRESET << " failed to start.. deleting instance" << std::endl << vprDEBUG_FLUSH;
      if ( NULL != new_device )
      {
         delete new_device;
      }

      ret_val = false;
   }

   return ret_val;
}

// Check if the device factory or proxy factory can handle the chunk
bool InputManager::configureProxy(jccl::ConfigChunkPtr chunk)
{
   std::string proxy_name = chunk->getFullName();

vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                                 std::string("vjInputManager::configureProxy: Named: ") + proxy_name + std::string("\n"),
                                 std::string("done configuring proxy\n"));

   Proxy* new_proxy;

   // Tell the factory to load the proxy
   // NOTE: The config for the proxy registers it with the input manager
   new_proxy = ProxyFactory::instance()->loadProxy(chunk);   

   // Check for success
   if(NULL == new_proxy)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:") << "vjInputManager::configureProxy: Proxy construction failed:" << proxy_name.c_str() << std::endl << vprDEBUG_FLUSH;
      return false;
   }
   vprASSERT(proxy_name == new_proxy->getName());

   // -- Add to proxy table
   if(false == addProxy(new_proxy))
   {
      return false;
   }

   return true;
}


/**
 * Removes the device associated with the given chunk.
 */
bool InputManager::removeDevice(jccl::ConfigChunkPtr chunk)
{
   return removeDevice(chunk->getFullName());
}


/**********************************************************
  operator<<()

  Dump the current Status of the InputManager, listing all
  the devices, proxies and internal settings

*********************************************** ahimberg */
GADGET_IMPLEMENT(std::ostream&) operator<<(std::ostream& out, InputManager& iMgr)
{
  out << "\n=============== InputManager Status: ===============================" << std::endl;

  out << "Devices:\n";

  for (InputManager::tDevTableType::iterator i = iMgr.mDevTable.begin(); i != iMgr.mDevTable.end(); i++)      // Dump DEVICES
     if ((*i).second != NULL)
       out << std::setw(2) << std::setfill(' ') << i->first << ":"
           << "  name:" << std::setw(30) << std::setfill(' ') << i->second->getInstanceName()
           << "  type:" << std::setw(12) << std::setfill(' ') << typeid(*(i->second)).name()
           << std::endl;

  out << "\nProxies:\n";
  for (std::map<std::string, Proxy*>::iterator i_p = iMgr.mProxyTable.begin();
       i_p != iMgr.mProxyTable.end(); i_p++)
  {
    out << (*i_p).second->getName()
        << "   Proxies:";
    if(NULL != ((*i_p).second->getProxiedInputDevice()))
       out << ((*i_p).second->getProxiedInputDevice())->getInstanceName();
    else
       out << "None (internal dummy)";
    out << std::endl;
  }

  out << std::endl;

      // Dump Alias list
  out << "Alias List:" << std::endl;
  for(std::map<std::string, std::string>::iterator cur_alias = iMgr.mProxyAliases.begin(); cur_alias != iMgr.mProxyAliases.end(); cur_alias++)
     out << "    " << (*cur_alias).first.c_str() << "  proxy:" << (*cur_alias).second << std::endl;

  out << "=============== InputManager Status =========================" << std::endl;
  return out;
}

/**********************************************************
  InputManager::addDevice(Input* devPtr)

  Add a device to the InputManager, returns the index
  where the device was placed

*********************************************** ahimberg */
bool InputManager::addDevice(Input* devPtr)
{
   mDevTable[devPtr->getInstanceName()] = devPtr;

   refreshAllProxies();

   return true;
}

/**
 *   Add a remote device from the RemoteInputManager to the InputManager.
 */
bool InputManager::addRemoteDevice(Input* devPtr, const std::string& device_name)
{
   mDevTable[device_name] = devPtr;

   refreshAllProxies();

   return true;
}

/**********************************************************
  InputManager::updateAllData()

  Call UpdateData() on all the devices and transform proxies

*********************************************** ahimberg */
void InputManager::updateAllData()
{
   for (tDevTableType::iterator i = mDevTable.begin(); i != mDevTable.end(); i++)      // all DEVICES
   {
      if ((*i).second != NULL)
      {  i->second->updateData(); }
   }

   // Update Logger - Done here so that device can be "rewritten" by logger
   //                 before rim or proxies get their hands on the data
   if(mInputLogger.get() != NULL)
   {  mInputLogger->process(); }

   // Update proxies MIGHT NOT NEED
   for (std::map<std::string, Proxy*>::iterator i_p = mProxyTable.begin();
       i_p != mProxyTable.end(); i_p++)
   {
      (*i_p).second->updateData();
   }
}



// Return a Input ptr to a deviced named
// RETURNS: NULL - Not found
Input* InputManager::getDevice(std::string deviceName)
{
   // Look up in Input Manager
   tDevTableType::iterator ret_dev;
   ret_dev = mDevTable.find(deviceName);
   if(ret_dev != mDevTable.end())
   {
      return ret_dev->second;
   }   
   return NULL;
}

DeviceFactory* InputManager::getDeviceFactory()
{
   return gadget::DeviceFactory::instance();
}

// Remove the device that is pointed to by devPtr
bool InputManager::removeDevice(const Input* devPtr)
{
   for (tDevTableType::iterator i = mDevTable.begin(); i != mDevTable.end(); i++)      // all DEVICES
     if ((*i).second == devPtr)
         return removeDevice((*i).first);

   return false;
}


/**********************************************************
  InputManager::removeDevice(char* mInstName)

  InputManager remove mInstName from the InputManager,
  currently stupifies all the proxies connected to it.

*********************************************** ahimberg */
bool InputManager::removeDevice(std::string mInstName)
{
   tDevTableType::iterator dev_found;
   dev_found = mDevTable.find(mInstName);
   if(dev_found == mDevTable.end())
      return false;

   Input* dev_ptr = dev_found->second;

   if(NULL == dev_ptr)
      return false;

   // Find any proxies connected to the device
   // Stupify any proxies connected to device
   // NOTE: Could just remove it and then refresh all, but this is a little safer
   //       since we explicitly stupify the one that we don't want anymore
   for (std::map<std::string, Proxy*>::iterator i_p = mProxyTable.begin();
       i_p != mProxyTable.end(); i_p++)
   {
      if((*i_p).second->getProxiedInputDevice() == dev_ptr)
      {
         (*i_p).second->stupify(true);
      }
   }

   // stop the device, delete it, set pointer to NULL
   dev_ptr->stopSampling();
   delete dev_ptr;
   mDevTable.erase(dev_found);

   // Refresh the proxies
   refreshAllProxies();

   return true;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   PRIVATE FUNCTIONS TO INPUTGROUP
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


// Is it a proxy alias
bool recognizeProxyAlias(jccl::ConfigChunkPtr chunk)
{
   return (chunk->getDescToken() == std::string("proxyAlias"));
}

/**
 * Configures proxy aliases in config database.
 * @pre none
 * @post (alias not already in list) ==> Alias is added to proxyAlias list<br>
 *       (alias was already is list) ==> Alias is set to point to the new proxy instead
 */
bool InputManager::configureProxyAlias(jccl::ConfigChunkPtr chunk)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                           std::string("vjInputManager::Configuring proxy alias\n"),
                           std::string("...done configuring alias.\n"));

   vprASSERT(chunk->getDescToken() == "proxyAlias");

   std::string alias_name, proxy_name;  // The string of the alias, name of proxy to pt to

   alias_name = chunk->getProperty<std::string>("aliasName");
   proxy_name = chunk->getProperty<std::string>("proxyPtr");

   addProxyAlias(alias_name, proxy_name);

   return true;
}

/**
 * Removes a proxy aliases in config database.
 * @pre none
 * @post (alias not in list) ==> returns = false<br>
 *       (alias is in list) ==> (alias is removed from list) returns true
 */
bool InputManager::removeProxyAlias(jccl::ConfigChunkPtr chunk)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                        std::string("vjInputManager::RemoveProxyAlias\n"),
                        std::string("...done removing alias.\n"));

   vprASSERT(chunk->getDescToken() == "proxyAlias");

   std::string alias_name, proxy_name;  // The string of the alias, name of proxy to pt to

   alias_name = chunk->getProperty<std::string>("aliasName");

   if(mProxyAliases.end() == mProxyAliases.find(alias_name))
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "vjInputManager::RemoveProxyAlias: Alias: " << alias_name.c_str()
         << "  cannot find proxy: " << proxy_name.c_str() << std::endl
         << vprDEBUG_FLUSH;
      return false;
   }

   mProxyAliases.erase(alias_name);

   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
      << "   alias:" << alias_name.c_str() << "   index:"
      << mProxyAliases[proxy_name] << "  has been removed." << std::endl
      << vprDEBUG_FLUSH;

   return true;
}


/**
 * Adds a Proxy alias to the alias list.
 * @pre none - BUT: Preferable if proxyName is for a valid proxy.
 * @post Alias list has alias str refering to proxyName.
 */
void InputManager::addProxyAlias(std::string alias_name, std::string proxy_name)
{
   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "Proxy alias [" << alias_name << "] added.\n"
      << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_VERB_LVL)
      << "   proxy:" << proxy_name << std::endl << vprDEBUG_FLUSH;

   mProxyAliases[alias_name] = proxy_name;
}


/**
 * Adds a proxy to the proxy table.
 */
bool InputManager::addProxy(Proxy* proxy)
{
   std::string proxy_name = proxy->getName();
   vprASSERT(!proxy_name.empty() && "Tried to add proxy with empty name");

   // Check if already in the table
   if(mProxyTable.end() != mProxyTable.find(proxy_name))     // Found it
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "Trying to add proxy: " << proxy_name
         << " but it is already in the proxy table.\n" << vprDEBUG_FLUSH;
      return false;
   }
   
   mProxyTable[proxy_name] = proxy;
   return true;
}

/**
 * Gets index to the proxy/alias named by str.
 */
Proxy* InputManager::getProxy(std::string proxyName)
{
   // Is it in table
   if(mProxyTable.end() != mProxyTable.find(proxyName))     // proxy name
   {
      return mProxyTable[proxyName];
   }

   // Is there an alias
   if(mProxyAliases.end() != mProxyAliases.find(proxyName))                // Found proxyAlias
   {
      if(mProxyTable.end() != mProxyTable.find(mProxyAliases[proxyName]))  // Found proxyAlias[name]
      {
         return mProxyTable[mProxyAliases[proxyName]];
      }
   }

   return false;
}

 // Refresh all the proxies to have then update what device they are pointing at
void InputManager::refreshAllProxies()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "vjInputManager::refreshAppProxies: Refreshing all....\n"
      << vprDEBUG_FLUSH;

   for(std::map<std::string, Proxy*>::iterator i = mProxyTable.begin(); i != mProxyTable.end(); i++)
   {
      (*i).second->refresh();
   }
}

bool InputManager::removeProxy(std::string proxyName)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                  std::string("vjInputManager::removeProxy\n"),
                  std::string("\n"));
   if(mProxyTable.end() == mProxyTable.find(proxyName))
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "vjInputManager::removeProxy: proxy: " << proxyName.c_str()
         << "  cannot find proxy: " << proxyName.c_str() << std::endl
         << vprDEBUG_FLUSH;
      return false;
   }

   mProxyTable.erase(proxyName);

   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_CONFIG_LVL)
      << "   proxy:" << proxyName.c_str() << "  has been removed."
      << std::endl << vprDEBUG_FLUSH;
   return true;
}

bool InputManager::removeProxy(jccl::ConfigChunkPtr chunk)
{
   std::string proxy_name;
   proxy_name = chunk->getFullName();
   return removeProxy(proxy_name);
}

vpr::ReturnStatus InputManager::loadDriverDSO(vpr::LibraryPtr driverDSO)
{
   vprASSERT(driverDSO.get() != NULL && "Invalid vpr::LibraryPtr object");

   const int lib_name_width(50);

   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "Loading device library: " << std::setiosflags(std::ios::right)
      << std::setfill(' ') << std::setw(lib_name_width) << driverDSO->getName()
      << std::resetiosflags(std::ios::right) << "     " << vprDEBUG_FLUSH;

   // Load the driver
   vpr::ReturnStatus status;
   status = driverDSO->load();

   if ( status.success() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "Loaded DSO success.\n" << vprDEBUG_FLUSH;

      void (*creator)(gadget::InputManager*);

      creator = (void (*)(gadget::InputManager*)) driverDSO->findSymbol("initDevice");

      if ( NULL != creator )
      {
         vprDEBUG_CONT(vprDBG_ALL,vprDBG_CONFIG_STATUS_LVL) << "[ " << clrSetNORM(clrGREEN) << "OK" << clrRESET << " ]\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << "Got pointer to driver factory\n" << vprDEBUG_FLUSH;

         mDeviceDrivers.push_back(driverDSO);
         (*creator)(this);
      }
      else
      {
         vprDEBUG_CONT(vprDBG_ALL,vprDBG_CONFIG_LVL) << "[ " << clrSetNORM(clrRED) << "FAILED lookup" << clrRESET << " ]\n" << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << clrOutNORM(clrYELLOW, "WARNING")
            << ": Failed to look up factory function in driver DSO '"
            << driverDSO->getName() << "'\n" << vprDEBUG_FLUSH;
      }
   }
   else
   {
      vprDEBUG_CONT(vprDBG_ALL,vprDBG_CONFIG_LVL) << "[ " << clrSetNORM(clrRED) << "FAILED" << clrRESET << " ]\n" << vprDEBUG_FLUSH;
   }

   return status;
}

/** Get the input logger connected to the system */
gadget::InputLoggerPtr InputManager::getInputLogger()
{
   // Make sure it is allocated -- Lazy allocation
   if(mInputLogger.get() == NULL)
   {
      mInputLogger = boost::shared_ptr<InputLogger>(new InputLogger);
   }

   return mInputLogger;
}


bool InputManager::configureInputLogger(jccl::ConfigChunkPtr chunk)
{
   // Configure it
   return getInputLogger()->config(chunk);
}

} // End of gadget namespace
