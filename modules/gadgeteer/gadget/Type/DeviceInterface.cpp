#include <Input/InputManager/vjDeviceInterface.h>
#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>


void vjDeviceInterface::init(string proxyName)
{
   mProxyIndex = vjKernel::instance()->getInputManager()->GetProxyIndex(proxyName);
   if (mProxyIndex == -1)
      vjDEBUG(0) << "ERROR: could not find proxy: " << proxyName << endl << vjDEBUG_FLUSH;
}

