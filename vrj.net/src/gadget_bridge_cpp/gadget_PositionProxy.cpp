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
#include <gadget/Type/PositionProxy.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget::PositionProxy* gadget_PositionProxy_PositionProxy__gadget_PositionProxy1(const gadget::PositionProxy* p0)
   {
      const gadget::PositionProxy& marshal_p0 = *p0;
      gadget::PositionProxy* obj = new gadget::PositionProxy(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::PositionProxy* gadget_PositionProxy_PositionProxy__0()
   {
      gadget::PositionProxy* obj = new gadget::PositionProxy();
      return obj;
   }

   SHARPPY_API void delete_gadget_PositionProxy(gadget::PositionProxy* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::PositionProxy::getTimeStamp()
   SHARPPY_API vpr::Interval* gadget_PositionProxy_getTimeStamp__0(gadget::PositionProxy* self_)
   {
      vpr::Interval* result;
      result = new vpr::Interval(self_->getTimeStamp());
      return result;
   }


   // Wrapper for non-virtual method gadget::PositionProxy::getData()
   SHARPPY_API gmtl::Matrix44f* gadget_PositionProxy_getData__float0(gadget::PositionProxy* self_)
   {
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(self_->getData());
      return result;
   }

   // Wrapper for non-virtual method gadget::PositionProxy::getData()
   SHARPPY_API gmtl::Matrix44f* gadget_PositionProxy_getData__float1(gadget::PositionProxy* self_, float p0)
   {
      gmtl::Matrix44f* result;
      result = new gmtl::Matrix44f(self_->getData(p0));
      return result;
   }


   // Wrapper for non-virtual method gadget::PositionProxy::getUnit()
   SHARPPY_API int gadget_PositionProxy_getUnit__0(gadget::PositionProxy* self_)
   {
      int result;
      result = self_->getUnit();
      return result;
   }


   // Wrapper for virtual method gadget::PositionProxy::updateData()
   SHARPPY_API void gadget_PositionProxy_updateData__0(gadget::PositionProxy* self_)
   {
      self_->updateData();
   }


   // Wrapper for virtual method gadget::PositionProxy::config()
   SHARPPY_API bool gadget_PositionProxy_config__boost_shared_ptr_jccl__ConfigElement1(gadget::PositionProxy* self_, boost::shared_ptr<jccl::ConfigElement> p0)
   {
      bool result;
      result = self_->config(p0);
      return result;
   }


   // Wrapper for virtual method gadget::PositionProxy::getProxiedInputDevice()
   SHARPPY_API gadget::Input* gadget_PositionProxy_getProxiedInputDevice__0(gadget::PositionProxy* self_)
   {
      gadget::Input* result;
      result = self_->getProxiedInputDevice();
      return result;
   }


   // Wrapper for static method gadget::PositionProxy::getElementType()
   SHARPPY_API char* gadget_PositionProxy_getElementType__0()
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > temp_result;
      char* result;
      temp_result = gadget::PositionProxy::getElementType();
      result = strdup(temp_result.c_str());
      return result;
   }


} // extern "C" for gadget::PositionProxy


