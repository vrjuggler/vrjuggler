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
   vjAnalogInterface() : mAnaProxy(NULL)
   {;}

   vjAnalogProxy* operator->()
   { return mAnaProxy; }

   vjAnalogProxy& operator*()
   { return *(mAnaProxy); }

   virtual void refresh()
   {
      vjDeviceInterface::refresh();
      if(mProxyIndex != -1)
         mAnaProxy = vjKernel::instance()->getInputManager()->GetAnaProxy(mProxyIndex);
      else
         mAnaProxy = NULL;
   }

private:
   vjAnalogProxy* mAnaProxy;     // The proxy that is being wrapped
};

#endif
