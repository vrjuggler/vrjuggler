// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <vrj/Kernel/App.h>
#include <vrj_App_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API vrj_App_Adapter* vrj_App_App__vrj_Kernel1(vrj::Kernel* p0, vrj_App_Adapter::init_callback_t cb0, vrj_App_Adapter::apiInit_callback_t cb1, vrj_App_Adapter::exit_callback_t cb2, vrj_App_Adapter::preFrame_callback_t cb3, vrj_App_Adapter::latePreFrame_callback_t cb4, vrj_App_Adapter::intraFrame_callback_t cb5, vrj_App_Adapter::postFrame_callback_t cb6, vrj_App_Adapter::reset_callback_t cb7, vrj_App_Adapter::focusChanged_callback_t cb8, vrj_App_Adapter::getDrawScaleFactor_callback_t cb9, vrj_App_Adapter::configCanHandle_callback_jccl_ConfigElementPtr_t cb10, vrj_App_Adapter::depSatisfied_callback_t cb11, vrj_App_Adapter::configAdd_callback_jccl_ConfigElementPtr_t cb12, vrj_App_Adapter::configRemove_callback_jccl_ConfigElementPtr_t cb13, vrj_App_Adapter::getDrawManager_callback_t cb14, vrj_App_Adapter::getSoundManager_callback_t cb15, vrj_App_Adapter::configProcessPending_callback_t cb16)
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
      obj->configCanHandle_callback_jccl_ConfigElementPtr = cb10;
      obj->depSatisfied_callback = cb11;
      obj->configAdd_callback_jccl_ConfigElementPtr = cb12;
      obj->configRemove_callback_jccl_ConfigElementPtr = cb13;
      obj->getDrawManager_callback = cb14;
      obj->getSoundManager_callback = cb15;
      obj->configProcessPending_callback = cb16;
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vrj_App_Adapter* vrj_App_App__0(vrj_App_Adapter::init_callback_t cb0, vrj_App_Adapter::apiInit_callback_t cb1, vrj_App_Adapter::exit_callback_t cb2, vrj_App_Adapter::preFrame_callback_t cb3, vrj_App_Adapter::latePreFrame_callback_t cb4, vrj_App_Adapter::intraFrame_callback_t cb5, vrj_App_Adapter::postFrame_callback_t cb6, vrj_App_Adapter::reset_callback_t cb7, vrj_App_Adapter::focusChanged_callback_t cb8, vrj_App_Adapter::getDrawScaleFactor_callback_t cb9, vrj_App_Adapter::configCanHandle_callback_jccl_ConfigElementPtr_t cb10, vrj_App_Adapter::depSatisfied_callback_t cb11, vrj_App_Adapter::configAdd_callback_jccl_ConfigElementPtr_t cb12, vrj_App_Adapter::configRemove_callback_jccl_ConfigElementPtr_t cb13, vrj_App_Adapter::getDrawManager_callback_t cb14, vrj_App_Adapter::getSoundManager_callback_t cb15, vrj_App_Adapter::configProcessPending_callback_t cb16)
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
      obj->configCanHandle_callback_jccl_ConfigElementPtr = cb10;
      obj->depSatisfied_callback = cb11;
      obj->configAdd_callback_jccl_ConfigElementPtr = cb12;
      obj->configRemove_callback_jccl_ConfigElementPtr = cb13;
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
   SHARPPY_API bool vrj_App_haveFocus__0(vrj_App_Adapter* self_)
   {
      bool result;
      result = self_->haveFocus();
      return result;
   }


   // Wrapper for non-virtual method vrj::App::setFocus()
   SHARPPY_API void vrj_App_setFocus__bool1(vrj_App_Adapter* self_, bool p0)
   {
      self_->setFocus(p0);
   }


   // Wrapper for virtual method vrj::App::init()
   SHARPPY_API void vrj_App_init__0(vrj_App_Adapter* self_)
   {
      self_->vrj::App::init();
   }


   // Wrapper for virtual method vrj::App::apiInit()
   SHARPPY_API void vrj_App_apiInit__0(vrj_App_Adapter* self_)
   {
      self_->vrj::App::apiInit();
   }


   // Wrapper for virtual method vrj::App::exit()
   SHARPPY_API void vrj_App_exit__0(vrj_App_Adapter* self_)
   {
      self_->vrj::App::exit();
   }


   // Wrapper for virtual method vrj::App::preFrame()
   SHARPPY_API void vrj_App_preFrame__0(vrj_App_Adapter* self_)
   {
      self_->vrj::App::preFrame();
   }


   // Wrapper for virtual method vrj::App::latePreFrame()
   SHARPPY_API void vrj_App_latePreFrame__0(vrj_App_Adapter* self_)
   {
      self_->vrj::App::latePreFrame();
   }


   // Wrapper for virtual method vrj::App::intraFrame()
   SHARPPY_API void vrj_App_intraFrame__0(vrj_App_Adapter* self_)
   {
      self_->vrj::App::intraFrame();
   }


   // Wrapper for virtual method vrj::App::postFrame()
   SHARPPY_API void vrj_App_postFrame__0(vrj_App_Adapter* self_)
   {
      self_->vrj::App::postFrame();
   }


   // Wrapper for virtual method vrj::App::reset()
   SHARPPY_API void vrj_App_reset__0(vrj_App_Adapter* self_)
   {
      self_->vrj::App::reset();
   }


   // Wrapper for virtual method vrj::App::focusChanged()
   SHARPPY_API void vrj_App_focusChanged__0(vrj_App_Adapter* self_)
   {
      self_->vrj::App::focusChanged();
   }


   // Wrapper for virtual method vrj::App::getDrawScaleFactor()
   SHARPPY_API float vrj_App_getDrawScaleFactor__0(vrj_App_Adapter* self_)
   {
      float result;
      result = self_->vrj::App::getDrawScaleFactor();
      return result;
   }


   // Wrapper for virtual method vrj::App::configCanHandle()
   SHARPPY_API bool vrj_App_configCanHandle__jccl_ConfigElementPtr1(vrj_App_Adapter* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->vrj::App::configCanHandle(*p0);
      return result;
   }


   // Wrapper for virtual method vrj::App::depSatisfied()
   SHARPPY_API bool vrj_App_depSatisfied__0(vrj_App_Adapter* self_)
   {
      bool result;
      result = self_->vrj::App::depSatisfied();
      return result;
   }


   // Wrapper for virtual method vrj::App::configAdd()
   SHARPPY_API bool vrj_App_configAdd__jccl_ConfigElementPtr1(vrj_App_Adapter* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->configAdd_wrapper(*p0);
      return result;
   }


   // Wrapper for virtual method vrj::App::configRemove()
   SHARPPY_API bool vrj_App_configRemove__jccl_ConfigElementPtr1(vrj_App_Adapter* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->configRemove_wrapper(*p0);
      return result;
   }


   // Wrapper for virtual method vrj::App::getDrawManager()
   SHARPPY_API vrj::DrawManager* vrj_App_getDrawManager__0(vrj_App_Adapter* self_)
   {
      vrj::DrawManager* result;
      result = self_->getDrawManager();
      return result;
   }


   // Wrapper for virtual method vrj::App::getSoundManager()
   SHARPPY_API vrj::SoundManager* vrj_App_getSoundManager__0(vrj_App_Adapter* self_)
   {
      vrj::SoundManager* result;
      result = self_->vrj::App::getSoundManager();
      return result;
   }


} // extern "C" for vrj::App


