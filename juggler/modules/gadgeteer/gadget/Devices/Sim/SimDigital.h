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

#ifndef _GADGET_SIM_DIGITAL_H
#define _GADGET_SIM_DIGITAL_H
//#pragma once

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Devices/Sim/SimInput.h>

namespace gadget
{

//: Simulated digital device
// Simulates a digital device from a keyboard device.
// It allows any number of simulated digital devices to be created.
//
// When the key configured is held down, the digital device is on.
// When the key is released, the device is no longer set.
//
// This class should not be used directly by the user.
//!PUBLIC_API:
class SimDigital : public Input, public Digital, public SimInput
{
public:
   SimDigital();
   virtual ~SimDigital();

   virtual bool config(jccl::ConfigChunk* chunk);

   //: Return digital data
   virtual int getDigitalData(int devNum=0)
   {
      vprASSERT(devNum < (int)mDigitalData.size());    // Make sure we have enough space
      return mDigitalData[devNum];
   }

   /* These functions don't do anything */
   virtual int startSampling() { return 1; }
   virtual int stopSampling() { return 1; }
   virtual int sample() { return 1; }

   //: Update the data
   virtual void updateData();

   static std::string getChunkType() { return std::string("SimDigital");}

private:
   std::vector<int>          mDigitalData;   //: The digital data that we have
   std::vector<KeyModPair> mSimKeys;       //: The keys to press for digital simulation
};

};

#endif
