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


#ifndef __GADGET_INPUT_ADAPTER__
#define __GADGET_INPUT_ADAPTER__

#include "sharppy.h"
#include <gadget/Type/Input.h>

class SHARPPY_API gadget_Input_Adapter : public gadget::Input
{
public:
   gadget_Input_Adapter()
      : gadget::Input()
   {;}

   virtual ~gadget_Input_Adapter()
   {;}

   typedef bool (*config_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   config_callback_jccl_ConfigElementPtr_t config_callback_jccl_ConfigElementPtr;

   /// Override for virtual function gadget::Input::config.
   virtual bool config(jccl::ConfigElementPtr p0)
   {
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = config_callback_jccl_ConfigElementPtr(p_p0);
      return result;
   }

   typedef bool (*sample_callback_t)();
   sample_callback_t sample_callback;

   /// Override for virtual function gadget::Input::sample.
   virtual bool sample()
   {
      bool result = sample_callback();
      return result;
   }

   typedef bool (*startSampling_callback_t)();
   startSampling_callback_t startSampling_callback;

   /// Override for virtual function gadget::Input::startSampling.
   virtual bool startSampling()
   {
      bool result = startSampling_callback();
      return result;
   }

   typedef bool (*stopSampling_callback_t)();
   stopSampling_callback_t stopSampling_callback;

   /// Override for virtual function gadget::Input::stopSampling.
   virtual bool stopSampling()
   {
      bool result = stopSampling_callback();
      return result;
   }

   typedef void (*updateData_callback_t)();
   updateData_callback_t updateData_callback;

   /// Override for virtual function gadget::Input::updateData.
   virtual void updateData()
   {
      updateData_callback();
   }

   typedef char* (*getBaseType_callback_t)();
   getBaseType_callback_t getBaseType_callback;

   /// Override for virtual function gadget::Input::getBaseType.
   virtual std::string getBaseType()
   {
      std::string result = getBaseType_callback();
      return result;
   }

   typedef vpr::ReturnStatus* (*writeObject_callback_vpr_ObjectWriter_t)(vpr::ObjectWriter*);
   writeObject_callback_vpr_ObjectWriter_t writeObject_callback_vpr_ObjectWriter;

   /// Override for virtual function gadget::Input::writeObject.
   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0)
   {
      vpr::ReturnStatus result = *(writeObject_callback_vpr_ObjectWriter(p0));
      return result;
   }

   typedef vpr::ReturnStatus* (*readObject_callback_vpr_ObjectReader_t)(vpr::ObjectReader*);
   readObject_callback_vpr_ObjectReader_t readObject_callback_vpr_ObjectReader;

   /// Override for virtual function gadget::Input::readObject.
   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* p0)
   {
      vpr::ReturnStatus result = *(readObject_callback_vpr_ObjectReader(p0));
      return result;
   }

   typedef void (*destroy_callback_t)();
   destroy_callback_t destroy_callback;

   /// Override for virtual function gadget::Input::destroy.
   virtual void destroy()
   {
      destroy_callback();
   }

   /// Public wrapper for protected virtual function gadget::Input::destroy.
   void destroy_wrapper()
   {
      gadget::Input::destroy();
   }

}; // class gadget_Input_Adapter




#endif
