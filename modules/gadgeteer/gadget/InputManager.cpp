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

#include <gadget/Type/Proxy.h>
#include <gadget/Type/DeviceFactory.h>
#include <gadget/ProxyFactory.h>
#include <gadget/Type/DeviceInterface.h>

#include <gadget/Util/Debug.h>

#include <jccl/Config/ConfigElement.h>
#include <jccl/RTRC/ConfigManager.h>

#include <cluster/ClusterManager.h>
#include <gadget/Type/BaseTypeFactory.h>
#include <gadget/InputLogger.h>

#include <gadget/InputManager.h> // my header...


namespace gadget
{

   vprSingletonImp( InputManager );    // Implementation of singleton

// Local helpers
static bool recognizeProxyAlias(jccl::ConfigElementPtr element);

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
   {
      if ((*a).second != NULL)
      {
         (*a).second->stopSampling();
         delete (*a).second;
      }
   }

   // Delete all the proxies
   for(std::map<std::string, Proxy*>::iterator j = mProxyTable.begin(); j != mProxyTable.end(); j++)
   {
      delete (*j).second;
   }
}

/**
 * This struct implements a callable object (a functor, basically).  An
 * instance can be passed in where a boost::function1<bool, void*> is expected.
 * In gadget::InputManager::configAdd(), instances are used to handle dynamic
 * loading of device drivers via vpr::LibraryLoader.
 */
struct Callable
{
   Callable(gadget::InputManager* inputMgr) : mgr(inputMgr)
   {
   }

   /**
    * This will be invoked as a callback by methods of vpr::LibraryLoader.
    *
    * @param func A function pointer for the entry point in a dynamically
    *             loaded device driver.  This must be cast to the correct
    *             signature before being invoked.
    */
   bool operator()(void* func)
   {
      void (*init_func)(InputManager*);

      // Cast the entry point function to the correct signature so that we can
      // call it.  All dynamically loaded drivers must have an entry point
      // function that takes a pointer to a gadget::InputManager instance and
      // returns nothing.
      init_func = (void (*)(InputManager*)) func;

      // Call the entry point function.
      (*init_func)(mgr);

      return true;
   }

   gadget::InputManager* mgr;
};

/** Adds the given config element to the input system. */
bool InputManager::configAdd(jccl::ConfigElementPtr element)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                              std::string("Input Manager: Adding pending config element.\n"),
                              std::string("...done adding element.\n"));

   vprASSERT(configCanHandle(element));

   bool ret_val = false;      // Flag to return success
   
   if (cluster::ClusterManager::instance()->recognizeRemoteDeviceConfig(element))
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "InputManager can not handle remote devices, we must use Remote Input Manager."
         << vprDEBUG_FLUSH;
      ret_val = false;
   }
   else if(DeviceFactory::instance()->recognizeDevice(element))
      ret_val = configureDevice(element);
   else if(ProxyFactory::instance()->recognizeProxy(element))
      ret_val = configureProxy(element);
   else if(recognizeProxyAlias(element))
      ret_val = configureProxyAlias(element);
   else if(element->getID() == std::string("display_system"))
   {
      // XXX: Put signal here to tell draw manager to lookup new stuff
      mDisplaySystemElement = element;     // Keep track of the display system element
      ret_val = true;
   }
   else if(element->getID() == std::string("gadget_logger"))
   {
      ret_val = configureInputLogger(element);
   }
   else if(element->getID() == std::string("input_manager"))
   {
      vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                                       std::string("Handling input_manager element:\n"),
                                       std::string("-- end state -- \n"));

      // Keep this up to date with the version of the element definition we're
      // expecting to handle.
      const unsigned int cur_version(2);

      // If the element version is less than cur_version, we will not try to
      // proceed.  Instead, we'll print an error message and return false so
      // that the Config Manager knows this element wasn't consumed.
      if ( element->getVersion() < cur_version )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR")
            << " [gadget::InputManager::configAdd()]: Element named '"
            << element->getName() << "'" << std::endl << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "is version " << element->getVersion()
            << ", but we require at least version " << cur_version
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "Ignoring this element and moving on." << std::endl
            << vprDEBUG_FLUSH;
         ret_val = false;
      }
      // We got the right version of the config element and can proceed.
      else
      {
         const std::string driver_path_prop_name("driver_path");
         const int path_count(element->getNum(driver_path_prop_name));
         std::vector<std::string> search_path(path_count);

         for ( unsigned int i = 0; i < search_path.size(); ++i )
         {
            search_path[i] =
               vpr::replaceEnvVars(element->getProperty<std::string>(driver_path_prop_name, i));
         }

         // --- Load device driver dsos -- //
         // - Load individual drivers
         const std::string driver_prop_name("driver");
         const std::string driver_init_func("initDevice");
         int driver_count = element->getNum(driver_prop_name);
         std::string driver_dso;

         for ( int i = 0; i < driver_count; ++i )
         {
            driver_dso = element->getProperty<std::string>(driver_prop_name, i);

            if ( ! driver_dso.empty() )
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                  << "InputMgr::config: Loading driver DSO '" << driver_dso
                  << "'\n" << vprDEBUG_FLUSH;

               Callable functor(this);
               mDriverLoader.findAndInitDSO(driver_dso, search_path,
                                            driver_init_func, functor);
            }
         }

         // - Load driver directory
         const std::string dir_prop_name("driver_scan_path");
         int dir_count = element->getNum(dir_prop_name);
         std::string driver_dir;

#if defined(VPR_OS_Win32)
         const std::string driver_ext("dll");
#elif defined(VPR_OS_Darwin)
         const std::string driver_ext("dylib");
#else
         const std::string driver_ext("so");
#endif

         for ( int i = 0; i < dir_count; ++i )
         {
            driver_dir = vpr::replaceEnvVars(element->getProperty<std::string>(dir_prop_name, i));

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
                  Callable functor(this);

                  for ( vpr::LibraryFinder::LibraryList::iterator lib = libs.begin();
                        lib != libs.end();
                        ++lib )
                  {
                     mDriverLoader.loadAndInitDSO(*lib, driver_init_func, functor);
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
 * Removes the element from the current configuration.
 */
bool InputManager::configRemove(jccl::ConfigElementPtr element)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                              std::string("InputManager: Removing config...\n"),
                              std::string("done removing config.\n"));
   vprASSERT(configCanHandle(element));

   bool ret_val = false;      // Flag to return success

   // NEED TO FIX!!!!
   if (cluster::ClusterManager::instance()->recognizeRemoteDeviceConfig(element))
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "InputManager can not handle remote devices, we must use Remote Input Manager."
         << vprDEBUG_FLUSH;
      ret_val = false;
   }
   else if(DeviceFactory::instance()->recognizeDevice(element))
      ret_val = removeDevice(element);
   else if(recognizeProxyAlias(element))
      ret_val = removeProxyAlias(element);
   else if(ProxyFactory::instance()->recognizeProxy(element))
      ret_val = removeProxy(element);
   else if(element->getID() == std::string("display_system"))
   {
      mDisplaySystemElement.reset();  // Keep track of the display system element
      ret_val = true;                 // We successfully configured.
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
bool InputManager::configCanHandle(jccl::ConfigElementPtr element)
{           // NEED TO FIX!!!!
   return ( (DeviceFactory::instance()->recognizeDevice(element) && 
             !cluster::ClusterManager::instance()->recognizeRemoteDeviceConfig(element)) ||
            ProxyFactory::instance()->recognizeProxy(element) ||
            recognizeProxyAlias(element) ||
            (element->getID() == std::string("display_system")) ||
            (element->getID() == std::string("input_manager")) ||
            (element->getID() == std::string("gadget_logger"))
          );
}

jccl::ConfigElementPtr InputManager::getDisplaySystemElement()
{
   if ( mDisplaySystemElement.get() == NULL )
   {
      jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();

      cfg_mgr->lockActive();
      {
         std::vector<jccl::ConfigElementPtr>::iterator i;
         for(i=cfg_mgr->getActiveBegin(); i != cfg_mgr->getActiveEnd();++i)
         {
            if((*i)->getID() == std::string("display_system"))
            {
               mDisplaySystemElement = *i;
               break;         // This guarantees that we get the first displaySystem element.
            }
         }
      }
      cfg_mgr->unlockActive();
      //vprASSERT(mDisplaySystemElement.get() != NULL && "No Display Manager element found!");
   }
   return mDisplaySystemElement;
}

// Check if the device factory or proxy factory can handle the element.
bool InputManager::configureDevice(jccl::ConfigElementPtr element)
{
   bool ret_val;
   std::string dev_name = element->getFullName();

   vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                                 std::string("InputManager::configureDevice: device[") + dev_name + std::string("]\n"),
                                 std::string("done configuring device\n"));

   Input* new_device;
   new_device = DeviceFactory::instance()->loadDevice(element);

   if ((new_device != NULL) && (new_device->startSampling()))
   {
      addDevice(new_device);
      ret_val = true;
      vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
         << "   Successfully added device: " << dev_name << std::endl
         << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:")
         << "New device " << clrSetBOLD(clrCYAN) << dev_name << clrRESET
         << " failed to start.  Deleting instance" << std::endl
         << vprDEBUG_FLUSH;
      if ( NULL != new_device )
      {
         delete new_device;
      }

      ret_val = false;
   }

   return ret_val;
}

// Check if the device factory or proxy factory can handle the element.
bool InputManager::configureProxy(jccl::ConfigElementPtr element)
{
   std::string proxy_name = element->getFullName();

vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                                 std::string("vjInputManager::configureProxy: Named: ") + proxy_name + std::string("\n"),
                                 std::string("done configuring proxy\n"));

   Proxy* new_proxy;

   // Tell the factory to load the proxy
   // NOTE: The config for the proxy registers it with the input manager
   new_proxy = ProxyFactory::instance()->loadProxy(element);   

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
 * Removes the device associated with the given element.
 */
bool InputManager::removeDevice(jccl::ConfigElementPtr element)
{
   return removeDevice(element->getFullName());
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
bool recognizeProxyAlias(jccl::ConfigElementPtr element)
{
   return (element->getID() == "alias");
}

/**
 * Configures proxy aliases in config database.
 * @pre none
 * @post (alias not already in list) ==> Alias is added to proxyAlias list<br>
 *       (alias was already is list) ==> Alias is set to point to the new proxy instead
 */
bool InputManager::configureProxyAlias(jccl::ConfigElementPtr element)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                           std::string("vjInputManager::Configuring proxy alias\n"),
                           std::string("...done configuring alias.\n"));

   vprASSERT(element->getID() == "alias");

   std::string alias_name, proxy_name;  // The string of the alias, name of proxy to pt to

   alias_name = element->getName();
   proxy_name = element->getProperty<std::string>("proxy");

   addProxyAlias(alias_name, proxy_name);

   return true;
}

/**
 * Removes a proxy aliases in config database.
 * @pre none
 * @post (alias not in list) ==> returns = false<br>
 *       (alias is in list) ==> (alias is removed from list) returns true
 */
bool InputManager::removeProxyAlias(jccl::ConfigElementPtr element)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                        std::string("vjInputManager::RemoveProxyAlias\n"),
                        std::string("...done removing alias.\n"));

   vprASSERT(element->getID() == "alias");

   std::string alias_name, proxy_name;  // The string of the alias, name of proxy to pt to

   alias_name = element->getName();

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

bool InputManager::removeProxy(jccl::ConfigElementPtr element)
{
   std::string proxy_name;
   proxy_name = element->getFullName();
   return removeProxy(proxy_name);
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


bool InputManager::configureInputLogger(jccl::ConfigElementPtr element)
{
   // Configure it
   return getInputLogger()->config(element);
}

} // End of gadget namespace
