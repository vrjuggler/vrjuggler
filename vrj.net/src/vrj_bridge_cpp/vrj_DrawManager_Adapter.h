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

// Generated from Revision: 1.18 of RCSfile: class_cxx_adapter.tmpl,v


#ifndef __VRJ_DRAWMANAGER_ADAPTER__
#define __VRJ_DRAWMANAGER_ADAPTER__

#include "sharppy.h"
#include <vrj/Draw/DrawManager.h>

class SHARPPY_API vrj_DrawManager_Adapter : public vrj::DrawManager
{
public:
   vrj_DrawManager_Adapter()
      : vrj::DrawManager()
   {;}

   virtual ~vrj_DrawManager_Adapter()
   {;}

   typedef bool (*configCanHandle_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   configCanHandle_callback_jccl_ConfigElementPtr_t configCanHandle_callback_jccl_ConfigElementPtr;

   /// Override for virtual function jccl::ConfigElementHandler::configCanHandle.
   virtual bool configCanHandle(jccl::ConfigElementPtr p0)
   {
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = configCanHandle_callback_jccl_ConfigElementPtr(p_p0);
      return result;
   }

   typedef int (*configProcessPending_callback_t)();
   configProcessPending_callback_t configProcessPending_callback;

   /// Override for virtual function jccl::ConfigElementHandler::configProcessPending.
   virtual int configProcessPending()
   {
      int result = configProcessPending_callback();
      return result;
   }

   typedef bool (*configAdd_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   configAdd_callback_jccl_ConfigElementPtr_t configAdd_callback_jccl_ConfigElementPtr;

   /// Override for virtual function jccl::ConfigElementHandler::configAdd.
   virtual bool configAdd(jccl::ConfigElementPtr p0)
   {
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = configAdd_callback_jccl_ConfigElementPtr(p_p0);
      return result;
   }

   typedef bool (*configRemove_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   configRemove_callback_jccl_ConfigElementPtr_t configRemove_callback_jccl_ConfigElementPtr;

   /// Override for virtual function jccl::ConfigElementHandler::configRemove.
   virtual bool configRemove(jccl::ConfigElementPtr p0)
   {
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = configRemove_callback_jccl_ConfigElementPtr(p_p0);
      return result;
   }

   typedef void (*draw_callback_t)();
   draw_callback_t draw_callback;

   /// Override for virtual function vrj::DrawManager::draw.
   virtual void draw()
   {
      draw_callback();
   }

   typedef void (*sync_callback_t)();
   sync_callback_t sync_callback;

   /// Override for virtual function vrj::DrawManager::sync.
   virtual void sync()
   {
      sync_callback();
   }

   typedef void (*setApp_callback_vrj_App_t)(vrj::App*);
   setApp_callback_vrj_App_t setApp_callback_vrj_App;

   /// Override for virtual function vrj::DrawManager::setApp.
   virtual void setApp(vrj::App* p0)
   {
      setApp_callback_vrj_App(p0);
   }

   typedef void (*initAPI_callback_t)();
   initAPI_callback_t initAPI_callback;

   /// Override for virtual function vrj::DrawManager::initAPI.
   virtual void initAPI()
   {
      initAPI_callback();
   }

   typedef void (*addDisplay_callback_vrj_Display_t)(vrj::Display*);
   addDisplay_callback_vrj_Display_t addDisplay_callback_vrj_Display;

   /// Override for virtual function vrj::DrawManager::addDisplay.
   virtual void addDisplay(vrj::Display* p0)
   {
      addDisplay_callback_vrj_Display(p0);
   }

   typedef void (*removeDisplay_callback_vrj_Display_t)(vrj::Display*);
   removeDisplay_callback_vrj_Display_t removeDisplay_callback_vrj_Display;

   /// Override for virtual function vrj::DrawManager::removeDisplay.
   virtual void removeDisplay(vrj::Display* p0)
   {
      removeDisplay_callback_vrj_Display(p0);
   }

   typedef void (*closeAPI_callback_t)();
   closeAPI_callback_t closeAPI_callback;

   /// Override for virtual function vrj::DrawManager::closeAPI.
   virtual void closeAPI()
   {
      closeAPI_callback();
   }

}; // class vrj_DrawManager_Adapter




#endif
