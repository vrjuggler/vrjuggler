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
#ifndef _VJ_DIGITALPROXY_H_
#define _VJ_DIGITALPROXY_H_

#include <vjConfig.h>
#include <Input/vjInput/vjDigital.h>
#include <Input/InputManager/vjProxy.h>
#include <Kernel/vjAssert.h>

//--------------------------------------------------------------------------
//: A proxy class to digital devices, used by the vjInputManager.
//
//  A vjDigitalProxy always points to a digital device and subUnit number,
//  the inputgroup can therefore keep an array of these around and
//  treat them as digital devices which only return a single
//  subDevice's amount of data.  (one int)
//!PUBLIC_API:
//--------------------------------------------------------------------------
class vjDigitalProxy : public vjProxy
{

public:
   /** @name Construction/Destruction */
   vjDigitalProxy() {
      m_digPtr = NULL;
      m_unitNum = -1;
      m_data = 0;
   }

   virtual ~vjDigitalProxy() {}

   void set(vjDigital* digPtr, int subNum)
   {
      vjASSERT( digPtr->fDeviceSupport(DEVICE_DIGITAL) );
      m_digPtr = digPtr;
      m_unitNum = subNum;
      m_data = m_digPtr->getDigitalData(m_unitNum);

      vjDEBUG(vjDBG_INPUT_MGR, vjDBG_VERB_LVL) << "digPtr: " << digPtr << std::endl
              << "subNum: " << subNum << std::endl << std::endl
              << vjDEBUG_FLUSH;
   }

   void updateData();


   //: Get the digital data
   // vjDigital::OFF - Button not pressed, and was not pressed last update either
   // vjDigital::ON  - Button on, and was on last frame as well
   // vjDigital::TOGGLE_ON - Button was off, now it is on
   // vjDigital::TOGGLE_OFF - Button was on, now it is going off
   //
   // The identifiers are defined so that a simple test for non-zero means the button is
   // pressed in some way.  NOTE: Because of how TOGGLE_OFF is defined, testing for non-zero
   // will result in a one update lag in detecting the button not being pressed
   int getData()
   {
      return m_data;
   }

   vjDigital* getDigitalPtr()
   {
      return m_digPtr;
   }

   int getUnit()
   {
      return m_unitNum;
   }

   static std::string getChunkType() { return "DigProxy"; }

   bool config(vjConfigChunk* chunk);

   virtual vjInput* getProxiedInputDevice()
   {
      vjInput* ret_val = dynamic_cast<vjInput*>(m_digPtr);
      vjASSERT(ret_val != NULL);
      return ret_val;
   }

private:
   vjDigital*  m_digPtr;      //: Pointer to the digital device
   int         m_unitNum;     //: The sub-unit of the device we are working with

      //: Copy of the digital data we are dealing with
      // See also: GetData - for definition of values for the data item
   int         m_data;

};

#endif
