#ifndef _VJ_GESTURE_INTERFACE_H_
#define _VJ_GESTURE_INTERFACE_H_

#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjGestureProxy.h>
#include <Input/InputManager/vjDeviceInterface.h>

//: Provides a simplified user interface to an gesture device
//
// See also: vjDeviceInterface
//!PUBLIC_API:
class vjGestureInterface : public vjDeviceInterface
{
public:
   vjGestureProxy* operator->()
   { return vjKernel::instance()->getInputManager()->GetGestureProxy(mProxyIndex); }

   vjGestureProxy& operator*()
   { return *(vjKernel::instance()->getInputManager()->GetGestureProxy(mProxyIndex)); }
};

#endif
