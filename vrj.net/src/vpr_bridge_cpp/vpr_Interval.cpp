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
#include <vpr/Util/Interval.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API vpr::Interval* vpr_Interval_Interval__vpr_Interval(const vpr::Interval* p0)
   {
      const vpr::Interval& marshal_p0 = *p0; // Pre-call marshaling for p0
      vpr::Interval* obj = new vpr::Interval(marshal_p0);
      ; // Post-call marshaling for p0
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vpr::Interval* vpr_Interval_Interval__()
   {
      vpr::Interval* obj = new vpr::Interval();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API vpr::Interval* vpr_Interval_Interval__long_long_unsigned_int_vpr_Interval_Unit(const long long unsigned int p0, const vpr::Interval::Unit p1)
   {
      vpr::Interval* obj = new vpr::Interval(p0, p1);
      return obj;
   }

   SHARPPY_API void delete_vpr_Interval(vpr::Interval* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method vpr::Interval::set()
   SHARPPY_API void vpr_Interval_set__long_long_unsigned_int_vpr_Interval_Unit(vpr::Interval* self_, const long long unsigned int p0, const vpr::Interval::Unit p1)
   {
      self_->set(p0, p1);
   }

   // Wrapper for non-virtual method vpr::Interval::setf()
   SHARPPY_API void vpr_Interval_setf__float_vpr_Interval_Unit(vpr::Interval* self_, const float p0, const vpr::Interval::Unit p1)
   {
      self_->setf(p0, p1);
   }

   // Wrapper for non-virtual method vpr::Interval::setd()
   SHARPPY_API void vpr_Interval_setd__double_vpr_Interval_Unit(vpr::Interval* self_, const double p0, const vpr::Interval::Unit p1)
   {
      self_->setd(p0, p1);
   }

   // Wrapper for non-virtual method vpr::Interval::setNow()
   SHARPPY_API void vpr_Interval_setNow__(vpr::Interval* self_)
   {
      self_->setNow();
   }

   // Wrapper for non-virtual method vpr::Interval::setNowReal()
   SHARPPY_API void vpr_Interval_setNowReal__(vpr::Interval* self_)
   {
      self_->setNowReal();
   }

   // Wrapper for non-virtual method vpr::Interval::sec()
   SHARPPY_API void vpr_Interval_sec__long_long_unsigned_int(vpr::Interval* self_, const long long unsigned int p0)
   {
      self_->sec(p0);
   }

   // Wrapper for non-virtual method vpr::Interval::sec()
   SHARPPY_API long long unsigned int vpr_Interval_sec__(vpr::Interval* self_)
   {
      long long unsigned int result;
      result = self_->sec();

      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::secf()
   SHARPPY_API void vpr_Interval_secf__float(vpr::Interval* self_, const float p0)
   {
      self_->secf(p0);
   }

   // Wrapper for non-virtual method vpr::Interval::secf()
   SHARPPY_API float vpr_Interval_secf__(vpr::Interval* self_)
   {
      float result;
      result = self_->secf();

      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::secd()
   SHARPPY_API void vpr_Interval_secd__double(vpr::Interval* self_, const double p0)
   {
      self_->secd(p0);
   }

   // Wrapper for non-virtual method vpr::Interval::secd()
   SHARPPY_API double vpr_Interval_secd__(vpr::Interval* self_)
   {
      double result;
      result = self_->secd();

      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::msec()
   SHARPPY_API void vpr_Interval_msec__long_long_unsigned_int(vpr::Interval* self_, const long long unsigned int p0)
   {
      self_->msec(p0);
   }

   // Wrapper for non-virtual method vpr::Interval::msec()
   SHARPPY_API long long unsigned int vpr_Interval_msec__(vpr::Interval* self_)
   {
      long long unsigned int result;
      result = self_->msec();

      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::msecf()
   SHARPPY_API void vpr_Interval_msecf__float(vpr::Interval* self_, const float p0)
   {
      self_->msecf(p0);
   }

   // Wrapper for non-virtual method vpr::Interval::msecf()
   SHARPPY_API float vpr_Interval_msecf__(vpr::Interval* self_)
   {
      float result;
      result = self_->msecf();

      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::msecd()
   SHARPPY_API void vpr_Interval_msecd__double(vpr::Interval* self_, const double p0)
   {
      self_->msecd(p0);
   }

   // Wrapper for non-virtual method vpr::Interval::msecd()
   SHARPPY_API double vpr_Interval_msecd__(vpr::Interval* self_)
   {
      double result;
      result = self_->msecd();

      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::usec()
   SHARPPY_API void vpr_Interval_usec__long_long_unsigned_int(vpr::Interval* self_, const long long unsigned int p0)
   {
      self_->usec(p0);
   }

   // Wrapper for non-virtual method vpr::Interval::usec()
   SHARPPY_API long long unsigned int vpr_Interval_usec__(vpr::Interval* self_)
   {
      long long unsigned int result;
      result = self_->usec();

      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::usecf()
   SHARPPY_API void vpr_Interval_usecf__float(vpr::Interval* self_, const float p0)
   {
      self_->usecf(p0);
   }

   // Wrapper for non-virtual method vpr::Interval::usecf()
   SHARPPY_API float vpr_Interval_usecf__(vpr::Interval* self_)
   {
      float result;
      result = self_->usecf();

      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::usecd()
   SHARPPY_API void vpr_Interval_usecd__double(vpr::Interval* self_, const double p0)
   {
      self_->usecd(p0);
   }

   // Wrapper for non-virtual method vpr::Interval::usecd()
   SHARPPY_API double vpr_Interval_usecd__(vpr::Interval* self_)
   {
      double result;
      result = self_->usecd();

      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::getBaseVal()
   SHARPPY_API long long unsigned int vpr_Interval_getBaseVal__(vpr::Interval* self_)
   {
      long long unsigned int result;
      result = self_->getBaseVal();

      return result;
   }




   // Wrapper for static method vpr::Interval::now()
   SHARPPY_API vpr::Interval* vpr_Interval_now__()
   {
      vpr::Interval* result;
      result = new vpr::Interval(vpr::Interval::now());

      return result;
   }


   // Wrapper for reading static data member vpr::Interval::NoWait
   SHARPPY_API const vpr::Interval* vpr_Interval_NoWait_get()
   {
      return &vpr::Interval::NoWait;
   }


   // Wrapper for reading static data member vpr::Interval::NoTimeout
   SHARPPY_API const vpr::Interval* vpr_Interval_NoTimeout_get()
   {
      return &vpr::Interval::NoTimeout;
   }


   // Wrapper for reading static data member vpr::Interval::HalfPeriod
   SHARPPY_API const vpr::Interval* vpr_Interval_HalfPeriod_get()
   {
      return &vpr::Interval::HalfPeriod;
   }


   // Wrapper for non-virtual method vpr::Interval::operator ==()
   SHARPPY_API bool vpr_Interval_equal__vpr_Interval(vpr::Interval* self_, const vpr::Interval* p0)
   {
      const vpr::Interval& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->operator==(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::operator !=()
   SHARPPY_API bool vpr_Interval_not_equal__vpr_Interval(vpr::Interval* self_, const vpr::Interval* p0)
   {
      const vpr::Interval& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->operator!=(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::operator <()
   SHARPPY_API bool vpr_Interval_less_than__vpr_Interval(vpr::Interval* self_, const vpr::Interval* p0)
   {
      const vpr::Interval& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->operator<(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::operator <=()
   SHARPPY_API bool vpr_Interval_less_than_or_equal__vpr_Interval(vpr::Interval* self_, const vpr::Interval* p0)
   {
      const vpr::Interval& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->operator<=(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::operator >()
   SHARPPY_API bool vpr_Interval_greater_than__vpr_Interval(vpr::Interval* self_, const vpr::Interval* p0)
   {
      const vpr::Interval& marshal_p0 = *p0; // Pre-call marshaling for p0
      bool result;
      result = self_->operator>(marshal_p0);

      ; // Post-call marshaling for p0
      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::operator +()
   SHARPPY_API vpr::Interval* vpr_Interval_add__vpr_Interval(vpr::Interval* self_, const vpr::Interval* p0)
   {
      const vpr::Interval& marshal_p0 = *p0; // Pre-call marshaling for p0
      vpr::Interval* result;
      result = new vpr::Interval(self_->operator+(marshal_p0));

      ; // Post-call marshaling for p0
      return result;
   }

   // Wrapper for non-virtual method vpr::Interval::operator -()
   SHARPPY_API vpr::Interval* vpr_Interval_subtract__vpr_Interval(vpr::Interval* self_, const vpr::Interval* p0)
   {
      const vpr::Interval& marshal_p0 = *p0; // Pre-call marshaling for p0
      vpr::Interval* result;
      result = new vpr::Interval(self_->operator-(marshal_p0));

      ; // Post-call marshaling for p0
      return result;
   }


} // extern "C" for vpr::Interval


