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
#include <vrj/Sound/SoundManager.h>
#include <vrj_SoundManager_Adapter.h>

extern "C"
{
   // Copy constructor wrapper.
   SHARPPY_API vrj::SoundManager* vrj_SoundManager_SoundManager__vrj_SoundManager(const vrj_SoundManager_Adapter* p)
   {
      vrj_SoundManager_Adapter* obj = new vrj_SoundManager_Adapter(*p);
      obj->configAdd_callback_boost_shared_ptr_jccl__ConfigElement = p->configAdd_callback_boost_shared_ptr_jccl__ConfigElement;
      obj->configRemove_callback_boost_shared_ptr_jccl__ConfigElement = p->configRemove_callback_boost_shared_ptr_jccl__ConfigElement;
      obj->configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement = p->configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement;
      obj->update_callback = p->update_callback;
      obj->sync_callback = p->sync_callback;
      obj->configProcessPending_callback = p->configProcessPending_callback;
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vrj_SoundManager_Adapter* vrj_SoundManager_SoundManager__(vrj_SoundManager_Adapter::configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t cb0, vrj_SoundManager_Adapter::configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t cb1, vrj_SoundManager_Adapter::configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t cb2, vrj_SoundManager_Adapter::update_callback_t cb3, vrj_SoundManager_Adapter::sync_callback_t cb4, vrj_SoundManager_Adapter::configProcessPending_callback_t cb5)
   {
      vrj_SoundManager_Adapter* obj = new vrj_SoundManager_Adapter();
      obj->configAdd_callback_boost_shared_ptr_jccl__ConfigElement = cb0;
      obj->configRemove_callback_boost_shared_ptr_jccl__ConfigElement = cb1;
      obj->configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement = cb2;
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
   SHARPPY_API bool vrj_SoundManager_configAdd__boost_shared_ptr_jccl__ConfigElement(vrj_SoundManager_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->vrj::SoundManager::configAdd(p0);

      return result;
   }

   // Wrapper for virtual method vrj::SoundManager::configRemove()
   SHARPPY_API bool vrj_SoundManager_configRemove__boost_shared_ptr_jccl__ConfigElement(vrj_SoundManager_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->vrj::SoundManager::configRemove(p0);

      return result;
   }

   // Wrapper for virtual method vrj::SoundManager::configCanHandle()
   SHARPPY_API bool vrj_SoundManager_configCanHandle__boost_shared_ptr_jccl__ConfigElement(vrj_SoundManager_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->vrj::SoundManager::configCanHandle(p0);

      return result;
   }

   // Wrapper for virtual method vrj::SoundManager::update()
   SHARPPY_API void vrj_SoundManager_update__(vrj_SoundManager_Adapter* self_)
   {
      self_->vrj::SoundManager::update();
   }

   // Wrapper for virtual method vrj::SoundManager::sync()
   SHARPPY_API void vrj_SoundManager_sync__(vrj_SoundManager_Adapter* self_)
   {
      self_->vrj::SoundManager::sync();
   }




} // extern "C" for vrj::SoundManager


