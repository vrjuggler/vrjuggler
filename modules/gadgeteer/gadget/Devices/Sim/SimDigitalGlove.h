/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#ifndef _GADGET_SIM_DIGITALGLOVE_H
#define _GADGET_SIM_DIGITALGLOVE_H

#include <gadget/gadgetConfig.h>

#include <vector>
#include <boost/mpl/inherit.hpp>

#include <gadget/Type/InputDevice.h>


namespace gadget
{

/** \class SimDigitalGlove SimDigitalGlove.h gadget/Devices/Sim/SimDigitalGlove.h
 *
 * Simulated digital glove device.
 * Simulates a digital glove device from a keyboard device.
 * It allows any number of simulated digital glove devices to be created.
 *
 * When the key configured is held down, the digital device is on.
 * When the key is released, the device is no longer set.
 *
 * This class should not be used directly by the user.
 */
class SimDigitalGlove
   : public InputDevice<boost::mpl::inherit<SimInput, Digital, Glove>::type>
{
public:
   /** Default constructor. */
   SimDigitalGlove();

   /** Destructor. */
   virtual ~SimDigitalGlove();

   /** Configures this device. */
   virtual bool config(jccl::ConfigElementPtr element);

   enum finger
   {
      LTHUMB = 0, LINDEX = 1, LMIDDLE = 2, LRING = 3, LPINKY = 4,
      RTHUMB = 6, RINDEX = 7, RMIDDLE = 8, RRING = 9, RPINKY = 10
   };

   virtual bool startSampling(){return true;}
   virtual bool stopSampling(){return true;}
   virtual bool sample(){return true;}

   /** Updates the data. */
   virtual void updateData();

   static std::string getElementType();

private:
   std::vector<KeyModPair>    mSimKeys;       /**< The keys to press for digital simulation */
};

} // End of gadget namespace

#endif
