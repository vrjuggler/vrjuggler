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
#include <vpr/Util/ReturnStatus.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API vpr::ReturnStatus* vpr_ReturnStatus_ReturnStatus__()
   {
      vpr::ReturnStatus* obj = new vpr::ReturnStatus();
      return obj;
   }

   // Copy constructor wrapper.
   SHARPPY_API vpr::ReturnStatus* vpr_ReturnStatus_ReturnStatus__vpr_ReturnStatus(const vpr::ReturnStatus* p)
   {
      vpr::ReturnStatus* obj = new vpr::ReturnStatus(*p);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vpr::ReturnStatus* vpr_ReturnStatus_ReturnStatus__vpr_ReturnStatus_Code(const vpr::ReturnStatus::Code* p0)
   {
      const vpr::ReturnStatus::Code& marshal_p0 = *p0; // Pre-call marshaling for p0
      vpr::ReturnStatus* obj = new vpr::ReturnStatus(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   SHARPPY_API void delete_vpr_ReturnStatus(vpr::ReturnStatus* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method vpr::ReturnStatus::copy()
   SHARPPY_API void vpr_ReturnStatus_copy__vpr_ReturnStatus(vpr::ReturnStatus* self_, const vpr::ReturnStatus* p0)
   {
      const vpr::ReturnStatus& marshal_p0 = *p0; // Pre-call marshaling for p0
      self_->copy(marshal_p0);
      ; // Post-call marshaling for p0
   }

   // Wrapper for non-virtual method vpr::ReturnStatus::setCode()
   SHARPPY_API void vpr_ReturnStatus_setCode__vpr_ReturnStatus_Code(vpr::ReturnStatus* self_, const vpr::ReturnStatus::Code* p0)
   {
      const vpr::ReturnStatus::Code& marshal_p0 = *p0; // Pre-call marshaling for p0
      self_->setCode(marshal_p0);
      ; // Post-call marshaling for p0
   }

   // Wrapper for non-virtual method vpr::ReturnStatus::code()
   SHARPPY_API  vpr::ReturnStatus::Code vpr_ReturnStatus_code__(vpr::ReturnStatus* self_)
   {
       vpr::ReturnStatus::Code result;
      result = self_->code();

      return result;
   }

   // Wrapper for non-virtual method vpr::ReturnStatus::success()
   SHARPPY_API bool vpr_ReturnStatus_success__(vpr::ReturnStatus* self_)
   {
      bool result;
      result = self_->success();

      return result;
   }

   // Wrapper for non-virtual method vpr::ReturnStatus::failure()
   SHARPPY_API bool vpr_ReturnStatus_failure__(vpr::ReturnStatus* self_)
   {
      bool result;
      result = self_->failure();

      return result;
   }

   // Wrapper for non-virtual method vpr::ReturnStatus::wouldBlock()
   SHARPPY_API bool vpr_ReturnStatus_wouldBlock__(vpr::ReturnStatus* self_)
   {
      bool result;
      result = self_->wouldBlock();

      return result;
   }

   // Wrapper for non-virtual method vpr::ReturnStatus::inProgress()
   SHARPPY_API bool vpr_ReturnStatus_inProgress__(vpr::ReturnStatus* self_)
   {
      bool result;
      result = self_->inProgress();

      return result;
   }

   // Wrapper for non-virtual method vpr::ReturnStatus::timeout()
   SHARPPY_API bool vpr_ReturnStatus_timeout__(vpr::ReturnStatus* self_)
   {
      bool result;
      result = self_->timeout();

      return result;
   }




   // Wrapper for non-virtual method vpr::ReturnStatus::operator ==()
   SHARPPY_API bool vpr_ReturnStatus_equal__vpr_ReturnStatus(vpr::ReturnStatus* self_, const vpr::ReturnStatus* p0)
   {
      const vpr::ReturnStatus& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->operator==(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }

   // Wrapper for non-virtual method vpr::ReturnStatus::operator ==()
   SHARPPY_API bool vpr_ReturnStatus_equal__vpr_ReturnStatus_Code(vpr::ReturnStatus* self_, const vpr::ReturnStatus::Code* p0)
   {
      const vpr::ReturnStatus::Code& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->operator==(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }

   // Wrapper for non-virtual method vpr::ReturnStatus::operator !=()
   SHARPPY_API bool vpr_ReturnStatus_not_equal__vpr_ReturnStatus(vpr::ReturnStatus* self_, const vpr::ReturnStatus* p0)
   {
      const vpr::ReturnStatus& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->operator!=(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }

   // Wrapper for non-virtual method vpr::ReturnStatus::operator !=()
   SHARPPY_API bool vpr_ReturnStatus_not_equal__vpr_ReturnStatus_Code(vpr::ReturnStatus* self_, const vpr::ReturnStatus::Code* p0)
   {
      const vpr::ReturnStatus::Code& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->operator!=(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }


} // extern "C" for vpr::ReturnStatus


