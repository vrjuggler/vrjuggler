#ifndef _VJ_KEYBOARD_PROXY_H_
#define _VJ_KEYBOARD_PROXY_H_

#include <vjConfig.h>
#include <Input/vjInput/vjKeyboard.h>

//: Proxy class to vjKeyboard based devices.
class vjKeyboardProxy
{
public:
   vjKeyboardProxy(vjKeyboard* keyboard)
   { Set(keyboard); }

   void Set(vjKeyboard* keyboard)
   {
      vjASSERT(keyboard->FDeviceSupport(DEVICE_KEYBOARD));
      mKeyboard = keyboard;
   }

   vjKeyboard* getKeyboardPtr()
   { return mKeyboard; }

   //: Determine if the modifier key is pressed exclusively.
   //! PRE: modKey is a valid modifier identifier
   //! RETURNS: true - modKey is the only modifier pressed
   bool modifierOnly(int modKey)
   { return mKeyboard->modifierOnly(modKey); }

   int keyPressed(int ret_val, int keyId)
   { return mKeyboard->keyPressed(keyId); }

private:
  //: Pointer to the real keyboard.
  vjKeyboard* mKeyboard;
};


#endif

