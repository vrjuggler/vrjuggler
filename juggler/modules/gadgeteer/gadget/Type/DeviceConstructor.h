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

#ifndef _GADGET_DEVICE_CONSTRUCTOR_H_
#define _GADGET_DEVICE_CONSTRUCTOR_H_

#include <gadget/gadgetConfig.h>

#include <string>
#include <vpr/Util/Assert.h>
#include <jccl/Config/ConfigChunk.h>

#include <gadget/InputManager.h>
#include <gadget/Type/DeviceFactory.h>
#include <gadget/Type/DeviceConstructorBase.h>


namespace gadget
{

template <class DEV>
class DeviceConstructor : public DeviceConstructorBase
{
public:
   DeviceConstructor(gadget::InputManager* inputMgr)
   {
      vprASSERT(DeviceFactory::instance() != NULL);
      vprASSERT(inputMgr != NULL);
      inputMgr->getDeviceFactory()->registerDevice(this);
   }

   Input* createDevice(jccl::ConfigChunkPtr chunk)
   {
      DEV* new_dev = new DEV;
      bool success = new_dev->config(chunk);
      if(success)
      {
         return new_dev;
      }
      else
      {
         delete new_dev;
         return NULL;
      }
   }

   virtual std::string getChunkType()
   {
      return DEV::getChunkType();
   }
};

} // End of gadget namespace


#endif
