#ifndef _VJ_POS_INTERFACE_H_
#define _VJ_POS_INTERFACE_H_

#include <Input/InputManager/vjPosProxy.h>
#include <Input/InputManager/vjDeviceInterface.h>

//: Provides a simplified user interface to a pos device
//
// See Also: vjDeviceInterface
//!PUBLIC_API:
class vjPosInterface : public vjDeviceInterface
{
public:
   vjPosProxy* operator->();
   vjPosProxy& operator*();
};

#endif
