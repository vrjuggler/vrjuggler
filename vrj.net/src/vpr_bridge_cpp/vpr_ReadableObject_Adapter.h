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

// Generated from Revision: 1.15 of RCSfile: class_cxx_adapter.tmpl,v


#ifndef __VPR_READABLEOBJECT_ADAPTER__
#define __VPR_READABLEOBJECT_ADAPTER__

#include "sharppy.h"
#include <vpr/IO/SerializableObject.h>

class SHARPPY_API vpr_ReadableObject_Adapter : public vpr::ReadableObject
{
public:
   virtual ~vpr_ReadableObject_Adapter()
   {;}

   typedef vpr::ReturnStatus* (*readObject_callback_vpr_ObjectReader_t)(vpr::ObjectReader*);
   readObject_callback_vpr_ObjectReader_t readObject_callback_vpr_ObjectReader;

   /// Override for virtual function vpr::ReadableObject::readObject.
   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* p0)
   {
      vpr::ReturnStatus result = *(readObject_callback_vpr_ObjectReader(p0));
      return result;
   }

}; // class vpr_ReadableObject_Adapter




#endif
