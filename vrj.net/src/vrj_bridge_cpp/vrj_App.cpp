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
#include <vrj/Kernel/App.h>
#include <vrj_App_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API vrj_App_Adapter* vrj_App_App__vrj_Kernel(vrj::Kernel* p0, vrj_App_Adapter::init_callback_t cb0, vrj_App_Adapter::apiInit_callback_t cb1, vrj_App_Adapter::exit_callback_t cb2, vrj_App_Adapter::preFrame_callback_t cb3, vrj_App_Adapter::latePreFrame_callback_t cb4, vrj_App_Adapter::intraFrame_callback_t cb5, vrj_App_Adapter::postFrame_callback_t cb6, vrj_App_Adapter::reset_callback_t cb7, vrj_App_Adapter::focusChanged_callback_t cb8, vrj_App_Adapter::getDrawScaleFactor_callback_t cb9, vrj_App_Adapter::configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t cb10, vrj_App_Adapter::depSatisfied_callback_t cb11, vrj_App_Adapter::configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t cb12, vrj_App_Adapter::configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t cb13, vrj_App_Adapter::getDrawManager_callback_t cb14, vrj_App_Adapter::getSoundManager_callback_t cb15, vrj_App_Adapter::configProcessPending_callback_t cb16)
   {
      vrj_App_Adapter* obj = new vrj_App_Adapter(p0);
      obj->init_callback = cb0;
      obj->apiInit_callback = cb1;
      obj->exit_callback = cb2;
      obj->preFrame_callback = cb3;
      obj->latePreFrame_callback = cb4;
      obj->intraFrame_callback = cb5;
      obj->postFrame_callback = cb6;
      obj->reset_callback = cb7;
      obj->focusChanged_callback = cb8;
      obj->getDrawScaleFactor_callback = cb9;
      obj->configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement = cb10;
      obj->depSatisfied_callback = cb11;
      obj->configAdd_callback_boost_shared_ptr_jccl__ConfigElement = cb12;
      obj->configRemove_callback_boost_shared_ptr_jccl__ConfigElement = cb13;
      obj->getDrawManager_callback = cb14;
      obj->getSoundManager_callback = cb15;
      obj->configProcessPending_callback = cb16;
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vrj_App_Adapter* vrj_App_App__(vrj_App_Adapter::init_callback_t cb0, vrj_App_Adapter::apiInit_callback_t cb1, vrj_App_Adapter::exit_callback_t cb2, vrj_App_Adapter::preFrame_callback_t cb3, vrj_App_Adapter::latePreFrame_callback_t cb4, vrj_App_Adapter::intraFrame_callback_t cb5, vrj_App_Adapter::postFrame_callback_t cb6, vrj_App_Adapter::reset_callback_t cb7, vrj_App_Adapter::focusChanged_callback_t cb8, vrj_App_Adapter::getDrawScaleFactor_callback_t cb9, vrj_App_Adapter::configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t cb10, vrj_App_Adapter::depSatisfied_callback_t cb11, vrj_App_Adapter::configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t cb12, vrj_App_Adapter::configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t cb13, vrj_App_Adapter::getDrawManager_callback_t cb14, vrj_App_Adapter::getSoundManager_callback_t cb15, vrj_App_Adapter::configProcessPending_callback_t cb16)
   {
      vrj_App_Adapter* obj = new vrj_App_Adapter();
      obj->init_callback = cb0;
      obj->apiInit_callback = cb1;
      obj->exit_callback = cb2;
      obj->preFrame_callback = cb3;
      obj->latePreFrame_callback = cb4;
      obj->intraFrame_callback = cb5;
      obj->postFrame_callback = cb6;
      obj->reset_callback = cb7;
      obj->focusChanged_callback = cb8;
      obj->getDrawScaleFactor_callback = cb9;
      obj->configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement = cb10;
      obj->depSatisfied_callback = cb11;
      obj->configAdd_callback_boost_shared_ptr_jccl__ConfigElement = cb12;
      obj->configRemove_callback_boost_shared_ptr_jccl__ConfigElement = cb13;
      obj->getDrawManager_callback = cb14;
      obj->getSoundManager_callback = cb15;
      obj->configProcessPending_callback = cb16;
      return obj;
   }

   SHARPPY_API void delete_vrj_App(vrj_App_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method vrj::App::haveFocus()
   SHARPPY_API bool vrj_App_haveFocus__(vrj_App_Adapter* self_)
   {
      bool result;
      result = self_->haveFocus();

      return result;
   }

   // Wrapper for non-virtual method vrj::App::setFocus()
   SHARPPY_API void vrj_App_setFocus__bool(vrj_App_Adapter* self_, bool p0)
   {
      self_->setFocus(p0);
   }


   // Wrapper for virtual method vrj::App::init()
   SHARPPY_API void vrj_App_init__(vrj_App_Adapter* self_)
   {
      self_->vrj::App::init();
   }

   // Wrapper for virtual method vrj::App::apiInit()
   SHARPPY_API void vrj_App_apiInit__(vrj_App_Adapter* self_)
   {
      self_->vrj::App::apiInit();
   }

   // Wrapper for virtual method vrj::App::exit()
   SHARPPY_API void vrj_App_exit__(vrj_App_Adapter* self_)
   {
      self_->vrj::App::exit();
   }

   // Wrapper for virtual method vrj::App::preFrame()
   SHARPPY_API void vrj_App_preFrame__(vrj_App_Adapter* self_)
   {
      self_->vrj::App::preFrame();
   }

   // Wrapper for virtual method vrj::App::latePreFrame()
   SHARPPY_API void vrj_App_latePreFrame__(vrj_App_Adapter* self_)
   {
      self_->vrj::App::latePreFrame();
   }

   // Wrapper for virtual method vrj::App::intraFrame()
   SHARPPY_API void vrj_App_intraFrame__(vrj_App_Adapter* self_)
   {
      self_->vrj::App::intraFrame();
   }

   // Wrapper for virtual method vrj::App::postFrame()
   SHARPPY_API void vrj_App_postFrame__(vrj_App_Adapter* self_)
   {
      self_->vrj::App::postFrame();
   }

   // Wrapper for virtual method vrj::App::reset()
   SHARPPY_API void vrj_App_reset__(vrj_App_Adapter* self_)
   {
      self_->vrj::App::reset();
   }

   // Wrapper for virtual method vrj::App::focusChanged()
   SHARPPY_API void vrj_App_focusChanged__(vrj_App_Adapter* self_)
   {
      self_->vrj::App::focusChanged();
   }

   // Wrapper for virtual method vrj::App::getDrawScaleFactor()
   SHARPPY_API float vrj_App_getDrawScaleFactor__(vrj_App_Adapter* self_)
   {
      float result;
      result = self_->vrj::App::getDrawScaleFactor();

      return result;
   }

   // Wrapper for virtual method vrj::App::configCanHandle()
   SHARPPY_API bool vrj_App_configCanHandle__boost_shared_ptr_jccl__ConfigElement(vrj_App_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->vrj::App::configCanHandle(p0);

      return result;
   }

   // Wrapper for virtual method vrj::App::depSatisfied()
   SHARPPY_API bool vrj_App_depSatisfied__(vrj_App_Adapter* self_)
   {
      bool result;
      result = self_->vrj::App::depSatisfied();

      return result;
   }

   // Wrapper for virtual method vrj::App::configAdd()
   SHARPPY_API bool vrj_App_configAdd__boost_shared_ptr_jccl__ConfigElement(vrj_App_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->configAdd_wrapper(p0);

      return result;
   }

   // Wrapper for virtual method vrj::App::configRemove()
   SHARPPY_API bool vrj_App_configRemove__boost_shared_ptr_jccl__ConfigElement(vrj_App_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->configRemove_wrapper(p0);

      return result;
   }

   // Wrapper for virtual method vrj::App::getDrawManager()
   SHARPPY_API vrj::DrawManager* vrj_App_getDrawManager__(vrj_App_Adapter* self_)
   {
      vrj::DrawManager* result;
      result = self_->getDrawManager();

      return result;
   }

   // Wrapper for virtual method vrj::App::getSoundManager()
   SHARPPY_API vrj::SoundManager* vrj_App_getSoundManager__(vrj_App_Adapter* self_)
   {
      vrj::SoundManager* result;
      result = self_->vrj::App::getSoundManager();

      return result;
   }




} // extern "C" for vrj::App


