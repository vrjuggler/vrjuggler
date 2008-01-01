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

#ifndef GADGET_INPUT_MANAGER_H
#define GADGET_INPUT_MANAGER_H

#include <gadget/gadgetConfig.h>

#include <map>
#include <vector>
#include <boost/smart_ptr.hpp>

#include <vpr/vpr.h>
#include <vpr/DynLoad/Library.h>
#include <vpr/Util/Singleton.h>

#include <jccl/RTRC/ConfigElementHandler.h>
#include <gadget/InputLoggerPtr.h>
#include <gadget/Type/InputPtr.h>
#include <gadget/Type/ProxyPtr.h>


namespace gadget
{

// Proxies
class DeviceFactory;

/** \class InputManager InputManager.h gadget/InputManager.h
 *
 * The Input Manager holds and manages all Gadgeteer Input devices.
 *
 * The Input Manager handles all the details behind organizing the input
 * devices in the library.  It provides an API for adding devices by their
 * JCCL config element and deleting them by their element (or just their string
 * name).
 *
 * Proxies are used to abstract away from the devices so any type and number
 * of devices may be accessed in the same way.  These proxies are also
 * set up by config elements and should be accessed by number, rather than by
 * name (for speed reasons).
 */
class GADGET_CLASS_API InputManager : public jccl::ConfigElementHandler
{
   vprSingletonHeader( InputManager );    // Make it a singleton

protected:
   /** Constructor is hidden, so no direct instantiation is allowed. */
   InputManager();

   /**
    * Shuts down this Input Manager instance (if it has not already been
    * shut down).
    *
    * @see shutdown()
    */
   virtual ~InputManager();

   /** Constructor is hidden, so no copying is allowed. */
   InputManager(const gadget::InputManager& obj)
      : jccl::ConfigElementHandler(obj)
   {
      /* Do nothing. */ ;
   }

   void operator=(const gadget::InputManager&)
   {
      /* Do nothing. */ ;
   }

public:
   friend GADGET_API(std::ostream&) operator<<(std::ostream& out,
                                               InputManager& iMgr);

      //------------------------//
      //       CONFIG API       //
      //------------------------//

   /**
    * Adds the element to the configuration.
    * @pre configCanHandle(element) == true
    * @return success
    */
   bool configAdd(jccl::ConfigElementPtr element);

   /**
    * Removes the element from the current configuration.
    * @pre configCanHandle(element) == true
    * @post (element is proxy) ==> (returns == false)<br>
    *       (element is device) ==> (device is removed && proxies are stupefied)<br>
    *       (element is proxy_alias) ==> (proxy_alias is removed && devInterfaces.refreshAll())
    * @return success
    */
   bool configRemove(jccl::ConfigElementPtr element);

   /**
    * Can we handle the given element?
    *
    * @return true is returned if we can handle it, false otherwise.
    */
   bool configCanHandle(jccl::ConfigElementPtr element);

   jccl::ConfigElementPtr getDisplaySystemElement();

   /**
    * Loads the device for the given element.
    * @return true if the device was configured and added.
    */
   bool configureDevice(jccl::ConfigElementPtr element);

   /**
    * Shuts down all devices and proxies.
    *
    * @post \c mDevTable is empty. \c mProxyTable is empty.
    *
    * @since 1.1.20
    */
   void shutdown();

private:
   /**
    * Loads the Proxy for the given element.
    * @return true if the proxy was configured and added.
    */
   bool configureProxy(jccl::ConfigElementPtr element);

   /**
    * Removes the device associated with the given element.
    * @return true if the device was removed.
    */
   bool removeDevice(jccl::ConfigElementPtr element);

   /** Configure the InputManager with an input manager element. */
   bool configureInputManager(jccl::ConfigElementPtr element);


public:
   /**
    * @name Device API.
    */
   //@{
   /**
    * @since 1.1.19
    */
   void resetAllDevicesAndProxies();

   /**
    * Updates the data in all the devices.
    * This calls updateData() on all the devices in the device table.
    */
   void updateAllDevices();

   /**
    * Updates all proxies by grabbing the correct data and
    * transforming it using the PositionFilters.
    */
   void updateAllProxies();

   /**
    * Returns a pointer to a gadget::Input object that is the named device.
    *
    * @return NULL if the device is not found.
    */
   InputPtr getDevice(const std::string& deviceName);

   DeviceFactory* getDeviceFactory();

   /**
    * Adds the given device to the Input Manager.
    * Adds the devPtr to the device table.  devPtr should not already be in
    * the array.
    *
    * @post mDevTable' = mDevTable \/ devPtr
    *
    * @return false if device addition fails.
    */
   bool addDevice(InputPtr devPtr);

   /**
    * Adds the given remote device to the Input Manager.
    * Adds the devPtr to the device table.  devPtr should not already be in
    * the array.
    *
    * @post mDevTable' = mDevTable \/ devPtr
    *
    * @return false if device addition fails.
    */
   bool addRemoteDevice(InputPtr devPtr, const std::string& device_name);

   /**
    * Removes the named device from the device table.
    *
    * @post mDevTable[devName]' = NULL
    *
    * @param devName The name of the device to remove.
    *
    * @return true on success.
    */
   bool removeDevice(const std::string& devName);

   /**
    * Removes the device identified by the given gadget::InputPtr.
    * Internally, this just uses removeDevice(const std::string&).
    *
    * @param devPtr The device object to remove.
    */
   bool removeDevice(const InputPtr devPtr);
   //@}

public:

   /**
    * @name Proxy API.
    */
   //@{
   /**
    * Adds the given gadget::Proxy object to the proxy table.
    *
    * @param proxy The proxy to add.  It is added using the name returned
    *              by calling proxy->getName().
    *
    * @return true if the proxy was added correctly.
    */
   bool addProxy(ProxyPtr proxy);

   /**
    * Gets a proxy having the given name (or alias).
    *
    * @param proxyName A string identifier that is a proxy name or alias.
    *
    * @return NULL if the proxy was not found.
    */
   ProxyPtr getProxy(const std::string& proxyName);

   /**
    * Refreshes all the proxies to have them update the device at which they
    * point.
    */
   void refreshAllProxies();
   //@}

protected:
   bool removeProxy(const std::string& proxyName);
   bool removeProxy(jccl::ConfigElementPtr element);

public:
   /** Gets the input logger connected to the Input Manager. */
   gadget::InputLoggerPtr getInputLogger();

   /* friends */
   friend class InputLogger;  /**< Make input logger a friend */

protected:
   std::vector<vpr::LibraryPtr> mLoadedDrivers;

   typedef std::map<std::string, InputPtr> tDevTableType;
   typedef std::map<std::string, ProxyPtr> proxy_map_t;

   tDevTableType        mDevTable;
   proxy_map_t          mProxyTable;    /**< list of proxies in the system */

   /**
    * List of alias names for proxies.
    *
    * The mProxyAlias table serves as a secondary lookup for proxies.  ie. if
    * the proxy name is not found in mProxyTable, then search mProxyAliases
    * for it.
    */
   std::map<std::string, std::string>   mProxyAliases;

   jccl::ConfigElementPtr mDisplaySystemElement; /**< Config element for the displaySystem */

   gadget::InputLoggerPtr mInputLogger;          /**< The input logger for the system. Constructed on demand. */

private:
   /** Function to configure the proxy Alias array. */
   bool configureProxyAlias(jccl::ConfigElementPtr element);

   /** Removes a proxy alias. */
   bool removeProxyAlias(jccl::ConfigElementPtr element);

   /** Adds a proxy alias. */
   void addProxyAlias(const std::string& aliasName,
                      const std::string& proxyName);

   /** Configure/create a logger for the system. */
   bool configureInputLogger(jccl::ConfigElementPtr element);

};

/** Writes out the status of the Input Manager. */
GADGET_API(std::ostream&) operator<<(std::ostream& out, InputManager& iMgr);

} // end namespace

#endif
