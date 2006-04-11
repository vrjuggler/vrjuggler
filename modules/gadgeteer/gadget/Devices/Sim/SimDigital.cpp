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

#include <gadget/gadgetConfig.h>
#include <jccl/Config/ConfigChunk.h>
#include <gadget/Devices/Sim/SimDigital.h>

namespace gadget
{

/** Default Constructor */
SimDigital::SimDigital()
{
   vprDEBUG(vprDBG_ALL,4)<<"*** SimDigital::SimDigital()\n"<< vprDEBUG_FLUSH;
}

/** Destructor */
SimDigital::~SimDigital()
{
   //vprDEBUG(vprDBG_ALL,4)<<"*** SimDigital::~SimDigital()\n"<< vprDEBUG_FLUSH;
}

bool SimDigital::config(jccl::ConfigChunkPtr chunk)
{
   //vprDEBUG(vprDBG_ALL,4)<<"*** SimDigital::config()\n"<< vprDEBUG_FLUSH;
   if(! (Input::config(chunk) && Digital::config(chunk) && SimInput::config(chunk)))
      return false;

   std::vector<jccl::ConfigChunkPtr> key_list;
   int key_count = chunk->getNum("keyPairs");
   for ( int i = 0; i < key_count; ++i )
   {
      key_list.push_back(chunk->getProperty<jccl::ConfigChunkPtr>("keyPairs", i));
   }
   mSimKeys = readKeyList(key_list);

   return true;
}

/**
 * Updates the state of the digital data vector.
 *
 * @note Digital is on when key is held down.
 *       When key is release, digital goes to off state.
 */
void SimDigital::updateData()
{
   //vprDEBUG(vprDBG_ALL,4)<<"*** SimDigital::updateData()\n"<< vprDEBUG_FLUSH;
   std::vector<DigitalData>  digital_data_sample(mSimKeys.size());   // The digital data that makes up the sample

   // -- Update digital data --- //
   for (unsigned int i = 0; i < mSimKeys.size(); i++)
   {
      digital_data_sample[i].setTime();
      if(checkKeyPair(mSimKeys[i]))             // If keys pressed
         digital_data_sample[i] = 1;
      else
         digital_data_sample[i] = 0;
   }

   // Add a sample
   addDigitalSample(digital_data_sample);
      
   swapDigitalBuffers();
}

} // End of gadget namespace
