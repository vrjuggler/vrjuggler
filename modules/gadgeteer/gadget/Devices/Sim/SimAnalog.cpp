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

#include <vrj/vrjConfig.h>
#include <vrj/Input/Devices/Sim/SimAnalog.h>
#include <vrj/Config/ConfigChunk.h>

namespace vrj
{

//: Default Constructor
SimAnalog::SimAnalog() : Analog(), SimInput()
{
   //vjDEBUG(vjDBG_ALL,4)<<"*** SimAnalog::SimPinchGlove()\n"<< vjDEBUG_FLUSH;
}

//: Destructor
SimAnalog::~SimAnalog()
{
   //vjDEBUG(vjDBG_ALL,4)<<"*** SimAnalog::~SimPinchGlove()\n"<< vjDEBUG_FLUSH;
}

bool SimAnalog::config(ConfigChunk* chunk)
{
   //vjDEBUG(vjDBG_ALL,4)<<"*** SimAnalog::config()\n"<< vjDEBUG_FLUSH;
   if(! (Input::config(chunk) && Analog::config(chunk) && SimInput::config(chunk)))
      return false;

   std::vector<VarValue*> key_inc_list = chunk->getAllProperties("keyPairsInc");
   std::vector<VarValue*> key_dec_list = chunk->getAllProperties("keyPairsDec");

   mSimKeysUp = readKeyList(key_inc_list);
   mSimKeysDown = readKeyList(key_dec_list);

   int num_pairs = mSimKeysUp.size();

   mAnaData = std::vector<float>( num_pairs, 0.0f ); // Initialize to all zeros
   mAnaStep = chunk->getProperty( "anastep" );

   return true;
}

void SimAnalog::updateData()
{
   //vjDEBUG(vjDBG_ALL,4)<<"*** SimAnalog::updateData()\n"<< vjDEBUG_FLUSH;

   // -- Update analog data --- //
   for (unsigned int i = 0; i < mSimKeysUp.size(); i++)
   {
      mAnaData[i] += (float)checkKeyPair(mSimKeysUp[i]) * mAnaStep;
      mAnaData[i] -= (float)checkKeyPair(mSimKeysDown[i]) * mAnaStep;

      if (mAnaData[i] < 0.0f) mAnaData[i] = 0.0f;
      if (mAnaData[i] > 255.0f) mAnaData[i] = 255.0f;
   }
}

};
