// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <vrj/Draw/OGL/GlApp.h>
#include <vrj_GlApp_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API vrj_GlApp_Adapter* vrj_GlApp_GlApp__vrj_Kernel1(vrj::Kernel* p0, vrj_GlApp_Adapter::draw_callback_t cb0, vrj_GlApp_Adapter::contextInit_callback_t cb1, vrj_GlApp_Adapter::contextClose_callback_t cb2, vrj_GlApp_Adapter::contextPreDraw_callback_t cb3, vrj_GlApp_Adapter::contextPostDraw_callback_t cb4, vrj_GlApp_Adapter::bufferPreDraw_callback_t cb5, vrj_GlApp_Adapter::pipePreDraw_callback_t cb6, vrj_GlApp_Adapter::getDrawManager_callback_t cb7, vrj_GlApp_Adapter::init_callback_t cb8, vrj_GlApp_Adapter::apiInit_callback_t cb9, vrj_GlApp_Adapter::exit_callback_t cb10, vrj_GlApp_Adapter::preFrame_callback_t cb11, vrj_GlApp_Adapter::latePreFrame_callback_t cb12, vrj_GlApp_Adapter::intraFrame_callback_t cb13, vrj_GlApp_Adapter::postFrame_callback_t cb14, vrj_GlApp_Adapter::reset_callback_t cb15, vrj_GlApp_Adapter::focusChanged_callback_t cb16, vrj_GlApp_Adapter::getDrawScaleFactor_callback_t cb17, vrj_GlApp_Adapter::configCanHandle_callback_jccl_ConfigElementPtr_t cb18, vrj_GlApp_Adapter::depSatisfied_callback_t cb19, vrj_GlApp_Adapter::configAdd_callback_jccl_ConfigElementPtr_t cb20, vrj_GlApp_Adapter::configRemove_callback_jccl_ConfigElementPtr_t cb21, vrj_GlApp_Adapter::getSoundManager_callback_t cb22)
   {
      vrj_GlApp_Adapter* obj = new vrj_GlApp_Adapter(p0);
      obj->draw_callback = cb0;
      obj->contextInit_callback = cb1;
      obj->contextClose_callback = cb2;
      obj->contextPreDraw_callback = cb3;
      obj->contextPostDraw_callback = cb4;
      obj->bufferPreDraw_callback = cb5;
      obj->pipePreDraw_callback = cb6;
      obj->getDrawManager_callback = cb7;
      obj->init_callback = cb8;
      obj->apiInit_callback = cb9;
      obj->exit_callback = cb10;
      obj->preFrame_callback = cb11;
      obj->latePreFrame_callback = cb12;
      obj->intraFrame_callback = cb13;
      obj->postFrame_callback = cb14;
      obj->reset_callback = cb15;
      obj->focusChanged_callback = cb16;
      obj->getDrawScaleFactor_callback = cb17;
      obj->configCanHandle_callback_jccl_ConfigElementPtr = cb18;
      obj->depSatisfied_callback = cb19;
      obj->configAdd_callback_jccl_ConfigElementPtr = cb20;
      obj->configRemove_callback_jccl_ConfigElementPtr = cb21;
      obj->getSoundManager_callback = cb22;
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vrj_GlApp_Adapter* vrj_GlApp_GlApp__0(vrj_GlApp_Adapter::draw_callback_t cb0, vrj_GlApp_Adapter::contextInit_callback_t cb1, vrj_GlApp_Adapter::contextClose_callback_t cb2, vrj_GlApp_Adapter::contextPreDraw_callback_t cb3, vrj_GlApp_Adapter::contextPostDraw_callback_t cb4, vrj_GlApp_Adapter::bufferPreDraw_callback_t cb5, vrj_GlApp_Adapter::pipePreDraw_callback_t cb6, vrj_GlApp_Adapter::getDrawManager_callback_t cb7, vrj_GlApp_Adapter::init_callback_t cb8, vrj_GlApp_Adapter::apiInit_callback_t cb9, vrj_GlApp_Adapter::exit_callback_t cb10, vrj_GlApp_Adapter::preFrame_callback_t cb11, vrj_GlApp_Adapter::latePreFrame_callback_t cb12, vrj_GlApp_Adapter::intraFrame_callback_t cb13, vrj_GlApp_Adapter::postFrame_callback_t cb14, vrj_GlApp_Adapter::reset_callback_t cb15, vrj_GlApp_Adapter::focusChanged_callback_t cb16, vrj_GlApp_Adapter::getDrawScaleFactor_callback_t cb17, vrj_GlApp_Adapter::configCanHandle_callback_jccl_ConfigElementPtr_t cb18, vrj_GlApp_Adapter::depSatisfied_callback_t cb19, vrj_GlApp_Adapter::configAdd_callback_jccl_ConfigElementPtr_t cb20, vrj_GlApp_Adapter::configRemove_callback_jccl_ConfigElementPtr_t cb21, vrj_GlApp_Adapter::getSoundManager_callback_t cb22)
   {
      vrj_GlApp_Adapter* obj = new vrj_GlApp_Adapter();
      obj->draw_callback = cb0;
      obj->contextInit_callback = cb1;
      obj->contextClose_callback = cb2;
      obj->contextPreDraw_callback = cb3;
      obj->contextPostDraw_callback = cb4;
      obj->bufferPreDraw_callback = cb5;
      obj->pipePreDraw_callback = cb6;
      obj->getDrawManager_callback = cb7;
      obj->init_callback = cb8;
      obj->apiInit_callback = cb9;
      obj->exit_callback = cb10;
      obj->preFrame_callback = cb11;
      obj->latePreFrame_callback = cb12;
      obj->intraFrame_callback = cb13;
      obj->postFrame_callback = cb14;
      obj->reset_callback = cb15;
      obj->focusChanged_callback = cb16;
      obj->getDrawScaleFactor_callback = cb17;
      obj->configCanHandle_callback_jccl_ConfigElementPtr = cb18;
      obj->depSatisfied_callback = cb19;
      obj->configAdd_callback_jccl_ConfigElementPtr = cb20;
      obj->configRemove_callback_jccl_ConfigElementPtr = cb21;
      obj->getSoundManager_callback = cb22;
      return obj;
   }

   SHARPPY_API void delete_vrj_GlApp(vrj_GlApp_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for virtual method vrj::GlApp::draw()
   SHARPPY_API void vrj_GlApp_draw__0(vrj_GlApp_Adapter* self_)
   {
      self_->draw();
   }


   // Wrapper for virtual method vrj::GlApp::contextInit()
   SHARPPY_API void vrj_GlApp_contextInit__0(vrj_GlApp_Adapter* self_)
   {
      self_->vrj::GlApp::contextInit();
   }


   // Wrapper for virtual method vrj::GlApp::contextClose()
   SHARPPY_API void vrj_GlApp_contextClose__0(vrj_GlApp_Adapter* self_)
   {
      self_->vrj::GlApp::contextClose();
   }


   // Wrapper for virtual method vrj::GlApp::contextPreDraw()
   SHARPPY_API void vrj_GlApp_contextPreDraw__0(vrj_GlApp_Adapter* self_)
   {
      self_->vrj::GlApp::contextPreDraw();
   }


   // Wrapper for virtual method vrj::GlApp::contextPostDraw()
   SHARPPY_API void vrj_GlApp_contextPostDraw__0(vrj_GlApp_Adapter* self_)
   {
      self_->vrj::GlApp::contextPostDraw();
   }


   // Wrapper for virtual method vrj::GlApp::bufferPreDraw()
   SHARPPY_API void vrj_GlApp_bufferPreDraw__0(vrj_GlApp_Adapter* self_)
   {
      self_->vrj::GlApp::bufferPreDraw();
   }


   // Wrapper for virtual method vrj::GlApp::pipePreDraw()
   SHARPPY_API void vrj_GlApp_pipePreDraw__0(vrj_GlApp_Adapter* self_)
   {
      self_->vrj::GlApp::pipePreDraw();
   }


   // Wrapper for virtual method vrj::GlApp::getDrawManager()
   SHARPPY_API vrj::DrawManager* vrj_GlApp_getDrawManager__0(vrj_GlApp_Adapter* self_)
   {
      vrj::DrawManager* result;
      result = self_->vrj::GlApp::getDrawManager();
      return result;
   }


} // extern "C" for vrj::GlApp


