/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#include <gadget/gadgetConfig.h>

#include <gadget/Devices/Sim/SimRelativePosition.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/Position/PositionUnitConversion.h>

namespace gadget
{

std::string SimRelativePosition::getElementType()
{
   return "simulated_relative_position";
}

bool SimRelativePosition::config(jccl::ConfigElementPtr element)
{
   if(! (Input::config(element) && Position::config(element)))
   {
      return false;
   }

   // Initialize the positional devices
   std::string base_frame_proxy = element->getProperty<std::string>("base_frame_proxy");
   std::string relative_pos_proxy = element->getProperty<std::string>("relative_proxy");

   mBaseFrame.init(base_frame_proxy);
   mRelativePos.init(relative_pos_proxy);
   return true;
}


void SimRelativePosition::updateData()
{
   // Make sure dependencies are updated.
   mBaseFrame->updateDataIfNeeded();
   mRelativePos->updateDataIfNeeded();

   gmtl::mult( mPos.mPosData, mBaseFrame->getData(gadget::PositionUnitConversion::ConvertToMeters), 
                              mRelativePos->getData(gadget::PositionUnitConversion::ConvertToMeters) );

   // NOTE: This is a little bit of an overkill, but it works and it allows for the
   //       buffering that could be needed in multi-threaded cases
   mPos.setTime();   // Set the time

   // Set the time for the position data to the KeyboardMouse timestamp
   mPos.setTime(mRelativePos->getTimeStamp());

   addPositionSample(std::vector< gadget::PositionData>(1, mPos) );   

   swapPositionBuffers(); // Swap the buffers
}


} // End of gadget namespace
