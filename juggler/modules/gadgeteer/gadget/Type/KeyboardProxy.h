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


#ifndef _VJ_KEYBOARD_PROXY_H_
#define _VJ_KEYBOARD_PROXY_H_

#include <vjConfig.h>
#include <Input/InputManager/vjProxy.h>
#include <Input/vjInput/vjKeyboard.h>

//: Proxy class to vjKeyboard based devices.
//!PUBLIC_API:
class VJ_CLASS_API vjKeyboardProxy : public vjTypedProxy<vjKeyboard>
{
public:
   vjKeyboardProxy()
   { ; }

   vjKeyboard* getKeyboardPtr()
   {
      if(mStupified)
         return NULL;
      else
         return mTypedDevice;
   }

   //: Determine if the modifier key is pressed exclusively.
   //! PRE: modKey is a valid modifier identifier
   //! RETURNS: true - modKey is the only modifier pressed
   bool modifierOnly(int modKey)
   {
      if(mStupified)
         return false;
      else
         return mTypedDevice->modifierOnly(modKey);
   }

   int keyPressed(int keyId)
   {
      if(mStupified)
         return 0;
      else
         return mTypedDevice->keyPressed(keyId);
   }

   static std::string getChunkType() { return "KeyboardProxy"; }

   bool config(vjConfigChunk* chunk);

   virtual vjInput* getProxiedInputDevice()
   {
      if(NULL == mTypedDevice)
         return NULL;

      vjInput* ret_val = dynamic_cast<vjInput*>(mTypedDevice);
      vjASSERT(ret_val != NULL);
      return ret_val;
   }

};


#endif
