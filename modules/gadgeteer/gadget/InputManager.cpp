/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <iomanip>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/DynLoad/LibraryFinder.h>
#include <vpr/DynLoad/LibraryLoader.h>
#include <vpr/Util/FileUtils.h>

#include <jccl/Config/ConfigElement.h>
#include <jccl/RTRC/ConfigManager.h>

#include <cluster/ClusterManager.h>
#include <gadget/InputLogger.h>
#include <gadget/ProxyFactory.h>
#include <gadget/Type/BaseTypeFactory.h>
#include <gadget/Type/DeviceFactory.h>
#include <gadget/Type/Proxy.h>
#include <gadget/Type/DeviceInterface.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <gadget/InputManager.h>


namespace fs = boost::filesystem;

namespace gadget
{

   vprSingletonImp( InputManager );    // Implementation of singleton

// Local helpers
static bool recognizeProxyAlias(jccl::ConfigElementPtr element);

/**
 * InputManager Constructor
 */
InputManager::InputManager()
{
}

/**
 * InputManager Destructor
 */
InputManager::~InputManager()
{
   for (tDevTableType::iterator a = mDevTable.begin(); a != mDevTable.end(); ++a)    // Stop all devices
   {
      if ((*a).second != NULL)
      {
         (*a).second->stopSampling();
         delete (*a).second;
      }
   }

   // Delete all the proxies
   for ( std::map<std::string, Proxy*>::iterator j = mProxyTable.begin();
         j != mProxyTable.end();
         ++j )
   {
      delete (*j).second;
   }
}

/**
 * This struct implements a callable object (a functor, basically).  An
 * instance can be passed in where a boost::function1<bool, void*> is expected.
 * In gadget::InputManager::configAdd(), instances are used to handle version
 * checking of device driver plug-ins via vpr::LibraryLoader.
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
    *             loaded device driver.  This must be cast to the correct
    *             signature before being invoked.
    */
   bool operator()(void* func)
   {
      vpr::Uint32 (*version_func)();
      version_func = (vpr::Uint32 (*)()) func;

      // Call the entry point function, which, in this case, returns the
      // version of Gadgeteer against which the driver was compiled.
      vpr::Uint32 driver_gadget_ver = (*version_func)();

      bool match = (driver_gadget_ver == mGadgetVersion);

      if ( ! match )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Gadgeteer version mismatch!\n" << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << "Driver was compiled against Gadgeteer version "
            << driver_gadget_ver << ",\n" << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << "but this is Gadgeteer version " << mGadgetVersion
            << std::endl << vprDEBUG_FLUSH;
      }

      return match;
   }

   static const vpr::Uint32 mGadgetVersion;
};

const vpr::Uint32 VersionCheckCallable::mGadgetVersion(__GADGET_version);

/**
 * This struct implements a callable object (a functor, basically).  An
 * instance can be passed in where a boost::function1<bool, void*> is expected.
 * In gadget::InputManager::configAdd(), instances are used to handle dynamic
 * loading of device drivers via vpr::LibraryLoader.
 */
struct DriverInitCallable
{
   DriverInitCallable(gadget::InputManager* inputMgr) : mgr(inputMgr)
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
   else if (element->getID() == std::string( "input_window" ))
   {
      // Get the KeyboardMouseDevice that this window should send events to.
      std::string keyboard_mouse_device_name =
         element->getProperty<std::string>( "keyboard_mouse_device_name" );

      // Get the KeyboardMouseDevice ConfigElement.
      // NOTE: We can assume that this config element is in the active
      //       list since the DependancyManager requires that all
      //       ConfigElementPointers be resolved first.
      jccl::ConfigManager* cfg_mgr = jccl::ConfigManager::instance();
      cfg_mgr->lockActive();
      jccl::ConfigElementPtr keyboard_mouse_element
         = cfg_mgr->getActiveConfig()->get( keyboard_mouse_device_name );
      cfg_mgr->unlockActive();

      // If a KeyboardMouseDevice with this name exists and is remote, we do not want
      // to start this device and open a window since this KeyboardMouseDevice will
      // be invalid.
      if (NULL != keyboard_mouse_element.get() &&
         cluster::ClusterManager::instance()->recognizeRemoteDeviceConfig( keyboard_mouse_element ))
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "InputManager::configAdd() InputWindow not being opened "
            << "since it points to a remote KeyboardMouseDevice."
            << vprDEBUG_FLUSH;
         ret_val = true;
      }
      else
      {
         ret_val = configureDevice( element );
      }
   }
   else if(DeviceFactory::instance()->recognizeDevice(element))
   {
      ret_val = configureDevice(element);
   }
   else if(ProxyFactory::instance()->recognizeProxy(element))
   {
      ret_val = configureProxy(element);
   }
   else if(recognizeProxyAlias(element))
   {
      ret_val = configureProxyAlias(element);
   }
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
      ret_val = configureInputManager(element);
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
      updateAllDevices();                             // Update all the input data
      updateAllProxies();                             // Update all the input data
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
   {
      ret_val = removeDevice(element);
   }
   else if(recognizeProxyAlias(element))
   {
      ret_val = removeProxyAlias(element);
   }
   else if(ProxyFactory::instance()->recognizeProxy(element))
   {
      ret_val = removeProxy(element);
   }
   else if(element->getID() == std::string("display_system"))
   {
      mDisplaySystemElement.reset();  // Keep track of the display system element
      ret_val = true;                 // We successfully configured.
                                       // This tell processPending to remove it to the active config
   }
   else
   {
      ret_val = false;
   }

   if(ret_val)
   {
      updateAllDevices();                             // Update all the input data
      updateAllProxies();                             // Update all the input data
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
         for( i = cfg_mgr->getActiveBegin(); i != cfg_mgr->getActiveEnd(); ++i)
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

/**
 * Check if the device factory or proxy factory can handle the element.
 */
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

/**
 * Check if the device factory or proxy factory can handle the element.
 */
bool InputManager::configureProxy(jccl::ConfigElementPtr element)
{
   std::string proxy_name = element->getFullName();

vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                                 std::string("gadget::InputManager::configureProxy: Named: ") + proxy_name + std::string("\n"),
                                 std::string("done configuring proxy\n"));

   Proxy* new_proxy;

   // Tell the factory to load the proxy
   // NOTE: The config for the proxy registers it with the input manager
   new_proxy = ProxyFactory::instance()->loadProxy(element);

   // Check for success
   if(NULL == new_proxy)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " gadget::InputManager::configureProxy: Proxy construction failed:"
         << proxy_name << std::endl << vprDEBUG_FLUSH;
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


/**
 * Dump the current Status of the InputManager, listing all
 * the devices, proxies and internal settings
 */
GADGET_IMPLEMENT(std::ostream&) operator<<(std::ostream& out, InputManager& iMgr)
{
   out << "\n========== Input Manager Status ==========" << std::endl;
   out << "Device List:\n";

   // Dump DEVICES
   for ( InputManager::tDevTableType::iterator i = iMgr.mDevTable.begin();
         i != iMgr.mDevTable.end();
         ++i)
   {
      if ((*i).second != NULL)
      {
         out << "    '" << i->first << "' (type: "
             << typeid(*(i->second)).name() << ")" << std::endl;
      }
   }

   out << "\nProxy List:\n";
   for (std::map<std::string, Proxy*>::iterator i_p = iMgr.mProxyTable.begin();
        i_p != iMgr.mProxyTable.end();
        ++i_p)
   {
      out << "    '" << (*i_p).second->getName() << "' refers to ";
      if(NULL != ((*i_p).second->getProxiedInputDevice()))
      {
         out << ((*i_p).second->getProxiedInputDevice())->getInstanceName();
      }
      else
      {
         out << "None (internal dummy)";
      }
      out << std::endl;
   }

   out << std::endl;

   // Dump Alias list
   out << "Alias List:" << std::endl;
   for ( std::map<std::string, std::string>::iterator cur_alias = iMgr.mProxyAliases.begin();
         cur_alias != iMgr.mProxyAliases.end();
         ++cur_alias )
   {
      out << "    '" << (*cur_alias).first.c_str() << "' (alias for "
          << (*cur_alias).second << ")" << std::endl;
   }

   out << "========== InputManager Status ==========" << std::endl;
   return out;
}

/**
 * Add a device to the InputManager, returns the index
 * where the device was placed
 */
bool InputManager::addDevice(Input* devPtr)
{
   mDevTable[devPtr->getInstanceName()] = devPtr;

   refreshAllProxies();

   return true;
}

/**
 *   Add a remote device to the InputManager that is being updated by a RemoteInputManager.
 */
bool InputManager::addRemoteDevice(Input* devPtr, const std::string& device_name)
{
   mDevTable[device_name] = devPtr;

   refreshAllProxies();

   return true;
}

void InputManager::updateAllProxies()
{
   // Update proxies MIGHT NOT NEED
   for (std::map<std::string, Proxy*>::iterator i_p = mProxyTable.begin();
        i_p != mProxyTable.end();
        ++i_p)
   {
      (*i_p).second->updateData();
   }
}


/**
 * Call UpdateData() on all the devices and transform proxies.
 */
void InputManager::updateAllDevices()
{
   for (tDevTableType::iterator i = mDevTable.begin(); i != mDevTable.end(); ++i)      // all DEVICES
   {
      if ((*i).second != NULL)
      {
         i->second->updateData();
      }
   }

   // Update Logger - Done here so that device can be "rewritten" by logger
   //                 before rim or proxies get their hands on the data
   if(mInputLogger.get() != NULL)
   {
      mInputLogger->process();
   }
}

/**
 * Return a Input ptr to a deviced named
 *
 * @returns - NULL if not found.
 */
Input* InputManager::getDevice(const std::string& deviceName)
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

/**
 * Remove the device that is pointed to by devPtr.
 */
bool InputManager::removeDevice(const Input* devPtr)
{
   for (tDevTableType::iterator i = mDevTable.begin(); i != mDevTable.end(); ++i)      // all DEVICES
   {
      if ((*i).second == devPtr)
      {
         return removeDevice((*i).first);
      }
   }

   return false;
}


/**
 * InputManager remove instName from the InputManager,
 * currently stupefies all the proxies connected to it.
 */
bool InputManager::removeDevice(const std::string& instName)
{
   tDevTableType::iterator dev_found;
   dev_found = mDevTable.find(instName);
   if(dev_found == mDevTable.end())
   {
      return false;
   }

   Input* dev_ptr = dev_found->second;

   if(NULL == dev_ptr)
   {
      return false;
   }

   // Find any proxies connected to the device
   // Stupefy any proxies connected to device
   // NOTE: Could just remove it and then refresh all, but this is a little safer
   //       since we explicitly stupefy the one that we don't want anymore
   for ( std::map<std::string, Proxy*>::iterator i_p = mProxyTable.begin();
        i_p != mProxyTable.end();
        ++i_p )
   {
      if((*i_p).second->getProxiedInputDevice() == dev_ptr)
      {
         (*i_p).second->stupefy(true);
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

bool InputManager::configureInputManager(jccl::ConfigElementPtr element)
{
   bool have_bad_elt = (element->getID() != std::string("input_manager"));
   vprASSERT(!have_bad_elt);
   if(have_bad_elt)
   {
      return false;
   }

   bool ret_val = false;

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
         << ": [gadget::InputManager::configureInputManager()] Element named '"
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
      std::vector<fs::path> search_path(path_count);

      for ( unsigned int i = 0; i < search_path.size(); ++i )
      {
         std::string temp_str =
            vpr::replaceEnvVars(element->getProperty<std::string>(driver_path_prop_name, i));

         try
         {
            search_path[i] = fs::path(temp_str, fs::native);
         }
         catch(fs::filesystem_error& fsEx)
         {
            vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
               << clrOutNORM(clrRED, "ERROR")
               << ": [gadget::InputManager::configureInputManager()] File "
               << "system exception caught while converting\n"
               << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
               << "'" << temp_str << "'\n" << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
               << "to a Boost.Filesystem path.\n" << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
               << fsEx.what() << std::endl << vprDEBUG_FLUSH;
         }
      }

      // Append a default driver search path to search_path.
      const std::string gadget_base_dir("GADGET_BASE_DIR");
      const std::string vj_base_dir("VJ_BASE_DIR");
      std::string base_dir;

      // Try get to the value of $GADGET_BASE_DIR first.  If that fails,
      // fall back on $VJ_BASE_DIR.
      if ( ! vpr::System::getenv(gadget_base_dir, base_dir).success() )
      {
         if ( ! vpr::System::getenv(vj_base_dir, base_dir).success() )
         {
            // If neither $GADGET_BASE_DIR nor $VJ_BASE_DIR is set, then
            // we can append a default driver search path.
            base_dir = VJ_ROOT_DIR;
         }
      }

#if defined(VPR_OS_IRIX) && defined(_ABIN32)
      const std::string bit_suffix("32");
#elif defined(VPR_OS_IRIX) && defined(_ABI64)
      const std::string bit_suffix("64");
#else
      const std::string bit_suffix("");
#endif

      fs::path default_search_dir =
         fs::path(base_dir, fs::native) /
            (std::string("lib") + bit_suffix) /
            std::string("gadgeteer") / std::string("drivers");

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
         << "[gadget::InputManager::configureInputManager()] Appending "
         << "default search path '"
         << default_search_dir.native_directory_string() << "'\n"
         << vprDEBUG_FLUSH;

      search_path.push_back(default_search_dir);

      // --- Load device driver dsos -- //
      // - Load individual drivers
      const std::string driver_prop_name("driver");
      const std::string get_version_func("getGadgeteerVersion");
      const std::string driver_init_func("initDevice");

      int driver_count = element->getNum(driver_prop_name);
      std::string driver_dso_name;

      for ( int i = 0; i < driver_count; ++i )
      {
         driver_dso_name =
            element->getProperty<std::string>(driver_prop_name, i);

         if ( ! driver_dso_name.empty() )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
               << "[gadget::InputManager::configureInputManager()] Loading "
               << "driver DSO '" << driver_dso_name << "'\n" << vprDEBUG_FLUSH;

            vpr::LibraryPtr dso = vpr::LibraryLoader::findDSO(driver_dso_name,
                                                              search_path);

            if ( dso.get() != NULL )
            {
               vpr::ReturnStatus version_status;
               VersionCheckCallable version_functor;
               version_status =
                  vpr::LibraryLoader::callEntryPoint(dso, get_version_func,
                                                     version_functor);

               if ( ! version_status.success() )
               {
                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                     << clrOutBOLD(clrRED, "ERROR")
                     << ": Version mismatch while loading driver DSO '"
                     << driver_dso_name << "'\n" << vprDEBUG_FLUSH;
                  vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                     << "This driver will not be usable.\n"
                     << vprDEBUG_FLUSH;
               }
               else
               {
                  vpr::ReturnStatus load_status;
                  DriverInitCallable init_functor(this);
                  load_status =
                     vpr::LibraryLoader::callEntryPoint(dso, driver_init_func,
                                                        init_functor);

                  if ( load_status.success() )
                  {
                     mLoadedDrivers.push_back(dso);
                  }
               }
            }
            else
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                  << clrOutBOLD(clrRED, "ERROR")
                  << ": Failed to find driver DSO '" << driver_dso_name
                  << "'\n" << vprDEBUG_FLUSH;
            }
         }
      }

      // - Load driver directory
      const std::string dir_prop_name("driver_scan_path");
      int dir_count = element->getNum(dir_prop_name);
      std::string driver_dir;

#if defined(VPR_OS_Windows)
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
            fs::path drv_path(driver_dir, fs::native);

            if ( fs::exists(drv_path) )
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                  << "[gadget::InputManager::configureInputManager()] "
                  << "Searching for driver DSOs in '" << driver_dir << "'\n"
                  << vprDEBUG_FLUSH;

               vpr::LibraryFinder finder(driver_dir, driver_ext);
               vpr::LibraryFinder::LibraryList libs = finder.getLibraries();
               DriverInitCallable functor(this);

               for ( vpr::LibraryFinder::LibraryList::iterator lib = libs.begin();
                     lib != libs.end();
                     ++lib )
               {
                  vpr::ReturnStatus load_status;
                  load_status =
                     vpr::LibraryLoader::callEntryPoint(*lib, driver_init_func,
                                                        functor);

                  if ( load_status.success() )
                  {
                     mLoadedDrivers.push_back(*lib);
                  }
               }
            }
            else
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                  << clrOutBOLD(clrYELLOW, "WARNING")
                  << ": [gadget::InputManager::configureInputManager()] "
                  << "Invalid directory for driver DSOs: " << driver_dir
                  << std::endl << vprDEBUG_FLUSH;
            }
         }
         catch(fs::filesystem_error& fsEx)
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << clrOutNORM(clrRED, "ERROR")
               << ": [gadget::InputManager::configureInputManager()] File "
               << "system exception caught!\n" << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << fsEx.what() << std::endl << vprDEBUG_FLUSH;
         }
      }

      ret_val = true;
   }

   return ret_val;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   PRIVATE FUNCTIONS TO INPUTGROUP
* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/**
 * Is it a proxy alias.
 */
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
                           std::string("gadget::InputManager: Configuring proxy alias\n"),
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
                        std::string("gadget::InputManager::removeProxyAlias\n"),
                        std::string("...done removing alias.\n"));

   vprASSERT(element->getID() == "alias");

   std::string alias_name, proxy_name;  // The string of the alias, name of proxy to pt to

   alias_name = element->getName();

   if(mProxyAliases.end() == mProxyAliases.find(alias_name))
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "gadget::InputManager::RemoveProxyAlias: Alias: " << alias_name
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
void InputManager::addProxyAlias(const std::string& aliasName,
                                 const std::string& proxyName)
{
   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_STATE_LVL)
      << "Proxy alias [" << aliasName << "] added.\n"
      << vprDEBUG_FLUSH;
   vprDEBUG(gadgetDBG_INPUT_MGR,vprDBG_VERB_LVL)
      << "   proxy:" << proxyName << std::endl << vprDEBUG_FLUSH;

   mProxyAliases[aliasName] = proxyName;
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
Proxy* InputManager::getProxy(const std::string& proxyName)
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

/**
 * Refresh all the proxies to have then update what device they are pointing at.
 */
void InputManager::refreshAllProxies()
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "gadget::InputManager::refreshAppProxies: Refreshing all...\n"
      << vprDEBUG_FLUSH;

   for ( std::map<std::string, Proxy*>::iterator i = mProxyTable.begin();
         i != mProxyTable.end();
         ++i )
   {
      (*i).second->refresh();
   }
}

bool InputManager::removeProxy(const std::string& proxyName)
{
   vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                  std::string("gadget::InputManager::removeProxy\n"),
                  std::string("\n"));
   if(mProxyTable.end() == mProxyTable.find(proxyName))
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << "gadget::InputManager::removeProxy: proxy: " << proxyName
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

/**
 * Get the input logger connected to the system.
 */
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
