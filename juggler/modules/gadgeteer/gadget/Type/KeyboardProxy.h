/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#ifndef _VJ_KEYBOARD_PROXY_H_
#define _VJ_KEYBOARD_PROXY_H_

#include <vjConfig.h>
#include <Input/InputManager/vjProxy.h>
#include <Input/vjInput/vjKeyboard.h>

//: Proxy class to vjKeyboard based devices.
//!PUBLIC_API:
class vjKeyboardProxy : public vjProxy
{
public:
   vjKeyboardProxy()
   { mKeyboard = NULL; }

   void set(vjKeyboard* keyboard)
   {
      vjASSERT(keyboard->fDeviceSupport(DEVICE_KEYBOARD));
      mKeyboard = keyboard;
   }

   vjKeyboard* getKeyboardPtr()
   { return mKeyboard; }

   //: Determine if the modifier key is pressed exclusively.
   //! PRE: modKey is a valid modifier identifier
   //! RETURNS: true - modKey is the only modifier pressed
   bool modifierOnly(int modKey)
   { return mKeyboard->modifierOnly(modKey); }

   int keyPressed(int keyId)
   { return mKeyboard->keyPressed(keyId); }

   static std::string getChunkType() { return "KeyboardProxy"; }

   bool config(vjConfigChunk* chunk);

   virtual vjInput* getProxiedInputDevice()
   {
      vjInput* ret_val = dynamic_cast<vjInput*>(mKeyboard);
      vjASSERT(ret_val != NULL);
      return ret_val;
   }

private:
  //: Pointer to the real keyboard.
  vjKeyboard* mKeyboard;
};


#endif
