/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#ifndef _GADGET_DIGITALPROXY_H_
#define _GADGET_DIGITALPROXY_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Proxy.h>
#include <vpr/Util/Assert.h>

namespace gadget
{

//--------------------------------------------------------------------------
//: A proxy class to digital devices, used by the InputManager.
//
//  A DigitalProxy always points to a digital device and subUnit number,
//  the inputgroup can therefore keep an array of these around and
//  treat them as digital devices which only return a single
//  subDevice's amount of data.  (one int)
//!PUBLIC_API:
//--------------------------------------------------------------------------
class GADGET_CLASS_API DigitalProxy : public TypedProxy<Digital>
{

public:
   /** @name Construction/Destruction */
   DigitalProxy() {
      m_unitNum = -1;
      m_data = 0;
   }

   virtual ~DigitalProxy() {}
   virtual void updateData();


   //: Get the digital data
   // Digital::OFF - Button not pressed, and was not pressed last update either
   // Digital::ON  - Button on, and was on last frame as well
   // Digital::TOGGLE_ON - Button was off, now it is on
   // Digital::TOGGLE_OFF - Button was on, now it is going off
   //
   // The identifiers are defined so that a simple test for non-zero means the button is
   // pressed in some way.  NOTE: Because of how TOGGLE_OFF is defined, testing for non-zero
   // will result in a one update lag in detecting the button not being pressed
   int getData()
   {
      const int defaultDigital(Digital::OFF);

      if(mStupified)
         return defaultDigital;
      else
         return m_data.getDigitalData();
   }


    DigitalData* getDigitalData () {
        return &m_data;
    }


   Digital* getDigitalPtr()
   {
      if(mStupified)
         return NULL;
      else
         return mTypedDevice;
   }

   int getUnit()
   {
      return m_unitNum;
   }

   static std::string getChunkType() { return "DigProxy"; }

   bool config(jccl::ConfigChunkPtr chunk);

   virtual Input* getProxiedInputDevice()
   {
      if(NULL == mTypedDevice)
         return NULL;

      Input* ret_val = dynamic_cast<Input*>(mTypedDevice);
      vprASSERT(ret_val != NULL);
      return ret_val;
   }

private:
   int         m_unitNum;     //: The sub-unit of the device we are working with

      //: Copy of the digital data we are dealing with
      // See also: GetData - for definition of values for the data item
   DigitalData         m_data;

};

} // End of gadget namespace

#endif
