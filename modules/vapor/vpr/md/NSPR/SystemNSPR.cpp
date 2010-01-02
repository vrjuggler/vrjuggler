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

#ifdef VPR_OS_Windows
#  include <winsock2.h> /* For struct tiemval */
#  include <cstdlib>    /* For std::getenv() and _putenv() */
#else
#  include <cstring>    /* For strdup() */
#endif

#include <sstream>
#include <boost/concept_check.hpp>

#include <vpr/md/NSPR/SystemNSPR.h>


namespace vpr
{

int SystemNSPR::gettimeofday(vpr::TimeVal* tp, vpr::TimeZone* tzp)
{
   boost::ignore_unused_variable_warning(tzp);
   PRTime now = PR_Now();

   tp->tv_sec  = now / 1000000;
   tp->tv_usec = now % 1000000;

   return 0;
}

bool SystemNSPR::getenv(const std::string& name, std::string& result)
{
   bool status(false);

#if defined(VPR_OS_Windows)
#if defined(_MSC_VER) && _MSC_VER >= 1400
   char* val(NULL);
   size_t num;
   _dupenv_s(&val, &num, name.c_str());
#else
   const char* val = std::getenv(name.c_str());
#endif
#else
   const char* val = PR_GetEnv(name.c_str());
#endif

   if ( val != NULL )
   {
      result = val;
      status = true;

#if defined(VPR_OS_Windows) && defined(_MSC_VER) && _MSC_VER >= 1400
      std::free(val);
      val = NULL;
#endif
   }

   return status;
}

bool SystemNSPR::setenv(const std::string& name, const std::string& value)
{
   bool status(false);
   // NSPR and _putenv() require the form "name=value".
   std::ostringstream env_stream;
   env_stream << name << "=" << value;

#if defined(VPR_OS_Windows)
   const int ret_val = _putenv(env_stream.str().c_str());
   status = ret_val == 0;
#else
   // NSPR takes possesion of the string memory.
   const PRStatus ret_val = PR_SetEnv(strdup(env_stream.str().c_str()));
   status = ret_val == PR_SUCCESS;
#endif

   return status;
}

}
