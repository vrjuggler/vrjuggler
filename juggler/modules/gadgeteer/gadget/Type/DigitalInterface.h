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
   vjDigitalInterface() : mDigProxy(NULL)
   {;}

   vjDigitalProxy* operator->()
   { return mDigProxy; }

   vjDigitalProxy& operator*()
   { return *(mDigProxy); }

   virtual void refresh()
   {
      vjDeviceInterface::refresh();
      if(mProxyIndex != -1)
         mDigProxy = vjKernel::instance()->getInputManager()->GetDigProxy(mProxyIndex);
      else
         mDigProxy = NULL;
   }

private:
   vjDigitalProxy* mDigProxy;     // The proxy that is being wrapped
};

#endif
