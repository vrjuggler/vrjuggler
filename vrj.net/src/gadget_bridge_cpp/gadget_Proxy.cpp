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

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/Proxy.h>
#include <gadget_Proxy_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget_Proxy_Adapter* gadget_Proxy_Proxy__0(gadget_Proxy_Adapter::config_callback_boost_shared_ptr_jccl__ConfigElement_t cb0, gadget_Proxy_Adapter::refresh_callback_t cb1, gadget_Proxy_Adapter::updateData_callback_t cb2, gadget_Proxy_Adapter::getProxiedInputDevice_callback_t cb3, gadget_Proxy_Adapter::isStupified_callback_t cb4, gadget_Proxy_Adapter::getTimeStamp_callback_t cb5)
   {
      gadget_Proxy_Adapter* obj = new gadget_Proxy_Adapter();
      obj->config_callback_boost_shared_ptr_jccl__ConfigElement = cb0;
      obj->refresh_callback = cb1;
      obj->updateData_callback = cb2;
      obj->getProxiedInputDevice_callback = cb3;
      obj->isStupified_callback = cb4;
      obj->getTimeStamp_callback = cb5;
      return obj;
   }

   SHARPPY_API void delete_gadget_Proxy(gadget_Proxy_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::Proxy::getName()
   SHARPPY_API char* gadget_Proxy_getName__0(gadget_Proxy_Adapter* self_)
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      char* result;
      temp_result = self_->getName();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method gadget::Proxy::setName()
   SHARPPY_API void gadget_Proxy_setName__std_basic_string_char_std__char_traits_char__std__allocator_char__1(gadget_Proxy_Adapter* self_, char* p0)
   {
      self_->setName(p0);
   }


   // Wrapper for non-virtual method gadget::Proxy::stupify()
   SHARPPY_API void gadget_Proxy_stupify__bool0(gadget_Proxy_Adapter* self_)
   {
      self_->stupify();
   }

   // Wrapper for non-virtual method gadget::Proxy::stupify()
   SHARPPY_API void gadget_Proxy_stupify__bool1(gadget_Proxy_Adapter* self_, bool p0)
   {
      self_->stupify(p0);
   }


   // Wrapper for virtual method gadget::Proxy::config()
   SHARPPY_API bool gadget_Proxy_config__boost_shared_ptr_jccl__ConfigElement1(gadget_Proxy_Adapter* self_, boost::shared_ptr<jccl::ConfigElement>* p0)
   {
      bool result;
      result = self_->gadget::Proxy::config(*p0);
      return result;
   }


   // Wrapper for virtual method gadget::Proxy::refresh()
   SHARPPY_API bool gadget_Proxy_refresh__0(gadget_Proxy_Adapter* self_)
   {
      bool result;
      result = self_->refresh();
      return result;
   }


   // Wrapper for virtual method gadget::Proxy::updateData()
   SHARPPY_API void gadget_Proxy_updateData__0(gadget_Proxy_Adapter* self_)
   {
      self_->gadget::Proxy::updateData();
   }


   // Wrapper for virtual method gadget::Proxy::getProxiedInputDevice()
   SHARPPY_API gadget::Input* gadget_Proxy_getProxiedInputDevice__0(gadget_Proxy_Adapter* self_)
   {
      gadget::Input* result;
      result = self_->getProxiedInputDevice();
      return result;
   }


   // Wrapper for virtual method gadget::Proxy::isStupified()
   SHARPPY_API bool gadget_Proxy_isStupified__0(gadget_Proxy_Adapter* self_)
   {
      bool result;
      result = self_->gadget::Proxy::isStupified();
      return result;
   }


   // Wrapper for virtual method gadget::Proxy::getTimeStamp()
   SHARPPY_API vpr::Interval* gadget_Proxy_getTimeStamp__0(gadget_Proxy_Adapter* self_)
   {
      vpr::Interval* result;
      result = new vpr::Interval(self_->getTimeStamp());
      return result;
   }


   // Wrapper for static method gadget::Proxy::getElementType()
   SHARPPY_API char* gadget_Proxy_getElementType__0()
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      char* result;
      temp_result = gadget::Proxy::getElementType();
      result = strdup(temp_result.c_str());
      return result;
   }


} // extern "C" for gadget::Proxy


