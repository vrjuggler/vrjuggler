// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/InputManager.h>

extern "C"
{
   // Wrapper for non-virtual method gadget::InputManager::getDisplaySystemElement()
   SHARPPY_API jccl::ConfigElementPtr* gadget_InputManager_getDisplaySystemElement__0(gadget::InputManager* self_)
   {
      jccl::ConfigElementPtr* result;
      result = new jccl::ConfigElementPtr(self_->getDisplaySystemElement());
      return result;
   }


   // Wrapper for non-virtual method gadget::InputManager::configureDevice()
   SHARPPY_API bool gadget_InputManager_configureDevice__jccl_ConfigElementPtr1(gadget::InputManager* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->configureDevice(*p0);
      return result;
   }


   // Wrapper for non-virtual method gadget::InputManager::updateAllData()
   SHARPPY_API void gadget_InputManager_updateAllData__0(gadget::InputManager* self_)
   {
      self_->updateAllData();
   }


   // Wrapper for non-virtual method gadget::InputManager::getDevice()
   SHARPPY_API gadget::Input* gadget_InputManager_getDevice__std_string1(gadget::InputManager* self_, const char* p0)
   {
      gadget::Input* result;
      result = self_->getDevice(p0);
      return result;
   }


   // Wrapper for non-virtual method gadget::InputManager::addDevice()
   SHARPPY_API bool gadget_InputManager_addDevice__gadget_Input1(gadget::InputManager* self_, gadget::Input* p0)
   {
      bool result;
      result = self_->addDevice(p0);
      return result;
   }


   // Wrapper for non-virtual method gadget::InputManager::addRemoteDevice()
   SHARPPY_API bool gadget_InputManager_addRemoteDevice__gadget_Input_std_string2(gadget::InputManager* self_, gadget::Input* p0, const char* p1)
   {
      bool result;
      result = self_->addRemoteDevice(p0, p1);
      return result;
   }


   // Wrapper for non-virtual method gadget::InputManager::removeDevice()
   SHARPPY_API bool gadget_InputManager_removeDevice__std_string1(gadget::InputManager* self_, const char* p0)
   {
      bool result;
      result = self_->removeDevice(p0);
      return result;
   }


   // Wrapper for non-virtual method gadget::InputManager::removeDevice()
   SHARPPY_API bool gadget_InputManager_removeDevice__gadget_Input1(gadget::InputManager* self_, const gadget::Input* p0)
   {
      bool result;
      result = self_->removeDevice(p0);
      return result;
   }


   // Wrapper for non-virtual method gadget::InputManager::addProxy()
   SHARPPY_API bool gadget_InputManager_addProxy__gadget_Proxy1(gadget::InputManager* self_, gadget::Proxy* p0)
   {
      bool result;
      result = self_->addProxy(p0);
      return result;
   }


   // Wrapper for non-virtual method gadget::InputManager::getProxy()
   SHARPPY_API gadget::Proxy* gadget_InputManager_getProxy__std_string1(gadget::InputManager* self_, const char* p0)
   {
      gadget::Proxy* result;
      result = self_->getProxy(p0);
      return result;
   }


   // Wrapper for non-virtual method gadget::InputManager::refreshAllProxies()
   SHARPPY_API void gadget_InputManager_refreshAllProxies__0(gadget::InputManager* self_)
   {
      self_->refreshAllProxies();
   }


   // Wrapper for virtual method gadget::InputManager::configAdd()
   SHARPPY_API bool gadget_InputManager_configAdd__jccl_ConfigElementPtr1(gadget::InputManager* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->configAdd(*p0);
      return result;
   }


   // Wrapper for virtual method gadget::InputManager::configRemove()
   SHARPPY_API bool gadget_InputManager_configRemove__jccl_ConfigElementPtr1(gadget::InputManager* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->configRemove(*p0);
      return result;
   }


   // Wrapper for virtual method gadget::InputManager::configCanHandle()
   SHARPPY_API bool gadget_InputManager_configCanHandle__jccl_ConfigElementPtr1(gadget::InputManager* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->configCanHandle(*p0);
      return result;
   }


   // Wrapper for static method gadget::InputManager::instance()
   SHARPPY_API gadget::InputManager* gadget_InputManager_instance__0()
   {
      gadget::InputManager* result;
      result = gadget::InputManager::instance();
      return result;
   }


} // extern "C" for gadget::InputManager


