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

#include <stdlib.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/Input.h>


namespace gadget
{

Input::Input()
   : mInstName("")
   , mThread(NULL)
   , mActive(false)
   , mNeedUpdate(true)
{
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
   if ( mNeedUpdate )
   {
      mNeedUpdate = false;
      updateData();
   }
}

} // End of gadget namespace
