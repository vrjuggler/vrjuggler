#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjPosInterface.h>


vjPosProxy* vjPosInterface::operator->()
{ return mPosProxy;}

vjPosProxy& vjPosInterface::operator*()
{ return *(mPosProxy);}

void vjPosInterface::refresh()
{
   vjDeviceInterface::refresh();
   if(mProxyIndex != -1)
      mPosProxy = vjKernel::instance()->getInputManager()->GetPosProxy(mProxyIndex);
   else
      mPosProxy = NULL;
}
