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

// Generated from Revision: 1.60 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <vrj/Draw/DrawManager.h>
#include <vrj_DrawManager_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API vrj_DrawManager_Adapter* vrj_DrawManager_DrawManager__(vrj_DrawManager_Adapter::draw_callback_t cb0, vrj_DrawManager_Adapter::sync_callback_t cb1, vrj_DrawManager_Adapter::setApp_callback_vrj_App_t cb2, vrj_DrawManager_Adapter::initAPI_callback_t cb3, vrj_DrawManager_Adapter::addDisplay_callback_vrj_Display_t cb4, vrj_DrawManager_Adapter::removeDisplay_callback_vrj_Display_t cb5, vrj_DrawManager_Adapter::closeAPI_callback_t cb6, vrj_DrawManager_Adapter::configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t cb7, vrj_DrawManager_Adapter::configProcessPending_callback_t cb8, vrj_DrawManager_Adapter::configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t cb9, vrj_DrawManager_Adapter::configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t cb10)
   {
      vrj_DrawManager_Adapter* obj = new vrj_DrawManager_Adapter();
      obj->draw_callback = cb0;
      obj->sync_callback = cb1;
      obj->setApp_callback_vrj_App = cb2;
      obj->initAPI_callback = cb3;
      obj->addDisplay_callback_vrj_Display = cb4;
      obj->removeDisplay_callback_vrj_Display = cb5;
      obj->closeAPI_callback = cb6;
      obj->configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement = cb7;
      obj->configProcessPending_callback = cb8;
      obj->configAdd_callback_boost_shared_ptr_jccl__ConfigElement = cb9;
      obj->configRemove_callback_boost_shared_ptr_jccl__ConfigElement = cb10;
      return obj;
   }

   SHARPPY_API void delete_vrj_DrawManager(vrj_DrawManager_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for virtual method vrj::DrawManager::draw()
   SHARPPY_API void vrj_DrawManager_draw__0(vrj_DrawManager_Adapter* self_)
   {
      self_->draw();
   }


   // Wrapper for virtual method vrj::DrawManager::sync()
   SHARPPY_API void vrj_DrawManager_sync__0(vrj_DrawManager_Adapter* self_)
   {
      self_->sync();
   }


   // Wrapper for virtual method vrj::DrawManager::setApp()
   SHARPPY_API void vrj_DrawManager_setApp__vrj_App1(vrj_DrawManager_Adapter* self_, vrj::App* p0)
   {
      self_->setApp(p0);
   }


   // Wrapper for virtual method vrj::DrawManager::initAPI()
   SHARPPY_API void vrj_DrawManager_initAPI__0(vrj_DrawManager_Adapter* self_)
   {
      self_->initAPI();
   }


   // Wrapper for virtual method vrj::DrawManager::addDisplay()
   SHARPPY_API void vrj_DrawManager_addDisplay__vrj_Display1(vrj_DrawManager_Adapter* self_, vrj::Display* p0)
   {
      self_->addDisplay(p0);
   }


   // Wrapper for virtual method vrj::DrawManager::removeDisplay()
   SHARPPY_API void vrj_DrawManager_removeDisplay__vrj_Display1(vrj_DrawManager_Adapter* self_, vrj::Display* p0)
   {
      self_->removeDisplay(p0);
   }


   // Wrapper for virtual method vrj::DrawManager::closeAPI()
   SHARPPY_API void vrj_DrawManager_closeAPI__0(vrj_DrawManager_Adapter* self_)
   {
      self_->closeAPI();
   }


} // extern "C" for vrj::DrawManager


