#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjPosInterface.h>


vjPosProxy* vjPosInterface::operator->()
{ return vjKernel::instance()->getInputManager()->GetPosProxy(mProxyIndex);}

vjPosProxy& vjPosInterface::operator*()
{ return *(vjKernel::instance()->getInputManager()->GetPosProxy(mProxyIndex));}
