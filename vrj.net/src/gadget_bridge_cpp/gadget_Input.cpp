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
#include <gadget/Type/Input.h>
#include <gadget_Input_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget_Input_Adapter* gadget_Input_Input__(gadget_Input_Adapter::config_callback_boost_shared_ptr_jccl__ConfigElement_t cb0, gadget_Input_Adapter::sample_callback_t cb1, gadget_Input_Adapter::startSampling_callback_t cb2, gadget_Input_Adapter::stopSampling_callback_t cb3, gadget_Input_Adapter::updateData_callback_t cb4, gadget_Input_Adapter::getBaseType_callback_t cb5, gadget_Input_Adapter::writeObject_callback_vpr_ObjectWriter_t cb6, gadget_Input_Adapter::readObject_callback_vpr_ObjectReader_t cb7, gadget_Input_Adapter::destroy_callback_t cb8)
   {
      gadget_Input_Adapter* obj = new gadget_Input_Adapter();
      obj->config_callback_boost_shared_ptr_jccl__ConfigElement = cb0;
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
   SHARPPY_API char* gadget_Input_getInstanceName__(gadget_Input_Adapter* self_)
   {
      char* result;
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      temp_result = self_->getInstanceName();
      result = strdup(temp_result.c_str());

      return result;
   }

   // Wrapper for non-virtual method gadget::Input::isActive()
   SHARPPY_API bool gadget_Input_isActive__(gadget_Input_Adapter* self_)
   {
      bool result;
      result = self_->isActive();

      return result;
   }


   // Wrapper for virtual method gadget::Input::config()
   SHARPPY_API bool gadget_Input_config__boost_shared_ptr_jccl__ConfigElement(gadget_Input_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->gadget::Input::config(p0);

      return result;
   }

   // Wrapper for virtual method gadget::Input::sample()
   SHARPPY_API bool gadget_Input_sample__(gadget_Input_Adapter* self_)
   {
      bool result;
      result = self_->sample();

      return result;
   }

   // Wrapper for virtual method gadget::Input::startSampling()
   SHARPPY_API bool gadget_Input_startSampling__(gadget_Input_Adapter* self_)
   {
      bool result;
      result = self_->startSampling();

      return result;
   }

   // Wrapper for virtual method gadget::Input::stopSampling()
   SHARPPY_API bool gadget_Input_stopSampling__(gadget_Input_Adapter* self_)
   {
      bool result;
      result = self_->stopSampling();

      return result;
   }

   // Wrapper for virtual method gadget::Input::updateData()
   SHARPPY_API void gadget_Input_updateData__(gadget_Input_Adapter* self_)
   {
      self_->updateData();
   }

   // Wrapper for virtual method gadget::Input::getBaseType()
   SHARPPY_API char* gadget_Input_getBaseType__(gadget_Input_Adapter* self_)
   {
      char* result;
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      temp_result = self_->gadget::Input::getBaseType();
      result = strdup(temp_result.c_str());

      return result;
   }

   // Wrapper for virtual method gadget::Input::writeObject()
   SHARPPY_API vpr::ReturnStatus* gadget_Input_writeObject__vpr_ObjectWriter(gadget_Input_Adapter* self_, vpr::ObjectWriter* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->gadget::Input::writeObject(p0));

      return result;
   }

   // Wrapper for virtual method gadget::Input::readObject()
   SHARPPY_API vpr::ReturnStatus* gadget_Input_readObject__vpr_ObjectReader(gadget_Input_Adapter* self_, vpr::ObjectReader* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->gadget::Input::readObject(p0));

      return result;
   }

   // Wrapper for virtual method gadget::Input::destroy()
   SHARPPY_API void gadget_Input_destroy__(gadget_Input_Adapter* self_)
   {
      self_->destroy();
   }


   // Wrapper for static method gadget::Input::getElementType()
   SHARPPY_API char* gadget_Input_getElementType__()
   {
      char* result;
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      temp_result = gadget::Input::getElementType();
      result = strdup(temp_result.c_str());

      return result;
   }



} // extern "C" for gadget::Input


