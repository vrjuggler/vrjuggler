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
#include <Input/InputManager/vjGloveProxy.h>

bool vjGloveProxy::config(vjConfigChunk* chunk)
{
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,3) << "------------------ GLOVE PROXY config() -----------------\n" << vjDEBUG_FLUSH;
   vjASSERT(((std::string)chunk->getType()) == "GloveProxy");

   int unitNum = chunk->getProperty("unit");
   std::string proxy_name = chunk->getProperty("name");
   std::string dev_name = chunk->getProperty("device");

   vjInput* input_dev = vjKernel::instance()->getInputManager()->getDevice(dev_name);
   if(NULL == input_dev)       // Not found, ERROR
   {
      vjDEBUG(vjDBG_INPUT_MGR, vjDBG_CONFIG_LVL) << "vjGloveProxy::config: Could not find device: " << dev_name << std::endl << vjDEBUG_FLUSH;
      return false;
   }

   vjGlove* glove_dev = dynamic_cast<vjGlove*>(input_dev);
   if(NULL == glove_dev)
   {
      vjDEBUG(vjDBG_INPUT_MGR, vjDBG_CRITICAL_LVL) << "vjGloveProxy::config: Device was of wrong type: " << dev_name
                                               << " type:" << typeid(input_dev).name() << std::endl << vjDEBUG_FLUSH;
      return false;
   }

   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   attaching to device named: " << dev_name.c_str() << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_INPUT_MGR,vjDBG_STATE_LVL) << "   at unit number: " << unitNum << std::endl << vjDEBUG_FLUSH;
   vjDEBUG_END(vjDBG_INPUT_MGR, vjDBG_STATE_LVL) << "   GloveProxy config()'ed" << std::endl << vjDEBUG_FLUSH;

   set(glove_dev,unitNum);    // Set the proxy

   return true;
}
