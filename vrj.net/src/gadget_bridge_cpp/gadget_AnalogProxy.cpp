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

// Generated from $Revision$ of $RCSfile$



#include "sharppy.h"
#include <gadget/Type/AnalogProxy.h>
#include <gadget_AnalogProxy_Adapter.h>

extern "C"
{
   // Copy constructor wrapper.
   SHARPPY_API gadget::AnalogProxy* gadget_AnalogProxy_AnalogProxy__gadget_AnalogProxy(const gadget_AnalogProxy_Adapter* p)
   {
      gadget_AnalogProxy_Adapter* obj = new gadget_AnalogProxy_Adapter(*p);
      obj->updateData_callback = p->updateData_callback;
      obj->config_callback_boost_shared_ptr_jccl__ConfigElement = p->config_callback_boost_shared_ptr_jccl__ConfigElement;
      obj->getProxiedInputDevice_callback = p->getProxiedInputDevice_callback;
      obj->refresh_callback = p->refresh_callback;
      obj->getDeviceName_callback = p->getDeviceName_callback;
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget_AnalogProxy_Adapter* gadget_AnalogProxy_AnalogProxy__(gadget_AnalogProxy_Adapter::updateData_callback_t cb0, gadget_AnalogProxy_Adapter::config_callback_boost_shared_ptr_jccl__ConfigElement_t cb1, gadget_AnalogProxy_Adapter::getProxiedInputDevice_callback_t cb2, gadget_AnalogProxy_Adapter::refresh_callback_t cb3, gadget_AnalogProxy_Adapter::getDeviceName_callback_t cb4)
   {
      gadget_AnalogProxy_Adapter* obj = new gadget_AnalogProxy_Adapter();
      obj->updateData_callback = cb0;
      obj->config_callback_boost_shared_ptr_jccl__ConfigElement = cb1;
      obj->getProxiedInputDevice_callback = cb2;
      obj->refresh_callback = cb3;
      obj->getDeviceName_callback = cb4;
      return obj;
   }

   SHARPPY_API void delete_gadget_AnalogProxy(gadget_AnalogProxy_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::AnalogProxy::getTimeStamp()
   SHARPPY_API vpr::Interval* gadget_AnalogProxy_getTimeStamp__(gadget_AnalogProxy_Adapter* self_)
   {
      vpr::Interval* result;
      result = new vpr::Interval(self_->getTimeStamp());

      return result;
   }

   // Wrapper for non-virtual method gadget::AnalogProxy::getData()
   SHARPPY_API float gadget_AnalogProxy_getData__(gadget_AnalogProxy_Adapter* self_)
   {
      float result;
      result = self_->getData();

      return result;
   }

   // Wrapper for non-virtual method gadget::AnalogProxy::getAnalogPtr()
   SHARPPY_API gadget::Analog* gadget_AnalogProxy_getAnalogPtr__(gadget_AnalogProxy_Adapter* self_)
   {
      gadget::Analog* result;
      result = self_->getAnalogPtr();

      return result;
   }

   // Wrapper for non-virtual method gadget::AnalogProxy::getUnit()
   SHARPPY_API int gadget_AnalogProxy_getUnit__(gadget_AnalogProxy_Adapter* self_)
   {
      int result;
      result = self_->getUnit();

      return result;
   }


   // Wrapper for virtual method gadget::AnalogProxy::updateData()
   SHARPPY_API void gadget_AnalogProxy_updateData__(gadget_AnalogProxy_Adapter* self_)
   {
      self_->gadget::AnalogProxy::updateData();
   }

   // Wrapper for virtual method gadget::AnalogProxy::config()
   SHARPPY_API bool gadget_AnalogProxy_config__boost_shared_ptr_jccl__ConfigElement(gadget_AnalogProxy_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->gadget::AnalogProxy::config(p0);

      return result;
   }

   // Wrapper for virtual method gadget::AnalogProxy::getProxiedInputDevice()
   SHARPPY_API gadget::Input* gadget_AnalogProxy_getProxiedInputDevice__(gadget_AnalogProxy_Adapter* self_)
   {
      gadget::Input* result;
      result = self_->gadget::AnalogProxy::getProxiedInputDevice();

      return result;
   }


   // Wrapper for static method gadget::AnalogProxy::getElementType()
   SHARPPY_API char* gadget_AnalogProxy_getElementType__()
   {
      char* result;
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      temp_result = gadget::AnalogProxy::getElementType();
      result = strdup(temp_result.c_str());

      return result;
   }



} // extern "C" for gadget::AnalogProxy


