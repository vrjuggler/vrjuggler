/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
#ifndef _VJ_DEVICE_INTERFACE_H_
#define _VJ_DEVICE_INTERFACE_H_

#include <vjConfig.h>
#include <Utils/vjDebug.h>

namespace vrj
{
   
class Proxy;
class AnalogProxy;
class DigitalProxy;
class GestureProxy;
class GloveProxy;
class KeyboardProxy;
class PosProxy;

//: Base class for simplified proxy interfaces
//
// Wrapper to provide an easier way to access proxy objects from
// within user applications. <br> <br>
//
// Users can simply declare a local interface variable and use it
// as a smart_ptr for the proxy
//
//! NOTE: The init function should be called in the init function of the user
//+         application
//!PUBLIC_API:
//------------------------------------------------------------------------------
class VJ_CLASS_API BaseDeviceInterface
{
protected:
   BaseDeviceInterface(const BaseDeviceInterface& other) {;}

public:
   BaseDeviceInterface();

   virtual ~BaseDeviceInterface();

   //: Initialize the object
   //! ARGS: proxyName - String name of the proxy to connect to
   void init(std::string proxyName);

   //: Refreshes the interface based on the current configuration
   //! POST: (mProxyIndex == -1) ==> Proxy not initi.ized yet
   //+       (mProxyIndex != -1) ==> mProxyName has name of device && local proxy ptr is set to the device
   virtual void refresh();

   //: Return the name of the proxy
   std::string getProxyName()
   { return mProxyName; }

   bool isConnected()
   { return (NULL != mProxyPtr); }

protected:
   Proxy*    mProxyPtr;     //: Ptr to the proxy
   std::string mProxyName;    //: The name of the proxy (or alias) we are looking at
   bool        mNameSet;      //: Has the user set a name??

public:
   static void refreshAllDevices();

private:    // Static information
   /* We need to keep track of all the allocated device interfaces
    * so we can update them when the system reconfigures itself
    */
   static void addDevInterface(BaseDeviceInterface* dev);
   static void removeDevInterface(BaseDeviceInterface* dev);

   static std::vector<BaseDeviceInterface*> mAllocatedDevices;
};


// ---- Type specific interfaces ----

template<class PROXY_TYPE>
class VJ_CLASS_API DeviceInterface : public BaseDeviceInterface
{
protected:
   DeviceInterface(DeviceInterface& other) {;}

public:
   DeviceInterface() : mTypeSpecificProxy(NULL)
   {;}

   PROXY_TYPE* operator->()
   { return mTypeSpecificProxy; }

   PROXY_TYPE& operator*()
   { return *(mTypeSpecificProxy); }

   virtual void refresh()
   {
      BaseDeviceInterface::refresh();
      if(mProxyPtr != NULL)
      {
         mTypeSpecificProxy = dynamic_cast<PROXY_TYPE*>(mProxyPtr);
         if(NULL == mTypeSpecificProxy)
         {
            vjDEBUG(vjDBG_INPUT_MGR, vjDBG_CRITICAL_LVL) << "vjAnalogInterface: Tried to point at proxy of incorrect type named: " << mProxyName << std::endl << vjDEBUG_FLUSH;
         }
      }

      if((NULL == mProxyPtr) || (NULL == mTypeSpecificProxy))
      {
         mTypeSpecificProxy = &mDummyProxy;
      }
   }

private:
   PROXY_TYPE*    mTypeSpecificProxy;    // The proxy that is being wrapped
   PROXY_TYPE     mDummyProxy;
};


// --- Typedefs to the old types --- //
typedef DeviceInterface<AnalogProxy>     AnalogInterface;
typedef DeviceInterface<DigitalProxy>    DigitalInterface;
typedef DeviceInterface<GestureProxy>    GestureInterface;
typedef DeviceInterface<GloveProxy>      GloveInterface;
typedef DeviceInterface<KeyboardProxy>   KeyboardInterface;
typedef DeviceInterface<PosProxy>        PosInterface;

} // end namespace

#endif
