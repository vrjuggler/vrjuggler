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

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <jccl/RTRC/ConfigElementHandler.h>
#include <jccl_ConfigElementHandler_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API jccl_ConfigElementHandler_Adapter* jccl_ConfigElementHandler_ConfigElementHandler__0(jccl_ConfigElementHandler_Adapter::configCanHandle_callback_jccl_ConfigElementPtr_t cb0, jccl_ConfigElementHandler_Adapter::configProcessPending_callback_t cb1, jccl_ConfigElementHandler_Adapter::configAdd_callback_jccl_ConfigElementPtr_t cb2, jccl_ConfigElementHandler_Adapter::configRemove_callback_jccl_ConfigElementPtr_t cb3)
   {
      jccl_ConfigElementHandler_Adapter* obj = new jccl_ConfigElementHandler_Adapter();
      obj->configCanHandle_callback_jccl_ConfigElementPtr = cb0;
      obj->configProcessPending_callback = cb1;
      obj->configAdd_callback_jccl_ConfigElementPtr = cb2;
      obj->configRemove_callback_jccl_ConfigElementPtr = cb3;
      return obj;
   }

   SHARPPY_API void delete_jccl_ConfigElementHandler(jccl_ConfigElementHandler_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for virtual method jccl::ConfigElementHandler::configCanHandle()
   SHARPPY_API bool jccl_ConfigElementHandler_configCanHandle__jccl_ConfigElementPtr1(jccl_ConfigElementHandler_Adapter* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->configCanHandle(*p0);
      return result;
   }


   // Wrapper for virtual method jccl::ConfigElementHandler::configProcessPending()
   SHARPPY_API int jccl_ConfigElementHandler_configProcessPending__0(jccl_ConfigElementHandler_Adapter* self_)
   {
      int result;
      result = self_->jccl::ConfigElementHandler::configProcessPending();
      return result;
   }


   // Wrapper for virtual method jccl::ConfigElementHandler::configAdd()
   SHARPPY_API bool jccl_ConfigElementHandler_configAdd__jccl_ConfigElementPtr1(jccl_ConfigElementHandler_Adapter* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->configAdd(*p0);
      return result;
   }


   // Wrapper for virtual method jccl::ConfigElementHandler::configRemove()
   SHARPPY_API bool jccl_ConfigElementHandler_configRemove__jccl_ConfigElementPtr1(jccl_ConfigElementHandler_Adapter* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->configRemove(*p0);
      return result;
   }


} // extern "C" for jccl::ConfigElementHandler


