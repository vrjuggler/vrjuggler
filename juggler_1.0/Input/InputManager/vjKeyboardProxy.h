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

   int keyPressed(int keyId)
   { return mKeyboard->keyPressed(keyId); }

   static std::string getChunkType() { return "KeyboardProxy"; }

   bool config(vjConfigChunk* chunk);

private:
  //: Pointer to the real keyboard.
  vjKeyboard* mKeyboard;
};


#endif

