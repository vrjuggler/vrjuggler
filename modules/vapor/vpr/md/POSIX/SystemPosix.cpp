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

#include <cstdlib>
#include <cstring>
#include <sys/utsname.h>

#include <vpr/md/POSIX/SystemPosix.h>


namespace vpr
{

int SystemPosix::usleep(vpr::Uint32 micro)
{
#ifdef VPR_OS_Linux
   ::usleep(micro);
   return 0;  // usleep can't report failure, so assume success.
#else
   return ::usleep(micro);
#endif
}

int SystemPosix::msleep(vpr::Uint32 milli)
{
   // usleep() cannot sleep for more than 1 second, so we have to work
   // around that here.  First, we sleep for N seconds.
   if ( milli >= 1000 )
   {
      SystemPosix::sleep(milli / 1000);
   }

   // Then we finish off by sleeping for (N mod 1000) milliseconds.
   return SystemPosix::usleep((milli % 1000) * 1000);
}

union uint64_holder
{
   vpr::Uint64 uint64_value;
   struct
   {
      vpr::Uint32 uint32_low_value;
      vpr::Uint32 uint32_high_value;
   } struct_value;
};

vpr::Uint64 SystemPosix::Ntohll(vpr::Uint64 conversion)
{
   uint64_holder input;
   uint64_holder output;

   input.uint64_value  = conversion;
   output.uint64_value = 0;

   if ( isLittleEndian() )
   {
      output.struct_value.uint32_high_value =
         Ntohl(input.struct_value.uint32_low_value);
      output.struct_value.uint32_low_value =
         Ntohl(input.struct_value.uint32_high_value);
   }
   else
   {
      output.struct_value.uint32_low_value =
         Ntohl(input.struct_value.uint32_low_value);
      output.struct_value.uint32_high_value =
         Ntohl(input.struct_value.uint32_high_value);
   }

   return output.uint64_value;
}

vpr::Uint64 SystemPosix::Htonll(vpr::Uint64 conversion)
{
   uint64_holder input;
   uint64_holder output;

   input.uint64_value  = conversion;
   output.uint64_value = 0;

   if ( isLittleEndian() )
   {
      output.struct_value.uint32_high_value =
         Htonl(input.struct_value.uint32_low_value);
      output.struct_value.uint32_low_value =
         Htonl(input.struct_value.uint32_high_value);
   }
   else
   {
      output.struct_value.uint32_low_value =
         Htonl(input.struct_value.uint32_low_value);
      output.struct_value.uint32_high_value =
         Htonl(input.struct_value.uint32_high_value);
   }

   return output.uint64_value;
}

bool SystemPosix::getenv(const std::string& name, std::string& result)
{
   char* val;
   bool status(false);

   val = std::getenv(name.c_str());

   if ( val != NULL )
   {
      result = val;
      status = true;
   }

   return status;
}

bool SystemPosix::setenv(const std::string& name, const std::string& value)
{
   std::string set_value(name);
   set_value += "=";
   set_value += value;

   // Purposely leak memory since putenv(3) may want to hold on to the
   // pointer we pass.
   char* env_str = strdup(set_value.c_str());
   const int ret_val = ::putenv(env_str);

   return ret_val == 0;
}

std::string SystemPosix::getHostname()
{
   struct utsname buffer;

   if ( uname(&buffer) == 0 )
   {
      char* temp;
      temp = std::strchr(buffer.nodename, '.');

      // If the node name contains the full host, dots and all, truncate it
      // at the first dot.
      if ( temp != NULL )
      {
         *temp = '\0';
      }

      return std::string(buffer.nodename);
   }
   else
   {
      return std::string("<hostname-lookup failed>");
   }
}

} // End of vpr namespace
