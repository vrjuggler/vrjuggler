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
#include <gadget/Type/Digital.h>
#include <gadget_Digital_Adapter.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget_Digital_Adapter* gadget_Digital_Digital__(gadget_Digital_Adapter::config_callback_boost_shared_ptr_jccl__ConfigElement_t cb0, gadget_Digital_Adapter::getBaseType_callback_t cb1, gadget_Digital_Adapter::writeObject_callback_vpr_ObjectWriter_t cb2, gadget_Digital_Adapter::readObject_callback_vpr_ObjectReader_t cb3)
   {
      gadget_Digital_Adapter* obj = new gadget_Digital_Adapter();
      obj->config_callback_boost_shared_ptr_jccl__ConfigElement = cb0;
      obj->getBaseType_callback = cb1;
      obj->writeObject_callback_vpr_ObjectWriter = cb2;
      obj->readObject_callback_vpr_ObjectReader = cb3;
      return obj;
   }

   SHARPPY_API void delete_gadget_Digital(gadget_Digital_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::Digital::getDigitalData()
   SHARPPY_API const gadget::DigitalData* gadget_Digital_getDigitalData__int0(gadget_Digital_Adapter* self_)
   {
      const gadget::DigitalData* result;
      result = new const gadget::DigitalData(self_->getDigitalData());
      return result;
   }

   // Wrapper for non-virtual method gadget::Digital::getDigitalData()
   SHARPPY_API const gadget::DigitalData* gadget_Digital_getDigitalData__int1(gadget_Digital_Adapter* self_, int p0)
   {
      const gadget::DigitalData* result;
      result = new const gadget::DigitalData(self_->getDigitalData(p0));
      return result;
   }


   // Wrapper for non-virtual method gadget::Digital::swapDigitalBuffers()
   SHARPPY_API void gadget_Digital_swapDigitalBuffers__0(gadget_Digital_Adapter* self_)
   {
      self_->swapDigitalBuffers();
   }


   // Wrapper for virtual method gadget::Digital::config()
   SHARPPY_API bool gadget_Digital_config__boost_shared_ptr_jccl__ConfigElement1(gadget_Digital_Adapter* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->gadget::Digital::config(p0);
      return result;
   }


   // Wrapper for virtual method gadget::Digital::getBaseType()
   SHARPPY_API char* gadget_Digital_getBaseType__0(gadget_Digital_Adapter* self_)
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      char* result;
      temp_result = self_->gadget::Digital::getBaseType();
      result = strdup(temp_result.c_str());
      return result;
   }


   // Wrapper for virtual method gadget::Digital::writeObject()
   SHARPPY_API vpr::ReturnStatus* gadget_Digital_writeObject__vpr_ObjectWriter1(gadget_Digital_Adapter* self_, vpr::ObjectWriter* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->gadget::Digital::writeObject(p0));
      return result;
   }


   // Wrapper for virtual method gadget::Digital::readObject()
   SHARPPY_API vpr::ReturnStatus* gadget_Digital_readObject__vpr_ObjectReader1(gadget_Digital_Adapter* self_, vpr::ObjectReader* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->gadget::Digital::readObject(p0));
      return result;
   }


} // extern "C" for gadget::Digital


