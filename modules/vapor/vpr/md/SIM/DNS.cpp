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
         vpr::Uint8 bytes[0];
         vpr::Uint32 value;
      } addr;
      int retval;

      retval = sscanf(addr_str.c_str(), "%3u.%3u.%3u.%3u", &addr.bytes[0],
                      &addr.bytes[1], &addr.bytes[2], &addr.bytes[3]);

      vprDEBUG(vprDBG_ALL, 0) << "retval: " << retval << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL, 0) << "addr.value: " << addr.value << "\n" << vprDEBUG_FLUSH;
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
