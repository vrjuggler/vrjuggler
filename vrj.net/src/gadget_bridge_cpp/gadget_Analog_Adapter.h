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

// Generated from Revision: 1.18 of RCSfile: class_cxx_adapter.tmpl,v


#ifndef __GADGET_ANALOG_ADAPTER__
#define __GADGET_ANALOG_ADAPTER__

#include "sharppy.h"
#include <gadget/Type/Analog.h>

class SHARPPY_API gadget_Analog_Adapter : public gadget::Analog
{
public:
   gadget_Analog_Adapter()
      : gadget::Analog()
   {;}

   virtual ~gadget_Analog_Adapter()
   {;}

   typedef vpr::ReturnStatus* (*writeObject_callback_vpr_ObjectWriter_t)(vpr::ObjectWriter*);
   writeObject_callback_vpr_ObjectWriter_t writeObject_callback_vpr_ObjectWriter;

   /// Override for virtual function gadget::Analog::writeObject.
   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0)
   {
      vpr::ReturnStatus result = *(writeObject_callback_vpr_ObjectWriter(p0));
      return result;
   }

   typedef vpr::ReturnStatus* (*readObject_callback_vpr_ObjectReader_t)(vpr::ObjectReader*);
   readObject_callback_vpr_ObjectReader_t readObject_callback_vpr_ObjectReader;

   /// Override for virtual function gadget::Analog::readObject.
   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* p0)
   {
      vpr::ReturnStatus result = *(readObject_callback_vpr_ObjectReader(p0));
      return result;
   }

   typedef bool (*config_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   config_callback_jccl_ConfigElementPtr_t config_callback_jccl_ConfigElementPtr;

   /// Override for virtual function gadget::Analog::config.
   virtual bool config(jccl::ConfigElementPtr p0)
   {
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = config_callback_jccl_ConfigElementPtr(p_p0);
      return result;
   }

   typedef char* (*getBaseType_callback_t)();
   getBaseType_callback_t getBaseType_callback;

   /// Override for virtual function gadget::Analog::getBaseType.
   virtual std::string getBaseType()
   {
      std::string result = getBaseType_callback();
      return result;
   }

   /// Public wrapper for protected non-virtual function gadget::Analog::normalizeMinToMax.
   void normalizeMinToMax(const float& p0, float& p1)
   {
      gadget::Analog::normalizeMinToMax(p0, p1);
   }

   /// Public wrapper for protected non-virtual function gadget::Analog::getMin.
   float getMin()
   {
      float result = gadget::Analog::getMin();
      return result;
   }

   /// Public wrapper for protected non-virtual function gadget::Analog::getMax.
   float getMax()
   {
      float result = gadget::Analog::getMax();
      return result;
   }

   /// Public wrapper for protected non-virtual function gadget::Analog::setMin.
   void setMin(float p0)
   {
      gadget::Analog::setMin(p0);
   }

   /// Public wrapper for protected non-virtual function gadget::Analog::setMax.
   void setMax(float p0)
   {
      gadget::Analog::setMax(p0);
   }

}; // class gadget_Analog_Adapter




#endif
