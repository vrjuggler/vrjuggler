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

#include <gadget/gadgetConfig.h>

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

#include <gadget/RemoteInputManager/RemoteInputManager.h>
#include <gadget/Type/BaseTypeFactory.h>

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
   mRemoteInputManager = new RemoteInputManager(this);
}


/**********************************************************
  InputManager::~InputManager()

  InputManager Destructor

*********************************************** ahimberg */
InputManager::~InputManager()
{
   mRemoteInputManager->shutdown();
   delete mRemoteInputManager;

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

   if (mRemoteInputManager->configCanHandle(chunk))
      ret_val = mRemoteInputManager->configAdd(chunk);
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
   else if(chunk->getDescToken() == std::string("InputManager"))
   {
      vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                                       std::string("Handling InputManager chunk:\n"),
                                       std::string("-- end state -- \n"));

      const std::string prop_name("driver");
      int driver_count = chunk->getNum(prop_name);
      std::string driver_dso;

      for ( int i = 0; i < driver_count; ++i )
      {
         driver_dso =
            vpr::replaceEnvVars(chunk->getProperty<std::string>(prop_name, i));
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "Loading driver DSO '" << driver_dso << "'\n" << vprDEBUG_FLUSH;

         // If any part of the driver loading fails, the object driver_library
         // will go out of scope at the end of this iteration, thereby freeing
         // the allocated memory.
         vpr::LibraryPtr driver_library =
            vpr::LibraryPtr(new vpr::Library(driver_dso));

         if ( driver_library->load().success() )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "Loaded driver DSO successfully\n" << vprDEBUG_FLUSH;

            void (*creator)(gadget::InputManager*);

            creator = (void (*)(gadget::InputManager*)) driver_library->findSymbol("initDevice");

            if ( NULL != creator )
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                  << "Got pointer to driver factory\n" << vprDEBUG_FLUSH;
               mDeviceDrivers.push_back(driver_library);
               (*creator)(this);
            }
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

   if (mRemoteInputManager->configCanHandle(chunk))
        ret_val = mRemoteInputManager->configRemove(chunk);
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
{
   return ( DeviceFactory::instance()->recognizeDevice(chunk) ||
            ProxyFactory::instance()->recognizeProxy(chunk) ||
            recognizeProxyAlias(chunk) ||
            mRemoteInputManager->configCanHandle(chunk) ||
            (chunk->getDescToken() == std::string("displaySystem")) ||
            (chunk->getDescToken() == std::string("InputManager"))
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
      delete new_device;
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

   // -- Add to proxy table
   if(false == addProxy(proxy_name, new_proxy))
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



/**********************************************************
  InputManager::updateAllData()

  Call UpdateData() on all the devices and transform proxies

*********************************************** ahimberg */
void InputManager::updateAllData()
{
   for (tDevTableType::iterator i = mDevTable.begin(); i != mDevTable.end(); i++)      // all DEVICES
     if ((*i).second != NULL)
         i->second->updateData();

     // send and receive net device messages
   if (mRemoteInputManager->isActive())
   {
      mRemoteInputManager->updateAll();
   }


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
   tDevTableType::iterator ret_dev;
   ret_dev = mDevTable.find(deviceName);
   if(ret_dev != mDevTable.end())
      return ret_dev->second;
   else
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

   alias_name = chunk->template getProperty<std::string>("aliasName");
   proxy_name = chunk->template getProperty<std::string>("proxyPtr");

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

   alias_name = chunk->template getProperty<std::string>("aliasName");

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
bool InputManager::addProxy(std::string proxyName, Proxy* proxy)
{
   // Check if already in the table
   if(mProxyTable.end() != mProxyTable.find(proxyName))     // Found it
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "Trying to add proxy: " << proxyName
         << " but it is already in the proxy table.\n" << vprDEBUG_FLUSH;
      return false;
   }

   mProxyTable[proxyName] = proxy;
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

} // End of gadget namespace
