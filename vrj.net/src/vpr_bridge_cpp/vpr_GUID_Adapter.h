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


#ifndef __VPR_GUID_ADAPTER__
#define __VPR_GUID_ADAPTER__

#include "sharppy.h"
#include <vpr/Util/GUID.h>

class SHARPPY_API vpr_GUID_Adapter : public vpr::GUID
{
public:
   vpr_GUID_Adapter(const vpr::GUID::GenerateTag& p0)
      : vpr::GUID(p0)
   {;}

   vpr_GUID_Adapter()
      : vpr::GUID()
   {;}

   vpr_GUID_Adapter(const vpr::GUID::StdGUID& p0)
      : vpr::GUID(p0)
   {;}

   vpr_GUID_Adapter(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& p0)
      : vpr::GUID(p0)
   {;}

   vpr_GUID_Adapter(const vpr::GUID& p0, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& p1)
      : vpr::GUID(p0, p1)
   {;}

   vpr_GUID_Adapter(const vpr::GUID& p0)
      : vpr::GUID(p0)
   {;}

   virtual ~vpr_GUID_Adapter()
   {;}

   typedef vpr::ReturnStatus* (*writeObject_callback_vpr_ObjectWriter_t)(vpr::ObjectWriter*);
   writeObject_callback_vpr_ObjectWriter_t writeObject_callback_vpr_ObjectWriter;

   /// Override for virtual function vpr::GUID::writeObject.
   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* p0)
   {
      return *(writeObject_callback_vpr_ObjectWriter(p0));
   }

   typedef vpr::ReturnStatus* (*readObject_callback_vpr_ObjectReader_t)(vpr::ObjectReader*);
   readObject_callback_vpr_ObjectReader_t readObject_callback_vpr_ObjectReader;

   /// Override for virtual function vpr::GUID::readObject.
   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* p0)
   {
      return *(readObject_callback_vpr_ObjectReader(p0));
   }

}; // class vpr_GUID_Adapter




#endif
