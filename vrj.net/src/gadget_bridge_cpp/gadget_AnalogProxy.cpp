// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/AnalogProxy.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget::AnalogProxy* gadget_AnalogProxy_AnalogProxy__gadget_AnalogProxy1(const gadget::AnalogProxy* p0)
   {
      const gadget::AnalogProxy& marshal_p0 = *p0;
      gadget::AnalogProxy* obj = new gadget::AnalogProxy(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::AnalogProxy* gadget_AnalogProxy_AnalogProxy__0()
   {
      gadget::AnalogProxy* obj = new gadget::AnalogProxy();
      return obj;
   }

   SHARPPY_API void delete_gadget_AnalogProxy(gadget::AnalogProxy* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::AnalogProxy::getData()
   SHARPPY_API float gadget_AnalogProxy_getData__0(gadget::AnalogProxy* self_)
   {
      float result;
      result = self_->getData();
      return result;
   }


   // Wrapper for non-virtual method gadget::AnalogProxy::getAnalogPtr()
   SHARPPY_API gadget::Analog* gadget_AnalogProxy_getAnalogPtr__0(gadget::AnalogProxy* self_)
   {
      gadget::Analog* result;
      result = self_->getAnalogPtr();
      return result;
   }


   // Wrapper for non-virtual method gadget::AnalogProxy::getUnit()
   SHARPPY_API int gadget_AnalogProxy_getUnit__0(gadget::AnalogProxy* self_)
   {
      int result;
      result = self_->getUnit();
      return result;
   }


   // Wrapper for virtual method gadget::AnalogProxy::updateData()
   SHARPPY_API void gadget_AnalogProxy_updateData__0(gadget::AnalogProxy* self_)
   {
      self_->updateData();
   }


   // Wrapper for virtual method gadget::AnalogProxy::getTimeStamp()
   SHARPPY_API vpr::Interval* gadget_AnalogProxy_getTimeStamp__0(gadget::AnalogProxy* self_)
   {
      vpr::Interval* result;
      result = new vpr::Interval(self_->getTimeStamp());
      return result;
   }


   // Wrapper for virtual method gadget::AnalogProxy::config()
   SHARPPY_API bool gadget_AnalogProxy_config__jccl_ConfigElementPtr1(gadget::AnalogProxy* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->config(*p0);
      return result;
   }


   // Wrapper for virtual method gadget::AnalogProxy::getProxiedInputDevice()
   SHARPPY_API gadget::Input* gadget_AnalogProxy_getProxiedInputDevice__0(gadget::AnalogProxy* self_)
   {
      gadget::Input* result;
      result = self_->getProxiedInputDevice();
      return result;
   }


   // Wrapper for static method gadget::AnalogProxy::getElementType()
   SHARPPY_API char* gadget_AnalogProxy_getElementType__0()
   {
      std::string temp_result;
      char* result;
      temp_result = gadget::AnalogProxy::getElementType();
      result = strdup(temp_result.c_str());
      return result;
   }


} // extern "C" for gadget::AnalogProxy


