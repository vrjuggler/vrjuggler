#ifndef _VJ_DEVICE_INTERFACE_H_
#define _VJ_DEVICE_INTERFACE_H_

#include <vjConfig.h>

//: Base class for simplified interfaces
//
// Interfaces provide an easier way to access proxy objects from
// within user applications. <br> <br>
//
// Users can simply declare a local interface variable and use it
// as a smart_ptr for the proxy
//
//! NOTE: The init function should be called in the init function of the user
//+         application
//!PUBLIC_API:
//------------------------------------------------------------------------------
class vjDeviceInterface
{
public:
   vjDeviceInterface() : mProxyIndex(-1)
   {;}

   //: Initialize the object
   //! ARGS: proxyName - String name of the proxy to connect to
   void init(string proxyName);

   //: Return the index of the proxy
   int getProxyIndex()
   {  return mProxyIndex; }

protected:
   int mProxyIndex;        //: The index of the proxy
};

#endif
