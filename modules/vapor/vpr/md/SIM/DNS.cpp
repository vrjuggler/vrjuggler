/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#include <vpr/System.h>
#include <vpr/Util/Debug.h>
#include <vpr/md/SIM/DNS.h>


namespace vpr
{

namespace sim
{

vpr::Uint32 DNS::lookupAddress (const std::string& addr_str)
{
   vpr::Uint32 found_addr;

   // if name is found in lookup, return the address
   if ( mDnsAddressLookup.count(addr_str) > 0 )
   {
      found_addr = mDnsAddressLookup[addr_str];
   }
   // if not found, then make an entry for the new address.
   else
   {
      union
      {
         vpr::Uint8 bytes[4];
         vpr::Uint32 value;
      } addr;
      int retval;

      vpr::Uint32 temp1, temp2, temp3, temp4;
      retval = sscanf(addr_str.c_str(), "%3u.%3u.%3u.%3u", &temp1, &temp2,
                      &temp3, &temp4);
      addr.bytes[0] = (vpr::Uint8) temp1;
      addr.bytes[1] = (vpr::Uint8) temp2;
      addr.bytes[2] = (vpr::Uint8) temp3;
      addr.bytes[3] = (vpr::Uint8) temp4;

      if ( retval == 4 )
      {
         found_addr = vpr::System::Ntohl(addr.value);
      }
      else
      {
         found_addr = mUniqueGenerator++;
      }

      mDnsAddressLookup[addr_str] = found_addr;
   }

   return found_addr;
}

}

}
