// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.64 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/DigitalProxy.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget::DigitalProxy* gadget_DigitalProxy_DigitalProxy__gadget_DigitalProxy1(const gadget::DigitalProxy* p0)
   {
      const gadget::DigitalProxy& marshal_p0 = *p0;
      gadget::DigitalProxy* obj = new gadget::DigitalProxy(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::DigitalProxy* gadget_DigitalProxy_DigitalProxy__0()
   {
      gadget::DigitalProxy* obj = new gadget::DigitalProxy();
      return obj;
   }

   SHARPPY_API void delete_gadget_DigitalProxy(gadget::DigitalProxy* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::DigitalProxy::getData()
   SHARPPY_API gadget::Digital::State gadget_DigitalProxy_getData__0(gadget::DigitalProxy* self_)
   {
      gadget::Digital::State result;
      result = self_->getData();
      return result;
   }


   // Wrapper for non-virtual method gadget::DigitalProxy::getDigitalData()
   SHARPPY_API gadget::DigitalData* gadget_DigitalProxy_getDigitalData__0(gadget::DigitalProxy* self_)
   {
      gadget::DigitalData* result;
      result = self_->getDigitalData();
      return result;
   }


   // Wrapper for non-virtual method gadget::DigitalProxy::getDigitalPtr()
   SHARPPY_API gadget::Digital* gadget_DigitalProxy_getDigitalPtr__0(gadget::DigitalProxy* self_)
   {
      gadget::Digital* result;
      result = self_->getDigitalPtr();
      return result;
   }


   // Wrapper for non-virtual method gadget::DigitalProxy::getUnit()
   SHARPPY_API int gadget_DigitalProxy_getUnit__0(gadget::DigitalProxy* self_)
   {
      int result;
      result = self_->getUnit();
      return result;
   }


   // Wrapper for virtual method gadget::DigitalProxy::updateData()
   SHARPPY_API void gadget_DigitalProxy_updateData__0(gadget::DigitalProxy* self_)
   {
      self_->updateData();
   }


   // Wrapper for virtual method gadget::DigitalProxy::getTimeStamp()
   SHARPPY_API vpr::Interval* gadget_DigitalProxy_getTimeStamp__0(gadget::DigitalProxy* self_)
   {
      vpr::Interval* result;
      result = new vpr::Interval(self_->getTimeStamp());
      return result;
   }


   // Wrapper for virtual method gadget::DigitalProxy::config()
   SHARPPY_API bool gadget_DigitalProxy_config__jccl_ConfigElementPtr1(gadget::DigitalProxy* self_, jccl::ConfigElementPtr* p0)
   {
      bool result;
      result = self_->config(*p0);
      return result;
   }


   // Wrapper for virtual method gadget::DigitalProxy::getProxiedInputDevice()
   SHARPPY_API gadget::Input* gadget_DigitalProxy_getProxiedInputDevice__0(gadget::DigitalProxy* self_)
   {
      gadget::Input* result;
      result = self_->getProxiedInputDevice();
      return result;
   }


   // Wrapper for static method gadget::DigitalProxy::getElementType()
   SHARPPY_API char* gadget_DigitalProxy_getElementType__0()
   {
      std::string temp_result;
      char* result;
      temp_result = gadget::DigitalProxy::getElementType();
      result = strdup(temp_result.c_str());
      return result;
   }


} // extern "C" for gadget::DigitalProxy


