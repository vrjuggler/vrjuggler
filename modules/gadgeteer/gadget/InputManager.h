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


/////////////////////////////////////////////////////////////////////////////
//
// Group manager for vj input devices and proxies
//
////////////////////////////////////////////////////////////////////////////
#ifndef GADGET_INPUT_MANAGER_H
#define GADGET_INPUT_MANAGER_H

#include <gadget/gadgetConfig.h>

#include <vpr/vpr.h>
#include <vpr/DynLoad/LibraryLoader.h>

#include <vpr/Util/Singleton.h>
#include <jccl/RTRC/ConfigElementHandler.h>
#include <gadget/InputLoggerPtr.h>
#include <map>
#include <vector>

#include <boost/smart_ptr.hpp>


namespace gadget
{

// Proxies
class Proxy;
class Input;
class DeviceFactory;

/**
 * The Input Manager holds an manages all Gadgeteer Input devices.
 *
 * The Input Manager, handles all the details behind organizing the input
 * devices in the library.  It provides an API for adding devices by their
 * JCCL config element and deleting them by their element (or just their string
 * name).
 *
 * Proxies are used to abstract away from the devices so any type and number
 * of devices may be accessed in the same way.  These proxies are also
 * set up by config elements and should be accessed by number, rather than name
 * (for speed).
 */
class GADGET_CLASS_API InputManager : public jccl::ConfigElementHandler
{
   vprSingletonHeader( InputManager );    // Make it a singleton

protected:
   /** Constructor is hidden, so no direct instantiation is allowed. */
   InputManager();

   virtual ~InputManager();

   /** Constructor is hidden, so no copying is allowed. */
   InputManager(const gadget::InputManager& obj)
      : jccl::ConfigElementHandler(obj)
   {
      /* Do nothing. */ ;
   }

   void operator=(const gadget::InputManager& k)
   {
      /* Do nothing. */ ;
   }

public:
   friend GADGET_API(std::ostream&) operator<<(std::ostream& out,
                                               InputManager& iMgr);

 //---------------------------//
 //      CONFIG               //
 //---------------------------//
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
    *       (element is device) ==> (devices is removed && proxies are stupified)<br>
    *       (element is proxyAlias) ==> (proxyAlias is removed && devInterfaces.refreshAll())
    * @return success
    */
   bool configRemove(jccl::ConfigElementPtr element);

   /**
    * Can the handler handle the given element?
    *
    * @return true if the handler can handle it, false otherwise.
    */
   bool configCanHandle(jccl::ConfigElementPtr element);

   jccl::ConfigElementPtr getDisplaySystemElement();

// MOVE FOR RIM's USE private:
   /**
    * Loads the device for the given element.
    * @return true if the device was configured and added.
    */
   bool configureDevice(jccl::ConfigElementPtr element);

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


   // ------------------------------- //
   //        DEVICE API               //
   // ------------------------------- //
public:
   /**
    * Updates the data in all the devices.
    * This calls updateData() on all the devices in the device table.
    * It also updates all proxies.
    *
    * @modifies Instances pointed to by device table.
    */
   void updateAllData();

public:
   /**
    * Returns an Input pointer to a named device.
    *
    * @return NULL if the device is not found.
    */
   Input* getDevice(const std::string& deviceName);

   DeviceFactory* getDeviceFactory();

   /**
    * Adds a device to InputManager.
    * Adds the devPtr to the device table.  devPtr should not already be in
    * the array.
    *
    * @modifies self
    * @post mDevTable' = mDevTable \/ devPtr
    *
    * @return false if device addition fails.
    */
   bool addDevice(Input* devPtr);

   /**
    * Adds a remote device to InputManager.
    * Adds the devPtr to the device table.  devPtr should not already be in
    * the array.
    *
    * @modifies self
    * @post mDevTable' = mDevTable \/ devPtr
    *
    * @return false if device addition fails.
    */
   bool addRemoteDevice(Input* devPtr, const std::string& device_name);

public:
   /**
    * Removes a device from the InputManager.
    * Remove the named device from the device table.
    *
    * @modifies self
    * @post mDevTable[devNum]' = NULL
    *
    * @return true on success.
    */
   bool removeDevice(const std::string& mInstName);

   /** Remove a device given a pointer to the device.
   * Internally this just uses removeDevice(dev_name)
   */
   bool removeDevice(const Input* devPtr);

   /*********************************************************
    *          PROXIES                                      *
    *********************************************************/
public:

   /**
    * Adds a proxy to the proxy table.
    * @param proxy - The proxy to add.  It is added with name: proxy->getName()
    * @return true if the proxy was added correctly.
    */
   bool addProxy(Proxy* proxy);

   /**
    * Gets a proxy for the given proxy name (or alias).
    *
    * @return NULL if the proxy was not found.
    */
   Proxy* getProxy(const std::string& proxyName);

   /**
    * Refreshes all the proxies to have them update the device at which they
    * point.
    */
   void refreshAllProxies();

protected:
   bool removeProxy(const std::string& proxyName);
   bool removeProxy(jccl::ConfigElementPtr element);

public:
   /** Get the input logger connected to the system */
   gadget::InputLoggerPtr getInputLogger();

   /* friends */
   friend class InputLogger;  /**< Make input logger a friend */

protected:
   vpr::LibraryLoader mDriverLoader;

   typedef std::map<std::string,Input*> tDevTableType;

   tDevTableType                        mDevTable;
   std::map<std::string, Proxy*>        mProxyTable;    /**< list of proxies in the system */

   /** List of alias names for proxies.
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
