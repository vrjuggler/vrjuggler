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

// Generated from Revision: 1.58 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/AnalogProxy.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget::AnalogProxy* gadget_AnalogProxy_AnalogProxy__gadget_AnalogProxy(const gadget::AnalogProxy* p0)
   {
      const gadget::AnalogProxy& marshal_p0 = *p0; // Pre-call marshaling for p0
      gadget::AnalogProxy* obj = new gadget::AnalogProxy(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::AnalogProxy* gadget_AnalogProxy_AnalogProxy__()
   {
      gadget::AnalogProxy* obj = new gadget::AnalogProxy();
      return obj;
   }

   SHARPPY_API void delete_gadget_AnalogProxy(gadget::AnalogProxy* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::AnalogProxy::getTimeStamp()
   SHARPPY_API vpr::Interval* gadget_AnalogProxy_getTimeStamp__(gadget::AnalogProxy* self_)
   {
      vpr::Interval* result;
      result = new vpr::Interval(self_->getTimeStamp());

      return result;
   }

   // Wrapper for non-virtual method gadget::AnalogProxy::getData()
   SHARPPY_API float gadget_AnalogProxy_getData__(gadget::AnalogProxy* self_)
   {
      float result;
      result = self_->getData();

      return result;
   }

   // Wrapper for non-virtual method gadget::AnalogProxy::getAnalogPtr()
   SHARPPY_API gadget::Analog* gadget_AnalogProxy_getAnalogPtr__(gadget::AnalogProxy* self_)
   {
      gadget::Analog* result;
      result = self_->getAnalogPtr();

      return result;
   }

   // Wrapper for non-virtual method gadget::AnalogProxy::getUnit()
   SHARPPY_API int gadget_AnalogProxy_getUnit__(gadget::AnalogProxy* self_)
   {
      int result;
      result = self_->getUnit();

      return result;
   }



   // Wrapper for virtual method gadget::AnalogProxy::updateData()
   SHARPPY_API void gadget_AnalogProxy_updateData__(gadget::AnalogProxy* self_)
   {
      self_->updateData();
   }

   // Wrapper for virtual method gadget::AnalogProxy::config()
   SHARPPY_API bool gadget_AnalogProxy_config__boost_shared_ptr_jccl__ConfigElement(gadget::AnalogProxy* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->config(p0);

      return result;
   }

   // Wrapper for virtual method gadget::AnalogProxy::getProxiedInputDevice()
   SHARPPY_API gadget::Input* gadget_AnalogProxy_getProxiedInputDevice__(gadget::AnalogProxy* self_)
   {
      gadget::Input* result;
      result = self_->getProxiedInputDevice();

      return result;
   }


   // Wrapper for static method gadget::AnalogProxy::getElementType()
   SHARPPY_API char* gadget_AnalogProxy_getElementType__()
   {
      char* result;
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      temp_result = gadget::AnalogProxy::getElementType();
      result = strdup(temp_result.c_str());

      return result;
   }



} // extern "C" for gadget::AnalogProxy


