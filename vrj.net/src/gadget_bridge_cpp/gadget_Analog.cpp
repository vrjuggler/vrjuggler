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

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/Analog.h>
#include <gadget_Analog_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget_Analog_Adapter* gadget_Analog_Analog__0(gadget_Analog_Adapter::writeObject_callback_vpr_ObjectWriter_t cb0, gadget_Analog_Adapter::readObject_callback_vpr_ObjectReader_t cb1, gadget_Analog_Adapter::config_callback_boost_shared_ptr_jccl__ConfigElement_t cb2, gadget_Analog_Adapter::getBaseType_callback_t cb3)
   {
      gadget_Analog_Adapter* obj = new gadget_Analog_Adapter();
      obj->writeObject_callback_vpr_ObjectWriter = cb0;
      obj->readObject_callback_vpr_ObjectReader = cb1;
      obj->config_callback_boost_shared_ptr_jccl__ConfigElement = cb2;
      obj->getBaseType_callback = cb3;
      return obj;
   }

   SHARPPY_API void delete_gadget_Analog(gadget_Analog_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::Analog::getAnalogData()
   SHARPPY_API gadget::AnalogData* gadget_Analog_getAnalogData__int0(gadget_Analog_Adapter* self_)
   {
      gadget::AnalogData* result;
      result = new gadget::AnalogData(self_->getAnalogData());
      return result;
   }

   // Wrapper for non-virtual method gadget::Analog::getAnalogData()
   SHARPPY_API gadget::AnalogData* gadget_Analog_getAnalogData__int1(gadget_Analog_Adapter* self_, int p0)
   {
      gadget::AnalogData* result;
      result = new gadget::AnalogData(self_->getAnalogData(p0));
      return result;
   }


   // Wrapper for non-virtual method gadget::Analog::swapAnalogBuffers()
   SHARPPY_API void gadget_Analog_swapAnalogBuffers__0(gadget_Analog_Adapter* self_)
   {
      self_->swapAnalogBuffers();
   }


   // Wrapper for non-virtual method gadget::Analog::normalizeMinToMax()
   SHARPPY_API void gadget_Analog_normalizeMinToMax__float_float2(gadget_Analog_Adapter* self_, const float p0, float& p1)
   {
      self_->normalizeMinToMax(p0, p1);
   }


   // Wrapper for non-virtual method gadget::Analog::getMin()
   SHARPPY_API float gadget_Analog_getMin__0(gadget_Analog_Adapter* self_)
   {
      float result;
      result = self_->getMin();
      return result;
   }


   // Wrapper for non-virtual method gadget::Analog::getMax()
   SHARPPY_API float gadget_Analog_getMax__0(gadget_Analog_Adapter* self_)
   {
      float result;
      result = self_->getMax();
      return result;
   }


   // Wrapper for non-virtual method gadget::Analog::setMin()
   SHARPPY_API void gadget_Analog_setMin__float1(gadget_Analog_Adapter* self_, float p0)
   {
      self_->setMin(p0);
   }


   // Wrapper for non-virtual method gadget::Analog::setMax()
   SHARPPY_API void gadget_Analog_setMax__float1(gadget_Analog_Adapter* self_, float p0)
   {
      self_->setMax(p0);
   }


   // Wrapper for virtual method gadget::Analog::writeObject()
   SHARPPY_API vpr::ReturnStatus* gadget_Analog_writeObject__vpr_ObjectWriter1(gadget_Analog_Adapter* self_, vpr::ObjectWriter* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->gadget::Analog::writeObject(p0));
      return result;
   }


   // Wrapper for virtual method gadget::Analog::readObject()
   SHARPPY_API vpr::ReturnStatus* gadget_Analog_readObject__vpr_ObjectReader1(gadget_Analog_Adapter* self_, vpr::ObjectReader* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->gadget::Analog::readObject(p0));
      return result;
   }


   // Wrapper for virtual method gadget::Analog::config()
   SHARPPY_API bool gadget_Analog_config__boost_shared_ptr_jccl__ConfigElement1(gadget_Analog_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->gadget::Analog::config(p0);
      return result;
   }


   // Wrapper for virtual method gadget::Analog::getBaseType()
   SHARPPY_API char* gadget_Analog_getBaseType__0(gadget_Analog_Adapter* self_)
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      char* result;
      temp_result = self_->gadget::Analog::getBaseType();
      result = strdup(temp_result.c_str());
      return result;
   }


} // extern "C" for gadget::Analog


