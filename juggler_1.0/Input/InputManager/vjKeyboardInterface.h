#ifndef _VJ_KEYBOARD_INTERFACE_H_
#define _VJ_KEYBOARD_INTERFACE_H_

#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjKeyboardProxy.h>
#include <Input/InputManager/vjDeviceInterface.h>

//: Provides simplified user interface to a keyboard device
//
// See also: vjDeviceInterface
//!PUBLIC_API
class vjKeyboardInterface : public vjDeviceInterface
{
public:
   vjKeyboardProxy* operator->()
   { return vjKernel::instance()->getInputManager()->GetKeyboardProxy(mProxyIndex); }

   vjKeyboardProxy& operator*()
   { return *(vjKernel::instance()->getInputManager()->GetKeyboardProxy(mProxyIndex)); }
};

#endif
