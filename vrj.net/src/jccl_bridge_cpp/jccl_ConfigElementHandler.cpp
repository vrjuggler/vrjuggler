// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

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


