// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/Input.h>
#include <gadget_Input_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget_Input_Adapter* gadget_Input_Input__0(gadget_Input_Adapter::config_callback_jccl_ConfigElementPtr_t cb0, gadget_Input_Adapter::sample_callback_t cb1, gadget_Input_Adapter::startSampling_callback_t cb2, gadget_Input_Adapter::stopSampling_callback_t cb3, gadget_Input_Adapter::updateData_callback_t cb4, gadget_Input_Adapter::getBaseType_callback_t cb5, gadget_Input_Adapter::writeObject_callback_vpr_ObjectWriter_t cb6, gadget_Input_Adapter::readObject_callback_vpr_ObjectReader_t cb7, gadget_Input_Adapter::destroy_callback_t cb8)
   {
      gadget_Input_Adapter* obj = new gadget_Input_Adapter();
      obj->config_callback_jccl_ConfigElementPtr = cb0;
      obj->sample_callback = cb1;
      obj->startSampling_callback = cb2;
      obj->stopSampling_callback = cb3;
      obj->updateData_callback = cb4;
      obj->getBaseType_callback = cb5;
      obj->writeObject_callback_vpr_ObjectWriter = cb6;
      obj->readObject_callback_vpr_ObjectReader = cb7;
      obj->destroy_callback = cb8;
      return obj;
   }

   SHARPPY_API void delete_gadget_Input(gadget_Input_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::Input::getInstanceName()
   SHARPPY_API char* gadget_Input_getInstanceName__0(gadget_Input_Adapter* self_)
   {
      std::string temp_result;
      char* result;
      temp_result = self_->getInstanceName();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for non-virtual method gadget::Input::isActive()
   SHARPPY_API bool gadget_Input_isActive__0(gadget_Input_Adapter* self_)
   {
      bool result;
      result = self_->isActive();
      return result;
   }


   // Wrapper for virtual method gadget::Input::config()
   SHARPPY_API bool gadget_Input_config__jccl_ConfigElementPtr1(gadget_Input_Adapter* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->gadget::Input::config(*p0);
      return result;
   }


   // Wrapper for virtual method gadget::Input::sample()
   SHARPPY_API bool gadget_Input_sample__0(gadget_Input_Adapter* self_)
   {
      bool result;
      result = self_->sample();
      return result;
   }


   // Wrapper for virtual method gadget::Input::startSampling()
   SHARPPY_API bool gadget_Input_startSampling__0(gadget_Input_Adapter* self_)
   {
      bool result;
      result = self_->startSampling();
      return result;
   }


   // Wrapper for virtual method gadget::Input::stopSampling()
   SHARPPY_API bool gadget_Input_stopSampling__0(gadget_Input_Adapter* self_)
   {
      bool result;
      result = self_->stopSampling();
      return result;
   }


   // Wrapper for virtual method gadget::Input::updateData()
   SHARPPY_API void gadget_Input_updateData__0(gadget_Input_Adapter* self_)
   {
      self_->updateData();
   }


   // Wrapper for virtual method gadget::Input::getBaseType()
   SHARPPY_API char* gadget_Input_getBaseType__0(gadget_Input_Adapter* self_)
   {
      std::string temp_result;
      char* result;
      temp_result = self_->gadget::Input::getBaseType();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for virtual method gadget::Input::writeObject()
   SHARPPY_API vpr::ReturnStatus* gadget_Input_writeObject__vpr_ObjectWriter1(gadget_Input_Adapter* self_, vpr::ObjectWriter* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->gadget::Input::writeObject(p0));
      return result;
   }


   // Wrapper for virtual method gadget::Input::readObject()
   SHARPPY_API vpr::ReturnStatus* gadget_Input_readObject__vpr_ObjectReader1(gadget_Input_Adapter* self_, vpr::ObjectReader* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->gadget::Input::readObject(p0));
      return result;
   }


   // Wrapper for virtual method gadget::Input::destroy()
   SHARPPY_API void gadget_Input_destroy__0(gadget_Input_Adapter* self_)
   {
      self_->destroy();
   }


   // Wrapper for static method gadget::Input::getElementType()
   SHARPPY_API char* gadget_Input_getElementType__0()
   {
      std::string temp_result;
      char* result;
      temp_result = gadget::Input::getElementType();
      result = strdup(temp_result.c_str());
      return result;
   }


} // extern "C" for gadget::Input


