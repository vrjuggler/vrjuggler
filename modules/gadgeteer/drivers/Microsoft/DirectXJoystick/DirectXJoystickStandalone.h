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

#ifndef _GADGET_DIRECTX_JOYSTICK_STANDALONE_H_
#define _GADGET_DIRECTX_JOYSTICK_STANDALONE_H_

#include <windows.h>
#include <dinput.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>

#define INITGUID

namespace gadget
{

class DirectXJoystickStandalone
{
public:
   enum EventMasks
   {
      EVENT_AXIS_MASK         = 0x000000ff,
      EVENT_AXIS_BITOFFSET    = 0,
      EVENT_BUTTON_MASK       = 0x0000ff00,
      EVENT_BUTTON_BITOFFSET  = 8,
      EVENT_KEY_MASK          = 0x00ff0000,
      EVENT_KEY_BITOFFSET     = 16,
      EVENT_HAT_MASK          = 0x0f000000,
      EVENT_HAT_BITOFFSET     = 24,
      EVENT_RELAXIS_MASK      = 0xf0000000,
      EVENT_RELAXIS_BITOFFSET = 28,
   };

   class Event
   {
      int   code;
      float value;
   };

   DirectXJoystickStandalone();

   HRESULT init();

   DIJOYSTATE  getData();

   void close();

   HRESULT poll();

   DWORD getType();

   std::string getProductName();

private:
   DWORD mType;
   std::string mProductName;
};

}

#endif
