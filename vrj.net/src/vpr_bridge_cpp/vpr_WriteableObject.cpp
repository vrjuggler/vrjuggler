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
#include <vpr/IO/SerializableObject.h>
#include <vpr_WriteableObject_Adapter.h>

extern "C"
{
   SHARPPY_API void delete_vpr_WriteableObject(vpr_WriteableObject_Adapter* self_)
   {
      delete self_;
   }


   // Wrapper for virtual method vpr::WriteableObject::writeObject()
   SHARPPY_API vpr::ReturnStatus* vpr_WriteableObject_writeObject__vpr_ObjectWriter(vpr_WriteableObject_Adapter* self_, vpr::ObjectWriter* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->writeObject(p0));

      return result;
   }




} // extern "C" for vpr::WriteableObject


