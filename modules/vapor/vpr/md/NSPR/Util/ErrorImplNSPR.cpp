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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <sstream>
#include <prerror.h>

#include <vpr/md/NSPR/Util/ErrorImplNSPR.h>


namespace vpr
{

std::string ErrorImplNSPR::getCurrentErrorMsg()
{
   PRErrorCode  err = PR_GetError();
   const char* err_name = PR_ErrorToName(err);
   const char* err_str = PR_ErrorToString(err,0);

   /*
   int os_err = PR_GetOSError();
   char* os_str = strerror(PR_GetOSError());
   */

   std::stringstream msg_stream;
   msg_stream << "NSPR error code " << err;

   if ( err_name != NULL || err_str != NULL )
   {
      if ( err_name != NULL )
      {
         msg_stream << ": " << err_name;
      } 

      if ( err_str != NULL )
      {
         msg_stream << ", " << err_str;
      }
   }

   return msg_stream.str();
}

}
