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

#ifndef _GADGET_SIM_ANALOG_H
#define _GADGET_SIM_ANALOG_H
//#pragma once

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Analog.h>
#include <gadget/Devices/Sim/SimInput.h>
#include <vrj/Util/Debug.h>

namespace gadget
{

//: Simulated analog device
// Simulates an analog device from a keyboard device.
// It allows any number of simulated analog devices to be created.
//
// This class should not be used directly by the user.
//!PUBLIC_API:
class SimAnalog : public Input, public Analog, public SimInput
{
public:
   SimAnalog();

   virtual ~SimAnalog();

   virtual bool config(jccl::ConfigChunkPtr chunk);

   //: Return analog data
   virtual float getAnalogData(int devNum=0)
   {
      vprASSERT(devNum < (int)mAnaData.size());    // Make sure we have enough space

      float value = static_cast<float>( mAnaData[devNum] );
      float normalized;
      this->normalizeMinToMax( value, normalized );
      return normalized;
   }

   /* These functions don't do anything */
   int startSampling() { return 1; }
   int stopSampling() { return 1; }
   int sample() { return 1; }

   //: Update the data
   virtual void updateData();

   static std::string getChunkType() { return std::string("SimAnalog");}

private:
   std::vector<float>          mAnaData;     //: The analog data that we have
   std::vector<KeyModPair> mSimKeysUp;   //: The keys to press for moving analog up
   std::vector<KeyModPair> mSimKeysDown; //: The keys to press for moving analog up
   float                     mAnaStep;      //: The analog step size
};

};

#endif
