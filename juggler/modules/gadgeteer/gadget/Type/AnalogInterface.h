#ifndef _VJ_ANA_INTERFACE_H_
#define _VJ_ANA_INTERFACE_H_

#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjAnalogProxy.h>
#include <Input/InputManager/vjDeviceInterface.h>

//: Provides a simplified user interface to an analog device
//
// See also: vjDeviceInterface
//!PUBLIC_API:
class vjAnalogInterface : public vjDeviceInterface
{
public:
   vjAnalogProxy* operator->()
   { return vjKernel::instance()->getInputManager()->GetAnaProxy(mProxyIndex); }

   vjAnalogProxy& operator*()
   { return *(vjKernel::instance()->getInputManager()->GetAnaProxy(mProxyIndex)); }
};

#endif
