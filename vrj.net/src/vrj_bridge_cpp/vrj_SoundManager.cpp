// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <vrj/Sound/SoundManager.h>
#include <vrj_SoundManager_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API vrj_SoundManager_Adapter* vrj_SoundManager_SoundManager__vrj_SoundManager1(const vrj::SoundManager* p0, vrj_SoundManager_Adapter::configAdd_callback_jccl_ConfigElementPtr_t cb0, vrj_SoundManager_Adapter::configRemove_callback_jccl_ConfigElementPtr_t cb1, vrj_SoundManager_Adapter::configCanHandle_callback_jccl_ConfigElementPtr_t cb2, vrj_SoundManager_Adapter::update_callback_t cb3, vrj_SoundManager_Adapter::sync_callback_t cb4, vrj_SoundManager_Adapter::configProcessPending_callback_t cb5)
   {
      const vrj::SoundManager& marshal_p0 = *p0;
      vrj_SoundManager_Adapter* obj = new vrj_SoundManager_Adapter(marshal_p0);
      obj->configAdd_callback_jccl_ConfigElementPtr = cb0;
      obj->configRemove_callback_jccl_ConfigElementPtr = cb1;
      obj->configCanHandle_callback_jccl_ConfigElementPtr = cb2;
      obj->update_callback = cb3;
      obj->sync_callback = cb4;
      obj->configProcessPending_callback = cb5;
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vrj_SoundManager_Adapter* vrj_SoundManager_SoundManager__0(vrj_SoundManager_Adapter::configAdd_callback_jccl_ConfigElementPtr_t cb0, vrj_SoundManager_Adapter::configRemove_callback_jccl_ConfigElementPtr_t cb1, vrj_SoundManager_Adapter::configCanHandle_callback_jccl_ConfigElementPtr_t cb2, vrj_SoundManager_Adapter::update_callback_t cb3, vrj_SoundManager_Adapter::sync_callback_t cb4, vrj_SoundManager_Adapter::configProcessPending_callback_t cb5)
   {
      vrj_SoundManager_Adapter* obj = new vrj_SoundManager_Adapter();
      obj->configAdd_callback_jccl_ConfigElementPtr = cb0;
      obj->configRemove_callback_jccl_ConfigElementPtr = cb1;
      obj->configCanHandle_callback_jccl_ConfigElementPtr = cb2;
      obj->update_callback = cb3;
      obj->sync_callback = cb4;
      obj->configProcessPending_callback = cb5;
      return obj;
   }

   SHARPPY_API void delete_vrj_SoundManager(vrj_SoundManager_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for virtual method vrj::SoundManager::configAdd()
   SHARPPY_API bool vrj_SoundManager_configAdd__jccl_ConfigElementPtr1(vrj_SoundManager_Adapter* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->vrj::SoundManager::configAdd(*p0);
      return result;
   }


   // Wrapper for virtual method vrj::SoundManager::configRemove()
   SHARPPY_API bool vrj_SoundManager_configRemove__jccl_ConfigElementPtr1(vrj_SoundManager_Adapter* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->vrj::SoundManager::configRemove(*p0);
      return result;
   }


   // Wrapper for virtual method vrj::SoundManager::configCanHandle()
   SHARPPY_API bool vrj_SoundManager_configCanHandle__jccl_ConfigElementPtr1(vrj_SoundManager_Adapter* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->vrj::SoundManager::configCanHandle(*p0);
      return result;
   }


   // Wrapper for virtual method vrj::SoundManager::update()
   SHARPPY_API void vrj_SoundManager_update__0(vrj_SoundManager_Adapter* self_)
   {
      self_->vrj::SoundManager::update();
   }


   // Wrapper for virtual method vrj::SoundManager::sync()
   SHARPPY_API void vrj_SoundManager_sync__0(vrj_SoundManager_Adapter* self_)
   {
      self_->vrj::SoundManager::sync();
   }


} // extern "C" for vrj::SoundManager


