/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#ifndef _GADGET_DEVICE_INTERFACE_H_
#define _GADGET_DEVICE_INTERFACE_H_

#include <gadget/gadgetConfig.h>
#include <vector>
#include <gadget/Util/Debug.h>
#include <gadget/Type/ProxyPtr.h>

#include <boost/concept_check.hpp>

namespace gadget
{

/** \class BaseDeviceInterface DeviceInterface.h gadget/Type/DeviceInterface.h
 *
 * Base class for simplified proxy interfaces.  Device interfaces are wrappers
 * that provide an easier way to access proxy objects from within user
 * applications.  Users can simply declare a local interface variable and use
 * it as a smart pointer for the proxy.
 *
 * @note The init() function should be called in the init() function of the
 *       user application.
 */
class GADGET_API BaseDeviceInterface
{
public:
   BaseDeviceInterface();

   virtual ~BaseDeviceInterface();

   /** Copy constructor. */
   BaseDeviceInterface(const BaseDeviceInterface& other);

   /**
    * Initializes the object.
    *
    * @param proxyName String name of the proxy to connect to.
    */
   void init(const std::string& proxyName);

   /**
    * Refreshes the interface based on the current configuration.
    *
    * This method is called by refreshAllInterfaces when system
    * @post (mProxyIndex == -1) ==> Proxy not initialized yet.<br>
    *       (mProxyIndex != -1) ==> mProxyName has name of device && local
    *       proxy pointer is set to the device.
    */
   virtual void refresh();

   /** Returns the name of the proxy. */
   const std::string& getProxyName() const
   {
      return mProxyName;
   }

   /** Identifies whether this device interface is connected to a proxy. */
   bool isConnected() const
   {
      return (NULL != mProxyPtr.get());
   }

protected:
   ProxyPtr    mProxyPtr;   /**<  Ptr to the proxy */
   std::string mProxyName;  /**< The name of the proxy (or alias) we are looking at */
   bool        mNameSet;    /**< Has the user set a name?? */

public:
   /**
    * Refreshes all the known device interface objects.
    *
    * @since 1.1.20
    *
    * @note Prior to version 1.1.20, this function was called
    *       refreshAllDevices().
    */
   static void refreshAllInterfaces();

private:    // Static information
   /* We need to keep track of all the allocated device interfaces
    * so we can update them when the system reconfigures itself
    */
   static void addDevInterface(BaseDeviceInterface* dev);
   static void removeDevInterface(BaseDeviceInterface* dev);

   static std::vector<BaseDeviceInterface*> mAllocatedDevices;
};


// ---- Type-specific interfaces ----

/** \class DeviceInterface DeviceInterface.h gadget/Type/DeviceInterface.h
 *
 * Type-specific device interface.
 */
template<typename ProxyType>
class DeviceInterface : public BaseDeviceInterface
{
public:
   typedef ProxyType                     proxy_type;
   typedef boost::shared_ptr<proxy_type> proxy_ptr_type;

   DeviceInterface()
      : BaseDeviceInterface()
      , mDummyProxy(ProxyType::create())
      , mTypeSpecificProxy(mDummyProxy)
   {
      /* Do nothing. */ ;
   }

   DeviceInterface(const DeviceInterface& other)
      : BaseDeviceInterface(other)
      , mDummyProxy(ProxyType::create())
   {
      if (NULL != other.mTypeSpecificProxy.get())
      {
         mTypeSpecificProxy = other.mTypeSpecificProxy;
      }
      else
      {
         mTypeSpecificProxy = mDummyProxy;
      }
   }

   /**
    * @name Smart pointer operator overloads.
    *
    * Device interfaces make use of the Smart Pointer design pattern.  Access
    * to the contained device proxy must occur through one of these operators.
    */
   //@{
   /**
    * Member selection (via pointer) operator overload.
    *
    * @pre init() has been invoked.
    *
    * @see init()
    */
   const proxy_ptr_type& operator->() const
   {
      return mTypeSpecificProxy;
   }

   /**
    * Dereference operator overload.
    *
    * @pre init() has been invoked.
    *
    * @see init()
    */
   proxy_type& operator*()
   {
      return *mTypeSpecificProxy;
   }
   //@}

   /** Returns the underlying proxy to which we are connected. */
   const proxy_ptr_type& getProxy() const
   {
      return mTypeSpecificProxy;
   }

   /** Sets the proxy to an explicit proxy. */
   void setProxy(const proxy_ptr_type& proxy)
   {
      vprASSERT(NULL != proxy.get());
      mProxyName = proxy->getName();    // Set the name
      mNameSet = true;
      this->refresh();

      // Verify that we found the correct proxy.
      vprASSERT(mTypeSpecificProxy == proxy &&
                "Found incorrect proxy for dev interface");
   }

   virtual void refresh()
   {
      BaseDeviceInterface::refresh();
      if (NULL!= mProxyPtr.get())
      {
         mTypeSpecificProxy = boost::dynamic_pointer_cast<ProxyType>(mProxyPtr);
         if (NULL == mTypeSpecificProxy.get())
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "[gadget::DeviceInterface::refresh()] Tried to point at "
               << "proxy of incorrect type named '" << mProxyName << "'"
               << " it was type:" << typeid(mProxyPtr).name()
               << std::endl << vprDEBUG_FLUSH;
         }
      }

      // If either one of the proxy pointers are NULL, then use a dummy
      if (NULL == mProxyPtr.get() || NULL == mTypeSpecificProxy.get())
      {
         mTypeSpecificProxy = mDummyProxy;
      }
   }

private:
   proxy_ptr_type mDummyProxy;
   proxy_ptr_type mTypeSpecificProxy;   /**< The proxy that is being wrapped */
};

/**
 * @name Convenience Types
 */
//@{
typedef DeviceInterface<class AnalogProxy>        AnalogInterface;
typedef DeviceInterface<class CommandProxy>       CommandInterface;
typedef DeviceInterface<class DigitalProxy>       DigitalInterface;
typedef DeviceInterface<class GestureProxy>       GestureInterface;
typedef DeviceInterface<class GloveProxy>         GloveInterface;
typedef DeviceInterface<class KeyboardMouseProxy> KeyboardMouseInterface;
typedef DeviceInterface<class PositionProxy>      PositionInterface;
typedef DeviceInterface<class RumbleProxy>        RumbleInterface;
typedef DeviceInterface<class StringProxy>        StringInterface;
//@}

} // end namespace


#endif /* _GADGET_DEVICE_INTERFACE_H_ */
