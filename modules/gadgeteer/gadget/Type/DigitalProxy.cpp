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
#include <gadget/Util/Debug.h>
#include <gadget/Type/DigitalProxy.h>

namespace gadget
{

bool DigitalProxy::config(jccl::ConfigChunkPtr chunk)
{
vpr::DebugOutputGuard dbg_output(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL,
                              std::string("----------- configuring DIGITAL PROXY -----------------\n"),
                              std::string("----------- exit: configuring digital proxy -----------\n"));

   vprASSERT(chunk->getDescToken() == "DigProxy");

   // if we are going to be receiving remote data, we need to connect to the remote device through a NetInput
   std::string location = chunk->getProperty<std::string>("location");
   if(location.size() > 0)
   {
      mDeviceName = chunk->getFullName();
      mDeviceName += "_NET_";
      m_unitNum = 0;
   }
   else  // normal local devices
   {
      m_unitNum = chunk->getProperty<int>("unit");
      mDeviceName = chunk->getProperty<std::string>("device");
   }

   refresh();
   return true;
}



void DigitalProxy::updateData()
{

   if (!isStupified())
   {
      int old_state = m_data.getDigital();
      m_data = mTypedDevice->getDigitalData(m_unitNum);
      int new_state = m_data.getDigital();

      if (Digital::OFF == old_state)
      {
         if (new_state)     // Button now pressed
            m_data = Digital::TOGGLE_ON;
         else              // Button now released
            m_data = Digital::OFF;
      }
      else if (Digital::ON == old_state)
      {
         if (new_state)     // Button now pressed
            m_data = Digital::ON;
         else              // Button now released
            m_data = Digital::TOGGLE_OFF;
      }
      else if (Digital::TOGGLE_ON == old_state)
      {
         if (new_state)     // Button now pressed
            m_data = Digital::ON;
         else              // Button now released
            m_data = Digital::TOGGLE_OFF;
      }
      else if (Digital::TOGGLE_OFF == old_state)
      {
         if (new_state)     // Button now pressed
            m_data = Digital::TOGGLE_ON;
         else              // Button now released
            m_data = Digital::OFF;
      }
   }
}

} // End of gadget namespace
