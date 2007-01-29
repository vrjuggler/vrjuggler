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

#ifndef _VPR_ERROR_IMPL_NSPR_H_
#define _VPR_ERROR_IMPL_NSPR_H_

#include <vpr/vprConfig.h>
#include <iostream>
#include <prerror.h>
#include <vpr/Util/ErrorBase.h>


namespace vpr
{

/** \class ErrorImplNSPR ErrorImplNSPR.h vpr/Util/Error.h
 *
 * NSPR Implementation of a cross-platform error reporting class.
 */
class VPR_CLASS_API ErrorImplNSPR : public ErrorBase
{
public:
   
   static void outputCurrentError(std::ostream& out, const std::string& prefix)
   {
      PRErrorCode  err = PR_GetError();
      const char* err_name = PR_ErrorToName(err);
      const char* err_str = PR_ErrorToString(err,0);
   
      /*
      int os_err = PR_GetOSError();
      char* os_str = strerror(PR_GetOSError());
      */
   
      out << "Error (NSPR): " << prefix << " (" << err;
      if(err_name != NULL)
      {
          out << ":" << err_name;
      } 

      if(err_str != NULL)
      {
          out << ", " << err_str;
      }
      out << ")" << std::endl;
   }

   static ErrorType getCurrentError()
   {
      return NoError;
   }

protected:
   static PRErrorCode convertErrorVprToNspr(ErrorType)
   {
      return PR_MAX_ERROR;
   }

   static ErrorType convertErrorNsprToVpr(PRErrorCode)
   {
      return NoError;
   }
};

} // End of vpr namespace


#endif  /* _VPR_ERROR_IMP_NSPR_H_ */
