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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <iostream>
#include <prerror.h>
#include <prerr.h>

#include <vpr/md/NSPR/NSPRHelpers.h>


namespace vpr
{

// Print out the current NSPR error message to the given output stream.
void NSPR_PrintError(const std::string& errorPrefixString, std::ostream& out)
{
   PRErrorCode  err = PR_GetError();
   const char* err_name = PR_ErrorToName(err);
   const char* err_str = PR_ErrorToString(err,0);

   out << "Error (NSPR): " << errorPrefixString << "(" << err;

   if ( err_name != NULL )
   {
      out << ":" << err_name;
   }

   if ( err_str != NULL )
   {
      out << ", " << err_str;
   }

   out << ")" << std::endl;
}

}   // namespace
