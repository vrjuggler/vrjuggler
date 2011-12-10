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
public:
   /** @since 2.1.1 */
   typedef TypedProxy<Analog> base_type;

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
    * @post \c mData holds the current raw data sample from the proxied
    *       analog device. \c mData holds the current normalized data sample
    *       from the proxied analog device.
    */
   virtual void updateData();

   /**
    * Gets the current normalized analog data value. This value will be in
    * the range [0.0,1.0].
    *
    * @return The normalized analog data from the device.
    */
   base_type::get_data_return_type getData() const
   {
      return isStupefied() ? 0.0f : mNormalizedData;
   }

   /**
    * Normalizes the given raw data value to the range [0.0,1.0]. If this
    * proxy is stupefid, 0.0 is always returned.
    *
    * @since 2.1.21
    */
   float normalizeData(const float value) const
   {
      return isStupefied() ? 0.0f : mTypedDevice->normalize(value);
   }

   static std::string getElementType();

private:
   float mNormalizedData;
};

} // End of gadget namespace

#endif
