// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/Analog.h>
#include <gadget_Analog_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget_Analog_Adapter* gadget_Analog_Analog__0(gadget_Analog_Adapter::writeObject_callback_vpr_ObjectWriter_t cb0, gadget_Analog_Adapter::readObject_callback_vpr_ObjectReader_t cb1, gadget_Analog_Adapter::config_callback_jccl_ConfigElementPtr_t cb2, gadget_Analog_Adapter::getBaseType_callback_t cb3)
   {
      gadget_Analog_Adapter* obj = new gadget_Analog_Adapter();
      obj->writeObject_callback_vpr_ObjectWriter = cb0;
      obj->readObject_callback_vpr_ObjectReader = cb1;
      obj->config_callback_jccl_ConfigElementPtr = cb2;
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
   SHARPPY_API bool gadget_Analog_config__jccl_ConfigElementPtr1(gadget_Analog_Adapter* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->gadget::Analog::config(*p0);
      return result;
   }


   // Wrapper for virtual method gadget::Analog::getBaseType()
   SHARPPY_API char* gadget_Analog_getBaseType__0(gadget_Analog_Adapter* self_)
   {
      std::string temp_result;
      char* result;
      temp_result = self_->gadget::Analog::getBaseType();
      result = strdup(temp_result.c_str());
      return result;
   }


} // extern "C" for gadget::Analog


