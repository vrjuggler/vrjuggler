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

#ifndef _GADGET_PROXY_H_
#define _GADGET_PROXY_H_

#include <gadget/gadgetConfig.h>

#include <typeinfo>
#include <boost/optional.hpp>
#include <boost/signal.hpp>
#include <boost/noncopyable.hpp>

#include <vpr/Util/Interval.h>
#include <vpr/Util/SignalProxy.h>

#include <jccl/Config/ConfigElement.h>

#include <gadget/InputManager.h>
#include <gadget/Type/Input.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/ProxyPtr.h>
#include <gadget/Type/DeviceTraits.h>


namespace gadget
{

   /** \class Proxy Proxy.h gadget/Type/Proxy.h
    *
    * Base class for all input proxies.
    */
   class GADGET_CLASS_API Proxy
      : private boost::noncopyable
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
      virtual const vpr::Interval& getTimeStamp() const = 0;

   protected:
      std::string mName;         /**< The name of the proxy */
      bool        mStupefied;    /**< Is the proxy current stupefied (returns default data) */
      bool        mNeedUpdate;   /**< @since 1.1.19 */
      
   };

   /** \class TypedProxy Proxy.h gadget/Type/Proxy.h
    *
    * Proxy for specific device types.
    */
   template<typename DeviceType>
   class TypedProxy : public Proxy
   {
   public:
      /**
       * Type Declarations
       *
       * @since 2.1.1
       */
      //@{
      typedef DeviceType                                   device_type;
      typedef DeviceTraits<DeviceType>                     device_traits_type;
      typedef typename device_traits_type::device_ptr_type device_ptr_type;
      typedef typename device_traits_type::data_type       device_data_type;
      typedef typename device_data_type::data_type         raw_data_type;
      typedef boost::signal<void (device_ptr_type)>        refresh_signal_type;
      //@}

   protected:
      TypedProxy(const std::string& deviceName = "Unknown",
                 const int unitNum = -1)
         : mDeviceName(deviceName)
         , mUnit(unitNum)
      {
         /* Do nothing. */ ;
      }

   public:
      virtual ~TypedProxy()
      {
         /* Do nothing. */ ;
      }

      /** @name gadget::Proxy Interface Implementation */
      //@{
      /**
       * Configures this typed proxy.
       *
       * @pre The config definition for \p element has a property named \c unit
       *      of type int and a propety named \c device of type string.
       * @post \c mUnit is set to the value of the \c unit property, and
       *       \c mDeviceName is set to the value of the \c device property.
       *       If the device identified in the configuration has been
       *       configured, then \c mTypedDevice points to that object.
       *
       * @return \c true is returned if this typed proxy is configured
       *         successfully; \c false is returned otherwise.
       *
       * @since 2.1.1
       */
      bool config(jccl::ConfigElementPtr element)
      {
         vpr::DebugOutputGuard dbg_output(
            gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
            "----------- configuring TYPED PROXY -----------------\n",
            "----------- exit: configuring typed proxy -----------\n"
         );

         //vprASSERT(element->getID() == getElementType());

         if (! Proxy::config(element))
         {
            return false;
         }

         mUnit = element->getProperty<int>("unit");
         mDeviceName = element->getProperty<std::string>("device");

         refresh();
         return true;
      }

      /**
       * Refreshes the proxy. This attempts to lookup the device that we are
       * proxying. If the lookup fails, then we become stupefied. If not, then
       * the proxy is pointed at this potentially new device.
       */
      virtual bool refresh()
      {
         bool result(true);
         InputPtr input_dev = InputManager::instance()->getDevice(mDeviceName);

         if (NULL == input_dev.get())       // Not found, so stupefy
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
               boost::dynamic_pointer_cast<DeviceType>(input_dev)
            );

            if (NULL == typed_dev.get())
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                  << "[gadget::TypedProxy::refresh()] Device "
                  << mDeviceName << " was of wrong type "
                  << typeid(input_dev).name() << std::endl << vprDEBUG_FLUSH;

               stupefy(true);
               result = false;
            }
            else
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                  << "   Proxy '" << mName << "' configured" << std::endl
                  << vprDEBUG_FLUSH;

               mTypedDevice = typed_dev;    // Set the proxy
               stupefy(false);
            }
         }

         mRefreshed(mTypedDevice);

         return result;
      }

      /**
       * Returns the proxied input device if this proxy is not stupefied. If
       * this proxy is stupefied or has no proxied input device, then a NULL
       * pointer is returned. This method is part of the gadget::Proxy
       * interface.
       *
       * @return If this proxy is not stupefied, a gadget::InputPtr is
       *         returned that points to proxied input device object. NULL is
       *         returned if this proxy is stupefied or has no input device
       *         associated with it.
       *
       * @see getTypedInputDevice()
       */
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

      /** Returns the time of the last update. */
      virtual const vpr::Interval& getTimeStamp() const
      {
         return mData.getTime();
      }
      //@}

      /**
       * Sets the proxy to point to the given type-specific device.
       *
       * @pre \p devPtr must be a valid device of the type identified by
       *      \p devName.
       * @post The proxy now references the given device.  The device name we
       *       are proxying is set to devPtr->getInstanceName().
       *
       * @param devName The name of the device at which we are pointing.
       * @param devPtr  Pointer to the device.
       * @param unitNum Integer to index the unit on the device. This
       *                parameter is optional. If not specified, then \c mUnit
       *                will not be changed.
       *
       * @note The \p unitNum parameter was added in Gadgeteer 2.1.14.
       */
      void set(const std::string& devName,
               const device_ptr_type& devPtr,
               const boost::optional<int>& unitNum = boost::optional<int>())
      {
         mTypedDevice = devPtr;
         if (NULL != mTypedDevice.get())
         {
            mDeviceName = devName;

            if (unitNum)
            {
               mUnit = *unitNum;
            }

            stupefy(false);
         }
         else
         {
            vprASSERT(false && "Tried to set proxy to NULL device");
         }
      }

      /** Returns the name of the device that we are proxying. */
      const std::string& getDeviceName() const
      {
         return mDeviceName;
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

      /**
       * Returns the unit index into the string speech recognition device from
       * which this proxy is reading data.
       *
       * @note Prior to Gadgeteer 2.1.1, every subclass of this class defined
       *       this method.
       *
       * @since 2.1.1
       */
      int getUnit() const
      {
         return mUnit;
      }

      /** @name Data Access */
      //@{
      /**
       * Returns the most current data for the input device proxied by this
       * object. The return type of this method depends on the type of the
       * device that is proxied. If this proxy is stupefied, then a
       * default-constructed value of type raw_data_type is returned to the
       * caller.
       *
       * @note Prior to Gadgeteer 2.1.1, most subclasses of this class defined
       *       this method.
       *
       * @since 2.1.1
       */
      virtual const raw_data_type getData() const
      {
         return isStupefied() ? raw_data_type() : mData.getValue();
      }

      /**
       * Returns a const reference to the internal "raw" data of this proxy.
       *
       * @note Prior to Gadgeteer 2.1.1, most subclasses of this class defined
       *       a method similar to this one that returned a \em pointer to the
       *       device-specific internal data object. Those methods had names
       *       such as getPositionData() or getStringData().
       *
       * @since 2.1.1
       */
      const device_data_type& getRawData() const
      {
         return mData;
      }
      //@}

      /**
       * Whenever refresh() is invoked, the proxyRefreshed signal is emitted.
       *
       * @since 2.1.17
       */
      vpr::SignalProxy<refresh_signal_type> proxyRefreshed()
      {
         return mRefreshed;
      }

   protected:
      std::string      mDeviceName;   /**< Name of the device to link up with */
      device_ptr_type  mTypedDevice;  /**< The device (type-specific pointer) */
      int              mUnit;
      device_data_type mData;

   private:
      refresh_signal_type mRefreshed;
   };

} // end namespace

#endif
