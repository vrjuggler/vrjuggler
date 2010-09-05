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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_PROXY_H_
#define _GADGET_PROXY_H_

#include <gadget/gadgetConfig.h>

#include <typeinfo>

#include <gadget/InputManager.h>
#include <gadget/Type/Input.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Util/Debug.h>
#include <vpr/Util/Interval.h>
#include <gadget/Type/ProxyPtr.h>

namespace gadget
{

   /** \class Proxy Proxy.h gadget/Type/Proxy.h
    *
    * Base class for all input proxies.
    */
   class GADGET_CLASS_API Proxy
   {
   protected:
      Proxy();

   public:
      virtual ~Proxy();

      /**
       * Configures the proxy.
       *
       * @post Proxy is configured (it is not registered yet though).
       *
       * @return \c true is always returned.
       */
      virtual bool config(jccl::ConfigElementPtr element);

      /**
       * Refreshes the proxy.
       * Checks for the device that it is supposed to point at.
       */
      virtual bool refresh() = 0;

      virtual void updateData();

      /**
       * @since 1.1.19
       */
      void updateDataIfNeeded()
      {
         if ( mNeedUpdate )
         {
            mNeedUpdate = false;
            updateData();
         }
      }

      /**
       * @since 1.1.19
       */
      void resetData()
      {
         mNeedUpdate = true;
      }

      /**
       * Returns a pointer to the base class of the devices being proxied.
       * @return NULL if no device is proxied.
       */
      virtual InputPtr getProxiedInputDevice() = 0;

      /**
       * Returns the string rep of the element type used to config this device.
       * Used by the Input Manager to find elements that construct devices.
       */
      static std::string getElementType()
      {
         return "Undefined";
      }

      /** Gets the name of the proxy. */
      const std::string& getName() const
      {
         return mName;
      }

      /** Sets the name of the proxy. */
      void setName(const std::string& name)
      {
         mName = name;
      }

      /**
       * Is the proxy currently stupefied?
       * If the device we are proxying does not exist, then this will return
       * true.
       *
       * @since 0.92.1
       *
       * @note Renamed from isStupified() in version 0.92.1.
       */
      virtual bool isStupefied() const;

      /**
       * Sets the stupefication state of this proxy.
       *
       * @param newState The new state of stupefication.
       *
       * @since 0.92.1
       *
       * @note Renamed from stupify() in version 0.92.1.
       */
      void stupefy(const bool newState = true)
      {
         mStupefied = newState;
      }

      /** Returns the time of last update. */
      virtual vpr::Interval getTimeStamp() const = 0;

   protected:
      std::string mName;         /**< The name of the proxy */
      bool        mStupefied;    /**< Is the proxy current stupefied (returns default data) */
      bool        mNeedUpdate;   /**< @since 1.1.19 */
   };

   /** \class TypedProxy Proxy.h gadget/Type/Proxy.h
    *
    * Proxy for specific device types.
    */
   template <class DEV_TYPE>
   class TypedProxy : public Proxy
   {
   public:
      /** @since 2.0.0 */
      typedef boost::shared_ptr<DEV_TYPE> device_ptr_type;

      TypedProxy(const std::string& deviceName = "Unknown")
         : mDeviceName(deviceName)
         , mTypedDevice()
      {
         ;
      }

      /**
       * Sets the proxy to point to the given type-specific device.
       *
       * @pre devPtr must be a valid device of type DEV_TYPE.
       * @post The proxy now references the given device.  The device name we
       *       are proxying is set to devPtr->getInstanceName().
       *
       * @param devName The name of the device at which we are pointing.
       * @param devPtr  Pointer to the device.
       */
      virtual void set(const std::string& devName, device_ptr_type devPtr)
      {
         mTypedDevice = devPtr;
         if(NULL != mTypedDevice.get())
         {
            mDeviceName = devName;
            stupefy(false);
         }
         else
         {
            vprASSERT(false && "Tried to set proxy to NULL device");
         }
      }

      /**
       * Refreshes the proxy.  This attempts to lookup the device that we are
       * proxying.  If the lookup fails, then we become stupefied.  If not,
       * then the proxy is pointed at this potentially new device.
       */
      virtual bool refresh()
      {
         InputPtr input_dev = InputManager::instance()->getDevice(mDeviceName);

         if ( NULL == input_dev.get() )       // Not found, so stupefy
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
               << "[gadget::TypedProxy::refresh()] Could not find device '"
               << mDeviceName << "' pointed to by '" << mName << "'"
               << std::endl << vprDEBUG_FLUSH;
            stupefy(true);
         }
         else
         {
            device_ptr_type typed_dev(
               boost::dynamic_pointer_cast<DEV_TYPE>(input_dev)
            );

            if ( NULL == typed_dev.get() )
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                  << "[gadget::TypedProxy::config()] Device was of wrong type: "
                  << mDeviceName << " it was type:" << typeid(input_dev).name()
                  << std::endl << vprDEBUG_FLUSH;
               stupefy(true);
               return false;
            }
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
               << "   Proxy '" << mName << "' configured" << std::endl
               << vprDEBUG_FLUSH;
            mTypedDevice = typed_dev;    // Set the proxy
            stupefy(false);
         }
         return true;
      }

      /** Returns the name of the device that we are proxying. */
      virtual std::string getDeviceName() const
      {
         return mDeviceName;
      }

      virtual InputPtr getProxiedInputDevice()
      {
         if (NULL == mTypedDevice.get() || mStupefied)
         {
            return InputPtr();
         }

         InputPtr ret_val = boost::dynamic_pointer_cast<Input>(mTypedDevice);
         vprASSERT((NULL != ret_val.get()) && "Cross-cast failed.");
         return ret_val;
      }

      /**
       * Returns the proxied input device if this proxy is not stupefied.
       *
       * @since 2.0.0
       */
      const device_ptr_type getTypedInputDevice() const
      {
         return mStupefied ? device_ptr_type() : mTypedDevice;
      }

   protected:
      std::string     mDeviceName;   /**< Name of the device to link up with */
      device_ptr_type mTypedDevice;  /**< The device (type-specific pointer) */
   };

} // end namespace

#endif
