#ifndef _VJ_POS_INTERFACE_H_
#define _VJ_POS_INTERFACE_H_

#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjPosProxy.h>
#include <Input/InputManager/vjDeviceInterface.h>

//: Provides a simplified user interface to a pos device
//
// See Also: vjDeviceInterface
class vjPosInterface : public vjDeviceInterface
{
public:
   vjPosProxy* operator->()
   { return vjKernel::instance()->getInputManager()->GetPosProxy(mProxyIndex); }

   vjPosProxy& operator*()
   { return *(vjKernel::instance()->getInputManager()->GetPosProxy(mProxyIndex)); }
};

#endif
