// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



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


   // Wrapper for virtual method gadget::PositionProxy::getTimeStamp()
   SHARPPY_API vpr::Interval* gadget_PositionProxy_getTimeStamp__0(gadget::PositionProxy* self_)
   {
      vpr::Interval* result;
      result = new vpr::Interval(self_->getTimeStamp());
      return result;
   }


   // Wrapper for virtual method gadget::PositionProxy::config()
   SHARPPY_API bool gadget_PositionProxy_config__jccl_ConfigElementPtr1(gadget::PositionProxy* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->config(*p0);
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
      std::string temp_result;
      char* result;
      temp_result = gadget::PositionProxy::getElementType();
      result = strdup(temp_result.c_str());
      return result;
   }


} // extern "C" for gadget::PositionProxy


