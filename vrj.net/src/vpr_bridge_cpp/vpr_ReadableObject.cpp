// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.60 of RCSfile: class_cxx.tmpl,v



#include "sharppy.h"
#include <vpr/IO/SerializableObject.h>
#include <vpr_ReadableObject_Adapter.h>

extern "C"
{
   SHARPPY_API void delete_vpr_ReadableObject(vpr_ReadableObject_Adapter* self_)
   {
      delete self_;
   }

   // Wrapper for virtual method vpr::ReadableObject::readObject()
   SHARPPY_API vpr::ReturnStatus* vpr_ReadableObject_readObject__vpr_ObjectReader1(vpr_ReadableObject_Adapter* self_, vpr::ObjectReader* p0)
   {
      vpr::ReturnStatus* result;
      result = new vpr::ReturnStatus(self_->readObject(p0));
      return result;
   }


} // extern "C" for vpr::ReadableObject


