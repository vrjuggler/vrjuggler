/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <string.h>

#include <vpr/Util/GUID.h>


namespace vpr
{

std::string GUID::toString () const
{
   std::string guid_str;
   char guid_c_str[39];

   snprintf(guid_c_str, 39,
            "%08X-%04hX-%04hX-%02X%02X-%02X%02X%02X%02X%02X%02X",
            m_guid.moz.m0, m_guid.moz.m1, m_guid.moz.m2, m_guid.moz.m3[0],
            (vpr::Uint32) m_guid.moz.m3[1], (vpr::Uint32) m_guid.moz.m3[2],
            (vpr::Uint32) m_guid.moz.m3[3], (vpr::Uint32) m_guid.moz.m3[4],
            (vpr::Uint32) m_guid.moz.m3[5], (vpr::Uint32) m_guid.moz.m3[6],
            (vpr::Uint32) m_guid.moz.m3[7]);
   guid_str = guid_c_str;

   return guid_str;
}

bool GUID::operator== (const GUID& guid) const
{
   return (uuid_compare(&m_guid.leach, &guid.m_guid.leach) == 0 ? true :
                                                                  false);
}

// ============================================================================
// Private methods.
// ============================================================================

GUID::GUID ()
{
   uuid_create(&m_guid.leach);
}

GUID::GUID (const struct vpr::GUID::StdGUID& guid)
{
   memcpy(&m_guid, &guid, sizeof(vpr::GUID::StdGUID));
}

GUID::GUID (const std::string& guid_string)
{
   sscanf(guid_string.c_str(),
          "%08X-%04hX-%04hX-%02X%02X-%02X%02X%02X%02X%02X%02X",
          &m_guid.moz.m0, &m_guid.moz.m1, &m_guid.moz.m2,
          &m_guid.moz.m3[0], &m_guid.moz.m3[1], &m_guid.moz.m3[2],
          &m_guid.moz.m3[3], &m_guid.moz.m3[4], &m_guid.moz.m3[5],
          &m_guid.moz.m3[6], &m_guid.moz.m3[7]);
}

GUID::GUID (const GUID& ns_guid, const std::string& name)
{
   uuid_create_from_name(&m_guid.leach, ns_guid.m_guid.leach,
                         (void*) name.c_str(), name.length());
}

} // End of vpr namespace
