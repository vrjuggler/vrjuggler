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

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/Position.h>
#include <gadget/Type/Proxy.h>
#include <gadget_TypedProxy_gadget__Position_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget_TypedProxy_gadget__Position_Adapter* gadget_TypedProxy_gadget_Position__TypedProxy__0(gadget_TypedProxy_gadget__Position_Adapter::refresh_callback_t cb0, gadget_TypedProxy_gadget__Position_Adapter::getDeviceName_callback_t cb1, gadget_TypedProxy_gadget__Position_Adapter::config_callback_boost_shared_ptr_jccl__ConfigElement_t cb2, gadget_TypedProxy_gadget__Position_Adapter::updateData_callback_t cb3, gadget_TypedProxy_gadget__Position_Adapter::getProxiedInputDevice_callback_t cb4, gadget_TypedProxy_gadget__Position_Adapter::isStupified_callback_t cb5)
   {
      gadget_TypedProxy_gadget__Position_Adapter* obj = new gadget_TypedProxy_gadget__Position_Adapter();
      obj->refresh_callback = cb0;
      obj->getDeviceName_callback = cb1;
      obj->config_callback_boost_shared_ptr_jccl__ConfigElement = cb2;
      obj->updateData_callback = cb3;
      obj->getProxiedInputDevice_callback = cb4;
      obj->isStupified_callback = cb5;
      return obj;
   }

   SHARPPY_API void delete_gadget_TypedProxy_gadget__Position(gadget_TypedProxy_gadget__Position_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for virtual method gadget::TypedProxy<gadget::Position>::refresh()
   SHARPPY_API bool gadget_TypedProxy_gadget_Position__refresh__0(gadget_TypedProxy_gadget__Position_Adapter* self_)
   {
      bool result;
      result = self_->gadget::TypedProxy<gadget::Position>::refresh();
      return result;
   }


   // Wrapper for virtual method gadget::TypedProxy<gadget::Position>::getDeviceName()
   SHARPPY_API char* gadget_TypedProxy_gadget_Position__getDeviceName__0(gadget_TypedProxy_gadget__Position_Adapter* self_)
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      char* result;
      temp_result = self_->gadget::TypedProxy<gadget::Position>::getDeviceName();
      result = strdup(temp_result.c_str());
      return result;
   }


} // extern "C" for gadget::TypedProxy<gadget::Position>


