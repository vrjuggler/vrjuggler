#ifndef _VJ_GLOVE_INTERFACE_H_
#define _VJ_GLOVE_INTERFACE_H_

#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjGloveProxy.h>
#include <Input/InputManager/vjDeviceInterface.h>

//: Provides a simplified user interface to an glove device
//
// See also: vjDeviceInterface
//!PUBLIC_API:
class vjGloveInterface : public vjDeviceInterface
{
public:
   vjGloveProxy* operator->()
   { return vjKernel::instance()->getInputManager()->GetGloveProxy(mProxyIndex); }

   vjGloveProxy& operator*()
   { return *(vjKernel::instance()->getInputManager()->GetGloveProxy(mProxyIndex)); }
};

#endif
