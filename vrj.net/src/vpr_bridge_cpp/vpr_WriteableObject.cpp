// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.60 of RCSfile: class_cxx.tmpl,v



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
   SHARPPY_API vpr::ReturnStatus* vpr_WriteableObject_writeObject__vpr_ObjectWriter1(vpr_WriteableObject_Adapter* self_, vpr::ObjectWriter* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->writeObject(p0));
      return result;
   }


} // extern "C" for vpr::WriteableObject


