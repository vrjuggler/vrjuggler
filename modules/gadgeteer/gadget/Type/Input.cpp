/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/gadgetConfig.h>

#include <stdlib.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/Input.h>


namespace gadget
{

Input::Input()
   : mInstName("")
   , mHardwareName("")
   , mThread(NULL)
   , mActive(false)
   , mNeedUpdate(true)
{
   /* Do nothing. */ ;
}

Input::~Input()
{
   /* Do nothing. */ ;
}

bool Input::config(jccl::ConfigElementPtr e)
{
  //if((!mPort.empty()) && (!mInstName.empty()))
  //{
     // ASSERT: We have already been configured
     //         this prevents config from being called multiple times (once for each derived class)
     //         ie. Digital, Analog, etc
  //   return true;
  //}

  mInstName = e->getFullName();

  return true;
}

void Input::updateDataIfNeeded()
{
   if (mNeedUpdate)
   {
      mNeedUpdate = false;
      updateData();
   }
}

/**
 * Returns the name identifying this instance of the device.
 * This is the name given to the device in its config element (e.g.,
 * "MyFlockOfBirds", "The Ibox", etc.).
 */
const std::string Input::getInstanceName() const
{
   return mInstName.empty() ? "Undefined" : mInstName;
}

const std::string Input::getHardwareName() const
{
   return mHardwareName.empty() ? "Not Set" : mHardwareName;
}

void Input::writeObject(vpr::ObjectWriter*)
{
   /* Do nothing. */ ;
}

void Input::readObject(vpr::ObjectReader*)
{
   /* Do nothing. */ ;
}

} // End of gadget namespace
