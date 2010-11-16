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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <cstdlib>
#include <cstring>
#include <time.h>

#include <vpr/md/WIN32/SystemWin32.h>

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#  define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#  define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

namespace vpr
{

int SystemWin32::gettimeofday(vpr::TimeVal* tv, vpr::TimeZone* tz)
{
   FILETIME ft;
   unsigned __int64 tmpres = 0;
   static int tzflag;

   if (NULL != tv)
   {
      GetSystemTimeAsFileTime(&ft);

      tmpres |= ft.dwHighDateTime;
      tmpres <<= 32;
      tmpres |= ft.dwLowDateTime;

      tmpres /= 10;
      tmpres -= DELTA_EPOCH_IN_MICROSECS;
      tv->tv_sec = static_cast<long>(tmpres / 1000000UL);
      tv->tv_usec = static_cast<long>(tmpres % 1000000UL);
   }

   if (NULL != tz)
   {
      if (! tzflag)
      {
         _tzset();
         tzflag++;
      }
      tz->tv_minuteswest = _timezone / 60;
      tz->tv_dsttime = _daylight;
   }

   return 0;
}

vpr::Uint64 SystemWin32::Ntohll(vpr::Uint64 conversion)
{
   vpr::Uint64 ret_val;

   if ( isLittleEndian() )
   {
      *(reinterpret_cast<vpr::Uint32*>(&ret_val) + 1) =
         Ntohl(*reinterpret_cast<vpr::Uint32*>(&conversion));
      *reinterpret_cast<vpr::Uint32*>(&ret_val) =
         Ntohl(*(reinterpret_cast<vpr::Uint32*>(&conversion) + 1));
   }
   else
   {
      *reinterpret_cast<vpr::Uint32*>(&ret_val) =
         Ntohl(*reinterpret_cast<vpr::Uint32*>(&conversion));
      *(reinterpret_cast<vpr::Uint32*>(&ret_val) + 1)  =
         Ntohl(*(reinterpret_cast<vpr::Uint32*>(&conversion) + 1));
   }

   return ret_val;
}

vpr::Uint64 SystemWin32::Htonll(vpr::Uint64 conversion)
{
   vpr::Uint64 ret_val;

   if ( isLittleEndian() )
   {
      *(reinterpret_cast<vpr::Uint32*>(&ret_val) + 1) =
         Htonl(*reinterpret_cast<vpr::Uint32*>(&conversion));
      *reinterpret_cast<vpr::Uint32*>(&ret_val) =
         Htonl(*(reinterpret_cast<vpr::Uint32*>(&conversion) + 1));
   }
   else
   {
      *reinterpret_cast<vpr::Uint32*>(&ret_val) =
         Htonl(*reinterpret_cast<vpr::Uint32*>(&conversion));
      *(reinterpret_cast<vpr::Uint32*>(&ret_val) + 1) =
         Htonl(*(reinterpret_cast<vpr::Uint32*>(&conversion) + 1));
   }

   return ret_val;
}

bool SystemWin32::getenv(const std::string& name, std::string& result)
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

bool SystemWin32::setenv(const std::string& name, const std::string& value)
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

std::string SystemWin32::getHostname()
{
   //struct utsname buffer;
   char buffer[256];

   if ( ::gethostname(buffer, 256) == 0 )
   {
      char* temp;
      temp = std::strchr(buffer, '.');

      // If the node name contains the full host, dots and all, truncate it
      // at the first dot.
      if ( temp != NULL )
      {
         *temp = '\0';
      }

      return std::string(buffer);
   }
   else
   {
      return std::string("<hostname-lookup failed>");
   }
}

} // End of vpr namespace
