// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.63 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <vpr/Util/ReturnStatus.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API vpr::ReturnStatus* vpr_ReturnStatus_ReturnStatus__0()
   {
      vpr::ReturnStatus* obj = new vpr::ReturnStatus();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vpr::ReturnStatus* vpr_ReturnStatus_ReturnStatus__vpr_ReturnStatus1(const vpr::ReturnStatus* p0)
   {
      const vpr::ReturnStatus& marshal_p0 = *p0;
      vpr::ReturnStatus* obj = new vpr::ReturnStatus(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vpr::ReturnStatus* vpr_ReturnStatus_ReturnStatus__vpr_ReturnStatus_Code1(const vpr::ReturnStatus::Code* p0)
   {
      const vpr::ReturnStatus::Code& marshal_p0 = *p0;
      vpr::ReturnStatus* obj = new vpr::ReturnStatus(marshal_p0);
      return obj;
   }

   SHARPPY_API void delete_vpr_ReturnStatus(vpr::ReturnStatus* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method vpr::ReturnStatus::copy()
   SHARPPY_API void vpr_ReturnStatus_copy__vpr_ReturnStatus1(vpr::ReturnStatus* self_, const vpr::ReturnStatus* p0)
   {
      const vpr::ReturnStatus& marshal_p0 = *p0;
      self_->copy(marshal_p0);
   }


   // Wrapper for non-virtual method vpr::ReturnStatus::setCode()
   SHARPPY_API void vpr_ReturnStatus_setCode__vpr_ReturnStatus_Code1(vpr::ReturnStatus* self_, const vpr::ReturnStatus::Code* p0)
   {
      const vpr::ReturnStatus::Code& marshal_p0 = *p0;
      self_->setCode(marshal_p0);
   }


   // Wrapper for non-virtual method vpr::ReturnStatus::code()
   SHARPPY_API  vpr::ReturnStatus::Code vpr_ReturnStatus_code__0(vpr::ReturnStatus* self_)
   {
       vpr::ReturnStatus::Code result;
      result = self_->code();
      return result;
   }


   // Wrapper for non-virtual method vpr::ReturnStatus::success()
   SHARPPY_API bool vpr_ReturnStatus_success__0(vpr::ReturnStatus* self_)
   {
      bool result;
      result = self_->success();
      return result;
   }


   // Wrapper for non-virtual method vpr::ReturnStatus::failure()
   SHARPPY_API bool vpr_ReturnStatus_failure__0(vpr::ReturnStatus* self_)
   {
      bool result;
      result = self_->failure();
      return result;
   }


   // Wrapper for non-virtual method vpr::ReturnStatus::wouldBlock()
   SHARPPY_API bool vpr_ReturnStatus_wouldBlock__0(vpr::ReturnStatus* self_)
   {
      bool result;
      result = self_->wouldBlock();
      return result;
   }


   // Wrapper for non-virtual method vpr::ReturnStatus::inProgress()
   SHARPPY_API bool vpr_ReturnStatus_inProgress__0(vpr::ReturnStatus* self_)
   {
      bool result;
      result = self_->inProgress();
      return result;
   }


   // Wrapper for non-virtual method vpr::ReturnStatus::timeout()
   SHARPPY_API bool vpr_ReturnStatus_timeout__0(vpr::ReturnStatus* self_)
   {
      bool result;
      result = self_->timeout();
      return result;
   }


   // Wrapper for non-virtual method vpr::ReturnStatus::operator ==()
   SHARPPY_API bool vpr_ReturnStatus_equal__vpr_ReturnStatus1(vpr::ReturnStatus* self_, const vpr::ReturnStatus* p0)
   {
      const vpr::ReturnStatus& marshal_p0 = *p0;
      bool result;
      result = self_->operator==(marshal_p0);
      return result;
   }


   // Wrapper for non-virtual method vpr::ReturnStatus::operator ==()
   SHARPPY_API bool vpr_ReturnStatus_equal__vpr_ReturnStatus_Code1(vpr::ReturnStatus* self_, const vpr::ReturnStatus::Code* p0)
   {
      const vpr::ReturnStatus::Code& marshal_p0 = *p0;
      bool result;
      result = self_->operator==(marshal_p0);
      return result;
   }


   // Wrapper for non-virtual method vpr::ReturnStatus::operator !=()
   SHARPPY_API bool vpr_ReturnStatus_not_equal__vpr_ReturnStatus1(vpr::ReturnStatus* self_, const vpr::ReturnStatus* p0)
   {
      const vpr::ReturnStatus& marshal_p0 = *p0;
      bool result;
      result = self_->operator!=(marshal_p0);
      return result;
   }


   // Wrapper for non-virtual method vpr::ReturnStatus::operator !=()
   SHARPPY_API bool vpr_ReturnStatus_not_equal__vpr_ReturnStatus_Code1(vpr::ReturnStatus* self_, const vpr::ReturnStatus::Code* p0)
   {
      const vpr::ReturnStatus::Code& marshal_p0 = *p0;
      bool result;
      result = self_->operator!=(marshal_p0);
      return result;
   }


} // extern "C" for vpr::ReturnStatus


