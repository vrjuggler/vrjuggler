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


#include <vjConfig.h>
#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjDigitalProxy.h>
#include <Config/vjConfigChunk.h>

bool vjDigitalProxy::config(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,3) << "----------- configuring DIGITAL proxy ----\n" << vjDEBUG_FLUSH;
   vjASSERT(((std::string)chunk->getType()) == "DigProxy");

   m_unitNum = chunk->getProperty("unit");
   mDeviceName = (std::string)chunk->getProperty("device");

   refresh();
   return true;
}



void vjDigitalProxy::updateData()
{
   if(!mStupified)
   {
      int new_state = mTypedDevice->getDigitalData(m_unitNum);
       int old_state = m_data;
       if(vjDigital::OFF == old_state)
      {
          if(new_state)     // Button now pressed
         m_data = vjDigital::TOGGLE_ON;
          else              // Button now released
         m_data = vjDigital::OFF;
      }
       else if(vjDigital::ON == old_state)
      {
          if(new_state)     // Button now pressed
         m_data = vjDigital::ON;
          else              // Button now released
         m_data = vjDigital::TOGGLE_OFF;
      }
       else if(vjDigital::TOGGLE_ON == old_state)
      {
          if(new_state)     // Button now pressed
         m_data = vjDigital::ON;
          else              // Button now released
         m_data = vjDigital::TOGGLE_OFF;
      }
       else if(vjDigital::TOGGLE_OFF == old_state)
      {
          if(new_state)     // Button now pressed
         m_data = vjDigital::TOGGLE_ON;
          else              // Button now released
         m_data = vjDigital::OFF;
      }
   }
}
