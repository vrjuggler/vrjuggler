#ifndef _VJ_DIGITAL_INTERFACE_H_
#define _VJ_DIGITAL_INTERFACE_H_

#include <Kernel/vjKernel.h>
#include <Math/vjMatrix.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjDigitalProxy.h>
#include <Input/InputManager/vjDeviceInterface.h>

//: Provides a simplified user interface to a digital device
//
// See also: vjDeviceInterface
//!PUBLIC_API:
class vjDigitalInterface : public vjDeviceInterface
{
public:
   vjDigitalProxy* operator->()
   { return vjKernel::instance()->getInputManager()->GetDigProxy(mProxyIndex); }

   vjDigitalProxy& operator*()
   { return *(vjKernel::instance()->getInputManager()->GetDigProxy(mProxyIndex)); }
};

#endif
