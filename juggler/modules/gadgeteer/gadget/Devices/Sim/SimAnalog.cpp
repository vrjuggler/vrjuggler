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

#include <gadget/Devices/Sim/SimAnalog.h>
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

bool SimAnalog::config(jccl::ConfigChunkPtr chunk)
{
   //vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)<<"*** SimAnalog::config()\n"<< vprDEBUG_FLUSH;
   if(! (Input::config(chunk) && Analog::config(chunk) && SimInput::config(chunk)))
      return false;

   std::vector<jccl::ConfigChunkPtr> key_inc_list, key_dec_list;

   int key_count = chunk->getNum("keyPairsInc");

   for ( int i = 0; i < key_count; ++i )
   {
      key_inc_list.push_back(chunk->getProperty<jccl::ConfigChunkPtr>("keyPairsInc", i));
   }

   key_count = chunk->getNum("keyPairsDec");

   for ( int i = 0; i < key_count; ++i )
   {
      key_dec_list.push_back(chunk->getProperty<jccl::ConfigChunkPtr>("keyPairsDec", i));
   }

   mSimKeysUp = readKeyList(key_inc_list);
   mSimKeysDown = readKeyList(key_dec_list);

   mAnaStep = chunk->getProperty<float>( "anastep" );
   mInitialValue = chunk->getProperty<float>("initial_value");

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

   // Make an list of the data that will be normalized
   std::vector<AnalogData> norm_data(mAnaData.size());

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

      float normalized_value;
      normalizeMinToMax(mAnaData[i].getAnalog(), normalized_value);

      // Set the normalized data
      norm_data[i] = normalized_value;
      norm_data[i].setTime();
   }

   // Locks and then swaps the indices.
   addAnalogSample(norm_data);
   swapAnalogBuffers();
}

} // End of gadget namespace
