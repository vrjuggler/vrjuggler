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

#include <gadget/Devices/Sim/SimSetablePosition.h>
#include <jccl/Config/ConfigChunk.h>
#include <vpr/Util/Debug.h>

namespace gadget
{

bool SimSetablePosition::config(jccl::ConfigChunkPtr chunk)
{
    if(! (Input::config(chunk) && Position::config(chunk)))
      return false;

   return true;
}


void SimSetablePosition::setData(const std::vector<PositionData> &pos_data_vec)
{
   mPos.setTime();   // Set the time
   addPositionSample(pos_data_vec);   

   swapPositionBuffers(); // Swap the buffers
}

void SimSetablePosition::setData(const PositionData& pos_data)
{
   std::vector<PositionData> pos_data_vec;
   pos_data_vec.push_back(pos_data);

   mPos.setTime();   // Set the time
   addPositionSample(pos_data_vec);   

   swapPositionBuffers(); // Swap the buffers

   //vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_DETAILED_LVL) << "SimSetablePosition: set Data:" << *(const_cast<PositionData*>(&pos_data)->getPosition()) << std::endl << vprDEBUG_FLUSH;
}

} // End of gadget namespace
