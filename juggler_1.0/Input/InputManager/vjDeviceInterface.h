#ifndef _VJ_DEVICE_INTERFACE_H_
#define _VJ_DEVICE_INTERFACE_H_


#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>

//: Base class for simplified interfaces
//
// It provides an easier interface to proxy objects from
// within user applications.
//
// Users can simply declare a local interface variable and use it
// as a smart_ptr for the proxy
//
//! NOTE: The init function should be called in the init function of the user 
//+         application
class vjDeviceInterface
{
public:
   vjDeviceInterface() : mProxyIndex(-1)
   {;}

   //: Initialize the object
   //  Set the object to reference the proxy named proxyName
   void init(string proxyName)
   {
      mProxyIndex = vjKernel::instance()->getInputManager()->GetProxyIndex(proxyName);
      if(mProxyIndex == -1)
         vjDEBUG(0) << "ERROR: could not find proxy: " << proxyName << endl << vjDEBUG_FLUSH;
   }

protected:
   int mProxyIndex;        // The index of the proxy
};

#endif
