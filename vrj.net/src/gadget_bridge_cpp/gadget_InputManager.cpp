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
#include <gadget/InputManager.h>
#include <gadget_InputManager_Adapter.h>

extern "C"
{
   // Copy constructor wrapper.
   SHARPPY_API gadget::InputManager* gadget_InputManager_InputManager__gadget_InputManager(const gadget_InputManager_Adapter* p)
   {
      gadget_InputManager_Adapter* obj = new gadget_InputManager_Adapter(*p);
      obj->configAdd_callback_boost_shared_ptr_jccl__ConfigElement = p->configAdd_callback_boost_shared_ptr_jccl__ConfigElement;
      obj->configRemove_callback_boost_shared_ptr_jccl__ConfigElement = p->configRemove_callback_boost_shared_ptr_jccl__ConfigElement;
      obj->configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement = p->configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement;
      obj->configProcessPending_callback = p->configProcessPending_callback;
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget_InputManager_Adapter* gadget_InputManager_InputManager__(gadget_InputManager_Adapter::configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t cb0, gadget_InputManager_Adapter::configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t cb1, gadget_InputManager_Adapter::configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t cb2, gadget_InputManager_Adapter::configProcessPending_callback_t cb3)
   {
      gadget_InputManager_Adapter* obj = new gadget_InputManager_Adapter();
      obj->configAdd_callback_boost_shared_ptr_jccl__ConfigElement = cb0;
      obj->configRemove_callback_boost_shared_ptr_jccl__ConfigElement = cb1;
      obj->configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement = cb2;
      obj->configProcessPending_callback = cb3;
      return obj;
   }

   SHARPPY_API void delete_gadget_InputManager(gadget_InputManager_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::InputManager::getDisplaySystemElement()
   SHARPPY_API boost::shared_ptr<jccl::ConfigElement>* gadget_InputManager_getDisplaySystemElement__(gadget_InputManager_Adapter* self_)
   {
      boost::shared_ptr<jccl::ConfigElement>* result;
      result = new boost::shared_ptr<jccl::ConfigElement>(self_->getDisplaySystemElement());

      return result;
   }

   // Wrapper for non-virtual method gadget::InputManager::configureDevice()
   SHARPPY_API bool gadget_InputManager_configureDevice__boost_shared_ptr_jccl__ConfigElement(gadget_InputManager_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->configureDevice(p0);

      return result;
   }

   // Wrapper for non-virtual method gadget::InputManager::updateAllData()
   SHARPPY_API void gadget_InputManager_updateAllData__(gadget_InputManager_Adapter* self_)
   {
      self_->updateAllData();
   }

   // Wrapper for non-virtual method gadget::InputManager::getDevice()
   SHARPPY_API gadget::Input* gadget_InputManager_getDevice__std_basic_string_char_std__char_traits_char__std__allocator_char__(gadget_InputManager_Adapter* self_, const char* p0)
   {
      gadget::Input* result;
      result = self_->getDevice(p0);

      return result;
   }

   // Wrapper for non-virtual method gadget::InputManager::addDevice()
   SHARPPY_API bool gadget_InputManager_addDevice__gadget_Input(gadget_InputManager_Adapter* self_, gadget::Input* p0)
   {
      bool result;
      result = self_->addDevice(p0);

      return result;
   }

   // Wrapper for non-virtual method gadget::InputManager::addRemoteDevice()
   SHARPPY_API bool gadget_InputManager_addRemoteDevice__gadget_Input_std_basic_string_char_std__char_traits_char__std__allocator_char__(gadget_InputManager_Adapter* self_, gadget::Input* p0, const char* p1)
   {
      bool result;
      result = self_->addRemoteDevice(p0, p1);

      return result;
   }

   // Wrapper for non-virtual method gadget::InputManager::removeDevice()
   SHARPPY_API bool gadget_InputManager_removeDevice__std_basic_string_char_std__char_traits_char__std__allocator_char__(gadget_InputManager_Adapter* self_, const char* p0)
   {
      bool result;
      result = self_->removeDevice(p0);

      return result;
   }

   // Wrapper for non-virtual method gadget::InputManager::removeDevice()
   SHARPPY_API bool gadget_InputManager_removeDevice__gadget_Input(gadget_InputManager_Adapter* self_, const gadget::Input* p0)
   {
      bool result;
      result = self_->removeDevice(p0);

      return result;
   }

   // Wrapper for non-virtual method gadget::InputManager::addProxy()
   SHARPPY_API bool gadget_InputManager_addProxy__gadget_Proxy(gadget_InputManager_Adapter* self_, gadget::Proxy* p0)
   {
      bool result;
      result = self_->addProxy(p0);

      return result;
   }

   // Wrapper for non-virtual method gadget::InputManager::getProxy()
   SHARPPY_API gadget::Proxy* gadget_InputManager_getProxy__std_basic_string_char_std__char_traits_char__std__allocator_char__(gadget_InputManager_Adapter* self_, const char* p0)
   {
      gadget::Proxy* result;
      result = self_->getProxy(p0);

      return result;
   }

   // Wrapper for non-virtual method gadget::InputManager::refreshAllProxies()
   SHARPPY_API void gadget_InputManager_refreshAllProxies__(gadget_InputManager_Adapter* self_)
   {
      self_->refreshAllProxies();
   }

   // Wrapper for non-virtual method gadget::InputManager::removeProxy()
   SHARPPY_API bool gadget_InputManager_removeProxy__std_basic_string_char_std__char_traits_char__std__allocator_char__(gadget_InputManager_Adapter* self_, const char* p0)
   {
      bool result;
      result = self_->removeProxy(p0);

      return result;
   }

   // Wrapper for non-virtual method gadget::InputManager::removeProxy()
   SHARPPY_API bool gadget_InputManager_removeProxy__boost_shared_ptr_jccl__ConfigElement(gadget_InputManager_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->removeProxy(p0);

      return result;
   }


   // Wrapper for virtual method gadget::InputManager::configAdd()
   SHARPPY_API bool gadget_InputManager_configAdd__boost_shared_ptr_jccl__ConfigElement(gadget_InputManager_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->gadget::InputManager::configAdd(p0);

      return result;
   }

   // Wrapper for virtual method gadget::InputManager::configRemove()
   SHARPPY_API bool gadget_InputManager_configRemove__boost_shared_ptr_jccl__ConfigElement(gadget_InputManager_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->gadget::InputManager::configRemove(p0);

      return result;
   }

   // Wrapper for virtual method gadget::InputManager::configCanHandle()
   SHARPPY_API bool gadget_InputManager_configCanHandle__boost_shared_ptr_jccl__ConfigElement(gadget_InputManager_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->gadget::InputManager::configCanHandle(p0);

      return result;
   }


   // Wrapper for static method gadget::InputManager::instance()
   SHARPPY_API gadget::InputManager* gadget_InputManager_instance__()
   {
      gadget::InputManager* result;
      result = gadget::InputManager::instance();

      return result;
   }



} // extern "C" for gadget::InputManager


