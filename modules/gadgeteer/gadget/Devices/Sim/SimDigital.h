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

#ifndef _GADGET_SIM_DIGITAL_H
#define _GADGET_SIM_DIGITAL_H
//#pragma once

#include <gadget/gadgetConfig.h>
#include <vector>
#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Devices/Sim/SimInput.h>

namespace gadget
{

/** \class SimDigital SimDigital.h gadget/Devices/Sim/SimDigital.h
 *
 * Simulated digital device.
 * Simulates a digital device from a keyboard device.
 * It allows any number of simulated digital devices to be created.
 *
 * When the key configured is held down, the digital device is on.
 * When the key is released, the device is no longer set.
 *
 * This class should not be used directly by the user.
 */
//class SimDigital : public Input, public Digital, public SimInput
class SimDigital
   : public siminput_input_digital
{
public:
   SimDigital();
   virtual ~SimDigital();

   virtual bool config(jccl::ConfigElementPtr element);

   /** These functions don't do anything. */
   virtual bool startSampling() { return 1; }
   virtual bool stopSampling() { return 1; }
   virtual bool sample() { return 1; }

   /** Updates the data. */
   virtual void updateData();

   static std::string getElementType();

   virtual std::vector<KeyModPair> getKeys() { return mSimKeys; }

private:
   std::vector<KeyModPair> mSimKeys;       /**< The keys to press for digital simulation */
};

} // End of gadget namespace

#endif
