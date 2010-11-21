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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_DIGITAL_PROXY_H_
#define _GADGET_DIGITAL_PROXY_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Proxy.h>
#include <gadget/Type/DigitalProxyPtr.h>
#include <vpr/Util/Assert.h>

namespace gadget
{

/** \class DigitalProxy DigitalProxy.h gadget/Type/DigitalProxy.h
 *
 * A proxy class to digital devices, used by the Input Manager.
 *
 * A digital proxy always points to a digital device and unit number within
 * that device.  The Input Manager can therefore keep an array of these
 * around and treat them as digital devices that only return a single
 * sub-device's amount of data (one int).
 *
 * @see gagdet::Digital
 */
class GADGET_CLASS_API DigitalProxy : public TypedProxy<Digital>
{
protected:
   DigitalProxy(const std::string& deviceName = "UnknownDigital",
                const int unitNum = -1);

public:
   /** @name Construction/Destruction */
   //@{
   /**
    * Creates a DigitalProxy instance and returns it wrapped in a
    * DigitalProxyPtr object.
    *
    * @since 1.3.7
    */
   static DigitalProxyPtr create(const std::string& deviceName = "UnknownDigital",
                                 const int unitNum = -1);

   virtual ~DigitalProxy();
   //@}

   virtual void updateData();

   /** Returns the time of the last update. */
   virtual vpr::Interval getTimeStamp() const;

   /**
    * Get the digital data.
    * Digital::OFF: Button not pressed, and was not pressed last update either.<br>
    * Digital::ON: Button on, and was on last frame as well.<br>
    * Digital::TOGGLE_ON: Button was off, now it is on.<br>
    * Digital::TOGGLE_OFF: Button was on, now it is going off.<br>
    *
    * The identifiers are defined so that a simple test for non-zero means the
    * button is pressed in some way.
    *
    * @note Because of how TOGGLE_OFF is defined, testing for non-zero
    *       will result in a one update lag in detecting the button not being
    *       pressed.
    */
   gadget::Digital::State getData() const
   {
      // If we're stupefied, return gadget::Digital::OFF.  Otherwise, return
      // the current digital value.
      return (isStupefied() ? Digital::OFF
                            : (gadget::Digital::State) mData.getDigital());
   }

   const DigitalData* getDigitalData() const
   {
      return &mData;
   }

   /**
    * Returns a pointer to the gadget::Digital object that we are proxying.
    *
    * @deprecated Use getTypedInputDevice() for forward compatibility with the
    *             next major release of Gadgeteer.
    */
   const DigitalPtr getDigitalPtr() const
   {
      // If we're stupefied, return NULL.  Otherwise, return mTypedDevice.
      return (isStupefied() ? DigitalPtr() : mTypedDevice);
   }

   /**
    * Returns the unit index into the digital device from which this proxy
    * is reading data.
    */
   int getUnit() const
   {
      return mUnitNum;
   }

   static std::string getElementType();

   bool config(jccl::ConfigElementPtr element);

private:
   int mUnitNum;     /**<  The sub-unit of the device we are working with */

   /**
    * Copy of the digital data we are dealing with.
    * @see getData()
    */
   DigitalData mData;
};

} // End of gadget namespace

#endif
