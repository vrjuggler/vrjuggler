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

public:
   /** Constructor. */
   AnalogProxy(const std::string& deviceName = "UnknownAnalog",
               const int unitNum = -1)
      : TypedProxy<Analog>(deviceName)
      , mUnitNum(unitNum)
      , mData(-1.0f)
   {;}

   virtual ~AnalogProxy()
   {;}

   /** Updates the cached data copy from the device. */
   virtual void updateData()
   {
      if(!isStupefied())
      {
         // Make sure dependencies are updated.
         getProxiedInputDevice()->updateDataIfNeeded();
         mData = mTypedDevice->getAnalogData(mUnitNum);
      }
   }

   /** Returns the time of last update. */
   virtual vpr::Interval getTimeStamp() const
   {
      return mData.getTime();
   }

   /**
    * Gets the current analog data value.
    * @return The analog data from the device.
    */
   float getData() const
   {
      const float analogDefault(0.0f);
      if(isStupefied())
      {
         return analogDefault;
      }
      else
      {
         return mData.getAnalog();
      }
   }

   /** Returns a pointer to the gadget::Analog object that we are proxying. */
   AnalogPtr getAnalogPtr()
   {
      if(isStupefied() || NULL == mTypedDevice.get())
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

   virtual InputPtr getProxiedInputDevice()
   {
      if((NULL == mTypedDevice.get()) || (mStupefied))
      {
         return InputPtr();
      }

      InputPtr ret_val = boost::dynamic_pointer_cast<Input>(mTypedDevice);
      vprASSERT((NULL != ret_val.get()) && "Cross-cast in AnalogProxy failed");
      return ret_val;
   }

private:
   int         mUnitNum;
   AnalogData  mData;
};

} // End of gadget namespace

#endif
