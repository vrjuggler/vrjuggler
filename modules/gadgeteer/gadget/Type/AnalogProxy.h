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

/////////////////////////////////////////////////////////////////////////
//
// positional proxy class
//
////////////////////////////////////////////////////////////////////////

#ifndef _GADGET_ANALOGPROXY_H_
#define _GADGET_ANALOGPROXY_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/Proxy.h>
#include <gadget/Type/AnalogData.h>

namespace gadget
{

/**
 * A proxy class to analog devices, used by the InputManager.
 *
 * A AnalogProxy always points to an analog device and subUnit number,
 * the inputgroup can therefore keep an array of these around and
 * treat them as analog devices which only return a single
 * subDevice's amount of data.  (one int)
 *
 * @see Analog
 *
 */
class GADGET_CLASS_API AnalogProxy : public TypedProxy<Analog>
{

public:
   /** Constructor. */
   AnalogProxy() : mUnitNum(-1), mData(-1.0f)
   {;}

   virtual ~AnalogProxy() {}

   /** Updates the cached data copy from the device. */
   virtual void updateData()
   {
      if(!isStupified())
      {
         mData = mTypedDevice->getAnalogData(mUnitNum);
      }
   }

   /** Returns time of last update. */
   vpr::Interval getTimeStamp() const
   {  return mData.getTime(); }


   /**
    * Gets the current analog data value.
    * @return The analog data from the device.
    */
   float getData() const
   {
      const float analogDefault(0.0f);
      if(isStupified())
         return analogDefault;
      else
         return mData.getAnalog();
   }

   Analog* getAnalogPtr()
   {
      if(isStupified())
         return NULL;
      else
         return mTypedDevice;
   }

   int getUnit() const
   { return mUnitNum;}

   static std::string getChunkType() { return "AnaProxy";}

   bool config(jccl::ConfigChunkPtr chunk);

   virtual Input* getProxiedInputDevice()
   {
      if((NULL == mTypedDevice) || (mStupified))
      {
         return NULL;
      }

      Input* ret_val = dynamic_cast<Input*>(mTypedDevice);
      vprASSERT((ret_val != NULL) && "Cross-cast in AnalogProxy failed");
      return ret_val;
   }

private:
   int         mUnitNum;
   AnalogData  mData;
};

} // End of gadget namespace

#endif
