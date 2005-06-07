/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
#include <vpr/Util/Assert.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Util/Debug.h>
#include <gadget/Devices/Sim/SimDigitalGlove.h>

namespace gadget
{

/** Default Constructor */
SimDigitalGlove::SimDigitalGlove()
{
   //vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)<<"*** SimDigitalGlove::SimDigitalGlove()\n"<< vprDEBUG_FLUSH;
}

/** Destructor */
SimDigitalGlove::~SimDigitalGlove()
{
  // vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)<<"*** SimDigitalGlove::~SimDigitalGlove()\n"<< vprDEBUG_FLUSH;
}

std::string SimDigitalGlove::getElementType()
{
   return "simulated_digital_glove";
}

bool SimDigitalGlove::config(jccl::ConfigElementPtr element)
{
   //vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)<<"*** SimDigitalGlove::config\n"<< vprDEBUG_FLUSH;
   if (! (Input::config(element) && Digital::config(element) &&
          SimInput::config(element) && Glove::config(element)) )
   {
      return false;
   }

   //mCurGesture = 0;     // We are in no gesture yet

   std::vector<jccl::ConfigElementPtr> key_list;
   int key_count = element->getNum("key_pair");
   for ( int i = 0; i < key_count; ++i )
   {
      key_list.push_back(element->getProperty<jccl::ConfigElementPtr>("key_pair", i));
   }
   mSimKeys = readKeyList( key_list );

   return true;
}

/**
 * Updates the state of the digital data vector.
 *
 * @note Digital is on when key is held down.
 *       When key is release, digital goes to off state.
 */
void SimDigitalGlove::updateData()
{
   std::vector<DigitalData> digitalSample(10);
	
    // -- Update digital data --- //
   for (unsigned int i = 0; i < mSimKeys.size(); i++)
   {
      if (checkKeyPair( mSimKeys[i] ))             // If keys pressed
         digitalSample[i] = 1;
      else
         digitalSample[i] = 0;
   }

   addDigitalSample(digitalSample);
   swapDigitalBuffers();

   std::vector<GloveData> gloveSample=getGloveDataFromDigitalData(digitalSample);
   addGloveSample(gloveSample);
   swapGloveBuffers();
   
   return;
}

} // End of gadget namespace
