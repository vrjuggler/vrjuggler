/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _GADGET_DEVICE_INTERFACE_H_
#define _GADGET_DEVICE_INTERFACE_H_

#include <gadget/gadgetConfig.h>
#include <vector>
#include <gadget/Util/Debug.h>

#include <boost/concept_check.hpp>

namespace gadget
{

class Proxy;
class AnalogProxy;
class DigitalProxy;
class GestureProxy;
class GloveProxy;
class KeyboardMouseProxy;
class PositionProxy;
class StringProxy;
class CommandProxy;

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
class GADGET_CLASS_API BaseDeviceInterface
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
   std::string getProxyName() const
   {
      return mProxyName;
   }

   /** Identifies whether this device interface is connected to a proxy. */
   bool isConnected() const
   {
      return (NULL != mProxyPtr);
   }

protected:
   Proxy*      mProxyPtr;   /**<  Ptr to the proxy */
   std::string mProxyName;  /**< The name of the proxy (or alias) we are looking at */
   bool        mNameSet;    /**< Has the user set a name?? */

public:
   /** Refreshes all the known device interface objects. */
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
template<class PROXY_TYPE>
class DeviceInterface : public BaseDeviceInterface
{
public:
   DeviceInterface(const DeviceInterface& other)
      : BaseDeviceInterface(other)
   {
      if (other.mTypeSpecificProxy != NULL)
      {
         mTypeSpecificProxy = other.mTypeSpecificProxy;
      }
      else
      {
         mTypeSpecificProxy = &mDummyProxy;
      }
   }

public:
   DeviceInterface()
      : BaseDeviceInterface()
   {
      mTypeSpecificProxy = &mDummyProxy;
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
   PROXY_TYPE* operator->()
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
   PROXY_TYPE& operator*()
   {
      return *(mTypeSpecificProxy);
   }
   //@}

   /** Returns the underlying proxy to which we are connected. */
   PROXY_TYPE* getProxy()
   {
      return mTypeSpecificProxy;
   }

   /** Sets the proxy to an explicit proxy. */
   void setProxy(PROXY_TYPE* proxy)
   {
      vprASSERT(NULL != proxy);
      mProxyName = proxy->getName();    // Set the name
      mNameSet = true;
      this->refresh();

      // Verify we found the correct proxy
      vprASSERT(mTypeSpecificProxy == proxy && "Found incorrect proxy for dev interface");
   }

   virtual void refresh()
   {
      BaseDeviceInterface::refresh();
      if(mProxyPtr != NULL)
      {
         mTypeSpecificProxy = dynamic_cast<PROXY_TYPE*>(mProxyPtr);
         if(NULL == mTypeSpecificProxy)
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "[gadget::DeviceInterface::refresh()] Tried to point at "
               << "proxy of incorrect type named '" << mProxyName << "'"
               << " it was type:" << typeid(mProxyPtr).name()
               << std::endl << vprDEBUG_FLUSH;
         }
      }

      // If either one of the proxy pointers are NULL, then use a dummy
      if((NULL == mProxyPtr) || (NULL == mTypeSpecificProxy))
      {
         mTypeSpecificProxy = &mDummyProxy;
      }
   }

private:
   PROXY_TYPE*    mTypeSpecificProxy;   /**< The proxy that is being wrapped */
   PROXY_TYPE     mDummyProxy;
};


// --- Typedefs to the old types --- //
typedef DeviceInterface<AnalogProxy>      AnalogInterface;
typedef DeviceInterface<DigitalProxy>     DigitalInterface;
typedef DeviceInterface<GestureProxy>     GestureInterface;
typedef DeviceInterface<GloveProxy>       GloveInterface;
typedef DeviceInterface<KeyboardMouseProxy> KeyboardMouseInterface;
typedef DeviceInterface<PositionProxy>    PositionInterface;
typedef DeviceInterface<CommandProxy>     CommandInterface;
typedef DeviceInterface<StringProxy>      StringInterface;

} // end namespace

#endif
