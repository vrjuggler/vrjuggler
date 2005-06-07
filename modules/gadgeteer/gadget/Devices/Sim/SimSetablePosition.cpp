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

#include <jccl/Config/ConfigElement.h>
//#include <vpr/Util/Debug.h>
#include <gadget/Devices/Sim/SimSetablePosition.h>


namespace gadget
{

std::string SimSetablePosition::getElementType()
{
   return "simulated_setable_position";
}

bool SimSetablePosition::config(jccl::ConfigElementPtr element)
{
   if(! (Input::config(element) && Position::config(element)))
   {
      return false;
   }

   return true;
}

void SimSetablePosition::setData(const std::vector<PositionData>& posDataVec)
{
   mPos.setTime();   // Set the time
   addPositionSample(posDataVec);   

   swapPositionBuffers(); // Swap the buffers
}

void SimSetablePosition::setData(const PositionData& posData)
{
   std::vector<PositionData> pos_data_vec;
   pos_data_vec.push_back(posData);

   mPos.setTime();   // Set the time
   addPositionSample(pos_data_vec);   

   swapPositionBuffers(); // Swap the buffers

   //vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL)
   //   << "[SimSetablePosition::setData()] "
   //   << *(const_cast<PositionData*>(&pos_data)->getPosition()) << std::endl
   //   << vprDEBUG_FLUSH;
}

} // End of gadget namespace
