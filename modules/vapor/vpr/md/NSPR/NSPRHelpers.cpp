/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <stdio.h>
#include <string.h>
#include <prerror.h>

#include <md/NSPR/NSPRHelpers.h>


namespace vpr {

// Print out the current NSPR error message to stderr
void NSPR_PrintError(const std::string error_prefix_string )
{
   PRInt32 textLength = PR_GetErrorTextLength();
   char *text = (char*)malloc(textLength);
   if ( PR_GetErrorText(text) > 0 ) {
      fprintf(stderr, "%s (%d, %s) (%d, %s)\n",
              error_prefix_string.c_str(), PR_GetError(), text, PR_GetOSError(), strerror(PR_GetOSError()));
   }
   else {
       fprintf(stderr, "%s (%d, %s)\n", 
               error_prefix_string.c_str(), PR_GetError(), strerror(PR_GetOSError()));
   }

   free(text);
}

};
