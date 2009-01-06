/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <gadget/Devices/Sim/SimAnalog.h>
#include <jccl/Config/ConfigElement.h>
#include <gmtl/Math.h>

namespace gadget
{

/** Default Constructor */
SimAnalog::SimAnalog()
   : mAnaStep(0.0f)
   , mInitialValue(0.0f)
{
   //vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)<<"*** SimAnalog::SimPinchGlove()\n"<< vprDEBUG_FLUSH;
}

/** Destructor */
SimAnalog::~SimAnalog()
{
   //vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)<<"*** SimAnalog::~SimPinchGlove()\n"<< vprDEBUG_FLUSH;
}

std::string SimAnalog::getElementType()
{
   return "simulated_analog_device";
}

bool SimAnalog::config(jccl::ConfigElementPtr element)
{
   //vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)<<"*** SimAnalog::config()\n"<< vprDEBUG_FLUSH;
   if (! (Input::config(element) && Analog::config(element) &&
          SimInput::config(element)) )
   {
      return false;
   }

   std::vector<jccl::ConfigElementPtr> key_inc_list, key_dec_list;

   int key_count = element->getNum("increment_keypress");

   for ( int i = 0; i < key_count; ++i )
   {
      key_inc_list.push_back(element->getProperty<jccl::ConfigElementPtr>("increment_keypress", i));
   }

   key_count = element->getNum("decrement_keypress");

   for ( int i = 0; i < key_count; ++i )
   {
      key_dec_list.push_back(element->getProperty<jccl::ConfigElementPtr>("decrement_keypress", i));
   }

   mSimKeysUp = readKeyList(key_inc_list);
   mSimKeysDown = readKeyList(key_dec_list);

   mAnaStep = element->getProperty<float>("delta");
   mInitialValue = element->getProperty<float>("initial_value");

   // Initialize all the data to the inital_value
   size_t num_pairs = mSimKeysUp.size();
   mAnaData = std::vector<AnalogData>(num_pairs);
   for (size_t i=0; i<num_pairs; ++i)
   {
      mAnaData[i].setAnalog(mInitialValue);
   }

   return true;
}

void SimAnalog::updateData()
{
   //vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)<<"*** SimAnalog::updateData()\n"<< vprDEBUG_FLUSH;

   // -- Update analog data --- //
   for (unsigned int i = 0; i < mSimKeysUp.size(); ++i)
   {
      mAnaData[i].setTime();
      mAnaData[i].setAnalog(mAnaData[i].getAnalog()
                                + (float)checkKeyPair(mSimKeysUp[i]) * mAnaStep);
      mAnaData[i].setAnalog(mAnaData[i].getAnalog()
                                - (float)checkKeyPair(mSimKeysDown[i]) * mAnaStep);

      // Clamp to the min/max range
      mAnaData[i].setAnalog(gmtl::Math::clamp(mAnaData[i].getAnalog(),
                                              getMin(), getMax()));
   }

   // Locks and then swaps the indices.
   addAnalogSample(mAnaData);
   swapAnalogBuffers();
}

} // End of gadget namespace
