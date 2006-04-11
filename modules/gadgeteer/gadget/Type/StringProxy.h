/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _GADGET_STRING_PROXY_H_
#define _GADGET_STRING_PROXY_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/String.h>
#include <gadget/Type/Proxy.h>
#include <vpr/Util/Assert.h>

namespace gadget
{

/** \class StringProxy StringProxy.h gadget/Type/StringProxy.h
 *
 * A proxy class to string speech recognition devices, used by the Input
 * Manager.
 *
 * A string speech recognition proxy always points to a string speech
 * recognition device and unit number within that device.  The Input Manager
 * can therefore keep an array of these around and treat them as digital
 * devices that only return a single sub-device's amount of data (one
 * std::string).
 *
 * @note This interface should be considered as being in an "alpha" state.
 *       String-oriented device types are very new to Gadgeteer, and they
 *       have not have the same amount of time to mature as the other
 *       device types (which have been around since VR Juggler 1.0).  This
 *       interface may change in a future release of Gadgeteer (such as
 *       Version 1.2).
 *
 * @see gagdet::String
 */
class GADGET_CLASS_API StringProxy : public TypedProxy<String>
{

public:
   /** @name Construction/Destruction */
   //@{
   StringProxy()
      : mUnitNum(-1), mData("")
   {;}

   virtual ~StringProxy()
   {;}
   //@}

   virtual void updateData();

   /** Returns the time of the last update. */
   virtual vpr::Interval getTimeStamp() const
   {
      return mData.getTime();
   }

   /**
    * Gets the string data.
    */
   std::string getData() const
   {
      // If we're stupefied, return empty string.  Otherwise, return
      // the current string value.
      return (isStupefied() ? ""
                            : mData.getString());
   }

   StringData* getStringData()
   {
      return &mData;
   }

   /**
    * Returns a pointer to the gadget::String object that we are proxying.
    */
   String* getStringPtr()
   {
      // If we're stupefied, return NULL.  Otherwise, return mTypedDevice.
      return (isStupefied() ? NULL : mTypedDevice);
   }

   /**
    * Returns the unit index into the string speech recognition device from
    * which this proxy is reading data.
    */
   int getUnit() const
   {
      return mUnitNum;
   }

   static std::string getElementType();

   bool config(jccl::ConfigElementPtr element);

   virtual Input* getProxiedInputDevice()
   {
      if((NULL == mTypedDevice) || (mStupefied))
      {
         return NULL;
      }

      Input* ret_val = dynamic_cast<Input*>(mTypedDevice);
      vprASSERT((ret_val != NULL) && "Cross-cast in StringProxy failed");
      return ret_val;
   }

private:
   int mUnitNum;     /**<  The sub-unit of the device we are working with */

   /**
    * Copy of the digital data we are dealing with.
    * @see getData()
    */
   StringData mData;
};

} // End of gadget namespace

#endif
