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

#include <gadget/gadgetConfig.h>
#include <vrj/Kernel/Kernel.h>
#include <gadget/Type/AnalogProxy.h>
#include <jccl/Config/ConfigChunk.h>

namespace gadget
{

bool AnalogProxy::config(jccl::ConfigChunk* chunk)
{
   vprDEBUG_BEGIN(vrjDBG_INPUT_MGR,vprDBG_STATE_LVL) << "----------- configuring ANALOG PROXY -----------------\n" << vprDEBUG_FLUSH;
   vprASSERT(((std::string)chunk->getType()) == "AnaProxy");

   m_unitNum = chunk->getProperty("unit");
   mDeviceName = (std::string)chunk->getProperty("device");

   refresh();     // Refresh the device now that we have something to point at

   return true;
}

};
