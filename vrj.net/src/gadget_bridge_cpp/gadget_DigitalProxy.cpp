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
#include <gadget/Type/DigitalProxy.h>
#include <gadget_DigitalProxy_Adapter.h>

extern "C"
{
   // Copy constructor wrapper.
   SHARPPY_API gadget::DigitalProxy* gadget_DigitalProxy_DigitalProxy__gadget_DigitalProxy(const gadget_DigitalProxy_Adapter* p)
   {
      gadget_DigitalProxy_Adapter* obj = new gadget_DigitalProxy_Adapter(*p);
      obj->updateData_callback = p->updateData_callback;
      obj->config_callback_boost_shared_ptr_jccl__ConfigElement = p->config_callback_boost_shared_ptr_jccl__ConfigElement;
      obj->getProxiedInputDevice_callback = p->getProxiedInputDevice_callback;
      obj->refresh_callback = p->refresh_callback;
      obj->getDeviceName_callback = p->getDeviceName_callback;
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget_DigitalProxy_Adapter* gadget_DigitalProxy_DigitalProxy__(gadget_DigitalProxy_Adapter::updateData_callback_t cb0, gadget_DigitalProxy_Adapter::config_callback_boost_shared_ptr_jccl__ConfigElement_t cb1, gadget_DigitalProxy_Adapter::getProxiedInputDevice_callback_t cb2, gadget_DigitalProxy_Adapter::refresh_callback_t cb3, gadget_DigitalProxy_Adapter::getDeviceName_callback_t cb4)
   {
      gadget_DigitalProxy_Adapter* obj = new gadget_DigitalProxy_Adapter();
      obj->updateData_callback = cb0;
      obj->config_callback_boost_shared_ptr_jccl__ConfigElement = cb1;
      obj->getProxiedInputDevice_callback = cb2;
      obj->refresh_callback = cb3;
      obj->getDeviceName_callback = cb4;
      return obj;
   }

   SHARPPY_API void delete_gadget_DigitalProxy(gadget_DigitalProxy_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::DigitalProxy::getTimeStamp()
   SHARPPY_API vpr::Interval* gadget_DigitalProxy_getTimeStamp__(gadget_DigitalProxy_Adapter* self_)
   {
      vpr::Interval* result;
      result = new vpr::Interval(self_->getTimeStamp());

      return result;
   }

   // Wrapper for non-virtual method gadget::DigitalProxy::getData()
   SHARPPY_API int gadget_DigitalProxy_getData__(gadget_DigitalProxy_Adapter* self_)
   {
      int result;
      result = self_->getData();

      return result;
   }

   // Wrapper for non-virtual method gadget::DigitalProxy::getDigitalData()
   SHARPPY_API gadget::DigitalData* gadget_DigitalProxy_getDigitalData__(gadget_DigitalProxy_Adapter* self_)
   {
      gadget::DigitalData* result;
      result = self_->getDigitalData();

      return result;
   }

   // Wrapper for non-virtual method gadget::DigitalProxy::getDigitalPtr()
   SHARPPY_API gadget::Digital* gadget_DigitalProxy_getDigitalPtr__(gadget_DigitalProxy_Adapter* self_)
   {
      gadget::Digital* result;
      result = self_->getDigitalPtr();

      return result;
   }

   // Wrapper for non-virtual method gadget::DigitalProxy::getUnit()
   SHARPPY_API int gadget_DigitalProxy_getUnit__(gadget_DigitalProxy_Adapter* self_)
   {
      int result;
      result = self_->getUnit();

      return result;
   }


   // Wrapper for virtual method gadget::DigitalProxy::updateData()
   SHARPPY_API void gadget_DigitalProxy_updateData__(gadget_DigitalProxy_Adapter* self_)
   {
      self_->gadget::DigitalProxy::updateData();
   }

   // Wrapper for virtual method gadget::DigitalProxy::config()
   SHARPPY_API bool gadget_DigitalProxy_config__boost_shared_ptr_jccl__ConfigElement(gadget_DigitalProxy_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->gadget::DigitalProxy::config(p0);

      return result;
   }

   // Wrapper for virtual method gadget::DigitalProxy::getProxiedInputDevice()
   SHARPPY_API gadget::Input* gadget_DigitalProxy_getProxiedInputDevice__(gadget_DigitalProxy_Adapter* self_)
   {
      gadget::Input* result;
      result = self_->gadget::DigitalProxy::getProxiedInputDevice();

      return result;
   }


   // Wrapper for static method gadget::DigitalProxy::getElementType()
   SHARPPY_API char* gadget_DigitalProxy_getElementType__()
   {
      char* result;
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      temp_result = gadget::DigitalProxy::getElementType();
      result = strdup(temp_result.c_str());

      return result;
   }



} // extern "C" for gadget::DigitalProxy


