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

#ifndef _VRJ_KEYBOARD_PROXY_H_
#define _VRJ_KEYBOARD_PROXY_H_

#include <gad/gadConfig.h>
#include <gad/Type/Proxy.h>
#include <gad/Type/Keyboard.h>

namespace vrj
{

//: Proxy class to Keyboard based devices.
//!PUBLIC_API:
class GAD_CLASS_API KeyboardProxy : public TypedProxy<Keyboard>
{
public:
   KeyboardProxy()
   { ; }

   Keyboard* getKeyboardPtr()
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

   bool config(ConfigChunk* chunk);

   virtual Input* getProxiedInputDevice()
   {
      if(NULL == mTypedDevice)
         return NULL;

      Input* ret_val = dynamic_cast<Input*>(mTypedDevice);
      vprASSERT(ret_val != NULL);
      return ret_val;
   }

};


};

#endif
