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
   vjDeviceInterface();

   virtual ~vjDeviceInterface();

   //: Initialize the object
   //! ARGS: proxyName - String name of the proxy to connect to
   void init(std::string proxyName);

   //: Refreshes the interface based on the current configuration
   void refresh();

   //: Return the index of the proxy
   int getProxyIndex()
   {  return mProxyIndex; }

protected:
   int         mProxyIndex;         //: The index of the proxy
   std::string mProxyName;          //: The name of the proxy (or alias) we are looking at


public:
   static void refreshAllDevices();

private:    // Static information
   /* We need to keep track of all the allocated device interfaces
    * so we can update them when the system reconfigures itself
    */
   static void addDevInterface(vjDeviceInterface* dev);
   static void removeDevInterface(vjDeviceInterface* dev);

   static std::vector<vjDeviceInterface*> mAllocatedDevices;
};

#endif
