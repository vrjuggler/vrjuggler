#ifndef _VJ_KEYBOARD_INTERFACE_H_
#define _VJ_KEYBOARD_INTERFACE_H_

#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjKeyboardProxy.h>
#include <Input/InputManager/vjDeviceInterface.h>

//: Provides simplified user interface to a keyboard device
//
// See also: vjDeviceInterface
//!PUBLIC_API:
class vjKeyboardInterface : public vjDeviceInterface
{
public:
   vjKeyboardInterface() : mKeyProxy(NULL)
   {;}

   vjKeyboardProxy* operator->()
   { return mKeyProxy; }

   vjKeyboardProxy& operator*()
   { return *(mKeyProxy); }

   virtual void refresh()
   {
      vjDeviceInterface::refresh();
      if(mProxyIndex != -1)
         mKeyProxy = vjKernel::instance()->getInputManager()->getKeyboardProxy(mProxyIndex);
      else
         mKeyProxy = NULL;
   }

private:
   vjKeyboardProxy* mKeyProxy;     // The proxy that is being wrapped
};

#endif
