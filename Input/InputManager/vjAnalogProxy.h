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
#ifndef _VJ_ANALOGPROXY_H_
#define _VJ_ANALOGPROXY_H_

#include <vjConfig.h>
#include <assert.h>
#include <Input/vjInput/vjAnalog.h>
#include <Input/InputManager/vjProxy.h>

//: A proxy class to analog devices, used by the vjInputManager.
//
//  A vjAnalogProxy always points to an analog device and subUnit number,
//  the inputgroup can therefore keep an array of these around and
//  treat them as analog devices which only return a single
//  subDevice's amount of data.  (one int)
//
// See also: vjAnalog
//
//!PUBLIC_API:
class vjAnalogProxy : public vjProxy
{

public:
   //: Constructor
   vjAnalogProxy() : m_anaPtr(NULL), m_unitNum(-1), m_data(-1.0f)
   {;}

   virtual ~vjAnalogProxy() {}

   //: Set the proxy to point to the given analog device
   //! PRE: anaPtr must be a valid analog device
   //+      subNum must be a valid subNum for the device
   //! POST: The proxy now references the analog device
   //! ARGS: anaPtr - Pointer to the analog device
   //! ARGS: subNum - The subunit number of the analog device
   void set(vjAnalog* anaPtr, int subNum)
   {
      assert( anaPtr->fDeviceSupport(DEVICE_ANALOG) );
      m_anaPtr = anaPtr;
      m_unitNum = subNum;

      vjDEBUG(vjDBG_INPUT_MGR, vjDBG_VERB_LVL) << "anaPtr: " << anaPtr << std::endl
              << "subNum: " << subNum << std::endl << vjDEBUG_FLUSH;
   }

   //: Update the cached data copy from the device
   void updateData()
   { m_data = m_anaPtr->getAnalogData(m_unitNum);}

   //: Get the current analog data value
   //! RETURNS: The analog data from the device
   float getData() const
   { return m_data;}

   vjAnalog* getAnalogPtr()
   { return m_anaPtr;}

   int getUnit() const
   { return m_unitNum;}

   static std::string getChunkType() { return "AnaProxy";}

   bool config(vjConfigChunk* chunk);

   virtual vjInput* getProxiedInputDevice()
   {
      vjInput* ret_val = dynamic_cast<vjInput*>(m_anaPtr);
      vjASSERT(ret_val != NULL);
      return ret_val;
   }

private:
   vjAnalog*   m_anaPtr;
   int         m_unitNum;
   float       m_data;
};

#endif
