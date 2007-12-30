/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _GADGET_ANALOGPROXY_H_
#define _GADGET_ANALOGPROXY_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/Proxy.h>
#include <gadget/Type/AnalogData.h>
#include <gadget/Type/AnalogProxyPtr.h>

namespace gadget
{

/** \class AnalogProxy AnalogProxy.h gadget/Type/AnalogProxy.h
 *
 * A proxy class to analog devices, used by the Input Manager.
 *
 * An analog proxy always points to an analog  device and unit number within
 * that device.  The Input Manager can therefore keep an array of these
 * around and treat them as analog devices that only return a single
 * sub-device's amount of data (one float).
 *
 * @see Analog
 */
class GADGET_CLASS_API AnalogProxy : public TypedProxy<Analog>
{
protected:
   /** Constructor. */
   AnalogProxy(const std::string& deviceName = "UnknownAnalog",
               const int unitNum = -1);

public:
   /**
    * Creates a AnalogProxy instance and returns it wrapped in a
    * AnalogProxyPtr object.
    *
    * @since 1.3.7
    */
   static AnalogProxyPtr create(const std::string& deviceName = "UnknownAnalog",
                                const int unitNum = -1);

   virtual ~AnalogProxy();

   /**
    * Updates the cached data copy from the device and the normalized form of
    * that data.
    *
    * @post \c mRawData holds the current raw data sample from the proxied
    *       analog device. \c mData holds the current normalized data sample
    *       from the proxied analog device.
    */
   virtual void updateData();

   /** Returns the time of last update. */
   virtual vpr::Interval getTimeStamp() const;

   /**
    * Gets the current normalized analog data value. This value will be in
    * the range [0.0,1.0].
    *
    * @return The normalized analog data from the device.
    */
   float getData() const
   {
      return isStupefied() ? 0.0f : mData;
   }

   /**
    * Gets the current raw analog data value. This is the value read direcctly
    * from the device without perfomring any normalization.
    *
    * @return The raw analog data from the device.
    *
    * @since 1.3.18
    */
   float getRawData() const
   {
      const float analogDefault(0.0f);
      if(isStupefied())
      {
         return analogDefault;
      }
      else
      {
         return mRawData.getAnalog();
      }
   }

   /** Returns a pointer to the gadget::Analog object that we are proxying. */
   const AnalogPtr getAnalogPtr() const
   {
      if ( isStupefied() || NULL == mTypedDevice.get() )
      {
         return AnalogPtr();
      }
      else
      {
         return mTypedDevice;
      }
   }

   /**
    * Returns the unit index into the analog device from which this proxy
    * is reading data.
    */
   int getUnit() const
   {
      return mUnitNum;
   }

   static std::string getElementType();

   bool config(jccl::ConfigElementPtr element);

private:
   int         mUnitNum;
   AnalogData  mRawData;
   float       mData;
};

} // End of gadget namespace

#endif
