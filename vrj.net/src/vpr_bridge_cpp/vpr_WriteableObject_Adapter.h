// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from Revision: 1.15 of RCSfile: class_cxx_adapter.tmpl,v


#ifndef __VPR_WRITEABLEOBJECT_ADAPTER__
#define __VPR_WRITEABLEOBJECT_ADAPTER__

#include "sharppy.h"
#include <vpr/IO/SerializableObject.h>

class SHARPPY_API vpr_WriteableObject_Adapter : public vpr::WriteableObject
{
public:
   virtual ~vpr_WriteableObject_Adapter()
   {;}

   typedef vpr::ReturnStatus* (*writeObject_callback_vpr_ObjectWriter_t)(vpr::ObjectWriter*);
   writeObject_callback_vpr_ObjectWriter_t writeObject_callback_vpr_ObjectWriter;

   /// Override for virtual function vpr::WriteableObject::writeObject.
   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0)
   {
      vpr::ReturnStatus result = *(writeObject_callback_vpr_ObjectWriter(p0));
      return result;
   }

}; // class vpr_WriteableObject_Adapter




#endif
