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


#ifndef __GADGET_POSITION_ADAPTER__
#define __GADGET_POSITION_ADAPTER__

#include "sharppy.h"
#include <gadget/Type/Position.h>

class SHARPPY_API gadget_Position_Adapter : public gadget::Position
{
public:
   gadget_Position_Adapter()
      : gadget::Position()
   {;}

   virtual ~gadget_Position_Adapter()
   {;}

   typedef bool (*config_callback_jccl_ConfigElementPtr_t)(jccl::ConfigElementPtr*);
   config_callback_jccl_ConfigElementPtr_t config_callback_jccl_ConfigElementPtr;

   /// Override for virtual function gadget::Position::config.
   virtual bool config(jccl::ConfigElementPtr p0)
   {
      jccl::ConfigElementPtr* p_p0 = new jccl::ConfigElementPtr(p0);
      bool result = config_callback_jccl_ConfigElementPtr(p_p0);
      return result;
   }

   typedef char* (*getBaseType_callback_t)();
   getBaseType_callback_t getBaseType_callback;

   /// Override for virtual function gadget::Position::getBaseType.
   virtual std::string getBaseType()
   {
      std::string result = getBaseType_callback();
      return result;
   }

   typedef vpr::ReturnStatus* (*writeObject_callback_vpr_ObjectWriter_t)(vpr::ObjectWriter*);
   writeObject_callback_vpr_ObjectWriter_t writeObject_callback_vpr_ObjectWriter;

   /// Override for virtual function gadget::Position::writeObject.
   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0)
   {
      vpr::ReturnStatus result = *(writeObject_callback_vpr_ObjectWriter(p0));
      return result;
   }

   typedef vpr::ReturnStatus* (*readObject_callback_vpr_ObjectReader_t)(vpr::ObjectReader*);
   readObject_callback_vpr_ObjectReader_t readObject_callback_vpr_ObjectReader;

   /// Override for virtual function gadget::Position::readObject.
   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* p0)
   {
      vpr::ReturnStatus result = *(readObject_callback_vpr_ObjectReader(p0));
      return result;
   }

}; // class gadget_Position_Adapter




#endif
