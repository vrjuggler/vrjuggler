/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#include <gadget/gadgetConfig.h>
#include <gadget/Devices/Sim/SimAnalog.h>
#include <jccl/Config/ConfigChunk.h>

namespace gadget
{

/** Default Constructor */
SimAnalog::SimAnalog() : Analog(), SimInput()
{
   //vprDEBUG(vprDBG_ALL,4)<<"*** SimAnalog::SimPinchGlove()\n"<< vprDEBUG_FLUSH;
}

/** Destructor */
SimAnalog::~SimAnalog()
{
   //vprDEBUG(vprDBG_ALL,4)<<"*** SimAnalog::~SimPinchGlove()\n"<< vprDEBUG_FLUSH;
}

bool SimAnalog::config(jccl::ConfigChunkPtr chunk)
{
   //vprDEBUG(vprDBG_ALL,4)<<"*** SimAnalog::config()\n"<< vprDEBUG_FLUSH;
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

   int num_pairs = mSimKeysUp.size();

   mAnaData = std::vector<AnalogData>( num_pairs ); // Initialize to all zeros
   mAnaStep = chunk->getProperty<float>( "anastep" );

   return true;
}

void SimAnalog::updateData()
{
   //vprDEBUG(vprDBG_ALL,4)<<"*** SimAnalog::updateData()\n"<< vprDEBUG_FLUSH;

   // -- Update analog data --- //
   for (unsigned int i = 0; i < mSimKeysUp.size(); ++i)
   {
      mAnaData[i].setTime();
      mAnaData[i].setAnalog(mAnaData[i].getAnalog()
                                + (float)checkKeyPair(mSimKeysUp[i]) * mAnaStep);
      mAnaData[i].setAnalog(mAnaData[i].getAnalog()
                                - (float)checkKeyPair(mSimKeysDown[i]) * mAnaStep);

      if (mAnaData[i].getAnalog() < 0.0f)
      {   mAnaData[i].setAnalog(0.0f); }
      if (mAnaData[i].getAnalog() > 255.0f)
      {  mAnaData[i].setAnalog(255.0f); }

      float f;
      this->normalizeMinToMax( mAnaData[i].getAnalog(), f );
      mAnaData[i] = f;

      mAnaData[i].setTime();     // Set the sample time
   }

   // Locks and then swaps the indices.
   mAnalogSamples.lock();
   mAnalogSamples.addSample(mAnaData);
   mAnalogSamples.unlock();

   mAnalogSamples.swapBuffers();
}

};
