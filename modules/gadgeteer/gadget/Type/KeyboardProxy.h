/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
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
