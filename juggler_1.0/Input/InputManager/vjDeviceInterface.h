#ifndef _VJ_DEVICE_INTERFACE_H_
#define _VJ_DEVICE_INTERFACE_H_

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

#include <mstring.h>

class vjDeviceInterface
{
public:
   vjDeviceInterface() : mProxyIndex(-1)
   {;}

   //: Initialize the object
   //  Set the object to reference the proxy named proxyName
   void init(string proxyName);

   int getProxyIndex()
   {  return mProxyIndex; }
protected:
   int mProxyIndex;        // The index of the proxy
};

#endif
