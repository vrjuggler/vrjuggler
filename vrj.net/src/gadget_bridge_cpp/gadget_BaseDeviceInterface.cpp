/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VRJ.NET is (C) Copyright 2004 by Patrick Hartling
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

// Generated from Revision: 1.62 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/DeviceInterface.h>
#include <gadget_BaseDeviceInterface_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget_BaseDeviceInterface_Adapter* gadget_BaseDeviceInterface_BaseDeviceInterface__gadget_BaseDeviceInterface(const gadget::BaseDeviceInterface* p0, gadget_BaseDeviceInterface_Adapter::refresh_callback_t cb0)
   {
      const gadget::BaseDeviceInterface& marshal_p0 = *p0;
      gadget_BaseDeviceInterface_Adapter* obj = new gadget_BaseDeviceInterface_Adapter(marshal_p0);
      obj->refresh_callback = cb0;
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget_BaseDeviceInterface_Adapter* gadget_BaseDeviceInterface_BaseDeviceInterface__(gadget_BaseDeviceInterface_Adapter::refresh_callback_t cb0)
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
   SHARPPY_API void gadget_BaseDeviceInterface_init__std_basic_string_char_std__char_traits_char__std__allocator_char__1(gadget_BaseDeviceInterface_Adapter* self_, const char* p0)
   {
      self_->init(p0);
   }


   // Wrapper for non-virtual method gadget::BaseDeviceInterface::getProxyName()
   SHARPPY_API char* gadget_BaseDeviceInterface_getProxyName__0(gadget_BaseDeviceInterface_Adapter* self_)
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
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


