// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/DeviceInterface.h>
#include <gadget_BaseDeviceInterface_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget_BaseDeviceInterface_Adapter* gadget_BaseDeviceInterface_BaseDeviceInterface__gadget_BaseDeviceInterface1(const gadget::BaseDeviceInterface* p0, gadget_BaseDeviceInterface_Adapter::refresh_callback_t cb0)
   {
      const gadget::BaseDeviceInterface& marshal_p0 = *p0;
      gadget_BaseDeviceInterface_Adapter* obj = new gadget_BaseDeviceInterface_Adapter(marshal_p0);
      obj->refresh_callback = cb0;
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget_BaseDeviceInterface_Adapter* gadget_BaseDeviceInterface_BaseDeviceInterface__0(gadget_BaseDeviceInterface_Adapter::refresh_callback_t cb0)
   {
      gadget_BaseDeviceInterface_Adapter* obj = new gadget_BaseDeviceInterface_Adapter();
      obj->refresh_callback = cb0;
      return obj;
   }

   SHARPPY_API void delete_gadget_BaseDeviceInterface(gadget_BaseDeviceInterface_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::BaseDeviceInterface::init()
   SHARPPY_API void gadget_BaseDeviceInterface_init__std_string1(gadget_BaseDeviceInterface_Adapter* self_, const char* p0)
   {
      self_->init(p0);
   }


   // Wrapper for non-virtual method gadget::BaseDeviceInterface::getProxyName()
   SHARPPY_API char* gadget_BaseDeviceInterface_getProxyName__0(gadget_BaseDeviceInterface_Adapter* self_)
   {
      std::string temp_result;
      char* result;
      temp_result = self_->getProxyName();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method gadget::BaseDeviceInterface::isConnected()
   SHARPPY_API bool gadget_BaseDeviceInterface_isConnected__0(gadget_BaseDeviceInterface_Adapter* self_)
   {
      bool result;
      result = self_->isConnected();
      return result;
   }


   // Wrapper for virtual method gadget::BaseDeviceInterface::refresh()
   SHARPPY_API void gadget_BaseDeviceInterface_refresh__0(gadget_BaseDeviceInterface_Adapter* self_)
   {
      self_->gadget::BaseDeviceInterface::refresh();
   }


   // Wrapper for static method gadget::BaseDeviceInterface::refreshAllDevices()
   SHARPPY_API void gadget_BaseDeviceInterface_refreshAllDevices__0()
   {
      gadget::BaseDeviceInterface::refreshAllDevices();
   }


} // extern "C" for gadget::BaseDeviceInterface


