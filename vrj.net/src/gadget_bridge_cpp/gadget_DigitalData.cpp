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

// Generated from Revision: 1.62 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <gadget/Type/DigitalData.h>

extern "C"
{
   // Constructor wrapper.
   SHARPPY_API gadget::DigitalData* gadget_DigitalData_DigitalData__gadget_DigitalData(const gadget::DigitalData* p0)
   {
      const gadget::DigitalData& marshal_p0 = *p0;
      gadget::DigitalData* obj = new gadget::DigitalData(marshal_p0);
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::DigitalData* gadget_DigitalData_DigitalData__()
   {
      gadget::DigitalData* obj = new gadget::DigitalData();
      return obj;
   }

   // Constructor wrapper.
   SHARPPY_API gadget::DigitalData* gadget_DigitalData_DigitalData__int(const int p0)
   {
      gadget::DigitalData* obj = new gadget::DigitalData(p0);
      return obj;
   }

   SHARPPY_API void delete_gadget_DigitalData(gadget::DigitalData* self_)
   {
      delete self_;
   }

   // Wrapper for non-virtual method gadget::DigitalData::getDigital()
   SHARPPY_API int gadget_DigitalData_getDigital__0(gadget::DigitalData* self_)
   {
      int result;
      result = self_->getDigital();
      return result;
   }


   // Wrapper for non-virtual method gadget::DigitalData::setDigital()
   SHARPPY_API void gadget_DigitalData_setDigital__int1(gadget::DigitalData* self_, const int p0)
   {
      self_->setDigital(p0);
   }


} // extern "C" for gadget::DigitalData


