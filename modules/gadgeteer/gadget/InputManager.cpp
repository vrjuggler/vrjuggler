/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#include <gadget/InputManager.h> // my header...
#include <gadget/Type/Proxy.h>
#include <gadget/Type/DeviceFactory.h>
#include <gadget/ProxyFactory.h>
#include <gadget/Type/DeviceInterface.h>

#include <vrj/Util/Debug.h>

#include <jccl/Config/ConfigChunk.h>




namespace gadget
{

   vprSingletonImp( InputManager );    // Implementation of singleton

// Local helpers
bool recognizeProxyAlias( jccl::ConfigChunk* chunk );

/**********************************************************
  InputManager::InputManager()

  InputManager Constructor

*********************************************** ahimberg */
InputManager::InputManager()
{;}


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


//: Add the given config chunk to the input system
bool InputManager::configAdd(jccl::ConfigChunk* chunk)
{
   vprDEBUG_BEGIN(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "\nInputManager: Adding pending config chunk... " << std::endl << vprDEBUG_FLUSH;
   vprASSERT(configCanHandle(chunk));

   bool ret_val = false;      // Flag to return success

   if(DeviceFactory::instance()->recognizeDevice(chunk))
      ret_val = configureDevice(chunk);
   else if(ProxyFactory::instance()->recognizeProxy(chunk))
      ret_val = configureProxy(chunk);
   else if(recognizeProxyAlias(chunk))
      ret_val = configureProxyAlias(chunk);

   //DumpStatus();                      // Dump the status
   vprDEBUG_BEGIN(vrjDBG_INPUT_MGR,vprDBG_VERB_LVL) << "New input manager state:\n" << vprDEBUG_FLUSH;
   vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_VERB_LVL) << (*this) << vprDEBUG_FLUSH;
   vprDEBUG_END(vrjDBG_INPUT_MGR,vprDBG_VERB_LVL) << std::endl << vprDEBUG_FLUSH;

   if(ret_val)
   {
      updateAllData();                             // Update all the input data
      BaseDeviceInterface::refreshAllDevices();      // Refresh all the device interface handles
      vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "Updated all devices" << std::endl << vprDEBUG_FLUSH;
   }

   vprDEBUG_END(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << std::endl << vprDEBUG_FLUSH;
   return ret_val;         // Return the success flag if we added at all
}


//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
//! POST: (chunk is proxy) ==> (returns == false)
//+       (chunk is device) ==> (devices is removed && proxies are stupified)
//+       (chunk is proxyAlias) ==> (proxyAlias is removed && devInterfaces.refreshAll())
//!RETURNS: success
bool InputManager::configRemove(jccl::ConfigChunk* chunk)
{
   vprDEBUG_BEGIN(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "\nInputManager: Removing config... " << std::endl << vprDEBUG_FLUSH;
   vprASSERT(configCanHandle(chunk));

   bool ret_val = false;      // Flag to return success

   if(DeviceFactory::instance()->recognizeDevice(chunk))
      ret_val = removeDevice(chunk);
   else if(recognizeProxyAlias(chunk))
      ret_val = removeProxyAlias(chunk);
   else if(ProxyFactory::instance()->recognizeProxy(chunk))
      ret_val = removeProxy(chunk);
   else
      ret_val = false;

   if(ret_val)
   {
      updateAllData();                             // Update all the input data
      BaseDeviceInterface::refreshAllDevices();      // Refresh all the device interface handles
      vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_VERB_LVL) << "InputManager::configRemove(): Updated all data" << std::endl << vprDEBUG_FLUSH;
   }

   vprDEBUG_END(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << std::endl << vprDEBUG_FLUSH;
   return ret_val;         // Return the success flag if we added at all
}


// Return true if:
//  It is recognized device, proxy, or alias.
bool InputManager::configCanHandle(jccl::ConfigChunk* chunk)
{
   return ( DeviceFactory::instance()->recognizeDevice(chunk) ||
            ProxyFactory::instance()->recognizeProxy(chunk) ||
            recognizeProxyAlias(chunk));
}

// Check if the device factory or proxy factory can handle the chunk
bool InputManager::configureDevice(jccl::ConfigChunk* chunk)
{
   bool ret_val;
   std::string dev_name = chunk->getProperty("name");
   vprDEBUG_BEGIN(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "ConfigureDevice: Named: " << dev_name.c_str() << std::endl << vprDEBUG_FLUSH;

   Input* new_device;
   new_device = DeviceFactory::instance()->loadDevice(chunk);

   if ((new_device != NULL) && (new_device->startSampling()))
   {
      addDevice(new_device);
      ret_val = true;
      vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "   Successfully added dev: " << dev_name.c_str() << std::endl << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:")
                  << "new dev " << clrSetBOLD(clrCYAN) << dev_name.c_str() << clrRESET << " failed to start.. deleting instance" << std::endl << vprDEBUG_FLUSH;
      delete new_device;
      ret_val = false;
   }

   vprDEBUG_END(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << std::endl << vprDEBUG_FLUSH;
   return ret_val;
}

// Check if the device factory or proxy factory can handle the chunk
bool InputManager::configureProxy(jccl::ConfigChunk* chunk)
{
   std::string proxy_name = chunk->getProperty("name");
   vprDEBUG_BEGIN(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjInputManager::configureProxy: Named: " << proxy_name.c_str() << std::endl << vprDEBUG_FLUSH;

   Proxy* new_proxy;

   // Tell the factory to load the proxy
   // NOTE: The config for the proxy registers it with the input manager
   new_proxy = ProxyFactory::instance()->loadProxy(chunk);

   // Check for success
   if(NULL == new_proxy)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:") << "vjInputManager::configureProxy: Proxy construction failed:" << proxy_name.c_str() << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_END(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   // -- Add to proxy table
   if(false == addProxy(proxy_name, new_proxy))
   { return false; }

   vprDEBUG_END(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "   Proxy is set" << std::endl << vprDEBUG_FLUSH;

   return true;
}


//: Remove the device associated with the given chunk
//!RETURNS: true - Device was removed
bool InputManager::removeDevice(jccl::ConfigChunk* chunk)
{
   char* dev_name = chunk->getProperty("name").cstring();      // Get the name of the device
   return removeDevice(dev_name);
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

   // Update proxies
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

// Remove the device that is pointed to by devPtr
bool InputManager::removeDevice(const Input* devPtr)
{
   for (tDevTableType::iterator i = mDevTable.begin(); i != mDevTable.end(); i++)      // all DEVICES
     if ((*i).second == devPtr)
         return removeDevice((*i).first);

   return false;
}


/**********************************************************
  InputManager::removeDevice(char* instName)

  InputManager remove instName from the InputManager,
  currently stupifies all the proxies connected to it.

*********************************************** ahimberg */
bool InputManager::removeDevice(std::string instName)
{
   tDevTableType::iterator dev_found;
   dev_found = mDevTable.find(instName);
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
bool recognizeProxyAlias(jccl::ConfigChunk* chunk)
{
   return (((std::string)chunk->getType()) == std::string("proxyAlias"));
}

// Configures proxy aliases in config database
// PRE: none
// POST: (alias not already in list) ==> Alias is added to proxyAlias list
//+      (alias was already is list) ==> Alias is set to point to the new proxy instead
bool InputManager::configureProxyAlias(jccl::ConfigChunk* chunk)
{
   vprDEBUG_BEGIN(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjInputManager::Configuring proxy alias" << std::endl << vprDEBUG_FLUSH;
   vprASSERT(((std::string)chunk->getType()) == "proxyAlias");

   std::string alias_name, proxy_name;  // The string of the alias, name of proxy to pt to

   alias_name = (std::string)chunk->getProperty("aliasName");
   proxy_name = (std::string)chunk->getProperty("proxyPtr");

   addProxyAlias(alias_name, proxy_name);

   vprDEBUG_END(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << std::endl << vprDEBUG_FLUSH;

   return true;
}

// Remove a proxy aliases in config database
// PRE: none
// POST: (alias not in list) ==> returns = false
//+      (alias is in list) ==> (alias is removed from list) returns true
bool InputManager::removeProxyAlias(jccl::ConfigChunk* chunk)
{
   vprDEBUG_BEGIN(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjInputManager::RemoveProxyAlias" << std::endl << vprDEBUG_FLUSH;
   vprASSERT(((std::string)chunk->getType()) == "proxyAlias");

   std::string alias_name, proxy_name;  // The string of the alias, name of proxy to pt to

   alias_name = (std::string)chunk->getProperty("aliasName");

   if(mProxyAliases.end() == mProxyAliases.find(alias_name))
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
                 << clrOutNORM(clrRED,"ERROR:") << "vjInputManager::RemoveProxyAlias: Alias: " << alias_name.c_str()
                 << "  cannot find proxy: " << proxy_name.c_str() << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   mProxyAliases.erase(alias_name);

   vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_CONFIG_LVL) << "   alias:" << alias_name.c_str() << "   index:"
                 << mProxyAliases[proxy_name] << "  has been removed." << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_END(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << std::endl << vprDEBUG_FLUSH;

   return true;
}


//: Adds a Proxy alias to the alias list
// PRE: none - BUT: Preferable if proxyName is for a valid proxy
// POST: Alias list has alias str refering to proxyName
void InputManager::addProxyAlias(std::string alias_name, std::string proxy_name)
{
   vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "Proxy alias [" << alias_name.c_str() << "] added.\n" << vprDEBUG_FLUSH;
   vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_VERB_LVL)   << "   proxy:" << proxy_name << std::endl << vprDEBUG_FLUSH;

   mProxyAliases[alias_name] = proxy_name;
}


//: Add a proxy to the proxy table
//! RETURN: true - added correctly
bool InputManager::addProxy(std::string proxyName, Proxy* proxy)
{
   // Check if already in the table
   if(mProxyTable.end() != mProxyTable.find(proxyName))     // Found it
   {
      vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_WARNING_LVL) << "Trying to add proxy: "
                             << proxyName << " but it is already in the proxy table.\n" << vprDEBUG_FLUSH;
      return false;
   }

   mProxyTable[proxyName] = proxy;
   return true;
}

//: Gets index to the proxy/alias named by str
// PRE: none
// POST: none
// RET: NULL - Not found
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
   vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_STATE_LVL) << "vjInputManager::refreshAppProxies: Refreshing all....\n" << vprDEBUG_FLUSH;

   for(std::map<std::string, Proxy*>::iterator i = mProxyTable.begin(); i != mProxyTable.end(); i++)
   {
      (*i).second->refresh();
   }

}

bool InputManager::removeProxy(std::string proxyName)
{
   vprDEBUG_BEGIN(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "vjInputManager::removeProxy" << std::endl << vprDEBUG_FLUSH;

   if(mProxyTable.end() == mProxyTable.find(proxyName))
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
                 << clrOutNORM(clrRED,"ERROR:") << "vjInputManager::removeProxy: proxy: " << proxyName.c_str()
                 << "  cannot find proxy: " << proxyName.c_str() << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   mProxyTable.erase(proxyName);

   vprDEBUG(vrjDBG_INPUT_MGR,vprDBG_CONFIG_LVL) << "   proxy:" << proxyName.c_str() << "  has been removed."\
                                             << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_END(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << std::endl << vprDEBUG_FLUSH;
   return true;
}

bool InputManager::removeProxy(jccl::ConfigChunk* chunk)
{
   std::string proxy_name;
   proxy_name = (std::string)chunk->getProperty("name");
   return removeProxy(proxy_name);
}


};
