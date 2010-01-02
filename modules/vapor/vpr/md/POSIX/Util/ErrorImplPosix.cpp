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
#include <cstring>
#include <errno.h>
#include <vpr/md/POSIX/Util/ErrorImplPosix.h>


namespace vpr
{

std::string ErrorImplPosix::getCurrentErrorMsg()
{
   const char* err_str(std::strerror(errno));

   if ( NULL != err_str )
   {
      return std::string(err_str);
   }
   else
   {
      return std::string("");
   }
}

void ErrorImplPosix::outputCurrentError(std::ostream& out,
                                        const std::string& prefix)
{
   const char* err_str = std::strerror(errno);

   out << "Error (POSIX): " << prefix << " (" << errno;

   if ( err_str != NULL )
   {
      out << ", " << err_str;
   }

   out << ")" << std::endl;
}

}
