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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <vpr/md/WiN32/Util/ErrorImplWin32.h>


namespace vpr
{

std::string ErrorImplWin32::getCurrentErrorMsg()
{
   std::string err_str;
   const DWORD error_code(GetLastError());

   if ( ERROR_SUCCESS != error_code )
   {
      char* msg_buffer(NULL);

      FormatMessage(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
         NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
         (LPTSTR) &msg_buffer, 0, NULL
      );

      if ( NULL != msg_buffer )
      {
         err_str = msg_buffer;
         LocalFree(msg_buffer);
      }
   }

   return err_str;
}

void ErrorImplWin32::outputCurrentError(std::ostream& out,
                                        const std::string& prefix)
{
   const std::string err_str(getCurrentErrorMsg());

   out << "Error (WIN32): " << prefix << " (" << errno;

   if ( ! err_str.empty() )
   {
      out << ", " << err_str;
   }

   out << ")" << std::endl;
}

}
